# Smart Priority Elevator System 🛗

A C++ simulation of a hospital elevator controller that routes passengers based on priority — emergency patients get a dedicated lift, doctors/staff get priority access, and normal visitors take whatever's nearest.

## How It Works

- 4 lifts run simultaneously, each starting on a random floor
- When someone requests a lift, they enter their **ID**, **current floor**, and **destination floor**
- Their ID determines their priority level, and they're assigned the nearest suitable lift

## Priority Levels

| Person | ID | Priority |
|---|---|---|
| Emergency Patient | `213` | Highest — gets a dedicated lift, non-stop |
| Doctor | `214` | High — shares the emergency lift if active |
| Staff | `215` | Medium — same as doctor |
| Normal Check-up | `216` | Low |
| Visitor | any other | Lowest |

## Files

| File | Description |
|---|---|
| `main.cpp` | Entry point, lift assignment logic |
| `Person.h` | Person class with priority assignment |
| `Lift.h / Lift.cpp` | Lift class — movement, doors, boarding, emergency mode |
| `db/schema.sql` | MySQL schema for persisting requests and events |

## Build & Run

```bash
g++ -std=c++17 -o lift_system main.cpp Lift.cpp
./lift_system
```

A pre-built `lift_system.exe` is also included for Windows.

## Team

| Member | Responsibility |
|---|---|
| **Jyothiradithya** | Person class, priority logic, lift assignment (`main.cpp`, `Person.h`) |
| **HY** | Lift movement, boarding, emergency mode (`Lift.h`, `Lift.cpp`) |
