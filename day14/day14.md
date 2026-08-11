# Day 14 — Pointers in Embedded C

## 1. Pointer Fundamentals

A pointer is a variable that stores the **address of another variable**.

```c
uint16_t voltage = 650U;
uint16_t *ptr = &voltage;
```

Here:

```text
voltage → stores 650
ptr     → stores address of voltage
```

### Important operators

| Operator | Meaning              |
| -------- | -------------------- |
| `&`      | Address-of operator  |
| `*`      | Dereference operator |

```c
&voltage
```

→ address of `voltage`

```c
*ptr
```

→ value stored at the address contained in `ptr`

### Key rule

```text
ptr  → address
*ptr → value
```

---

## 2. Modifying Data Through a Pointer

A pointer can be used to modify the original variable.

```c
uint16_t current = 100U;
uint16_t *ptr = &current;

*ptr = 120U;
```

Now:

```text
current = 120
```

The pointer does not contain the value `120`; it contains the address of `current`.

---

## 3. Passing a Pointer to a Function

Pointers allow a function to modify the original variable.

```c
void update_current(uint16_t *ptr)
{
    *ptr = 120U;
}
```

Call:

```c
uint16_t current = 100U;

update_current(&current);
```

The address of `current` is passed to the function.

```text
main()
  │
  │ &current
  ↓
update_current()
  │
  │ *ptr = 120
  ↓
original current = 120
```

### Important distinction

```c
update_current(&current);   // address
```

not:

```c
update_current(current);    // value
```

---

# 4. Pointers and Arrays

Consider:

```c
uint16_t charger_data[3] = {650U, 120U, 45U};

uint16_t *ptr = charger_data;
```

The array name generally represents the address of its first element when used in an expression.

```text
charger_data
     ↓
┌───────┬───────┬───────┐
│  650  │  120  │  45   │
└───────┴───────┴───────┘
   [0]     [1]     [2]
```

Therefore:

```c
*ptr
```

→ `650`

```c
*(ptr + 1)
```

→ `120`

```c
*(ptr + 2)
```

→ `45`

### Important relationship

```text
data[0]  == *(data + 0)
data[1]  == *(data + 1)
data[2]  == *(data + 2)
```

This relationship is fundamental for Embedded C buffer processing.

---

# 5. Pointer Arithmetic

For:

```c
uint16_t *ptr;
```

when we write:

```c
ptr + 1
```

the pointer moves to the **next `uint16_t` element**, not simply one byte forward.

Pointer arithmetic depends on the pointer's data type.

Example:

```c
ptr++;
```

moves:

```text
data[0] → data[1]
```

Another:

```c
ptr++;
```

moves:

```text
data[1] → data[2]
```

This is commonly used when processing:

* UART buffers
* CAN data
* Modbus frames
* ADC samples
* Communication packets

---

# 6. Pointer-to-Pointer (`**`)

A pointer-to-pointer stores the address of another pointer.

```c
uint16_t voltage = 650U;

uint16_t *ptr = &voltage;

uint16_t **pptr = &ptr;
```

Conceptually:

```text
pptr
 ↓
ptr
 ↓
voltage
 ↓
650
```

### Levels of dereferencing

```text
pptr
→ address of ptr

*pptr
→ value of ptr
→ address of voltage

**pptr
→ value of voltage
→ 650
```

Therefore:

```c
**pptr = 700U;
```

changes the original `voltage` to:

```text
700
```

Pointer-to-pointer is useful when a function needs to modify the pointer itself.

---

# 7. Pointer to Structure

For a structure:

```c
struct charger
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};
```

Create a structure and pointer:

```c
struct charger data;

struct charger *ptr = &data;
```

A normal structure variable uses:

```c
data.voltage
```

A structure pointer uses:

```c
ptr->voltage
```

### `.` vs `->`

```text
Structure variable → .
Structure pointer  → ->
```

Example:

```c
data.voltage = 650U;

ptr->voltage = 700U;
```

---

# 8. `->` and `(*ptr).member`

These are equivalent:

```c
ptr->voltage
```

and:

```c
(*ptr).voltage
```

The `->` operator is preferred because it is shorter and clearer.

---

# 9. Structure Pointer + Function

A structure pointer can be passed to a function.

