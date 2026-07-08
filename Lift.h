#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int MAX_FLOOR = 10;  // highest floor index (floors 0 to MAX_FLOOR)

// ── Space allocation per person type ──────────────────
// Total lift space = 100 
// Normal person      →  10 units
// Staff              →  20 units  (emergency only)
// Doctor             →  30 units  (emergency only)
// EmergencyPatient   →  50 units
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

struct FloorCall {
    int floor;
    Direction wantedDir;   // direction the waiting passenger intends to travel
};

class Lift {
public:
    int LiftID;
    int CurrentFloor;
    int TotalSpace;        // (default 100)
    int UsedSpace;         // space currently occupied
    int CurrentPassengers; // number of people inside
    LiftStatus status;
    Direction direction;

    // Door state
    bool doorsOpen;             // true while doors are open
    int  doorHoldSeconds;       // how long doors stay open (default 3s)

    // Floor requests from passengers
    std::vector<int>       destinations;  // internal: passenger inside pressed a floor button
    std::vector<FloorCall> floorCalls;    // external: someone outside pressed UP or DOWN on a floor

    // Emergency mode
    bool isEmergency;
    int  emergencyDestination; // no unnecessary stops

    Lift(int id, int startFloor = 0, int totalSpace = 100, int doorHoldSecs = 3);

    // Space helper — returns how many units a person type consumes
    static int spaceOf(PersonType type);

    // Movement
    void moveUp();
    void moveDown();
    void moveTo(int targetFloor);

    // Doors
    void openDoors();
    void closeDoors();
    void holdDoorOpen();        // opens doors then closes after hold period
    void closeDoorManually();   // simulates pressing the Close button

    // Passengers
    bool boardPassenger(PersonType type = PersonType::Normal);
    bool dropPassenger(PersonType type = PersonType::Normal);
    void servePassengers(int source, int destination, PersonType type);

    // Floor selection
    void requestFloor(int floor);                         // inside: passenger presses a floor button
    void floorCall(int floor, Direction wantedDir);        // outside: passenger presses UP or DOWN
    void clearFloor(int floor);                           // remove internal floor once served
    void displayDestinations() const;                     // show all pending requests

    // Emergency
    void activateEmergency(int destinationFloor);
    void deactivateEmergency();

    // Status
    bool isAvailable() const;
    void setStatus(LiftStatus s);

    // Display
    void displayStatus() const;
};