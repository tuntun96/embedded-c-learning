# Day 7 — Structures in Embedded C

## Learning Objectives

By the end of Day 7, I learned how to:

* Define and use structures in C
* Group related data using a structure
* Declare and initialize structure variables
* Access structure members using the dot (`.`) operator
* Update structure members
* Create multiple structure objects
* Pass structures to functions
* Understand structure pass-by-value
* Use pointers to structures
* Access structure members using the arrow (`->`) operator
* Pass structure addresses to functions
* Update original structure data through pointers
* Create reusable functions for displaying structure data
* Apply structures to an EV dual-gun charger example

---

# 1. What Is a Structure?

A structure is a user-defined data type that groups multiple related variables under one name.

The members of a structure can have different data types.

Example:

```c
struct Charger_Status
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault_status;
};
```

In this example:

* `voltage` stores charger voltage
* `current` stores charger current
* `temperature` stores charger temperature
* `fault_status` stores the charger fault condition

Structures improve:

* Code readability
* Data organization
* Code maintenance
* Function design
* Reusability

---

# 2. Declaring a Structure Variable

After defining a structure, a variable can be created using:

```c
struct Charger_Status charger;
```

The variable `charger` contains all the members defined in `struct Charger_Status`.

Conceptually:

```text
charger
├── voltage
├── current
├── temperature
└── fault_status
```

---

# 3. Accessing Structure Members Using the Dot Operator

The dot (`.`) operator is used to access members of a normal structure variable.

Example:

```c
charger.voltage = 650U;
charger.current = 120U;
charger.temperature = 45U;
charger.fault_status = 0U;
```

The syntax is:

```c
structure_variable.member_name
```

Example:

```c
charger.current
```

---

# 4. Initializing a Structure

A structure can be initialized when it is declared.

Example:

```c
struct Charger_Status charger =
{
    650U,
    120U,
    45U,
    0U
};
```

The values are assigned according to the order of the members in the structure.

```text
650U → voltage
120U → current
45U  → temperature
0U   → fault_status
```

---

# 5. Updating Structure Members

Structure members can be updated after initialization.

Example:

```c
charger.current = 150U;
charger.fault_status = 1U;
```

Only the selected members are updated.

Other members remain unchanged.

---

# 6. Multiple Structure Variables

Multiple objects can be created from the same structure type.

Example:

```c
struct Charger_Status gun_1;
struct Charger_Status gun_2;
```

Each structure variable stores independent data.

Example:

```text
Gun 1
Voltage: 650 V
Current: 120 A

Gun 2
Voltage: 680 V
Current: 150 A
```

Updating `gun_1` does not automatically update `gun_2`.

---

# 7. Passing a Structure to a Function

A structure can be passed directly to a function.

Function declaration:

```c
void print_charger_status(
    struct Charger_Status charger
);
```

Function call:

```c
print_charger_status(gun_1);
```

When a structure is passed directly, the function receives a copy of the structure.

Therefore, changes made inside the function do not modify the original structure.

Example:

```c
void update_current(
    struct Charger_Status charger
)
{
    charger.current = 150U;
}
```

The original structure remains unchanged because only the copied structure is modified.

---

# 8. Structure Pointer

A pointer can store the address of a structure.

Example:

```c
struct Charger_Status *charger_pointer;
```

The pointer can be assigned the address of a structure:

```c
charger_pointer = &charger;
```

The pointer now points to the original `charger` structure.

Concept:

```text
charger_pointer
       │
       │ stores the address of charger
       ▼
charger
├── voltage
├── current
├── temperature
└── fault_status
```

---

# 9. Arrow Operator (`->`)

The arrow operator is used to access structure members through a structure pointer.

Example:

```c
charger_pointer->voltage
```

Other examples:

```c
charger_pointer->current;
charger_pointer->temperature;
charger_pointer->fault_status;
```

The arrow operator is equivalent to:

```c
(*charger_pointer).voltage
```

The arrow operator is easier to read and is commonly used in embedded C.

