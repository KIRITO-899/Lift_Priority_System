#pragma once
#include "Person.h"
#include <vector>
#include <queue>

using namespace std;

// TODO: Jyothiradithya - Implement Controller class
// Responsibilities:
//   - Store multiple lift definitions
//   - Accept passenger input
//   - Find nearest available lift
//   - Assign requests to lifts
//   - Emergency queue logic (priority queue)

class Controller {
public:
    int numLifts;
    int numFloors;

    Controller(int lifts, int floors);

    void addPassengerRequest(Person p);
    int findNearestLift(int floor);
    void assignRequest(Person p);
    void displayQueue() const;
};
