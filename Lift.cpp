#include "Lift.h"

using namespace std;   // added here since 'using namespace std' was removed from the header


// Constructor
//   void functions  — always succeed, no return needed
//   bool functions  — can fail, caller reacts to the result

Lift::Lift(int id, int startFloor, int totalSpace, int doorHoldSecs)
    : lift_id(id),
      currentFloor(startFloor),
      totalSpace(totalSpace),
      usedSpace(0),
      currentPassengers(0),
      status(LiftStatus::Available),
      direction(Direction::Idle),
      doorsOpen(false),
      doorHoldSeconds(doorHoldSecs),
      isEmergency(false),
      emergencyDestination(-1)
    {}

int Lift::spaceOf(PersonType type) {
    switch (type) {
        case PersonType::EmergencyPatient: return 50;
        case PersonType::Doctor:           return 30;
        case PersonType::Staff:            return 20;
        case PersonType::Normal:
        default:                           return 10;
    }
}


// Movement

// BUG 1 fix: added upper-bound guard — lift cannot go above MAX_FLOOR
void Lift::moveUp() {
    if (currentFloor >= MAX_FLOOR) {
        cout << "[Lift " << lift_id << " ] Already at top floor (" << MAX_FLOOR << "). Cannot go higher.\n";
        return;
    }
    currentFloor++;
    direction = Direction::Up;
    status    = LiftStatus::Busy;
    cout << "[Lift " << lift_id << " ] Moving Up -> Floor " << currentFloor << "\n";
}

// BUG 1 fix: added lower-bound guard — lift cannot go below floor 0
void Lift::moveDown() {
    if (currentFloor <= 0) {
        cout << "[Lift " << lift_id << " ] Already at ground floor (0). Cannot go lower.\n";
        return;
    }
    currentFloor--;
    direction = Direction::Down;
    status    = LiftStatus::Busy;
    cout << "[Lift " << lift_id << " ] Moving Down -> Floor " << currentFloor << "\n";
}

void Lift::moveTo(int targetFloor) {
    if (targetFloor == currentFloor) {
        cout << "[Lift " << lift_id << " ] Already at floor " << targetFloor << "\n";
        return;
    }

    Direction travelDir = (targetFloor > currentFloor) ? Direction::Up : Direction::Down;

    // Emergency: go non-stop, skip any middle floor
    if (isEmergency) {
        cout << "[Lift " << lift_id << " ] !! EMERGENCY -- non-stop to floor "
             << targetFloor << " (skipping all intermediate stops)\n";
        while (currentFloor != targetFloor) {
            if (currentFloor < targetFloor) { moveUp();   }
            else                            { moveDown(); }
        }
        direction = Direction::Idle;
        cout << "[Lift " << lift_id << " ] Arrived at floor " << currentFloor << "\n";
        return;
    }

    // Sort internal destinations to travel in the correct order
    if (travelDir == Direction::Up) {
        sort(destinations.begin(), destinations.end());
    } else {
        sort(destinations.begin(), destinations.end(), greater<int>());
    }

    cout << "[Lift " << lift_id << " ] Heading from floor " << currentFloor
         << " to floor " << targetFloor << " ...\n";

    while (currentFloor != targetFloor) {
        if (currentFloor < targetFloor) { moveUp();   }
        else                            { moveDown(); }

        if (currentFloor == targetFloor) { break; } // final stop handled by caller

        // Check internal destination — passenger inside wants off here
        auto dit = find(destinations.begin(), destinations.end(), currentFloor);
        bool internalStop = (dit != destinations.end());
        if (internalStop) {
            destinations.erase(dit);
        }

        // Check floor call — someone outside pressed UP/DOWN on this floor
        // Only stop if: direction matches AND there is room to board
        bool floorStop = false;
        if (usedSpace < totalSpace) {
            for (auto it = floorCalls.begin(); it != floorCalls.end(); ++it) {
                if (it->floor == currentFloor && it->wantedDir == travelDir) {
                    floorStop = true;
                    floorCalls.erase(it);
                    break;
                }
            }
        }

        if (internalStop || floorStop) {
            openDoors();
            if (internalStop) {
                cout << "[Lift " << lift_id << " ] Passenger alighting at floor "
                     << currentFloor << "\n";
            }
            if (floorStop) {
                cout << "[Lift " << lift_id << " ] Picking up passenger at floor "
                     << currentFloor << " (going "
                     << (travelDir == Direction::Up ? "Up" : "Down") << ")\n";
            }
            closeDoors();
        }
    }

    direction = Direction::Idle;
    cout << "[Lift " << lift_id << " ] Arrived at floor " << currentFloor << "\n";
}


