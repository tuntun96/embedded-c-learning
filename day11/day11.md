# Day 11 — `const`, `volatile`, and `static` in Embedded C

## Learning Objectives

By the end of Day 11, I learned:

* The purpose of the `const` keyword
* The purpose of the `volatile` keyword
* The difference between `const` and `volatile`
* How a static local variable retains its value between function calls
* How file-level `static` makes variables and functions private to a source file
* How to use these keywords in EV charger firmware

---

# 1. `const` Keyword

## Definition

The `const` keyword is used to declare a value that should not be modified through that variable after initialization.

### Syntax

```c
const data_type variable_name = value;
```

### Example

```c
const uint16_t max_voltage = 900U;
```

In this example:

* `const` indicates that the value should not be modified through `max_voltage`.
* `uint16_t` specifies an unsigned 16-bit data type.
* `max_voltage` stores the maximum allowed voltage.
* `900U` is the initial value.

The value can be read:

```c
printf(
  "Maximum Voltage: %u V\n",
  (unsigned int)max_voltage
);
```

However, modifying it is not allowed:

```c
max_voltage = 950U;
```

This produces a compiler error because `max_voltage` is declared as `const`.

---

## EV Charger Example

```c
const uint16_t max_voltage = 900U;
const uint16_t max_current = 200U;
const uint8_t max_temperature = 80U;
```

These values can be used as safety limits:

```c
if (voltage > max_voltage)
{
  printf(
    "Voltage Status: OVER-VOLTAGE FAULT\n"
  );
}
else
{
  printf(
    "Voltage Status: NORMAL\n"
  );
}
```

---

## Why `const` Is Useful in Embedded Systems

`const` helps:

* Prevent accidental modification of fixed values
* Protect configuration values
* Improve code readability
* Improve code safety
* Clearly indicate that a value is read-only through that declaration

Examples in EV charger firmware:

* Maximum voltage limit
* Maximum current limit
* Maximum temperature limit
* Fixed calibration values
* Read-only configuration values

---

## `const` vs `#define`

| `const`                           | `#define`                                     |
| --------------------------------- | --------------------------------------------- |
| Typed C object                    | Preprocessor macro                            |
| Processed by the compiler         | Processed before compilation                  |
| Has a data type                   | Does not have a C data type                   |
| Uses normal C syntax              | Uses preprocessor syntax                      |
| Useful for typed read-only values | Useful for macros and conditional compilation |

Example:

```c
#define MAX_VOLTAGE 900U
```

```c
const uint16_t max_voltage = 900U;
```

Both can represent a limit, but they work differently.

---

# 2. `volatile` Keyword

## Definition

The `volatile` keyword tells the compiler:

> The value may change unexpectedly outside the normal program flow, so the compiler must not assume that a previously read value is still valid.

### Syntax

```c
volatile data_type variable_name;
```

### Example

```c
volatile uint16_t charger_voltage;
```

The value may be updated by:

* Hardware peripherals
* Interrupt Service Routines (ISRs)
* DMA
* Another execution context
* External hardware events

---

## EV Charger Example

```c
volatile uint16_t charger_voltage = 950U;

const uint16_t max_voltage = 900U;
```

The voltage can be checked using:

```c
if (charger_voltage > max_voltage)
{
  printf(
    "Voltage Status: OVER-VOLTAGE FAULT\n"
  );
}
else
{
  printf(
    "Voltage Status: NORMAL\n"
  );
}
```

---

## Why `volatile` Is Important

Suppose a variable is updated by an interrupt:

```c
volatile uint8_t charging_complete = 0U;
```

The main program waits for the value to change:

```c
while (charging_complete == 0U)
{
  /* Wait for charging completion */
}
```

The interrupt may update the variable:

```c
charging_complete = 1U;
```

Because the variable is `volatile`, the compiler must treat its value as capable of changing unexpectedly.

---

## Important Limitations of `volatile`

`volatile` does not:

* Make code thread-safe
* Prevent race conditions
* Make operations atomic
* Synchronize multiple tasks
* Protect a variable from modification

`volatile` only informs the compiler that the value may change unexpectedly.

---

# 3. `const volatile`

A variable can use both keywords:

```c
const volatile uint16_t status_register;
```

Meaning:

* `const` → The program should not modify the value through this declaration.
* `volatile` → The value may change unexpectedly because of hardware or another external source.

This can be used for a read-only hardware status register.

The firmware can read:

```c
uint16_t status;

status = status_register;
```

The firmware should not write:

```c
status_register = 1U;
```

---

# 4. `static` Local Variable

## Definition

A static local variable:

* Is initialized only once
* Retains its value between function calls
* Is accessible only inside the function where it is declared

### Syntax

```c
static data_type variable_name = initial_value;
```

### Example

```c
static uint32_t session_count = 0U;
```

---

## EV Charger Session Counter

```c
void update_session_count(void)
{
  static uint32_t session_count = 0U;

  session_count++;

  printf(
    "Total Charging Sessions: %u\n",
    (unsigned int)session_count
  );
}
```

Calling the function five times:

```c
update_session_count();
update_session_count();
update_session_count();
update_session_count();
update_session_count();
```

Output:

```text
Total Charging Sessions: 1
Total Charging Sessions: 2
Total Charging Sessions: 3
Total Charging Sessions: 4
Total Charging Sessions: 5
```

The value is retained after every function call.

---

## Normal Local Variable vs Static Local Variable

### Normal Local Variable

```c
void count(void)
{
  uint8_t value = 0U;

  value++;

  printf(
    "%u\n",
    (unsigned int)value
  );
}
```

