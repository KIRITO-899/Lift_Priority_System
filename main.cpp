#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Person.h"
#include "Lift.h"

using namespace std;

// ── Global definitions (extern-declared in Person.h) ──────────────────────
priority_queue<int> pq_1;
vector<Person>      people;

// ── BUG 4 fix: global lift state — preserved across ALL assign_Lift calls ──
// (was a local array inside assign_Lift, so state reset on every new person)
Lift        lifts[4]       = { Lift(1), Lift(2), Lift(3), Lift(4) };
vector<int> availableLifts = { 1, 2, 3, 4 };
int         emLift         = -1;   // -1 means no emergency lift locked

// ── BUG 7 fix: bridge Person.Priority (int) to Lift's PersonType enum ─────
inline PersonType toPersonType(int priority) {
    if (priority == EMERGENCY) return PersonType::EmergencyPatient;
    if (priority == DOCTOR)    return PersonType::Doctor;
    if (priority == STAFF)     return PersonType::Staff;
    return PersonType::Normal;
}

// Returns the lifts[] array index for a given lift_id (-1 if not found)
int liftIndex(int id) {
    for (int i = 0; i < 4; i++)
        if (lifts[i].lift_id == id) return i;
    return -1;
}

// BUG 5 fix: Person passed by reference so caller sees any mutations.
void assign_Lift(Person& p1) {
    while (true) {
        p1.input();           // read id, currentFloor, DestFloor → sets Priority
        people.push_back(p1);

        int lift_fl = -1;
        int curr    = 9999;

        // ── EMERGENCY ────────────────────────────────────────────────────────
        if (p1.Priority == EMERGENCY) {
            // Find the closest lift from all 4
            for (int i = 0; i < 4; i++) {
                int diff = abs(p1.currentFloor - lifts[i].currentFloor);
                if (diff < curr) { curr = diff; lift_fl = lifts[i].lift_id; }
            }
            emLift = lift_fl;

            // Remove from regular pool so normal/staff passengers can't take it
            for (int i = 0; i < (int)availableLifts.size(); i++) {
                if (availableLifts[i] == emLift) {
                    availableLifts.erase(availableLifts.begin() + i);
                    break;
                }
            }

            int idx = liftIndex(emLift);
            if (idx != -1) {
                lifts[idx].activateEmergency(p1.DestFloor);
                lifts[idx].servePassengers(   // BUG 7 fix: actually invoke lift mechanics
                    p1.currentFloor, p1.DestFloor, toPersonType(p1.Priority));
            }
            cout << "Emergency mode -- assigned Lift " << emLift << "\n";

        // ── DOCTOR / STAFF ───────────────────────────────────────────────────
        } else if (p1.Priority == DOCTOR || p1.Priority == STAFF) {
            if (emLift == -1) {
                // No emergency active — pick closest from available pool
                for (int id : availableLifts) {
                    int idx = liftIndex(id);
                    if (idx != -1) {
                        int diff = abs(p1.currentFloor - lifts[idx].currentFloor);
                        if (diff < curr) { curr = diff; lift_fl = id; }
                    }
                }
                int idx = liftIndex(lift_fl);
                if (idx != -1)
                    lifts[idx].servePassengers(
                        p1.currentFloor, p1.DestFloor, toPersonType(p1.Priority));
                cout << "Take Lift " << lift_fl << "\n";
            } else {
                // Emergency lift is shared with doctors/staff
                int idx = liftIndex(emLift);
                if (idx != -1)
                    lifts[idx].servePassengers(
                        p1.currentFloor, p1.DestFloor, toPersonType(p1.Priority));
                cout << "Take Emergency Lift (Lift " << emLift << ")\n";
            }

        // ── NORMAL / VISITOR ─────────────────────────────────────────────────
        } else {
            // Use available pool only — never the locked emergency lift
            for (int id : availableLifts) {
                int idx = liftIndex(id);
                if (idx != -1) {
                    int diff = abs(p1.currentFloor - lifts[idx].currentFloor);
                    if (diff < curr) { curr = diff; lift_fl = id; }
                }
            }
            cout << "Available Lifts: ";
            for (int lift : availableLifts) cout << lift << " ";
            cout << "\n";

            int idx = liftIndex(lift_fl);
            if (idx != -1)
                lifts[idx].servePassengers(
                    p1.currentFloor, p1.DestFloor, toPersonType(p1.Priority));
            cout << "Take Lift " << lift_fl << "\n";
        }

        cout << "Still Emergency?? (1/0) ";
        int em;
        cin >> em;
        if (em == 0) break;
    }
}

int main() {
    // BUG M4 fix: srand BEFORE any rand() call
    // (previously lifts were initialised with rand()%10 before srand in main)
    srand(time(0));

    // Set random starting floors now that the seed is valid
    for (int i = 0; i < 4; i++)
        lifts[i].currentFloor = rand() % (MAX_FLOOR + 1);

    cout << "=== Lift Priority System ===\n";
    cout << "IDs: Emergency=" << EMERGENCY_ID
         << "  Doctor=" << DOCTOR_ID
         << "  Staff=" << STAFF_ID
         << "  CheckUp=" << NORMAL_CHECK_UP_ID
         << "  Visitor=any other\n\n";

    while (true) {
        Person p;
        assign_Lift(p);

        cout << "\nNew Person?? (1/0): ";
        int c;
        cin >> c;
        if (c == 0) break;
    }

    return 0;
}