// ── Doors ─────────────────────────────────────────────

void Lift::openDoors() {
    doorsOpen = true;
    cout << "[Lift " << lift_id << " ] Doors OPENING at floor " << currentFloor << "\n";
}

void Lift::closeDoors() {
    doorsOpen = false;
    cout << "[Lift " << lift_id << " ] Doors CLOSING at floor " << currentFloor << "\n";
}

// BUG 2 fix: removed always-false `if (!doorsOpen)` dead code branch.
// openDoors() sets doorsOpen=true so the old check could never trigger.
void Lift::holdDoorOpen() {
    openDoors();
    cout << "[Lift " << lift_id << " ] Doors open for "
         << doorHoldSeconds << " second(s).\n";
    // In a real system a timer/thread would wait here.
    // For simulation, close immediately after boarding/alighting.
    closeDoors();
}

// closeDoorManually: simulates pressing the physical Close button inside the lift.
void Lift::closeDoorManually() {
    if (!doorsOpen) {
        cout << "[Lift " << lift_id << " ] Doors are already closed.\n";
        return;
    }
    doorsOpen = false;
    cout << "[Lift " << lift_id << " ] [CLOSE BUTTON] Door-close requested by passenger.\n";
}


// ── Passengers ────────────────────────────────────────
// boardPassenger: returns false if not enough space remaining.
// Space used depends on PersonType

bool Lift::boardPassenger(PersonType type) {
    int needed = spaceOf(type);
    if (usedSpace + needed > totalSpace) {
        cout << "[Lift " << lift_id << " ] Not enough space for this passenger "
             << "(used " << usedSpace << "/" << totalSpace
             << ", needs " << needed << " units)\n";
        return false;
    }
    usedSpace += needed;
    currentPassengers++;
    cout << "[Lift " << lift_id << " ] Passenger boarded "
         << "(space: " << usedSpace << "/" << totalSpace << ")\n";
    return true;
}

// dropPassenger: returns false if nobody is inside.
// Caller passes the type so the correct space is freed.

bool Lift::dropPassenger(PersonType type) {
    if (currentPassengers <= 0) {
        cout << "[Lift " << lift_id << " ] No passengers to drop off.\n";
        return false;
    }
    usedSpace -= spaceOf(type);
    if (usedSpace < 0) usedSpace = 0; // safety clamp
    currentPassengers--;
    cout << "[Lift " << lift_id << " ] Passenger dropped at floor " << currentFloor
         << " (space: " << usedSpace << "/" << totalSpace << ")\n";
    return true;
}

// BUG 3 fix: status is now only set to Available and emergency deactivated
// when boarding actually succeeds. If boarding fails (lift full) we still
// return to idle but print an appropriate message instead of "Trip complete".
void Lift::servePassengers(int source, int destination, PersonType type) {
    moveTo(source);
    holdDoorOpen();   // open doors for boarding

    if (boardPassenger(type)) {
        moveTo(destination);
        holdDoorOpen();   // open doors for alighting
        dropPassenger(type);

        status    = LiftStatus::Available;
        direction = Direction::Idle;
        cout << "[Lift " << lift_id << " ] Trip complete -- now Available.\n";

        if (isEmergency) {
            deactivateEmergency();
        }
    } else {
        // Boarding failed (lift full) — abort trip, return to idle
        status    = LiftStatus::Available;
        direction = Direction::Idle;
        cout << "[Lift " << lift_id << " ] Boarding failed -- lift returning to idle.\n";
    }
}

// ── Floor selection ───────────────────────────────────

