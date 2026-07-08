#include "Lift.h"

using namespace std;

// Constructor
Lift::Lift(int id, int startFloor, int totalSpace, int doorHoldSecs)
    : LiftID(id),
      CurrentFloor(startFloor),
      TotalSpace(totalSpace),
      UsedSpace(0),
      CurrentPassengers(0),
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

void Lift::moveUp() {
    if (CurrentFloor >= MAX_FLOOR) {
        cout << "[Lift " << LiftID << " ] Already at top floor (" << MAX_FLOOR << "). Cannot go higher.\n";
        return;
    }
    CurrentFloor++;
    direction = Direction::Up;
    status    = LiftStatus::Busy;
    cout << "[Lift " << LiftID << " ] Moving Up -> Floor " << CurrentFloor << "\n";
}

void Lift::moveDown() {
    if (CurrentFloor <= 0) {
        cout << "[Lift " << LiftID << " ] Already at ground floor (0). Cannot go lower.\n";
        return;
    }
    CurrentFloor--;
    direction = Direction::Down;
    status    = LiftStatus::Busy;
    cout << "[Lift " << LiftID << " ] Moving Down -> Floor " << CurrentFloor << "\n";
}

void Lift::moveTo(int targetFloor) {
    if (targetFloor == CurrentFloor) {
        cout << "[Lift " << LiftID << " ] Already at floor " << targetFloor << "\n";
        return;
    }

    Direction travelDir = (targetFloor > CurrentFloor) ? Direction::Up : Direction::Down;

    // Emergency: go non-stop, skip any middle floor
    if (isEmergency) {
        cout << "[Lift " << LiftID << " ] !! EMERGENCY -- non-stop to floor "
             << targetFloor << " (skipping all intermediate stops)\n";
        while (CurrentFloor != targetFloor) {
            if (CurrentFloor < targetFloor) { moveUp();   }
            else                            { moveDown(); }
        }
        direction = Direction::Idle;
        cout << "[Lift " << LiftID << " ] Arrived at floor " << CurrentFloor << "\n";
        return;
    }

    // Sort internal destinations to travel in the correct order
    if (travelDir == Direction::Up) {
        sort(destinations.begin(), destinations.end());
    } else {
        sort(destinations.begin(), destinations.end(), greater<int>());
    }

    cout << "[Lift " << LiftID << " ] Heading from floor " << CurrentFloor
         << " to floor " << targetFloor << " ...\n";

    while (CurrentFloor != targetFloor) {
        if (CurrentFloor < targetFloor) { moveUp();   }
        else                            { moveDown(); }

        if (CurrentFloor == targetFloor) { break; }

        // Check internal destination
        auto dit = find(destinations.begin(), destinations.end(), CurrentFloor);
        bool internalStop = (dit != destinations.end());
        if (internalStop) {
            destinations.erase(dit);
        }

        // Check floor call
        bool floorStop = false;
        if (UsedSpace < TotalSpace) {
            for (auto it = floorCalls.begin(); it != floorCalls.end(); ++it) {
                if (it->floor == CurrentFloor && it->wantedDir == travelDir) {
                    floorStop = true;
                    floorCalls.erase(it);
                    break;
                }
            }
        }

        if (internalStop || floorStop) {
            openDoors();
            if (internalStop) {
                cout << "[Lift " << LiftID << " ] Passenger alighting at floor "
                     << CurrentFloor << "\n";
            }
            if (floorStop) {
                cout << "[Lift " << LiftID << " ] Picking up passenger at floor "
                     << CurrentFloor << " (going "
                     << (travelDir == Direction::Up ? "Up" : "Down") << ")\n";
            }
            closeDoors();
        }
    }

    direction = Direction::Idle;
    cout << "[Lift " << LiftID << " ] Arrived at floor " << CurrentFloor << "\n";
}


// Doors

void Lift::openDoors() {
    doorsOpen = true;
    cout << "[Lift " << LiftID << " ] Doors OPENING at floor " << CurrentFloor << "\n";
}

void Lift::closeDoors() {
    doorsOpen = false;
    cout << "[Lift " << LiftID << " ] Doors CLOSING at floor " << CurrentFloor << "\n";
}

void Lift::holdDoorOpen() {
    openDoors();
    cout << "[Lift " << LiftID << " ] Doors open for "
         << doorHoldSeconds << " second(s).\n";
    closeDoors();
}

void Lift::closeDoorManually() {
    if (!doorsOpen) {
        cout << "[Lift " << LiftID << " ] Doors are already closed.\n";
        return;
    }
    doorsOpen = false;
    cout << "[Lift " << LiftID << " ] [CLOSE BUTTON] Door-close requested by passenger.\n";
}


// Passengers

