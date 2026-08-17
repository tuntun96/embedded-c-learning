# Day 8 — Enums and EV Charger State Machines

## 1. Learning Objectives

Today I learned:

* What an enum is
* Default enum values
* Custom enum values
* `typedef enum`
* Enum variables
* Using enums with `switch-case`
* EV charger operating states
* State behavior
* State transitions
* Event-based state changes
* Basic EV charger state-machine design

---

# 2. What is an Enum?

`enum` means **enumeration**.

An enum is a user-defined type in C that defines a fixed set of named integer constants.

Example:

```c
enum Charger_State
{
    CHARGER_IDLE,
    CHARGER_CONNECTED,
    CHARGER_CHARGING,
    CHARGER_COMPLETE,
    CHARGER_FAULT
};
```

The compiler assigns integer values automatically:

| Enum Member         | Default Value |
| ------------------- | ------------: |
| `CHARGER_IDLE`      |             0 |
| `CHARGER_CONNECTED` |             1 |
| `CHARGER_CHARGING`  |             2 |
| `CHARGER_COMPLETE`  |             3 |
| `CHARGER_FAULT`     |             4 |

---

# 3. Why Are Enums Useful?

Enums provide meaningful names instead of unclear numeric values.

Less readable:

```c
charger_state = 2;
```

More readable:

```c
charger_state = CHARGER_CHARGING;
```

Advantages:

* Improves code readability
* Improves code maintenance
* Makes debugging easier
* Helps manage system states
* Helps manage operating modes
* Helps manage fault conditions
* Works well with `switch-case`
* Supports state-machine design

---

# 4. Enum Variable Declaration

An enum variable can be declared as:

```c
enum Charger_State charger_state;
```

A value can be assigned using:

```c
charger_state = CHARGER_CHARGING;
```

The value of `CHARGER_CHARGING` is:

```text
2
```

---

# 5. Custom Enum Values

Enum members can be assigned custom values.

Example:

```c
enum Charger_Fault
{
    FAULT_NONE = 0,
    FAULT_OVER_VOLTAGE = 10,
    FAULT_OVER_CURRENT,
    FAULT_OVER_TEMPERATURE,
    FAULT_COMMUNICATION = 20
};
```

The values are:

| Fault                    | Value |
| ------------------------ | ----: |
| `FAULT_NONE`             |     0 |
| `FAULT_OVER_VOLTAGE`     |    10 |
| `FAULT_OVER_CURRENT`     |    11 |
| `FAULT_OVER_TEMPERATURE` |    12 |
| `FAULT_COMMUNICATION`    |    20 |

Rule:

> After an explicitly assigned enum value, the following enum members increase by `1` unless another value is explicitly assigned.

---

# 6. `typedef enum`

`typedef` creates an alias or alternative name for a type.

Example:

```c
typedef enum
{
    CHARGER_IDLE,
    CHARGER_CONNECTED,
    CHARGER_CHARGING,
    CHARGER_COMPLETE,
    CHARGER_FAULT
} Charger_State;
```

Now the variable can be declared using:

```c
Charger_State charger_state;
```

Instead of:

```c
enum Charger_State charger_state;
```

Advantages:

* Shorter declarations
* Better readability
* Easier code maintenance
* Commonly used in embedded C projects

Important:

> `typedef` does not change enum values or behavior. It only provides another type name.

---

# 7. Enum with `switch-case`

Enums work well with `switch-case`.

Example:

```c
switch (charger_state)
{
    case CHARGER_IDLE:
        printf("EV Charger State: IDLE\n");
        break;

    case CHARGER_CONNECTED:
        printf("EV Charger State: CONNECTED\n");
        break;

    case CHARGER_CHARGING:
        printf("EV Charger State: CHARGING\n");
        break;

    case CHARGER_COMPLETE:
        printf("EV Charger State: COMPLETE\n");
        break;

    case CHARGER_FAULT:
        printf("EV Charger State: FAULT\n");
        break;

    default:
        printf("EV Charger State: UNKNOWN\n");
        break;
}
```

`break` exits the `switch` after the matched case executes and prevents unintended fall-through into the next case.

The `default` case handles an unexpected or invalid state.

---

# 8. What Is a State Machine?

