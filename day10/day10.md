# Day 10 — Macros and Conditional Compilation

## 1. Learning Objectives

Today, I learned:

* Object-like macros
* Function-like macros
* Why parentheses are important in macros
* Conditional compilation
* `#if`
* `#ifdef`
* `#ifndef`
* `#else`
* `#endif`
* `#undef`
* Debug-mode configuration
* Feature configuration for an EV charger

---

# 2. What Is a Macro?

A macro is defined using the `#define` preprocessor directive.

Example:

```c
#define MAX_VOLTAGE 900U
```

Before compilation, the preprocessor replaces `MAX_VOLTAGE` with `900U`.

Example:

```c
if (voltage > MAX_VOLTAGE)
{
    printf("OVER-VOLTAGE FAULT\n");
}
```

The preprocessor replaces:

```c
MAX_VOLTAGE
```

with:

```c
900U
```

---

# 3. Object-Like Macro

An object-like macro is used to define a constant value.

Example:

```c
#define MAX_VOLTAGE 900U
#define MAX_CURRENT 200U
#define MAX_TEMPERATURE 80U
```

Advantages:

* Avoids magic numbers
* Provides a single point of change
* Improves readability
* Improves maintainability
* Makes configuration easier

Example:

```c
if (voltage > MAX_VOLTAGE)
{
    printf("OVER-VOLTAGE FAULT\n");
}
```

If the voltage limit changes, only the macro value needs to be updated.

---

# 4. Function-Like Macro

A function-like macro accepts parameters and performs an operation.

Example:

```c
#define CALCULATE_POWER(voltage, current) \
    ((uint32_t)(voltage) * (uint32_t)(current))
```

Usage:

```c
power = CALCULATE_POWER(
    voltage,
    current
);
```

For:

```text
Voltage = 650 V
Current = 120 A
```

The power is:

```text
Power = 650 × 120
Power = 78000 W
```

---

# 5. Why Are Parentheses Important?

Incorrect macro:

```c
#define SQUARE(x) x * x
```

If used as:

```c
SQUARE(2 + 3)
```

the result may become:

```c
2 + 3 * 2 + 3
```

This gives an incorrect result because of operator precedence.

Correct macro:

```c
#define SQUARE(x) ((x) * (x))
```

Now:

```c
SQUARE(2 + 3)
```

becomes:

```c
((2 + 3) * (2 + 3))
```

Result:

```text
25
```

Therefore, macro parameters and the complete macro expression should be enclosed in parentheses.

---

# 6. Why Is `uint32_t` Used for Power?

Power is calculated as:

```text
Power = Voltage × Current
```

Example:

```text
800 V × 200 A = 160000 W
```

The maximum value of `uint16_t` is:

```text
65535
```

Since:

```text
160000 > 65535
```

`uint16_t` cannot store the result safely.

Therefore:

```c
uint32_t power;
```

is used.

---

# 7. Overflow

Overflow occurs when a value is larger than the maximum range supported by its data type.

Example:

```c
uint16_t power;
```

If the calculated value is:

```text
160000
```

the value cannot be stored correctly in `uint16_t`.

This may result in data truncation and an incorrect stored value.

---

# 8. Conditional Compilation

Conditional compilation is controlled by preprocessor directives.

The preprocessor decides which code is included before compilation.

Common directives:

```c
#if
#ifdef
#ifndef
#else
#elif
#endif
```

Conditional compilation is useful for:

* Debug and release builds
* Hardware variants
* Product feature configuration
* Enabling or disabling CAN, RFID, display, or other features
* Reducing unnecessary code in a firmware build

---

# 9. `#if`

`#if` checks the value of a macro.

Example:

```c
#define DEBUG_MODE 1

#if DEBUG_MODE

printf(
    "Debug information\n"
);

#endif
```

If:

```c
#define DEBUG_MODE 1
```

the debug code is included.

If:

```c
#define DEBUG_MODE 0
```

the debug code is excluded before compilation.