bool Lift::boardPassenger(PersonType type) {
    int needed = spaceOf(type);
    if (UsedSpace + needed > TotalSpace) {
        cout << "[Lift " << LiftID << " ] Not enough space for this passenger "
             << "(used " << UsedSpace << "/" << TotalSpace
             << ", needs " << needed << " units)\n";
        return false;
    }
    UsedSpace += needed;
    CurrentPassengers++;
    cout << "[Lift " << LiftID << " ] Passenger boarded "
         << "(space: " << UsedSpace << "/" << TotalSpace << ")\n";
    return true;
}

bool Lift::dropPassenger(PersonType type) {
    if (CurrentPassengers <= 0) {
        cout << "[Lift " << LiftID << " ] No passengers to drop off.\n";
        return false;
    }
    UsedSpace -= spaceOf(type);
    if (UsedSpace < 0) UsedSpace = 0;
    CurrentPassengers--;
    cout << "[Lift " << LiftID << " ] Passenger dropped at floor " << CurrentFloor
         << " (space: " << UsedSpace << "/" << TotalSpace << ")\n";
    return true;
}

void Lift::servePassengers(int source, int destination, PersonType type) {
    moveTo(source);
    holdDoorOpen();

    if (boardPassenger(type)) {
        moveTo(destination);
        holdDoorOpen();
        dropPassenger(type);

        status    = LiftStatus::Available;
        direction = Direction::Idle;
        cout << "[Lift " << LiftID << " ] Trip complete -- now Available.\n";

        if (isEmergency) {
            deactivateEmergency();
        }
    } else {
        status    = LiftStatus::Available;
        direction = Direction::Idle;
        cout << "[Lift " << LiftID << " ] Boarding failed -- lift returning to idle.\n";
    }
}

// Floor selection

void Lift::requestFloor(int floor) {
    if (floor == CurrentFloor) {
        cout << "[Lift " << LiftID << " ] Already at floor " << floor << " -- no need to queue.\n";
        return;
    }
    if (find(destinations.begin(), destinations.end(), floor) == destinations.end()) {
        destinations.push_back(floor);
        cout << "[Lift " << LiftID << " ] Floor " << floor << " requested.\n";
    } else {
        cout << "[Lift " << LiftID << " ] Floor " << floor << " already queued.\n";
    }
}

void Lift::floorCall(int floor, Direction wantedDir) {
    for (auto& hc : floorCalls)
        if (hc.floor == floor && hc.wantedDir == wantedDir) {
            cout << "[Lift " << LiftID << " ] floor call at floor " << floor << " already registered.\n";
            return;
        }
    floorCalls.push_back({floor, wantedDir});
    cout << "[Lift " << LiftID << " ] floor call: floor " << floor
         << " going " << (wantedDir == Direction::Up ? "Up" : "Down") << " registered.\n";
}

void Lift::clearFloor(int floor) {
    auto it = find(destinations.begin(), destinations.end(), floor);
    if (it != destinations.end()) {
        destinations.erase(it);
        cout << "[Lift " << LiftID << " ] Floor " << floor << " served and cleared.\n";
    }
}

void Lift::displayDestinations() const {
    if (destinations.empty() && floorCalls.empty()) {
        cout << "[Lift " << LiftID << " ] No pending floor requests.\n";
        return;
    }
    if (!destinations.empty()) {
        cout << "[Lift " << LiftID << " ] Internal stops : ";
        for (int f : destinations) cout << f << " ";
        cout << "\n";
    }
    if (!floorCalls.empty()) {
        cout << "[Lift " << LiftID << " ] floor calls     : ";
        for (auto& hc : floorCalls)
            cout << hc.floor << "(" << (hc.wantedDir == Direction::Up ? "Up" : "Down") << ") ";
        cout << "\n";
    }
}


// Emergency

void Lift::activateEmergency(int destinationFloor) {
    isEmergency          = true;
    emergencyDestination = destinationFloor;
    status               = LiftStatus::Busy;
    bool hadRequests = !destinations.empty() || !floorCalls.empty();
    destinations.clear();
    floorCalls.clear();
    cout << "[Lift " << LiftID << " ] !! EMERGENCY MODE ACTIVATED -- destination: floor "
         << destinationFloor << "\n";
    if (hadRequests)
        cout << "[Lift " << LiftID << " ] All pending requests cleared for emergency.\n";
}

void Lift::deactivateEmergency() {
    isEmergency          = false;
    emergencyDestination = -1;
    cout << "[Lift " << LiftID << " ] Emergency mode cleared.\n";
}


// Status helpers

bool Lift::isAvailable() const {
    return status == LiftStatus::Available;
}

void Lift::setStatus(LiftStatus s) {
    status = s;
}


// Display

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
         << "|  Lift ID     : " << LiftID              << "\n"
         << "|  Floor       : " << CurrentFloor         << "\n"
         << "|  Status      : " << statusStr             << "\n"
         << "|  Direction   : " << dirStr                << "\n"
         << "|  Passengers  : " << CurrentPassengers     << "\n"
         << "|  Space Used  : " << UsedSpace << "/" << TotalSpace << "\n"
         << "|  Emergency   : " << (isEmergency ? "YES (!!)": "No") << "\n"
         << "+-----------------------------+\n";
}