A state machine is a system that operates in one defined state at a time.

Events or conditions cause the system to transition from one state to another.

Simplified EV charger flow:

```text
CHARGER_IDLE
      ↓ Vehicle connected
CHARGER_CONNECTED
      ↓ Authorization successful
CHARGER_CHARGING
      ↓ Charging completed
CHARGER_COMPLETE
      ↓ Vehicle disconnected or reset
CHARGER_IDLE
```

If a fault occurs:

```text
Any active state
      ↓ Fault detected
CHARGER_FAULT
```

---

# 9. State vs State Transition

## State

A state is the current operating condition of a system.

Examples:

```text
CHARGER_IDLE
CHARGER_CONNECTED
CHARGER_CHARGING
CHARGER_COMPLETE
CHARGER_FAULT
```

## State Behavior

State behavior describes what the system does while it is in a state.

Example:

```text
CHARGER_CHARGING
→ Monitor voltage
→ Monitor current
→ Monitor temperature
→ Monitor faults
→ Update charging data
```

## State Transition

A state transition is a change from one state to another because of an event or condition.

Example:

```text
Current State:
CHARGER_CONNECTED

Event:
Authorization successful

Transition:

CHARGER_CONNECTED
        ↓
CHARGER_CHARGING
```

---

# 10. Event-Based State Transition

Example:

```c
uint8_t vehicle_connected = 1U;
uint8_t authorization_success = 1U;
```

Logic:

```c
if (vehicle_connected == 1U)
{
    charger_state = CHARGER_CONNECTED;

    if (authorization_success == 1U)
    {
        charger_state = CHARGER_CHARGING;
    }
}
else
{
    charger_state = CHARGER_IDLE;
}
```

State flow:

```text
Vehicle not connected
→ CHARGER_IDLE

Vehicle connected
→ CHARGER_CONNECTED

Vehicle connected
+ Authorization successful
→ CHARGER_CHARGING
```

---

# 11. Important Design Rule

Authorization should be checked only after the vehicle is connected.

Correct logic:

```text
Vehicle connected?
    │
    ├── No
    │     ↓
    │   CHARGER_IDLE
    │
    └── Yes
          ↓
    CHARGER_CONNECTED
          ↓
    Authorization successful?
          │
          ├── No
          │     ↓
          │   Remain CONNECTED
          │
          └── Yes
                ↓
          CHARGER_CHARGING
```

---

# 12. Important Interview Answers

## What is an enum?

An enum, or enumeration, is a user-defined type in C that defines a fixed set of named integer constants. It improves readability and makes states, modes, and fault conditions easier to manage.

## Why are enums useful in embedded systems?

Enums represent fixed states, operating modes, commands, and fault conditions using meaningful names. They improve readability, maintainability, debugging, and state-machine design.

## What is a state machine?

A state machine is a system that operates in one defined state at a time. Events or conditions cause it to transition from the current state to another state, and each state can have different behavior.

## What is the difference between a state and a state transition?

A state is the current operating condition of a system. A state transition is the change from one state to another due to an event or condition.

## What is the advantage of `typedef enum`?

`typedef enum` creates a shorter and more readable type name. It does not change the enum values or behavior.

---

# 13. Day 8 Programs

* `enum_basic.c`
* `custom_enum.c`
* `typedef_enum.c`
* `charger_state_display.c`
* `charger_state_machine.c`
* `charger_state_event.c`

---

# 14. Day 8 Key Takeaways

```text
enum
↓
Defines a fixed set of named values

typedef enum
↓
Creates a shorter and more readable enum type

Enum variable
↓
Stores the current system state

switch-case
↓
Executes behavior based on the current state

Events and conditions
↓
Control state transitions

State machine
↓
Defines system states, behavior, and transitions
```

---

# 15. Self-Assessment

| Skill                                  | Status    |
| -------------------------------------- | --------- |
| Understand enum                        | Completed |
| Use default enum values                | Completed |
| Assign custom enum values              | Completed |
| Use `typedef enum`                     | Completed |
| Use enum with `switch-case`            | Completed |
| Understand state behavior              | Completed |
| Understand state transitions           | Completed |
| Build a basic EV charger state machine | Completed |
| Build event-based state logic          | Completed |

**Day 8 Status: Completed**