If the function is called three times:

```text
1
1
1
```

The variable is initialized to `0U` during every function call.

### Static Local Variable

```c
void count(void)
{
  static uint8_t value = 0U;

  value++;

  printf(
    "%u\n",
    (unsigned int)value
  );
}
```

If the function is called three times:

```text
1
2
3
```

The variable retains its previous value.

---

## EV Charger Uses of Static Local Variables

Static local variables can be used for:

* Charging session counters
* Fault occurrence counters
* Communication retry counters
* Function-call counters
* Timeout counters
* State tracking

---

# 5. File-Level `static`

## Definition

When `static` is used outside a function, it limits the visibility of a variable or function to the current `.c` file.

This is called internal linkage.

### Example: Static Variable

Inside `charger.c`:

```c
static uint16_t charger_voltage = 650U;
```

The variable can be accessed inside `charger.c`, but it cannot be directly accessed from `main.c`.

---

## File-Level Static Function

```c
static void check_internal_fault(void)
{
  printf(
    "Internal Fault Check: PASSED\n"
  );
}
```

This function can be called only inside the source file where it is declared and defined.

For example:

```c
void charger_monitor(void)
{
  check_internal_fault();
}
```

The public function can call the private helper function.

---

## Multi-File EV Charger Example

### `charger.h`

```c
#ifndef CHARGER_H
#define CHARGER_H

/* Public function declaration */
void charger_monitor(void);

#endif
```

Only the public function is declared in the header.

---

### `charger.c`

```c
#include <stdio.h>

#include "charger.h"

/* Private function declaration */
static void check_internal_fault(void);

/* Public function */
void charger_monitor(void)
{
  printf(
    "EV Charger Monitoring Started\n"
  );

  check_internal_fault();
}

/* Private helper function */
static void check_internal_fault(void)
{
  printf(
    "Internal Fault Check: PASSED\n"
  );
}
```

---

### `main.c`

```c
#include "charger.h"

int main(void)
{
  charger_monitor();

  return 0;
}
```

---

## Program Flow

```text
main.c
   |
   | Calls public function
   v
charger_monitor()
   |
   | Calls private helper function
   v
check_internal_fault()
```

`main.c` can call:

```c
charger_monitor();
```

`main.c` cannot directly call:

```c
check_internal_fault();
```

because the function is private to `charger.c`.

---

## Why File-Level `static` Is Useful

File-level `static` helps:

* Hide internal implementation details
* Protect internal module data
* Improve modularity
* Reduce accidental access from other source files
* Improve code organization
* Reduce unwanted external dependencies

---

# Important Memory Rule

```text
static inside a function
→ Retains its value between function calls

static outside a function
→ Private to the current .c file
```

---

# Common Mistakes

## Mistake 1: Modifying a `const` Variable

Incorrect:

```c
const uint16_t max_voltage = 900U;

max_voltage = 950U;
```

This causes a compiler error.

---

## Mistake 2: Assuming `volatile` Makes Code Thread-Safe

Incorrect assumption:

```text
volatile = thread-safe
```

Correct:

```text
volatile informs the compiler that the value may change unexpectedly.
```

---

## Mistake 3: Expecting a Normal Local Variable to Retain Its Value

```c
uint8_t count = 0U;
```

This value is initialized again every time the function is called.

Use:

```c
static uint8_t count = 0U;
```

when the value must be retained between function calls.

---

## Mistake 4: Declaring a Private Static Function in a Header

Incorrect:

```c
/* charger.h */

static void check_internal_fault(void);
```

A private helper function should normally remain inside the corresponding `.c` file.

Correct:

```c
/* charger.c */

static void check_internal_fault(void);
```

---

# Interview Questions

### Q1. What is the purpose of `const`?

`const` indicates that a value should not be modified through that variable after initialization.

### Q2. What is the purpose of `volatile`?

`volatile` tells the compiler that a variable may change unexpectedly due to hardware, an ISR, DMA, or another execution context.

### Q3. Does `volatile` make code thread-safe?

No. `volatile` does not provide thread safety, atomicity, or race-condition protection.

### Q4. What is a static local variable?

A static local variable retains its value between function calls while remaining accessible only inside its function.

### Q5. What is file-level `static`?

File-level `static` limits a variable or function to the current `.c` file.

### Q6. Why use file-level `static` functions?

They hide internal helper functions and improve modularity and encapsulation.

### Q7. What does `const volatile` mean?

The program should not modify the value through that declaration, but the value may be changed by hardware or another external source.

---

# Day 11 Summary

| Keyword                    | Main Purpose                                          | Embedded Example              |
| -------------------------- | ----------------------------------------------------- | ----------------------------- |
| `const`                    | Prevents modification through a variable              | Maximum voltage limit         |
| `volatile`                 | Indicates that a value may change unexpectedly        | ADC measurement or ISR flag   |
| `static` inside a function | Retains a value between function calls                | Charging session counter      |
| File-level `static`        | Makes a variable or function private to one `.c` file | Internal fault-check function |

---

# Day 11 Completion Status

```text
const                              ✅ Completed
volatile                           ✅ Completed
const volatile                     ✅ Completed
Static local variables             ✅ Completed
File-level static variables        ✅ Completed
File-level static functions        ✅ Completed
EV charger examples                ✅ Completed
Multi-file static project          ✅ Completed
Concept checks                     ✅ Completed
```

## Final Key Takeaway

```text
const
→ Do not modify through this variable

volatile
→ The value may change unexpectedly

static inside a function
→ Retains its value between function calls

static outside a function
→ Private to the current .c file
```
