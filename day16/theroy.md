# Day 16 — Dynamic Memory Allocation

## Today's goal

By the end of today's learning:

* Why dynamic memory is needed
* What `malloc()` does
* How `malloc()` works with pointers
* How to check allocation failure
* How to access dynamically allocated memory
* How `free()` releases memory
* Static allocation vs dynamic allocation

We'll leave `calloc()`, `realloc()`, memory leaks, dangling pointers, fragmentation, etc. for the next part if needed.

---

# 1. Why do we need dynamic memory?

So far, we've mostly used fixed-size memory:

```c
uint16_t voltage;
uint16_t buffer[100];
```

The size is known when we write the program.

But imagine:

EV charger receives:
Number of CAN messages = unknown
Number of diagnostic records = unknown
Number of users = unknown

We may not know at compile time how much memory we need.

For example:
uint16_t buffer[1000];

This reserves space for 1000 elements, even if we only need 100.

Dynamic allocation allows the program to request memory at runtime.

---

# 2. Static allocation vs dynamic allocation

### Fixed/static approach

uint16_t buffer[100];

Memory is reserved with a known size.

Program
   ↓
100 × uint16_t
   ↓
fixed memory

### Dynamic approach

uint16_t *buffer;

buffer = malloc(100 * sizeof(uint16_t));

Now the program requests the required memory at runtime.

Program
   ↓
malloc()
   ↓
Heap
   ↓
100 × uint16_t

The pointer stores the address of the allocated block.

---

# 3. What is `malloc()`?

`malloc()` means:

> Allocate memory dynamically.

It is declared in:

#include <stdlib.h>

Basic syntax:

ptr = malloc(size);

Example:
uint16_t *buffer;

buffer = malloc(100 * sizeof(uint16_t));

This requests enough heap memory for:

100 × sizeof(uint16_t);

elements.

If `uint16_t` is 2 bytes:

100 × 2 = 200 bytes

---

# 4. Why do we use `sizeof()`?

Don't write:

buffer = malloc(200);

if the intention is 100 `uint16_t` elements.

Prefer:

buffer = malloc(100 * sizeof(uint16_t));

Why?

Because `sizeof(uint16_t)` automatically represents the size of that type on the platform.

This is safer and clearer.

---

# 5. What does `malloc()` return?

This is extremely important.

`malloc()` returns the address of the allocated memory block.

Conceptually:

Heap

0x5000 ┌───────────────┐
       │               │
       │ allocated     │
       │ memory        │
       │               │
       └───────────────┘
            ↑
            │
          buffer

So:

uint16_t *buffer;

buffer = malloc(100 * sizeof(uint16_t));

means:

buffer
  ↓
stores address of dynamically allocated memory

This connects directly to what we learned about pointers.

---

# 6. Very important: `malloc()` returns `void *`

The actual function conceptually looks like:

void *malloc(size_t size);

`void *` means a generic pointer.

So:

uint16_t *buffer;

buffer = malloc(100 * sizeof(uint16_t));

The returned memory address is assigned to `buffer`.

In C, explicit casting of `malloc()` is normally unnecessary.

Prefer:

buffer = malloc(100 * sizeof(uint16_t));

rather than:

buffer = (uint16_t *)malloc(100 * sizeof(uint16_t));

---

# 7. Always check for allocation failure

This is essential.

`malloc()` can fail.

If it cannot provide the requested memory, it returns:

NULL

Therefore:

uint16_t *buffer;

buffer = malloc(100 * sizeof(uint16_t));

if (buffer == NULL)
{
    printf("Memory allocation failed\n");
    return 1;
}

If allocation succeeds:

buffer != NULL

If allocation fails:

buffer == NULL

---

# 8. Using dynamically allocated memory

Once allocation succeeds, we can use the pointer just like an array.

buffer[0] = 650U;
buffer[1] = 120U;
buffer[2] = 65U;
```

we can also use pointer arithmetic:

*(buffer + 0) = 650U;
*(buffer + 1) = 120U;
*(buffer + 2) = 65U;

These are equivalent:

buffer[1]

and:

*(buffer + 1)
```

We've already learned this relationship in Day 14/earlier pointer work.

---

# 9. Example — EV Charger

Imagine dynamically creating a diagnostic buffer:

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
    uint16_t *diagnostic_data;

    diagnostic_data = malloc(3 * sizeof(uint16_t));

    if (diagnostic_data == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    diagnostic_data[0] = 750U;
    diagnostic_data[1] = 150U;
    diagnostic_data[2] = 65U;

    printf("Voltage     = %u V\n", diagnostic_data[0]);
    printf("Current     = %u A\n", diagnostic_data[1]);
    printf("Temperature = %u C\n", diagnostic_data[2]);

    free(diagnostic_data);

    return 0;
}

The flow is:

Declare pointer
      ↓