```c
void update_charger(struct charger *ptr)
{
    ptr->voltage = 750U;
    ptr->current = 150U;
}
```

Call:

```c
update_charger(&data);
```

The function can then modify the original structure.

This is useful for:

* Charger configuration
* Diagnostic information
* CAN messages
* Sensor data
* System status structures

---

# 10. `const` with Pointers

`const` is important when data should be read but not modified through a pointer.

## Pointer to constant data

```c
const uint16_t *ptr;
```

Meaning:

```text
Data → cannot be modified through ptr
Pointer → can point somewhere else
```

Example:

```c
const uint16_t *ptr = &voltage;
```

This is not allowed:

```c
*ptr = 700U;    // ❌
```

But the pointer itself can be changed:

```c
ptr = &current; // ✅
```

---

## Constant Pointer

```c
uint16_t *const ptr = &voltage;
```

Meaning:

```text
Pointer → cannot change
Data    → can change
```

Therefore:

```c
*ptr = 700U;    // ✅
```

but:

```c
ptr = &current; // ❌
```

---

## Constant Pointer to Constant Data

```c
const uint16_t *const ptr = &voltage;
```

Meaning:

```text
Pointer → cannot change
Data    → cannot be modified through ptr
```

Therefore:

```c
*ptr = 700U;    // ❌
ptr = &current; // ❌
```

---

# 11. `const` in Function Parameters

If a function only needs to read data:

```c
void print_voltage(const uint16_t *ptr)
{
    printf("Voltage = %u V\n", *ptr);
}
```

`const` prevents accidental modification through that pointer.

Professional interview wording:

> "`const` provides read-only access through the pointer and prevents modification of the referenced data through that pointer."

---

# 12. Common Pointer Mistakes

### Mistake 1 — Printing a value as an address

Wrong:

```c
printf("%p", (void *)voltage);
```

Correct:

```c
printf("%p", (void *)&voltage);
```

`%p` expects a pointer/address.

---

### Mistake 2 — Passing a value instead of its address

If a function expects:

```c
void update_current(uint16_t *ptr);
```

call:

```c
update_current(&current);
```

not:

```c
update_current(current);
```

---

### Mistake 3 — Confusing `ptr` and `*ptr`

```text
ptr
→ address

*ptr
→ value
```

---

### Mistake 4 — Confusing `pptr`, `*pptr`, and `**pptr`

```text
pptr
→ address of pointer

*pptr
→ address stored in pointer

**pptr
→ actual data value
```

---

### Mistake 5 — Confusing `.` and `->`

```text
data.voltage
```

when `data` is a structure.

```text
ptr->voltage
```

when `ptr` is a structure pointer.

---

# 13. EV Charger Application

A charger can maintain its operating parameters using a structure:

```c
struct charger
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};
```

A pointer allows efficient access:

```c
struct charger *ptr = &charger_data;

ptr->voltage
ptr->current
ptr->temperature
```

This approach is useful when implementing:

```text
EV Charger
    │
    ├── Voltage
    ├── Current
    ├── Temperature
    ├── Fault status
    └── Charging state
```

Pointers are heavily used in embedded firmware because they provide direct and efficient access to memory and data structures.

---

# 14. Interview Quick Points

### What is a pointer?

A variable that stores the address of another variable.

### What does `&` do?

Returns the address of a variable.

### What does `*` do?

Dereferences a pointer and accesses the value at the stored address.

### Difference between `.` and `->`?

`.` is used with a structure variable.

`->` is used with a pointer to a structure.

### Why use pointers?

* Modify original data
* Avoid unnecessary copying
* Process arrays/buffers
* Access structures efficiently
* Interface with hardware/registers
* Pass data efficiently to functions

### Why use `const` with pointers?

To prevent modification of data through that pointer and improve code safety.

---

# 15. Day 14 Key Takeaways

```text
ptr       → address
*ptr      → value
&variable → address of variable

data[i]   == *(data + i)

ptr++     → move to next element

**pptr    → access data through two pointer levels

data.member
    ↓
structure variable

ptr->member
    ↓
structure pointer

const pointer
    ↓
protect pointer and/or data depending on placement
```

## Day 14 Status

**Core learning: COMPLETED ✅**

You have covered the major pointer concepts required for the current Embedded C roadmap and practiced them using EV-charger examples.