---

# 10. `#ifdef`

`#ifdef` checks whether a macro is defined.

Example:

```c
#define ENABLE_CAN

#ifdef ENABLE_CAN

printf(
    "CAN Communication Enabled\n"
);

#endif
```

The code is included because `ENABLE_CAN` is defined.

Important:

```c
#define DEBUG_MODE 0
```

still means that `DEBUG_MODE` is defined.

Therefore:

```c
#ifdef DEBUG_MODE
```

is true even when the value is `0`.

---

# 11. `#ifndef`

`#ifndef` means:

> If the macro is not defined.

It is commonly used for header guards.

Example:

```c
#ifndef CHARGER_H
#define CHARGER_H

/* Function declarations */

#endif
```

Header guards prevent a header file from being included multiple times.

---

# 12. `#else`

`#else` provides an alternative section when a condition is false.

Example:

```c
#define DEBUG_MODE 0

#if DEBUG_MODE

printf(
    "Debug Mode Enabled\n"
);

#else

printf(
    "Production Mode Enabled\n"
);

#endif
```

Output:

```text
Production Mode Enabled
```

---

# 13. `#undef`

`#undef` removes a previously defined macro.

Example:

```c
#define DEBUG_MODE 1

#undef DEBUG_MODE
```

After `#undef`, the macro is no longer defined.

Therefore:

```c
#ifdef DEBUG_MODE
```

will be false.

---

# 14. Difference Between `#if` and `#ifdef`

| `#if`                | `#ifdef`                      |
| -------------------- | ----------------------------- |
| Checks a macro value | Checks whether a macro exists |
| Used with `0` or `1` | Used as a feature flag        |
| `#if DEBUG_MODE`     | `#ifdef ENABLE_CAN`           |
| `0` means disabled   | Any defined value means true  |

Example:

```c
#define DEBUG_MODE 0

#if DEBUG_MODE
```

The condition is false.

Example:

```c
#define DEBUG_MODE 0

#ifdef DEBUG_MODE
```

The condition is true because the macro exists.

---

# 15. EV Charger Feature Configuration

Example:

```c
#define ENABLE_CAN 1
#define ENABLE_DISPLAY 1
#define ENABLE_RFID 0
```

Usage:

```c
#if ENABLE_CAN

printf(
    "CAN Communication Enabled\n"
);

#endif
```

The CAN code is included because:

```c
ENABLE_CAN = 1
```

RFID code is excluded because:

```c
ENABLE_RFID = 0
```

This approach can be used to create different EV charger product variants.

---

# 16. Practical Programs Completed

1. EV charger configuration using macros
2. EV charger voltage, current, and temperature checks
3. Function-like macro for power calculation
4. Debug mode using `#if`
5. EV charger feature configuration using:

   * CAN
   * Display
   * RFID

---

# 17. Key Takeaways

* Macros are processed before compilation.
* Object-like macros are useful for constants and configuration values.
* Function-like macros accept parameters.
* Parentheses should be used carefully in function-like macros.
* `uint32_t` should be used when the result can exceed the range of `uint16_t`.
* Overflow occurs when a value exceeds the range of a data type.
* `#if` checks a macro value.
* `#ifdef` checks whether a macro is defined.
* `#ifndef` checks whether a macro is not defined.
* `#undef` removes a previously defined macro.
* Conditional compilation is useful for debug builds, production builds, and EV charger feature variants.

---

# 18. Interview Questions

1. What is a macro in C?
2. What is the difference between a macro and a function?
3. What is an object-like macro?
4. What is a function-like macro?
5. Why are parentheses important in function-like macros?
6. What is conditional compilation?
7. What is the difference between `#if` and `#ifdef`?
8. What is `#ifndef` used for?
9. What does `#undef` do?
10. Why is conditional compilation useful in embedded systems?
11. Why is `uint32_t` used for EV charger power calculation?
12. What is overflow?
13. How can macros be used to create different EV charger product variants?
