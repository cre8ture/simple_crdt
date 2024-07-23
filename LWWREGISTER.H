#ifndef LWWREGISTER_H
#define LWWREGISTER_H

#include <string>
#include <tuple>

template<typename T>
class LWWRegister {
public:
    using StateType = std::tuple<std::string, int, T*>;

    // Default constructor
    LWWRegister() : id(""), currentState({ "", 0, nullptr }) {}

    // Constructor with parameters
    LWWRegister(const std::string& id, const StateType& initialState)
        : id(id), currentState(initialState) {}

    // Copy constructor
    LWWRegister(const LWWRegister& other)
        : id(other.id), currentState(other.currentState) {}

    T& value() const {
        return *std::get<2>(currentState);
    }

    StateType getState() const {
        return currentState;
    }

    void set(T* value) {
        int newTimestamp = std::get<1>(currentState) + 1;
        currentState = { id, newTimestamp, value };
    }

    void merge(const StateType& remoteState) {
        if (std::get<1>(remoteState) > std::get<1>(currentState) ||
            (std::get<1>(remoteState) == std::get<1>(currentState) && std::get<0>(remoteState) > id)) {
            currentState = remoteState;
        }
    }

private:
    std::string id;
    StateType currentState;
};

#endif // LWWREGISTER_H
