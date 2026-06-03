# Smart Priority Elevator System 🛗

A collaborative C++ project implementing a Smart Priority Elevator System with emergency queue logic.

## Team Structure

| Member | Branch | Responsibility |
|--------|--------|----------------|
| Jyothiradithya | `jyothiradithya` | Controller & Scheduling (Person class, Priority Assignment, Nearest Lift Selection) |
| HY | `hy` | Lift Operations & Simulation (Lift class, Movement Logic, Boarding, Capacity Handling) |

## Branch Strategy (No Merge Conflicts!)

```
main              ← Final integration only (DO NOT code here directly)
├── hy            ← HY's working branch
└── jyothiradithya ← Jyothiradithya's working branch
```

**Rules:**
- Each person works **only on their own branch**
- Never edit the other person's files
- Merge into `main` only during integration (Phase 4)

## File Ownership

| File | Owner |
|------|-------|
| `Lift.cpp` / `Lift.h` | HY |
| `simulation.cpp` | HY |
| `Controller.cpp` / `Controller.h` | Jyothiradithya |
| `Person.cpp` / `Person.h` | Jyothiradithya |
| `main.cpp` | Both (merge carefully during Phase 4) |

## Build Instructions

```bash
g++ Lift.cpp simulation.cpp -o lift_system
./lift_system
```

## Development Phases

- **Phase 1**: Core Classes & Assignment ✅
- **Phase 2**: Lift Simulation (HY) + Controller (Jyothiradithya)
- **Phase 3**: Emergency Mode + Priority Queue
- **Phase 4**: Integration & Testing
- **Phase 5**: Demo Presentation
