# Day 13 — Union, Tagged Union & Practical Embedded C

## 1. `union`

A `union` is a user-defined data type in C where **all members share the same memory location**.

### Syntax

```c
union ChargerData
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};
```

### Key point

Only one shared memory area is allocated for the members.

```text
union
┌──────────────────┐
│   Shared Memory  │
└──────────────────┘
     ↑    ↑    ↑
     │    │    │
 voltage current temperature
```

---

## 2. `struct` vs `union`

### Structure

Each member has separate storage.

```c
struct Charger
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};
```

For three `uint16_t` members:

```text
voltage      → 2 bytes
current      → 2 bytes
temperature  → 2 bytes
-------------------------
total        → 6 bytes
```

### Union

Members share storage.

```c
union Charger
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};
```

The size is generally determined by the largest member, subject to alignment.

```text
voltage      → 2 bytes
current      → 2 bytes
temperature  → 2 bytes
-------------------------
union size   → 2 bytes
```

### Memory rule

```text
STRUCT
→ Members have separate storage

UNION
→ Members share storage
```

---

## 3. Why use a union?

Use a union when different pieces of data are **mutually exclusive** and only one needs to be stored at a time.

Example:

```text
EV Charger Diagnostic

Voltage
OR
Current
OR
Temperature
```

If all three values must exist simultaneously → use `struct`.

If only one value is active at a time → `union` can save memory.

---

## 4. Important union behavior

Consider:

```c
union Data
{
    uint16_t voltage;
    uint16_t current;
};

union Data data;

data.voltage = 650U;
data.current = 120U;
```

After assigning `current`, the union does **not** contain two independently stored values.

The same memory is reused.

```text
First:

data.voltage = 650
       ↓
┌──────────────┐
│     650      │
└──────────────┘


Then:

data.current = 120
       ↓
┌──────────────┐
│     120      │
└──────────────┘
```

The previous voltage value is no longer available as an independently stored value.

---

# 5. `enum`

An `enum` is used to define named integer constants.

Example:

```c
enum ChargerDataType
{
    DATA_VOLTAGE,
    DATA_CURRENT,
    DATA_TEMPERATURE,
    DATA_FAULT
};
```

The enum can identify the current state/type of the data.

```text
DATA_VOLTAGE
DATA_CURRENT
DATA_TEMPERATURE
DATA_FAULT
```

---

# 6. Enum + Union = Tagged Union

A union by itself doesn't tell us which member currently contains valid data.

Therefore, we can use an `enum` as a **tag**.

```c
enum ChargerDataType
{
    DATA_VOLTAGE,
    DATA_CURRENT,
    DATA_TEMPERATURE,
    DATA_FAULT
};

union ChargerValue
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
};
```

Then combine them:

```c
struct Diagnostic
{
    enum ChargerDataType type;
    union ChargerValue data;
};
```

This creates a **tagged union** design.

### Concept

```text
struct Diagnostic
│
├── type
│    ↓
│   Tells us WHAT the data is
│
└── data
     ↓
    Stores the actual value
```

Example:

```c
diagnostic.type = DATA_VOLTAGE;
diagnostic.data.voltage = 750U;
```

The enum tells us:

```text
Current active type = VOLTAGE
```

and the union stores:

```text
750
```

---

# 7. `switch` with Tagged Union

The `switch` can use the enum to determine which union member should be accessed.

```c
switch (data.type)
{
case DATA_VOLTAGE:
    printf("Type: Voltage\n");
    printf("Value: %u V\n",
           (unsigned int)data.data.voltage);
    break;

case DATA_CURRENT:
    printf("Type: Current\n");
    printf("Value: %u A\n",
           (unsigned int)data.data.current);
    break;

case DATA_TEMPERATURE:
    printf("Type: Temperature\n");
    printf("Value: %u C\n",
           (unsigned int)data.data.temperature);
    break;

default:
    printf("Invalid Data Type\n");
    break;
}
```

### Important relationship

```text
enum value
     ↓
switch
     ↓
select correct union member
     ↓
print/process the value
```

---

# 8. Passing a Structure to a Function

We implemented:

```c
void print_diagnostic(struct diagnostic data);
```

This is a function declaration.

Breakdown:

```text
void
→ function returns nothing

print_diagnostic
→ function name

struct diagnostic
→ parameter type

data
→ parameter name
```

Implementation:

```c
void print_diagnostic(struct diagnostic data)
{
    switch (data.type)
    {
        ...
    }
}
```