malloc()
      ↓
Check NULL
      ↓
Use allocated memory
      ↓
free()

---

# 10. What does `free()` do?

After we finished with dynamically allocated memory:


free(diagnostic_data);

This tells the memory allocator:

> I no longer need this allocated block.

Conceptually:

Before free():

Heap
┌──────────────────┐
│ diagnostic_data  │ ← allocated
└──────────────────┘


After free():

Heap
┌──────────────────┐
│      free        │
└──────────────────┘


---

# 11. `free()` does NOT erase the pointer

This is an important point.

After:

free(diagnostic_data);

the pointer variable still contains the old address.

So it is good practice to do:

free(diagnostic_data);
diagnostic_data = NULL;

Now:

diagnostic_data == NULL

This helps prevent accidental use of the released memory.

We'll study dangling pointers in more detail later.

---

# 12. Today's mental model

Remember this:


             POINTER
                │
                │ address
                ↓
          ┌──────────────┐
          │    HEAP      │
          │              │
          │ allocated    │
          │ memory       │
          │              │
          └──────────────┘
                ↑
              malloc()
                │
              request

              free()
                ↓
        memory returned
        to allocator

### The four key steps

uint16_t *buffer;

buffer = malloc(100 * sizeof(uint16_t));

if (buffer == NULL)
{
    /* allocation failed */
}

/* use buffer */

free(buffer);
buffer = NULL;

---

# 🎯 Your first Day 16 questions

Answer these before we write the hands-on program.

Q1. Where does memory allocated by `malloc()` normally come from?

Q2. What does `malloc()` return?

Q3. Why do we write:

malloc(100 * sizeof(uint16_t))

instead of simply:
malloc(100)

Q4. What does `malloc()` return if allocation fails?

Q5. Why do we use `free()`?

Q6. After:

free(buffer);

does the pointer variable automatically become `NULL`?

Q7. What is the relationship between:

buffer[2]

and:

*(buffer + 2)

-------------------------------------------------------------------------------------------
# 2. `calloc()` — Contiguous Allocation

`calloc()` is another function from:

#include <stdlib.h>

Its purpose is also to dynamically allocate memory.

The important difference is:

> **`calloc()` allocates memory and initializes all allocated bytes to zero.**

---

## 1. `malloc()` vs `calloc()`

### `malloc()`

uint16_t *data;

data = malloc(3 * sizeof(uint16_t));

Memory is allocated, but its initial contents should **not be relied upon**.

Conceptually:

Heap
┌──────┬──────┬──────┐
│ ???  │ ???  │ ???  │
└──────┴──────┴──────┘


### `calloc()`

uint16_t *data;

data = calloc(3, sizeof(uint16_t));

Memory is allocated and initialized to zero.

Heap
┌──────┬──────┬──────┐
│  0   │  0   │  0   │
└──────┴──────┴──────┘

---

# 2. Syntax

`malloc()`:

ptr = malloc(total_size);

`calloc()`:

ptr = calloc(number_of_elements, size_of_each_element);

For example:

uint16_t *charger_data;

charger_data = calloc(3, sizeof(uint16_t));

This means:

3 elements
×
sizeof(uint16_t)

---

# 3. Why is `calloc()` useful?

Suppose we create a diagnostic structure:

uint16_t *charger_data;

charger_data = calloc(3, sizeof(uint16_t));

Immediately after allocation:

charger_data[0] = 0
charger_data[1] = 0
charger_data[2] = 0

We can then update only the values you actually receive:

charger_data[0] = 750U;
charger_data[1] = 150U;

Temperature remains:

charger_data[2] = 0U;


This can be useful when a buffer needs a known zero state.

---

# 4. Important difference

Don't confuse these:

```c
malloc(10 * sizeof(uint16_t));
```

with:

```c
calloc(10, sizeof(uint16_t));
```

Both allocate space for 10 `uint16_t` elements.

But:

malloc()
→ allocation only
→ contents are indeterminate

calloc()
→ allocation
→ memory initialized to zero

---

# 5. `calloc()` still can fail

Just like `malloc()`:

```c
uint16_t *data;

data = calloc(3, sizeof(uint16_t));

if (data == NULL)
{
    printf("Memory allocation failed\n");
    return 1;
}
```

Always check the returned pointer before using it.

---

# 6. `free()` works with `calloc()`

After we finished:

```c
free(data);
data = NULL;
```

The same `free()` function is used for memory allocated by:

malloc()
calloc()
realloc()

We'll learn `realloc()` separately.

---

# 7. EV Charger Example

Suppose we want a dynamically allocated diagnostic buffer:

```c
uint16_t *charger_data;

charger_data = calloc(3, sizeof(uint16_t));

if (charger_data == NULL)
{
    printf("Allocation failed\n");
    return 1;
}
```

Initially:

Voltage     = 0
Current     = 0
Temperature = 0

