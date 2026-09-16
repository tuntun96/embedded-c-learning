# Day 21 — Embedded C: State Machines

## 1. What is a State Machine?

A **state machine** is a programming structure where the behavior of a system depends on its **current state** and the **event** that occurs.

Basic idea:

**Current State + Event → Next State**

Example:

```text
CHARGING + FAULT_DETECTED → FAULTED
```

A state machine is useful when an embedded system can exist in several well-defined conditions and must react differently to events in each condition.

---

# 2. Important State-Machine Terms

## State

A state represents the current condition of the system.

Example:

```c
typedef enum
{
    IDLE,
    VEHICLE_CONNECTED,
    AUTHENTICATING,
    AUTH_FAILED,
    READY,
    CHARGING,
    FAULTED
} ChargerState;
```

Examples:

* `IDLE` → no vehicle connected
* `VEHICLE_CONNECTED` → vehicle physically connected
* `AUTHENTICATING` → authentication is being checked
* `READY` → authenticated and waiting for charging start
* `CHARGING` → charging is active
* `FAULTED` → charging has stopped because of a fault

---

# 3. Event

An **event** is something that happens and may cause a state transition.

For the EV charger:

```c
#define VEHICLE_CONNECTED_MASK    (1U << 1U)
#define AUTH_REQUESTED_MASK       (1U << 2U)
#define AUTH_ACCEPTED_MASK        (1U << 3U)
#define AUTH_REJECTED_MASK        (1U << 4U)
#define AUTH_INVALID_MASK         (1U << 5U)
#define AUTH_TIMEOUT_MASK         (1U << 6U)
#define START_REQUESTED_MASK      (1U << 7U)
#define CHARGING_COMPLETE_MASK    (1U << 8U)
#define FAULT_DETECTED_MASK       (1U << 9U)
#define RETRY_AUTH_MASK           (1U << 10U)
#define VEHICLE_DISCONNECTED_MASK (1U << 11U)
```

The event register can contain several event bits simultaneously.

Example:

```c
charger_event_register =
    VEHICLE_CONNECTED_MASK |
    AUTH_REQUESTED_MASK;
```

---

# 4. Transition

A **transition** means moving from one state to another.

Example:

```text
IDLE
 ↓ Vehicle Connected
VEHICLE_CONNECTED
```

Another:

```text
AUTHENTICATING
 ↓ Authentication accepted
READY
```

Another:

```text
CHARGING
 ↓ Fault detected
FAULTED
```

---

# 5. Action

An **action** is something the firmware performs while entering, remaining in, or leaving a state.

For example:

```c
void handle_charging_state(void)
{
    printf("Monitoring voltage...\n");
    printf("Monitoring Current...\n");
    printf("Monitoring Temperature...\n");
}
```

This is a **state action** because it runs while the system is in `CHARGING`.

---

# 6. EV Charger State Flow

The state machine developed during this session:

```text
                         Vehicle Connected
IDLE ─────────────────────────────────────────► VEHICLE_CONNECTED
                                                   │
                                                   │ Auth Requested
                                                   ▼
                                            AUTHENTICATING
                                             /     |      \
                                            /      |       \
                                  Accepted /       |        \ Rejected
                                          /        |         \
                                         ▼         ▼          ▼
                                      READY     FAULTED    AUTH_FAILED
                                        │                    │
                                        │ Start              │ Retry
                                        ▼                    ▼
                                    CHARGING          AUTHENTICATING
                                        │
                              ┌─────────┴─────────┐
                              │                   │
                         Fault detected      Complete
                              │                   │
                              ▼                   ▼
                           FAULTED              READY
```

---

# 7. Why Use `enum` for States?

Using an enum makes the state machine readable.

Instead of:

```c
state = 5;
```

use:

```c
state = CHARGING;
```

This makes the code easier to understand and reduces mistakes.

---

# 8. Basic State-Machine Structure

The main structure is:

```c
while (1)
{
    switch (state)
    {
        case IDLE:
            /* state action */

            /* check events */

            break;

        case READY:
            /* state action */

            /* check events */

            break;

        case CHARGING:
            /* state action */

            /* check events */

            break;

        case FAULTED:
            /* state action */

            /* check events */

            break;

        default:
            /* unexpected state */
            break;
    }
}
```

The `switch` decides what the firmware should do based on the current state.

---

# 9. State Actions vs Transitions

These are different.

### State action

"What should the firmware continuously do while I am here?"

Example:

```c
handle_charging_state();
```

### Transition

"When should I leave this state?"

Example:

```c
if ((charger_event_register & FAULT_DETECTED_MASK) != 0U)
{
    state = FAULTED;
}
```

---

# 10. Authentication State

The authentication state became more realistic by supporting multiple outcomes.

```text
AUTHENTICATING
      │
      ├── ACCEPTED ──► READY
      │
      ├── REJECTED ──► AUTH_FAILED
      │
      ├── INVALID ───► AUTH_FAILED
      │
      └── TIMEOUT ───► AUTH_FAILED
```

An authentication failure reason can be stored separately:

