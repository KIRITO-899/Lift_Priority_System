# What Needs to Be Done Next

## Current Build Status
✅ **Compiles clean** — zero warnings, zero errors (`g++ -std=c++17 -Wall -Wextra`)

---

## Project Plan vs. Current Code — Full Audit

### 10 Features from the Plan

| # | Feature | Status | Notes |
|---|---|---|---|
| 1 | Multiple Lift Management | ✅ Done | 4 persistent lifts in Controller |
| 2 | Nearest Lift Allocation | ✅ Done | `findNearest()` in Controller |
| 3 | Emergency Priority Scheduling | ✅ Done | `activateEmergency()` + reservation |
| 4 | Doctor and Staff Priority | ✅ Done | Routes to emLift or nearest |
| 5 | Lift Availability Tracking | ✅ Done | `isAvailable()` used in `findNearest()` |
| 6 | Passenger Capacity Management | ✅ Done | `boardPassenger()` checks space units |
| 7 | Lift Movement Simulation | ✅ Done | `moveTo()` + floor-by-floor steps |
| 8 | Door Operations | ✅ Done | `openDoors()`, `closeDoors()`, `holdDoorOpen()` |
| 9 | Emergency Lift Reservation | ✅ Done | `emLift` ID, `lv` excludes it |
| 10 | Passenger Request Handling | ✅ Done | `assign_Lift()` in Controller |

---

### 3 Modules

#### Person Module
| Item | Status | Issue |
|---|---|---|
| Current floor, Dest floor | ✅ | — |
| Passenger type (4 types) | ✅ | Uses ID numbers — unintuitive |
| Priority assignment | ✅ | — |
| Input validation | ❌ | No check: negative floors, floor > MAX_FLOOR (10), invalid ID |
| `pq_1` extern linkage | ⚠️ | Declared `extern` in Person.h, defined globally in main.cpp — works but messy |

#### Controller Module
| Item | Status | Issue |
|---|---|---|
| `assign_Lift()` | ✅ | — |
| `findNearest()` | ✅ | — |
| Emergency reservation | ✅ | — |
| Emergency **release** | ❌ | `emLift` is never reset to `-1` after emergency ends; `lv` never gets the lift ID back |
| Priority queue actually scheduling | ❌ | `pq_1` is pushed to but never used to order/serve requests |
| `displayAllLifts()` | ✅ | — |

#### Lift Module
| Item | Status | Issue |
|---|---|---|
| Movement (up/down/moveTo) | ✅ | — |
| Door operations | ✅ | — |
| Board/Drop passengers | ✅ | — |
| `servePassengers()` | ✅ | — |
| Emergency mode | ✅ | — |
| `displayStatus()` | ✅ | — |
| Floor selection (`requestFloor`, `floorCall`) | ✅ | Implemented but never called from Controller |

---

## Prioritized TODO List

### 🔴 Critical (breaks correctness)

1. **Emergency lift is never released back** after the trip ends
   - After `servePassengers()` completes, `emLift` stays set and `lv` never gets that lift ID back
   - Fix: after `servePassengers()` for emergency, call `releaseEmergency()` on Controller which resets `emLift = -1` and pushes the ID back to `lv`

2. **Input validation in `Person::input()`** — user can enter floor `-5` or floor `99` and the system accepts it
   - Fix: add a loop checking `0 <= currntFloor <= MAX_FLOOR` and `0 <= DestFloor <= MAX_FLOOR`

3. **Passenger type entry is obscure** — user must know IDs 213/214/215 to get the right type
   - Fix: show a menu ("1=Emergency, 2=Doctor, 3=Staff, 4=Normal Visitor") instead of a raw ID

### 🟡 Important (missing from plan spec)

4. **Priority queue not driving scheduling** — `pq_1` is filled but never dequeued or used to sort/prioritize which request is served first when multiple arrive

5. **`lv` (available lifts list) shown to Normal passengers is stale** — it lists IDs 1-4 minus the emergency lift, but doesn't check if a lift is actually `Available`. A busy lift could appear in the list.

### 🟢 Polish / Nice-to-have

6. **`displayAllLifts()` called after every request** — currently only called in `main.cpp`; consider calling it inside `assign_Lift()` for automatic status update

7. **Passenger history `people` vector** — it's stored but never displayed; add a `displayPassengerHistory()` method to Controller

8. **`README.md`** — currently minimal; update to describe how to compile, run, and what IDs to enter

---

## Suggested Work Split

**HY Yeshwanth (your side — Lift module is done)**
- [ ] Fix emergency lift release (add `releaseEmergency()` logic to Controller — it touches `lv` and `emLift`)
- [ ] Fix stale `lv` list shown to normal passengers

**Jyothiradithya (Controller/Person side)**
- [ ] Fix `Person::input()` — add floor range validation + type menu
- [ ] Hook up priority queue to drive request ordering
- [ ] Add `displayPassengerHistory()`

---

## To Compile & Run

```bash
g++ -std=c++17 main.cpp Lift.cpp -o hospital_lift.exe
./hospital_lift.exe
```

**Test IDs to use right now:**
- `213` → Emergency Patient
- `214` → Doctor
- `215` → Staff
- `216` → Normal Check-up
- any other → Visitor