Then:

```c
charger_data[0] = 750U;
charger_data[1] = 150U;
charger_data[2] = 65U;
```

Now:

Voltage     = 750
Current     = 150
Temperature = 65

Finally:

```c
free(charger_data);
charger_data = NULL;
```

---

# 8. One Embedded-C warning

Although `calloc()` is convenient, **zero initialization does not automatically make data logically valid**.

For example:

```c
charger_data[0] = 0;
```

doesn't necessarily mean:

> "EV charger voltage is actually 0 V."

It could simply mean:

> "No voltage measurement has been received yet."

In real firmware, you may need a separate state/validity flag.

For example:

```c
bool voltage_valid;
```

This is an important engineering distinction.

---

# Quick Check

Answer these before the hands-on exercise:

**Q1.** What is the main difference between `malloc()` and `calloc()`?

**Q2.** What does this allocate?

```c
calloc(5, sizeof(uint16_t));
```

**Q3.** What are the initial values of the allocated `uint16_t` elements after `calloc()`?

**Q4.** Can `calloc()` return `NULL`?

**Q5.** Which function should you use to release memory allocated by `calloc()`?

**Q6.** If `charger_data[2] == 0` after `calloc()`, can you automatically conclude that the EV charger temperature is actually 0°C? Explain why.


## 1. Why do we need `realloc()`?

Suppose initially we allocate space for 3 charger measurements:

uint16_t *data;

data = malloc(3 * sizeof(uint16_t));

Now you need 6 measurements.

We could:
1. Allocate a new 6-element block.
2. Copy the old data.
3. Free the old block.

But C provides `realloc()` to resize an existing dynamically allocated block.

data = realloc(data, 6 * sizeof(uint16_t));

Conceptually:

Before:

Heap
┌─────┬─────┬─────┐
│ 750 │ 150 │  65 │
└─────┴─────┴─────┘


After realloc():

Heap
┌─────┬─────┬─────┬─────┬─────┬─────┐
│ 750 │ 150 │  65 │ ??? │ ??? │ ??? │
└─────┴─────┴─────┴─────┴─────┴─────┘
```

The existing data is preserved up to the amount that fits in the new block.

---

## 2. Syntax


ptr = realloc(ptr, new_size);

Example:

data = realloc(data, 6 * sizeof(uint16_t));

`new_size` is specified in bytes.

---

## 3. `realloc()` can move the memory

This is very important.

Suppose:

Before:

0x5000
┌─────────────┐
│ old memory  │
└─────────────┘
      ↑
     ptr

There might not be enough free space immediately after it.

The allocator could move it:

After:

0x8000
┌─────────────────────────┐
│ resized memory          │
└─────────────────────────┘
             ↑
            ptr

So we should never assume the address remains unchanged.

---

# 4. ⚠️ Important `realloc()` mistake

Avoid directly doing this:

data = realloc(data, new_size);


if we need to safely handle allocation failure.

Why?

Suppose `realloc()` fails.

It returns:

NULL

and our original pointer could be lost if you overwrite it.

Instead:

uint16_t *temp;

temp = realloc(data, new_size);

if (temp == NULL)
{
    printf("Reallocation failed\n");
}
else
{
    data = temp;
}
```

This preserves the original pointer if the resize fails.

This is an important practical C pattern.

---

# 5. Example

```c
uint16_t *data;
uint16_t *temp;

data = malloc(3 * sizeof(uint16_t));

if (data == NULL)
{
    return 1;
}

data[0] = 750U;
data[1] = 150U;
data[2] = 65U;

temp = realloc(data, 6 * sizeof(uint16_t));

if (temp == NULL)
{
    printf("Reallocation failed\n");
    free(data);
    data = NULL;
    return 1;
}

data = temp;

data[3] = 230U;
data[4] = 240U;
data[5] = 250U;
```

Now we have:

data[0] = 750
data[1] = 150
data[2] = 65
data[3] = 230
data[4] = 240
data[5] = 250

---

## 6. `realloc()` doesn't initialize new memory to zero

This is another important difference.

If you resize:

data = realloc(data, 6 * sizeof(uint16_t));

the newly added portion should **not be assumed to be zero**.

If we need known values, initialize them yourself:

```c
data[3] = 0U;
data[4] = 0U;
data[5] = 0U;
```

---

# Your quick check

**Q1.** What is the main purpose of `realloc()`?

**Q2.** If you increase a 3-element allocation to 6 elements, are the original 3 values preserved?

**Q3.** Can `realloc()` move the memory to a different address?

**Q4.** What happens if `realloc()` fails?

**Q5.** Why is this pattern safer?

```c
temp = realloc(data, new_size);

if (temp != NULL)
{
    data = temp;
}
```

**Q6.** Are the newly allocated bytes guaranteed to be zero after `realloc()`?





