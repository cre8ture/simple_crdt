#include <iostream>
#include "LWWRegister.h"
#include "LWW_MAP.h"

int main() {
    // Initialize the LWWRegister with some initial state
    //LWWRegister<double> register1("peer1", 1, "0", 0.0);

    //// Print initial state
    //std::cout << "Initial state:" << std::endl;
    //register1.printState();

    //// Prompt the user to input two values
    //double value1, value2;
    //std::cout << "Enter first value: ";
    //std::cin >> value1;
    //register1.set(value1);

    //std::cout << "Updated state after first value:" << std::endl;
    //register1.printState();

    //std::cout << "Enter second value: ";
    //std::cin >> value2;
    //register1.set(value2);

    //std::cout << "Updated state after second value:" << std::endl;
    //register1.printState();

    testLWWMap();


    return 0;
}
