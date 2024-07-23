#include <iostream>
#include <tuple>
#include <string>
#include <set>

// Interface in C++ using a class with pure virtual functions
template <typename T, typename S>
class CvRDTs {
public:
    T values; // The entire point of the CRDT is to reliably sync the value between peers.
    S state; // This is the metadata needed for peers to agree on the same value. To update other peers, the whole state is serialized and sent to them.

    virtual void merge(const S& state) = 0; // A merge function. This is a function that takes some state (probably received from another peer) and merges it with the local state.
};

// LWWRegister implementation
template <typename T, typename S>
class LWWRegister : public CvRDTs<T, S> {
    std::string id;
    std::tuple<int, std::string, double> state;

public:
    // Constructor
    LWWRegister(const std::string& id, std::tuple<int, std::string, double> state, T value) {
        this->id = id;
        this->state = state;
    }

    // Getter for value
    int value() {
        return std::get<2>(state);
    }

    // Setter for value
    void set(T value) {
        this->state = std::make_tuple(std::get<0>(state), std::get<1>(state) + 1, value);
    }

    // Merge function
    void merge(const std::tuple<int, std::string, double>& remoteState) override {
        int remotePeer = std::get<0>(remoteState);
        std::string remoteTimestamp = std::get<1>(remoteState);
        int localPeer = std::get<0>(this->state);
        std::string localTimestamp = std::get<1>(this->state);

        if (localTimestamp < remoteTimestamp || (localTimestamp == remoteTimestamp && localPeer < remotePeer)) {
            this->state = remoteState;
        }
    }
};

// GSet implementation
template <typename T>
class GSet {
public:
    std::set<T> state;

    void add(const T& element) {
        state.insert(element);
    }

    void merge(const GSet<T>& other) {
        state.insert(other.state.begin(), other.state.end());
    }

    bool lookup(const T& element) const {
        return state.count(element) > 0;
    }
};

int main() {
    // Example usage
    LWWRegister<int, std::tuple<int, std::string, double>> reg("peer1", std::make_tuple(1, "timestamp", 42.0), 42);
    std::cout << "Initial value: " << reg.value() << std::endl;

    reg.set(100);
    std::cout << "Updated value: " << reg.value() << std::endl;

    GSet<int> gset1;
    gset1.add(1);
    gset1.add(2);

    GSet<int> gset2;
    gset2.add(3);

    gset1.merge(gset2);

    std::cout << "GSet lookup 1: " << gset1.lookup(1) << std::endl;
    std::cout << "GSet lookup 3: " << gset1.lookup(3) << std::endl;

    return 0;
}
