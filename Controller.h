#pragma once
#include "Person.h"
#include "Lift.h"
#include <vector>
#include <queue>
#include <cmath>
#include <cstdlib>

using namespace std;

// ── Controller ────────────────────────────────────────
// Responsibilities (Member 1 — Jyothiradithya):
//   - Store 4 persistent Lift objects
//   - Accept passenger input
//   - Find nearest available lift
//   - Emergency detection and reservation
//   - Doctor/Staff priority routing
//   - Normal passenger allocation
//   - Priority queue for incoming requests

class Controller {
public:
    priority_queue<int> pq_1;
    vector<Person>      people;

    // 4 lifts with fixed starting floors (A=6, B=2, C=0, D=3)
    Lift lifts[4] = {
        Lift(1, 6),
        Lift(2, 2),
        Lift(3, 0),
        Lift(4, 3)
    };

    int emLift = -1;          // ID of the reserved emergency lift (-1 = none)
    vector<int> lv = {1, 2, 3, 4};  // IDs of lifts not reserved for emergency

    // ── Helper: map Person::Priority int → PersonType enum ──
    static PersonType toPersonType(int priority) {
        if (priority == EMERGENCY)    return PersonType::EmergencyPatient;
        if (priority == DOCTOR)       return PersonType::Doctor;
        if (priority == STAFF)        return PersonType::Staff;
        return PersonType::Normal;
    }

    // ── Helper: find index in lifts[] for a given lift ID ──
    int indexOfLift(int liftId) const {
        for (int i = 0; i < 4; i++)
            if (lifts[i].lift_id == liftId) return i;
        return -1;
    }

    // ── Helper: find the nearest available lift from a set of IDs ──
    // Returns the lift ID of the nearest one, or -1 if none available.
    int findNearest(int fromFloor, const vector<int>& ids) {
        int best = -1, bestDiff = 9999;
        for (int id : ids) {
            int i = indexOfLift(id);
            if (i < 0) continue;
            if (!lifts[i].isAvailable()) continue;
            int diff = abs(fromFloor - lifts[i].currentFloor);
            if (diff < bestDiff) {
                bestDiff = diff;
                best     = id;
            }
        }
        return best;
    }

    // ── Main entry point: handle one passenger request ──
    void assign_Lift(Person& p1) {
        p1.input();
        people.push_back(p1);
        pq_1.push(p1.Priority);

        PersonType ptype = toPersonType(p1.Priority);
        int lift_fl = -1;

        // ── EMERGENCY ──────────────────────────────────────
        if (p1.Priority == EMERGENCY) {
            lift_fl = findNearest(p1.currntFloor, lv);

            if (lift_fl == -1) {
                cout << "[Controller] No available lift for emergency!\n";
                return;
            }

            // Reserve this lift for emergency
            emLift = lift_fl;
            for (int i = 0; i < (int)lv.size(); i++) {
                if (lv[i] == emLift) { lv.erase(lv.begin() + i); break; }
            }

            // Activate emergency mode on the Lift object
            int idx = indexOfLift(lift_fl);
            lifts[idx].activateEmergency(p1.DestFloor);

            cout << "[Controller] Emergency mode assigned for lift: " << emLift << "\n";
            cout << "[Controller] Take Lift " << lift_fl << "\n";

            // Execute the trip
            lifts[idx].servePassengers(p1.currntFloor, p1.DestFloor, ptype);
        }

        // ── DOCTOR or STAFF ───────────────────────────────
        else if (p1.Priority == DOCTOR || p1.Priority == STAFF) {
            if (emLift == -1) {
                // No emergency active — assign nearest available lift
                lift_fl = findNearest(p1.currntFloor, lv);
                if (lift_fl == -1) {
                    cout << "[Controller] No available lift for Doctor/Staff!\n";
                    return;
                }
                cout << "[Controller] Take Lift " << lift_fl << "\n";
            } else {
                // Emergency active — direct to the reserved emergency lift
                lift_fl = emLift;
                cout << "[Controller] Take Emergency Lift!!!\n";
                cout << "[Controller] Take Lift " << lift_fl << "\n";
            }
            int idx = indexOfLift(lift_fl);
            lifts[idx].servePassengers(p1.currntFloor, p1.DestFloor, ptype);
        }

        // ── NORMAL / CHECK-UP ─────────────────────────────
        else {
            lift_fl = findNearest(p1.currntFloor, lv);
            if (lift_fl == -1) {
                cout << "[Controller] No available lift for passenger!\n";
                return;
            }
            cout << "[Controller] Available Lifts: ";
            for (int id : lv) cout << id << " ";
            cout << "\n";
            cout << "[Controller] Take Lift " << lift_fl << "\n";

            int idx = indexOfLift(lift_fl);
            lifts[idx].servePassengers(p1.currntFloor, p1.DestFloor, ptype);
        }
    }

    // ── Display status of all lifts ──
    void displayAllLifts() const {
        cout << "\n========= LIFT STATUS =========\n";
        for (int i = 0; i < 4; i++) lifts[i].displayStatus();
        cout << "================================\n";
    }
};
