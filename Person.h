#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <vector>
#include <queue>

#define EMERGENCY        100
#define DOCTOR            90
#define STAFF             70
#define NORMAL_CHECK_UP   30
#define VISITOR           20

#define EMERGENCY_ID        213
#define DOCTOR_ID           214
#define STAFF_ID            215
#define NORMAL_CHECK_UP_ID  216
#define VISITOR_ID          217

extern std::priority_queue<int> pq_1;

class Person {
public:
    int id;
    int currentFloor;          // BUG M1 fix: was 'currntFloor' (typo)
    int DestFloor;
    int Priority = VISITOR;    // BUG 6 fix: default to lowest priority (was uninitialized)

    void display() {
        std::cout << "ID: "                << id           << "\n";
        std::cout << "Current Floor: "     << currentFloor << "\n";
        std::cout << "Destination Floor: " << DestFloor    << "\n";
    }

    void input() {
        std::cout << "Your ID: ";
        std::cin  >> id;

        std::cout << "Your Current Floor (0-10): ";
        std::cin  >> currentFloor;

        std::cout << "Your Destination Floor (0-10): ";
        std::cin  >> DestFloor;

        if      (id == EMERGENCY_ID)       Priority = EMERGENCY;
        else if (id == DOCTOR_ID)          Priority = DOCTOR;
        else if (id == STAFF_ID)           Priority = STAFF;
        else if (id == NORMAL_CHECK_UP_ID) Priority = NORMAL_CHECK_UP;
        else                               Priority = VISITOR;

        pq_1.push(Priority);
    }
};

extern std::vector<Person> people;

#endif