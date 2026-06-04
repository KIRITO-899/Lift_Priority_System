#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ── Space allocation per person type ──────────────────
// Total lift space = 100 
// Normal person  → 10 
// Staff          → 20  (emergency only)
// Doctor         → 30  (emergency only)
// EmergencyPatient → 50 units
// ──────────────────────────────────────────────────────

enum class PersonType {
    Normal,            // regular passenger    — 10 
    Staff,             // hospital staff       — 20  (emergency)
    Doctor,            // doctor               — 30  (emergency)
    EmergencyPatient   // emergency patient    — 50 
};

enum class LiftStatus {
    Available,
    Busy,
};

enum class Direction {
    Up,
    Down,
    Idle
};

class Lift {
public:
    int lift_id;
    int currentFloor;
    int totalSpace;        // (default 100)
    int usedSpace;         // space currently occupied
    int currentPassengers; // number of people inside
    LiftStatus status;
    Direction direction;

    // Emergency mode
    bool isEmergency;
    int  emergencyDestination; // no unnecessary stops

    Lift(int id, int startFloor = 0, int totalSpace = 100);

    // Space helper — returns how many units a person type consumes
    static int spaceOf(PersonType type);

    // Movement
    void moveUp();
    void moveDown();
    void moveTo(int targetFloor);

    // Doors
    void openDoors();
    void closeDoors();

    // Passengers 
    bool boardPassenger(PersonType type = PersonType::Normal);
    bool dropPassenger(PersonType type = PersonType::Normal);

    // Emergency
    void activateEmergency(int destinationFloor);
    void deactivateEmergency();

    // Status
    bool isAvailable() const;
    void setStatus(LiftStatus s);

    // Display
    void displayStatus() const;
};
