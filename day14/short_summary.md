# Day 14 — Pointer Revision Sheet

## 🔑 Pointer Basics

```c
int x = 10;
int *p = &x;
```

```text
p   → address of x
*p  → value of x
&x  → address of x
```

---

## 🔹 Function + Pointer

```c
void update(uint16_t *p)
{
    *p = 120U;
}
```

Call:

```c
update(&current);
```

**Use pointer when a function must modify the original variable.**

---

## 🔹 Pointer + Array

```c
uint16_t data[3] = {650, 120, 45};
uint16_t *p = data;
```

```text
*p          → data[0]
*(p + 1)    → data[1]
*(p + 2)    → data[2]
```

```text
data[i] == *(data + i)
```

---

## 🔹 Pointer Arithmetic

```c
p++;
```

Moves the pointer to the **next element of its data type**.

---

## 🔹 Pointer-to-Pointer

```c
uint16_t **pp;
```

```text
pp      → address of pointer
*pp     → pointer/address of data
**pp    → actual data value
```

---

## 🔹 Structure Pointer

```c
struct charger *p;
```

Normal structure:

```c
data.voltage
```

Structure pointer:

```c
p->voltage
```

Equivalent:

```c
p->voltage
(*p).voltage
```

---

## 🔹 `const` Pointer

### Pointer to constant data

```c
const uint16_t *p;
```

```text
Data ❌ modify
Pointer ✅ change
```

### Constant pointer

```c
uint16_t *const p;
```

```text
Data ✅ modify
Pointer ❌ change
```

### Both constant

```c
const uint16_t *const p;
```

```text
Data ❌ modify
Pointer ❌ change
```

---

## 🚗 Embedded C Reminder

Pointers are heavily used for:

```text
✓ Buffers
✓ CAN data
✓ UART data
✓ Modbus frames
✓ Structures
✓ Hardware/register access
✓ Efficient function parameters
```

### ⭐ Interview Rule

```text
&  → address
*  → value
.  → structure variable
-> → structure pointer
```

**Day 14: COMPLETED ✅**