```c
typedef enum
{
    AUTH_REASON_NONE,
    AUTH_REASON_REJECTED,
    AUTH_REASON_INVALID,
    AUTH_REASON_TIMEOUT
} AuthFailureReason;
```

Initialize it:

```c
AuthFailureReason auth_failure_reason = AUTH_REASON_NONE;
```

---

# 11. Event Checking with Bit Masks

Correct pattern:

```c
if ((charger_event_register & AUTH_ACCEPTED_MASK) != 0U)
{
    /* event detected */
}
```

Do not write:

```c
if (charger_event_register & AUTH_ACCEPTED_MASK == 1U)
```

because operator precedence can make this behave differently from what is intended.

Use explicit parentheses:

```c
if ((charger_event_register & AUTH_ACCEPTED_MASK) != 0U)
```

---

# 12. Clearing an Event

After consuming an event:

```c
charger_event_register &= ~AUTH_ACCEPTED_MASK;
```

This clears only that bit.

For multiple events:

```c
charger_event_register &=
    ~(AUTH_ACCEPTED_MASK | AUTH_REJECTED_MASK);
```

This clears both bits.

---

# 13. Event Conflict

Sometimes two incompatible events can be set simultaneously.

Example:

```text
AUTH_ACCEPTED
      +
AUTH_REJECTED
```

Both cannot logically represent a valid authentication result at the same time.

Therefore:

```c
if (((charger_event_register & AUTH_ACCEPTED_MASK) != 0U) &&
    ((charger_event_register & AUTH_REJECTED_MASK) != 0U))
{
    state = FAULTED;

    charger_event_register &=
        ~(AUTH_ACCEPTED_MASK | AUTH_REJECTED_MASK);
}
```

This treats the contradictory condition as a fault.

---

# 14. Event Priority

Events may occur simultaneously.

Example:

```text
CHARGING
   │
   ├── FAULT_DETECTED
   └── CHARGING_COMPLETE
```

The charger should prioritize the safety-critical fault.

Therefore:

```c
if ((charger_event_register & FAULT_DETECTED_MASK) != 0U)
{
    state = FAULTED;
}
else if ((charger_event_register & CHARGING_COMPLETE_MASK) != 0U)
{
    state = READY;
}
```

### General rule

> Safety-critical events should normally have higher priority than normal operational events.

---

# 15. Stale Events

A stale event is an event that remains set after it should have been consumed.

Example:

```text
FAULT_DETECTED = 1
```

If the event is handled but not cleared, the state machine may see the same event again on the next loop.

Therefore:

```text
Detect
   ↓
Handle
   ↓
Clear consumed event
```

But an event should only be cleared when the current architecture considers it **consumed**.

---

# 16. Entry Action

An **entry action** runs once when entering a state.

Example:

```c
void charging_entry(void)
{
    printf("Charging started.\n");
}
```

Typical real embedded actions:

```text
Enable charging output
Reset session energy
Record start timestamp
Turn ON charging indicator
```

---

# 17. State Action

A state action runs repeatedly while the system remains in that state.

```c
void handle_charging_state(void)
{
    printf("Monitoring voltage...\n");
    printf("Monitoring Current...\n");
    printf("Monitoring Temperature...\n");
}
```

Typical real actions:

```text
Monitor voltage
Monitor current
Monitor temperature
Monitor energy
Check protection conditions
Update HMI
```

---

# 18. Exit Action

An **exit action** runs once when leaving a state.

```c
void charging_exit(void)
{
    printf("Charging stopped.\n");
}
```

Typical real actions:

```text
Disable charging output
Save session information
Record stop reason
Stop charging timer
```

---

# 19. Detecting State Changes

A common embedded pattern is to maintain:

```c
ChargerState state = IDLE;
ChargerState previous_state = IDLE;
```

After the state-machine switch:

```c
if (state != previous_state)
{
    if (state == READY)
    {
        ready_entry();
    }

    if (previous_state == READY)
    {
        ready_exit();
    }

    if (state == CHARGING)
    {
        charging_entry();
    }

    if (previous_state == CHARGING)
    {
        charging_exit();
    }
}

previous_state = state;
```

This allows the firmware to distinguish:

```text
Just entered state
        vs
Still inside state
        vs
Just left state
```

---

# 20. Example: READY → CHARGING

Before:

```text
state = READY
previous_state = READY
```

A start event occurs:

```c
state = CHARGING;
```

After the switch:

```text
state != previous_state
```

is true.

Then:

```text
READY exit
     ↓
CHARGING entry
```

Then:

```c
previous_state = state;
```

Now both are:

```text
state          = CHARGING
previous_state = CHARGING
```

On the next loop, no entry action runs again.

---

# 21. Complete Architecture

The overall firmware pattern becomes:

```text
                 ┌──────────────────┐
                 │ Read/Receive     │
                 │ Events           │
                 └────────┬─────────┘
                          ↓
                 ┌──────────────────┐
                 │ Current State    │
                 └────────┬─────────┘
                          ↓
                 ┌──────────────────┐
                 │ State Action     │
                 └────────┬─────────┘
                          ↓
                 ┌──────────────────┐
                 │ Check Events     │
                 └────────┬─────────┘
                          ↓
                 ┌──────────────────┐
                 │ Decide Next      │
                 │ State            │
                 └────────┬─────────┘
                          ↓
                 ┌──────────────────┐
                 │ Entry / Exit     │
                 │ Actions          │
                 └────────┬─────────┘
                          ↓
                    Next Loop
```

---

# 22. Important Embedded-C Lessons

### `enum` → state representation

```c
ChargerState state;
```

### Bit masks → event representation

```c
#define FAULT_DETECTED_MASK (1U << 9U)
```

### `switch` → state behavior

```c
switch (state)
```

### `if` → event detection

```c
if ((events & MASK) != 0U)
```

### Bit clear → event consumption

```c
events &= ~MASK;
```

### `previous_state` → entry/exit detection

```c
if (state != previous_state)
```

### Priority → deterministic behavior

```c
if (FAULT)
{
    ...
}
else if (COMPLETE)
{
    ...
}
```

---

# 23. Common Mistakes

### Mistake 1 — Treating states as event bits

Bad architecture:

```text
BIT 5 = READY
BIT 6 = CHARGING
```

Better:

```text
state = READY
```

and events are separate:

```text
START_REQUESTED
FAULT_DETECTED
CHARGING_COMPLETE
```

---

### Mistake 2 — Missing `break`

```c
case AUTHENTICATING:
    ...
    /* missing break */

case AUTH_FAILED:
    ...
```

This causes unintended fall-through.

---

### Mistake 3 — Wrong bit comparison

Avoid:

```c
(reg & MASK) == 1U
```

Use:

```c
(reg & MASK) != 0U
```

---

### Mistake 4 — Running entry action continuously

Wrong:

```c
case CHARGING:
    charging_entry();
```

That runs every loop.

Correct:

```c
if (state != previous_state)
{
    if (state == CHARGING)
    {
        charging_entry();
    }
}
```

---

### Mistake 5 — No event priority

If both fault and completion occur, blindly processing completion first could incorrectly move the charger to `READY`.

Safety-critical events need explicit priority.

---

### Mistake 6 — Stale events

If an event has been consumed but isn't cleared, it can affect later loops or states.

---

# 24. EV Charger Example

A practical state machine can eventually look like:

```text
IDLE
 │
 │ Vehicle connected
 ↓
VEHICLE_CONNECTED
 │
 │ Authentication requested
 ↓
AUTHENTICATING
 │
 ├── Accepted ───────► READY
 │
 ├── Rejected ───────► AUTH_FAILED
 │
 ├── Invalid ────────► AUTH_FAILED
 │
 └── Timeout ────────► AUTH_FAILED

AUTH_FAILED
 │
 ├── Retry ──────────► AUTHENTICATING
 │
 └── Disconnected ──► IDLE

READY
 │
 ├── Start ──────────► CHARGING
 │
 └── Disconnected ──► IDLE

CHARGING
 │
 ├── Fault ──────────► FAULTED
 │
 ├── Complete ───────► READY
 │
 └── Disconnected ──► IDLE

FAULTED
 │
 └── Recovery/reset logic
```

---

# 25. Interview Questions to Remember

**Q: Why use a state machine in embedded systems?**

Because it makes behavior deterministic, organized, and easier to maintain when a system has multiple operating conditions and events.

**Q: Difference between state and event?**

A state is the current condition of the system; an event is something that happens and may cause a transition.

**Q: Why use `enum` for states?**

It makes state representation readable and type-oriented instead of using unexplained numeric values.

**Q: Why use bit masks for events?**

They allow multiple independent events to be represented efficiently in one integer.

**Q: Why use `!= 0U` when checking a mask?**

Because a bit mask operation produces a nonzero value when the selected bit is set; that value does not necessarily equal `1U`.

**Q: What is an entry action?**

An action executed once when entering a state.

**Q: What is a state action?**

An action repeatedly executed while remaining in a state.

**Q: What is an exit action?**

An action executed once when leaving a state.

**Q: What happens if two events occur simultaneously?**

The state machine needs a defined priority or conflict-handling rule.

**Q: Which should normally have higher priority: charging complete or a critical fault?**

The critical fault.

---

# 26. Day 21 Completion Checklist

* [x] State-machine concept
* [x] State vs event
* [x] Transition
* [x] Action
* [x] `enum` states
* [x] `switch`-based state machine
* [x] EV charger state flow
* [x] Authentication states
* [x] Authentication failure reasons
* [x] Event bit masks
* [x] Event detection
* [x] Event clearing
* [x] Event conflicts
* [x] Event priority
* [x] Stale-event concept
* [x] State entry actions
* [x] State actions
* [x] State exit actions
* [x] `previous_state` transition detection
* [x] Complete EV-charger state-machine implementation
* [x] Code review/debugging

## Status

**State Machines — COMPLETE**

According to the official roadmap, the next topic is:

**Day 22 → Error Handling**
