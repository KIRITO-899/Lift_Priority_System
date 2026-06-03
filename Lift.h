#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// HY - Lift Class
// Responsibilities:
//   - Lift Status Management (Available / Busy / Trip Complete)
//   - Movement Logic (Move Up / Move Down)
//   - Open / Close Doors
//   - Passenger Boarding
//   - Passenger Drop-off
//   - Capacity Handling
//   - Trip Completion Updates

enum class LiftStatus {
    Available,
    Busy,
    TripComplete
};

enum class Direction {
    Up,
    Down,
    Idle
};

class Lift {
public:
    int id;
    int currentFloor;
    int capacity;
    int currentPassengers;
    LiftStatus status;
    Direction direction;

    Lift(int id, int startFloor = 0, int capacity = 10);

    // Movement
    void moveUp();
    void moveDown();
    void moveTo(int targetFloor);

    // Doors
    void openDoors();
    void closeDoors();

    // Passengers
    bool boardPassenger();
    bool dropPassenger();

    // Status
    bool isAvailable() const;
    void setStatus(LiftStatus s);
    void completTrip();

    // Display
    void displayStatus() const;
};
