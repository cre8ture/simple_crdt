#ifndef LWW_MAP_H
#define LWW_MAP_H

#include <map>
#include <string>
#include <memory>
#include "LWWRegister.h"

template<typename T>
using Value = std::map<std::string, T>;

template<typename T>
using State = std::map<std::string, typename LWWRegister<T>::StateType>;

template<typename T>
class LWW_MAP {
private:
    std::string id = "";
    std::map<std::string, std::unique_ptr<LWWRegister<T>>> data;  // Using smart pointers for better memory management

public:
    LWW_MAP(const std::string& id, const std::map<std::string, LWWRegister<T>>& state);

    Value<T> getValue() const;
    State<T> getState() const;
    void merge(const State<T>& remoteState);
    T* get(const std::string& key) const;
    void set(const std::string& key, T* value);
    void deleteKey(const std::string& key);
    bool has(const std::string& key) const;
};

template<typename T>
LWW_MAP<T>::LWW_MAP(const std::string& id, const std::map<std::string, LWWRegister<T>>& state)
    : id(id) {
    for (const auto& [s, j] : state) {
        data[s] = std::make_unique<LWWRegister<T>>(j);  // Using copy constructor
    }
}

template<typename T>
Value<T> LWW_MAP<T>::getValue() const {
    Value<T> value;
    for (const auto& [s, j] : data) {
        if (j) {
            value[s] = j->value();
        }
    }
    return value;
}

template<typename T>
State<T> LWW_MAP<T>::getState() const {
    State<T> state;
    for (const auto& [s, j] : data) {
        if (j) {
            state[s] = j->getState();
        }
    }
    return state;
}

template<typename T>
void LWW_MAP<T>::merge(const State<T>& remoteState) {
    for (const auto& [key, remote] : remoteState) {
        auto local = data.find(key);
        if (local != data.end() && local->second) {
            local->second->merge(remote);
        }
        else {
            data[key] = std::make_unique<LWWRegister<T>>(this->id, remote);
        }
    }
}

template<typename T>
void LWW_MAP<T>::set(const std::string& key, T* value) {
    auto it = data.find(key);
    if (it != data.end() && it->second) {
        it->second->set(value);
    }
    else {
        data[key] = std::make_unique<LWWRegister<T>>(this->id, typename LWWRegister<T>::StateType{this->id, 0, value});
    }
}

template<typename T>
T* LWW_MAP<T>::get(const std::string& key) const {
    auto it = data.find(key);
    if (it != data.end() && it->second) {
        return &it->second->value();
    }
    return nullptr;
}

template<typename T>
void LWW_MAP<T>::deleteKey(const std::string& key) {
    auto it = data.find(key);
    if (it != data.end() && it->second) {
        it->second->set(nullptr);  // Assuming LWWRegister::set accepts a pointer
    }
}

template<typename T>
bool LWW_MAP<T>::has(const std::string& key) const {
    auto it = data.find(key);
    return (it != data.end() && it->second && std::get<2>(it->second->getState()) != nullptr);
}

// Prototype for test function
void testLWWMap();

#endif // LWW_MAP_H
