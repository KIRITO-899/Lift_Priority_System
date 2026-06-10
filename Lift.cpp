#include "Lift.h"

// Constructor
//   void functions  — always succeed, no return needed
//   bool functions  — can fail, caller reacts to the result

Lift::Lift(int id, int startFloor, int totalSpace)
    : lift_id(id),
      currentFloor(startFloor),
      totalSpace(totalSpace),
      usedSpace(0),
      currentPassengers(0),
      status(LiftStatus::Available),
      direction(Direction::Idle),
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
    currentFloor++;
    direction = Direction::Up;
    status    = LiftStatus::Busy;
    cout << "[Lift " << lift_id << " ] Moving Up -> Floor " << currentFloor << "\n";
}

void Lift::moveDown() {
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

    // Emergency mode: go straight to destination, no stops in between
    if (isEmergency) {
        cout << "[Lift " << lift_id << " ] !! EMERGENCY -- non-stop to floor "
             << targetFloor << " (skipping all intermediate stops)\n";
    } else {
        cout << "[Lift " << lift_id << " ] Heading from floor " << currentFloor
             << " to floor " << targetFloor << " ...\n";
    }

    while (currentFloor != targetFloor) {
        if (currentFloor < targetFloor)
            moveUp();
        else
            moveDown();
    }

    direction = Direction::Idle;
    cout << "[Lift " << lift_id << " ] Arrived at floor " << currentFloor << "\n";
}


// ── Doors ─────────────────────────────────────────────

void Lift::openDoors() {
    cout << "[Lift " << lift_id << " ] Doors OPENING at floor " << currentFloor << "\n";
}

void Lift::closeDoors() {
    cout << "[Lift " << lift_id << " ] Doors CLOSING at floor " << currentFloor << "\n";
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

void Lift::servePassengers(int source, int destination, PersonType type)
{
    moveTo(source);
    openDoors();
    if (boardPassenger(type))
    {
        closeDoors();
        moveTo(destination);
        openDoors();
        dropPassenger(type);
        closeDoors();
    }

    // Restore status to Available after the trip ends
    status    = LiftStatus::Available;
    direction = Direction::Idle;
    cout << "[Lift " << lift_id << " ] Trip complete -- now Available.\n";

    if (isEmergency) {
        deactivateEmergency();
    }
}

// ── Emergency ─────────────────────────────────────────
// activateEmergency: locks the lift on a non-stop route to destinationFloor.
// deactivateEmergency: clears emergency mode after arrival.

void Lift::activateEmergency(int destinationFloor) {
    isEmergency           = true;
    emergencyDestination  = destinationFloor;
    status                = LiftStatus::Busy;
    cout << "[Lift " << lift_id << " ] !! EMERGENCY MODE ACTIVATED -- destination: floor "
         << destinationFloor << "\n";
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
         << "|  Floor       : " << currentFloor      << "\n"
         << "|  Status      : " << statusStr         << "\n"
         << "|  Direction   : " << dirStr            << "\n"
         << "|  Passengers  : " << currentPassengers << "\n"
         << "|  Space Used  : " << usedSpace << "/" << totalSpace << "\n"
         << "|  Emergency   : " << (isEmergency ? "YES (!!)" : "No") << "\n"
         << "+-----------------------------+\n";
}

