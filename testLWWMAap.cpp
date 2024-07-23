#include <iostream>
#include <cassert>
#include "LWWRegister.h"
#include "LWW_MAP.h"

void testLWWMap() {
    std::string id1 = "peer1";
    std::string id2 = "peer2";

    int value1 = 42;
    int value2 = 100;

    LWWRegister<int>::StateType initialState1 = { id1, 1, &value1 };
    LWWRegister<int>::StateType initialState2 = { id2, 1, &value2 };

    std::map<std::string, LWWRegister<int>> initialState;
    initialState["key1"] = LWWRegister<int>(id1, initialState1);
    initialState["key2"] = LWWRegister<int>(id2, initialState2);

    LWW_MAP<int> map1(id1, initialState);
    LWW_MAP<int> map2(id2, initialState);

    // Test getValue
    auto values = map1.getValue();
    assert(values["key1"] == 42);
    assert(values["key2"] == 100);

    // Test get
    assert(*map1.get("key1") == 42);
    assert(*map1.get("key2") == 100);

    // Test set and deleteKey
    int newValue = 50;
    map1.set("key1", &newValue);  // Pass by pointer

    assert(*map1.get("key1") == 50);

    map1.deleteKey("key1");
    assert(map1.get("key1") == nullptr);

    // Test has
    assert(!map1.has("key1"));
    assert(map1.has("key2"));

    // Test merge
    int anotherValue = 200;
    LWWRegister<int>::StateType newState = { id2, 2, &anotherValue };
    map2.set("key2", &anotherValue);  // Pass by pointer

    map1.merge(map2.getState());

    assert(*map1.get("key2") == 200);

    std::cout << "All tests passed!" << std::endl;
}
