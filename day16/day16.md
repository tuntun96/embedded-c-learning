# Day 16 — Professional Notes

## Dynamic Memory Allocation in C

### 1. Overview

Dynamic memory allocation allows a C program to request memory **at runtime** from the heap.

It is useful when the required memory size is not known at compile time or changes during execution.

```c
uint16_t *data;

data = malloc(10 * sizeof(uint16_t));
```

Here, `data` stores the **address** of the dynamically allocated memory.

---

## 2. `malloc()`

`malloc()` allocates a requested number of bytes.

```c
uint16_t *data;

data = malloc(10 * sizeof(uint16_t));
```

Conceptually:

```text
malloc()
   ↓
Heap memory allocated
   ↓
Address returned
   ↓
Pointer stores address
```

### Important

`malloc()` does **not** initialize the allocated memory to a known value.

Always check for allocation failure:

```c
if (data == NULL)
{
    return 1;
}
```

---

## 3. `calloc()`

`calloc()` allocates memory for multiple elements and initializes the allocated memory to zero.

```c
uint16_t *data;

data = calloc(10, sizeof(uint16_t));
```

This allocates:

```text
10 × sizeof(uint16_t)
```

If `uint16_t` is 2 bytes:

```text
10 × 2 = 20 bytes
```

After successful `calloc()`:

```text
data[0] = 0
data[1] = 0
...
data[9] = 0
```

### Main difference

```text
malloc() → allocate
calloc() → allocate + initialize to zero
```

---

## 4. `realloc()`

`realloc()` changes the size of an existing dynamic allocation.

Example:

```c
data = malloc(3 * sizeof(uint16_t));
```

Later:

```c
uint16_t *temp;

temp = realloc(data, 6 * sizeof(uint16_t));

if (temp != NULL)
{
    data = temp;
}
```

The existing data is preserved up to the applicable new size.

### Important

`realloc()` **may move the memory to a different address**.

Therefore, never assume the pointer address remains unchanged.

---

## 5. Safe `realloc()` Pattern

Prefer:

```c
uint16_t *temp;

temp = realloc(data, new_size);

if (temp == NULL)
{
    /* Original data is still valid */
}
else
{
    data = temp;
}
```

Avoid blindly doing:

```c
data = realloc(data, new_size);
```

when failure handling matters.

Otherwise, a failed `realloc()` can cause you to lose the original pointer.

---

## 6. `free()`

When dynamically allocated memory is no longer required:

```c
free(data);
```

Good practice:

```c
free(data);
data = NULL;
```

Remember:

> `free()` does not automatically make the pointer `NULL`.

---

## 7. Pointer and Dynamic Memory

Dynamic memory works directly with pointers.

```c
uint16_t *data;

data = malloc(3 * sizeof(uint16_t));
```

Then:

```c
data[0] = 750U;
data[1] = 150U;
data[2] = 65U;
```

Array indexing and pointer arithmetic are equivalent:

```c
data[1]
```

is equivalent to:

```c
*(data + 1)
```

---

## 8. Allocation Failure

Both `malloc()` and `calloc()` can fail.

```c
data = malloc(size);

if (data == NULL)
{
    /* Handle allocation failure */
}
```

`realloc()` can also fail:

```c
temp = realloc(data, new_size);

if (temp == NULL)
{
    /* data is still valid */
}
```

Never dereference a `NULL` pointer.

---

## 9. Static vs Dynamic Allocation in Embedded Systems

Dynamic allocation is **not automatically better** than static allocation.

If the memory requirement is known:

```c
uint16_t diagnostic_data[100];
```

a fixed allocation may be preferable.

Advantages:

* predictable memory usage
* no runtime allocation failure
* no heap fragmentation
* easier testing
* deterministic behavior

Dynamic allocation is useful when the required size genuinely changes at runtime.

Example:

```text
Diagnostic records:
20 → 100 → 500
```

In such a case, dynamic allocation may be appropriate.

---

## 10. EV Charger Example

A dynamic diagnostic buffer:

```c
uint16_t *charger_data;

charger_data = calloc(3, sizeof(uint16_t));

if (charger_data == NULL)
{
    return 1;
}

charger_data[0] = 750U;
charger_data[1] = 150U;
charger_data[2] = 65U;

free(charger_data);
charger_data = NULL;
```

For a larger runtime requirement:

```c
uint16_t *temp;

temp = realloc(charger_data, 6 * sizeof(uint16_t));

if (temp != NULL)
{
    charger_data = temp;
}
```

---

# Key Engineering Rules

```text
malloc()    → allocate memory
calloc()    → allocate + zero memory
realloc()   → resize allocation
free()      → release allocation
```

And:

```text
malloc/calloc failure → NULL
realloc failure       → original allocation remains valid
free()                → pointer does NOT automatically become NULL
```

### Embedded principle

> **Use dynamic memory when runtime-sized memory is genuinely required, not simply because it is available.**

This helps maintain predictable and reliable firmware behavior.
