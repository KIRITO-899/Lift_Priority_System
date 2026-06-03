#pragma once
#include <iostream>
#include <string>

using namespace std;

// TODO: Jyothiradithya - Implement Person class
// Responsibilities:
//   - Name, passenger type (Emergency / Doctor / Staff / Visitor)
//   - Source floor, destination floor
//   - Priority value (Emergency=100, Doctor=90, Staff=70, Visitor=20)

enum class PassengerType {
    Emergency = 100,
    Doctor    = 90,
    Staff     = 70,
    Visitor   = 20
};

class Person {
public:
    string name;
    PassengerType type;
    int sourceFloor;
    int destFloor;
    int priority;

    Person(string name, PassengerType type, int src, int dest);
    int getPriority() const;
    void display() const;
};
