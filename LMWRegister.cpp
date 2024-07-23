#include "LWWRegister.h"

// Constructor implementation
template <typename T, typename S>
LWWRegister<T, S>::LWWRegister(const std::string& id, std::tuple<int, std::string, double> state, T value) {
    this->id = id;
    this->state = state;
}

// Getter for value implementation
template <typename T, typename S>
int LWWRegister<T, S>::value() {
    return std::get<2>(state);
}

// Setter for value implementation
template <typename T, typename S>
void LWWRegister<T, S>::set(T value) {
    this->state = std::make_tuple(std::get<0>(state), std::get<1>(state) + 1, value);
}

// Merge function implementation
template <typename T, typename S>
void LWWRegister<T, S>::merge(const std::tuple<int, std::string, double>& remoteState) {
    int remotePeer = std::get<0>(remoteState);
    std::string remoteTimestamp = std::get<1>(remoteState);
    int localPeer = std::get<0>(this->state);
    std::string localTimestamp = std::get<1>(this->state);

    if (localTimestamp < remoteTimestamp || (localTimestamp == remoteTimestamp && localPeer < remotePeer)) {
        this->state = remoteState;
    }
}

// GSet class implementation
template <typename T>
void GSet<T>::add(const T& element) {
    state.insert(element);
}

template <typename T>
void GSet<T>::merge(const GSet<T>& other) {
    state.insert(other.state.begin(), other.state.end());
}

template <typename T>
bool GSet<T>::lookup(const T& element) const {
    return state.count(element) > 0;
}