void Lift::requestFloor(int floor) {
    if (floor == currentFloor) {
        cout << "[Lift " << lift_id << " ] Already at floor " << floor << " -- no need to queue.\n";
        return;
    }
    if (find(destinations.begin(), destinations.end(), floor) == destinations.end()) {
        destinations.push_back(floor);
        cout << "[Lift " << lift_id << " ] Floor " << floor << " requested.\n";
    } else {
        cout << "[Lift " << lift_id << " ] Floor " << floor << " already queued.\n";
    }
}

void Lift::floorCall(int floor, Direction wantedDir) {
    for (auto& hc : floorCalls)
        if (hc.floor == floor && hc.wantedDir == wantedDir) {
            cout << "[Lift " << lift_id << " ] floor call at floor " << floor << " already registered.\n";
            return;
        }
    floorCalls.push_back({floor, wantedDir});
    cout << "[Lift " << lift_id << " ] floor call: floor " << floor
         << " going " << (wantedDir == Direction::Up ? "Up" : "Down") << " registered.\n";
}

void Lift::clearFloor(int floor) {
    auto it = find(destinations.begin(), destinations.end(), floor);
    if (it != destinations.end()) {
        destinations.erase(it);
        cout << "[Lift " << lift_id << " ] Floor " << floor << " served and cleared.\n";
    }
}

void Lift::displayDestinations() const {
    if (destinations.empty() && floorCalls.empty()) {
        cout << "[Lift " << lift_id << " ] No pending floor requests.\n";
        return;
    }
    if (!destinations.empty()) {
        cout << "[Lift " << lift_id << " ] Internal stops : ";
        for (int f : destinations) cout << f << " ";
        cout << "\n";
    }
    if (!floorCalls.empty()) {
        cout << "[Lift " << lift_id << " ] floor calls     : ";
        for (auto& hc : floorCalls)
            cout << hc.floor << "(" << (hc.wantedDir == Direction::Up ? "Up" : "Down") << ") ";
        cout << "\n";
    }
}


// ── Emergency ─────────────────────────────────────────
// activateEmergency: locks the lift on a non-stop route to destinationFloor.
// deactivateEmergency: clears emergency mode after arrival.

void Lift::activateEmergency(int destinationFloor) {
    isEmergency          = true;
    emergencyDestination = destinationFloor;
    status               = LiftStatus::Busy;
    bool hadRequests = !destinations.empty() || !floorCalls.empty();
    destinations.clear();
    floorCalls.clear();
    cout << "[Lift " << lift_id << " ] !! EMERGENCY MODE ACTIVATED -- destination: floor "
         << destinationFloor << "\n";
    if (hadRequests)
        cout << "[Lift " << lift_id << " ] All pending requests cleared for emergency.\n";
}

void Lift::deactivateEmergency() {
    isEmergency          = false;
    emergencyDestination = -1;
    cout << "[Lift " << lift_id << " ] Emergency mode cleared.\n";
}


// ── Status helpers ────────────────────────────────────

bool Lift::isAvailable() const {
    return status == LiftStatus::Available;
}

void Lift::setStatus(LiftStatus s) {
    status = s;
}


// ── Display ───────────────────────────────────────────

void Lift::displayStatus() const {
    string statusStr, dirStr;

    switch (status) {
        case LiftStatus::Available: statusStr = "Available"; break;
        case LiftStatus::Busy:      statusStr = "Busy";      break;
    }

    switch (direction) {
        case Direction::Up:   dirStr = "Up";   break;
        case Direction::Down: dirStr = "Down"; break;
        case Direction::Idle: dirStr = "Idle"; break;
    }

    cout << "+-----------------------------+\n"
         << "|  Lift ID     : " << lift_id                << "\n"
         << "|  Floor       : " << currentFloor           << "\n"
         << "|  Status      : " << statusStr              << "\n"
         << "|  Direction   : " << dirStr                 << "\n"
         << "|  Passengers  : " << currentPassengers      << "\n"
         << "|  Space Used  : " << usedSpace << "/" << totalSpace << "\n"
         << "|  Emergency   : " << (isEmergency ? "YES (!!)" : "No") << "\n"
         << "+-----------------------------+\n";
}