---

# 10. Dot Operator vs Arrow Operator

| Situation                 | Operator | Example                    |
| ------------------------- | -------- | -------------------------- |
| Normal structure variable | `.`      | `charger.voltage`          |
| Pointer to a structure    | `->`     | `charger_pointer->voltage` |

Important:

```c
charger.voltage
```

is used when `charger` is the actual structure variable.

```c
charger_pointer->voltage
```

is used when `charger_pointer` is a pointer to the structure.

---

# 11. Passing a Structure Address to a Function

The address of a structure can be passed using the address operator (`&`).

Function declaration:

```c
void update_charger_status(
    struct Charger_Status *charger_pointer
);
```

Function call:

```c
update_charger_status(&charger);
```

Inside the function:

```c
charger_pointer->current = 150U;
charger_pointer->fault_status = 1U;
```

Because the pointer contains the address of the original structure, the original structure is modified.

---

# 12. Pass-by-Value vs Passing by Pointer

| Method          | Function Call      | Function Receives    | Original Data Changes? |
| --------------- | ------------------ | -------------------- | ---------------------- |
| Pass by value   | `update(charger)`  | Copy of structure    | No                     |
| Pass by pointer | `update(&charger)` | Address of structure | Yes                    |

---

# 13. Reusable Display Function

A reusable function can be created to display charger information.

Example:

```c
void charger_status_display(
    struct Charger_Status *charger_pointer
)
{
    printf(
        "Voltage: %u V\n",
        (unsigned int)charger_pointer->voltage
    );

    printf(
        "Current: %u A\n",
        (unsigned int)charger_pointer->current
    );

    printf(
        "Temperature: %u C\n",
        (unsigned int)charger_pointer->temperature
    );

    printf(
        "Fault Status: %u\n",
        (unsigned int)charger_pointer->fault_status
    );
}
```

The function can be called using:

```c
charger_status_display(&charger);
```

This avoids repeating the same `printf()` code.

---

# 14. EV Charger Application

Structures can be used to store charger information together.

Example:

```c
struct Charger_Status
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault_status;
};
```

For a dual-gun charger:

```c
struct Charger_Status gun_1;
struct Charger_Status gun_2;
```

Each gun can store independent data.

Possible future additions:

```c
uint32_t energy;
uint8_t connector_status;
uint8_t charging_state;
uint8_t communication_status;
```

---

# 15. Important Interview Questions

### Q1. What is a structure in C?

A structure is a user-defined data type that groups related variables, possibly with different data types, under one name.

### Q2. What is the difference between `.` and `->`?

The dot operator (`.`) accesses a member through a normal structure variable.

The arrow operator (`->`) accesses a member through a pointer to a structure.

### Q3. What happens when a structure is passed directly to a function?

A copy of the structure is passed. Changes inside the function do not affect the original structure.

### Q4. Why use a structure pointer?

A structure pointer allows a function to access or modify the original structure without creating a full structure copy.

### Q5. Why is `->` used with a structure pointer?

The `->` operator accesses structure members through the address stored in the pointer.

### Q6. Are these equivalent?

```c
charger_pointer->current
```

and:

```c
(*charger_pointer).current
```

Yes. Both access the same structure member through a pointer.

---

# 16. Day 7 Key Takeaways

* A structure groups related data.
* Use `.` with a normal structure variable.
* Use `->` with a structure pointer.
* Use `&` to pass the address of a structure.
* Passing a structure directly creates a copy.
* Passing a structure address allows the original data to be modified.
* Structure pointers are useful for efficient and reusable embedded firmware functions.
* Structures are commonly used for sensor data, communication frames, device status, configuration, and state information.

---

# Day 7 Practical Files

* `dual_gun_charger.c`
* `structure_function.c`
* `structure_pointer.c`
* `structure_pointer_update.c`

---

# Day 7 Completion Status

**Status: Completed ✅**

Next Topic:

**Day 8 — Enums and EV Charger State Machines**