Calling the function:

```c
print_diagnostic(diagnostic_data);
```

This keeps the diagnostic-printing logic separate from `main()`.

---

# 9. EV Charger Diagnostic Example

Our practical design was:

```text
EV Charger
    │
    ▼
Diagnostic Data
    │
    ├── Voltage
    ├── Current
    ├── Temperature
    └── Fault
```

Example:

```c
data.dtype = DATA_VOLTAGE;
data.dvalue.voltage = 750U;

print_diagnostic(data);
```

Then:

```c
data.dtype = DATA_CURRENT;
data.dvalue.current = 150U;

print_diagnostic(data);
```

Then:

```c
data.dtype = DATA_TEMPERATURE;
data.dvalue.temperature = 65U;

print_diagnostic(data);
```

Expected:

```text
Type: Voltage
Value: 750 V

Type: Current
Value: 150 A

Type: Temperature
Value: 65 C
```

---

# 10. Fault State

We also added:

```c
DATA_FAULT
```

For a fault condition, we don't need to read a numeric union member.

```c
case DATA_FAULT:
    printf("Type: FAULT\n");
    printf("Value: Invalid diagnostic data\n");
    break;
```

The enum itself identifies the state.

---

# 11. Important Coding Rule

Always access the union member corresponding to the active enum value.

Correct:

```c
data.type = DATA_CURRENT;
data.data.current = 150U;
```

and:

```c
case DATA_CURRENT:
    printf("%u", data.data.current);
    break;
```

Avoid doing this:

```c
data.type = DATA_CURRENT;
data.data.voltage = 150U;
```

Even though it may appear to work when the members have the same underlying type, it makes the code misleading and breaks the intended tagged-union design.

---

# 12. When to Use `struct` vs `union`

### Use `struct` when:

All values are required at the same time.

Example:

```text
EV Charger Status

Voltage     = 750 V
Current     = 150 A
Temperature = 65 C
```

All three values need to exist simultaneously.

```c
struct ChargerStatus
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};
```

### Use `union` when:

Only one representation/value is active at a time.

```text
Diagnostic message

Voltage
OR
Current
OR
Temperature
```

```c
union ChargerValue
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};
```

---

# 13. Interview Points

### Q: What is a union?

> A union is a user-defined data type in C in which all members share the same memory location.

### Q: Why does a union save memory?

> Because all members use the same storage, so the union generally requires storage based on its largest member rather than the sum of all members.

### Q: Can all union members hold independent values simultaneously?

> No. They share the same memory, so writing to one member replaces the stored representation. Only the currently active member should normally be treated as valid.

### Q: Why combine enum with union?

> The enum acts as a tag that identifies which union member is currently valid.

### Q: What is a tagged union?

> A tagged union combines an enum that identifies the active data with a union that stores the corresponding value.

### Q: Why use a union for an EV charger diagnostic packet?

> If the packet contains voltage, current, or temperature but only one of them at a time, a union can represent the mutually exclusive data efficiently.

---

# 14. Common Mistakes

### Mistake 1 — Treating a union like a structure

Incorrect assumption:

```text
union → all members retain independent values
```

Correct:

```text
union → members share storage
```

### Mistake 2 — Forgetting the active-data tag

A union alone doesn't tell the program which member should be interpreted as valid.

Use:

```c
enum type;
union data;
```

### Mistake 3 — Reading the wrong union member

If:

```c
type = DATA_CURRENT;
```

the program should access:

```c
data.current
```

not:

```c
data.voltage
```

### Mistake 4 — Putting everything inside `main()`

Move reusable processing into functions:

```c
void print_diagnostic(struct diagnostic data);
```

This improves modularity and readability.

---

# 15. Day 13 Key Takeaways

```text
union
→ Members share the same memory

struct
→ Members have separate storage

enum
→ Named constants used to identify states/types

enum + union
→ Tagged union

tag
→ Identifies which union member is currently valid

switch
→ Selects the correct union member

function
→ Keeps processing logic modular

EV Charger example
→ Voltage OR Current OR Temperature
```

---

# 16. Interview Memory Trick

```text
STRUCT
→ Store ALL

UNION
→ Store ONE

ENUM
→ Identify WHICH ONE

SWITCH
→ Decide HOW to process it
```

### One-line professional answer

> **A tagged union uses an enum to identify the active data and a union to store that data efficiently in shared memory.**
