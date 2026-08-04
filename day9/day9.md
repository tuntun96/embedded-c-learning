# Embedded C Learning — Day 9

## Topic: Functions, Modular Programming, Multi-File Projects, Header Files, and Pass by Reference

---

# 1. Day 9 Learning Objectives

By the end of Day 9, I should be able to:

* Understand the purpose of functions in C
* Create and use function prototypes
* Understand function parameters and return values
* Use different types of functions
* Write reusable and modular code
* Separate a C project into multiple files
* Understand the role of `.c` and `.h` files
* Use header guards
* Compile multiple C source files using GCC
* Understand pass by value
* Use pointers to update original variables
* Build and test a modular EV charger monitoring application

---

# 2. What Is a Function?

A **function** is a named block of code designed to perform a specific task.

Instead of writing the same code multiple times, we can write the code once inside a function and call the function whenever required.

Example:

```c
void charger_init(void)
{
    printf(
        "EV Charger Initialized\n"
    );
}
```

Function call:

```c
charger_init();
```

Output:

```text
EV Charger Initialized
```

## Benefits of Functions

Functions improve:

* Code reusability
* Modularity
* Readability
* Maintainability
* Testing
* Debugging
* Code organization

### EV Charger Example

Instead of writing all charger operations inside `main()`, separate them into functions:

```c
charger_init();

voltage = read_voltage();

power = calculate_power(
    voltage,
    current
);

fault_status = check_over_voltage(
    voltage
);
```

Each function performs one specific task.

---

# 3. Function Syntax

General syntax:

```c
return_type function_name(
    parameters
)
{
    /* Function body */

    return value;
}
```

Example:

```c
uint16_t read_voltage(void)
{
    return 650U;
}
```

Explanation:

* `uint16_t` → Return type
* `read_voltage` → Function name
* `void` → No input parameters
* `return 650U` → Returns a voltage value

---

# 4. Function Prototype

A **function prototype** tells the compiler:

* Function name
* Return type
* Number of parameters
* Parameter data types

Example:

```c
uint16_t read_voltage(void);
```

The function prototype is usually placed before `main()` or inside a header file.

Example:

```c
void charger_init(void);

uint16_t read_voltage(void);

uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
);
```

---

# 5. Function Definition

A **function definition** contains the actual code that performs the task.

Example:

```c
uint16_t read_voltage(void)
{
    return 650U;
}
```

Difference:

```text
Function declaration:
Tells the compiler that the function exists.

Function definition:
Contains the actual implementation of the function.
```

---

# 6. Function Call

A function call executes the function.

Example:

```c
charger_init();
```

Function with a return value:

```c
voltage = read_voltage();
```

The returned value is stored in the `voltage` variable.

---

# 7. Function Parameters

Parameters allow data to be passed into a function.

Example:

```c
uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
)
{
    return (uint32_t)voltage * current;
}
```

Function call:

```c
power = calculate_power(
    voltage,
    current
);
```

The values of `voltage` and `current` are passed to the function.

---

# 8. Function Return Value

A return value sends data from a function back to the calling function.

Example:

```c
uint16_t read_voltage(void)
{
    return 650U;
}
```

Function call:

```c
voltage = read_voltage();
```

The value `650U` is returned and stored in `voltage`.

---

# 9. Four Types of Functions

## Type 1: No Parameters and No Return Value

Example:

```c
void charger_init(void)
{
    printf(
        "EV Charger Initialized\n"
    );
}
```

Call:

```c
charger_init();
```

---

## Type 2: Parameters and No Return Value

Example:

```c
void display_voltage(
    uint16_t voltage
)
{
    printf(
        "Voltage: %u V\n",
        (unsigned int)voltage
    );
}
```

Call:

```c
display_voltage(
    voltage
);
```

---

## Type 3: No Parameters and Return Value

Example:

```c
uint16_t read_voltage(void)
{
    return 650U;
}
```

Call:

```c
voltage = read_voltage();
```

---

## Type 4: Parameters and Return Value

Example:

```c
uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
)
{
    return (uint32_t)voltage * current;
}
```

Call:

```c
power = calculate_power(
    voltage,
    current
);
```

---

# 10. Why Use `uint32_t` for Power?

Power is calculated as:

```text
Power = Voltage × Current
```

Example:

```text
650 × 120 = 78000 W
```

A `uint16_t` can store values from:

```text
0 to 65535
```

Since `78000` is greater than `65535`, `uint16_t` is not sufficient.

Therefore:

```c
uint32_t power;
```

is used.

The multiplication is written as:

```c
return (uint32_t)voltage * current;
```

The cast ensures that the multiplication is performed using a wider unsigned type.

---

# 11. Modular Programming

**Modular programming** means dividing a large program into smaller modules.

Example:

```text
EV Charger Application

├── Charger initialization
├── Voltage reading
├── Voltage update
├── Power calculation
├── Fault checking
└── Data display
```

Benefits:

* Easier to understand
* Easier to test
* Easier to debug
* Easier to maintain
* Functions can be reused
* Different developers can work on different modules

---

# 12. Multi-File Programming

A professional embedded project is usually divided into multiple files.

Day 9 project structure:

```text
day9/
│
├── main.c
├── charger.c
└── charger.h
```

---

# 13. Role of `main.c`

`main.c` contains the `main()` function.

The `main()` function is the entry point of the application.

`main.c` is responsible for:

* Declaring application variables
* Calling functions
* Controlling the program flow
* Processing results
* Displaying data

Example:

```c
int main(void)
{
    uint16_t voltage;
    uint16_t current = 120U;
    uint32_t power;
    uint8_t fault_status;

    charger_init();

    voltage = read_voltage();

    update_voltage(
        &voltage,
        700U
    );

    fault_status = check_over_voltage(
        voltage
    );

    power = calculate_power(
        voltage,
        current
    );

    return 0;
}
```

---

# 14. Role of `charger.c`

`charger.c` contains the **function definitions** or implementations.

Example:

```c
uint16_t read_voltage(void)
{
    return 650U;
}
```

Example:

```c
uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
)
{
    return (uint32_t)voltage * current;
}
```

The actual charger-related logic is implemented in `charger.c`.

---

# 15. Role of `charger.h`

`charger.h` contains:

* Function declarations
* Shared data types
* Macros
* Constants
* Structure declarations
* Enumeration declarations
* Header guards

Example:

```c
#ifndef CHARGER_H
#define CHARGER_H

#include <stdint.h>

void charger_init(void);

uint16_t read_voltage(void);

uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
);

uint8_t check_over_voltage(
    uint16_t voltage
);

void update_voltage(
    uint16_t *voltage,
    uint16_t new_voltage
);

#endif
```

---

# 16. Header Guards

Header guards prevent a header file from being included and processed multiple times.

Syntax:

```c
#ifndef CHARGER_H
#define CHARGER_H

/* Header file content */

#endif
```

Meaning:

```text
If CHARGER_H is not defined:
    Define CHARGER_H
    Process the header content

If CHARGER_H is already defined:
    Skip the header content
```

Benefits:

* Prevents multiple declarations
* Prevents redefinition errors
* Makes header files safe to include from multiple source files

---

# 17. Including a Header File

Use:

```c
#include "charger.h"
```

Double quotes are generally used for project header files.

Example:

```c
#include <stdio.h>
#include <stdint.h>

#include "charger.h"
```

Angle brackets are generally used for standard library headers:

```c
#include <stdio.h>
#include <stdint.h>
```

---

# 18. Pass by Value

In C, function arguments are passed by value.

Example:

```c
uint8_t check_over_voltage(
    uint16_t voltage
);
```

Function call:

```c
fault_status = check_over_voltage(
    voltage
);
```

The function receives a copy of the value.

Example:

```text
Original value in main.c:

voltage = 650U

Function receives:

Copy of voltage = 650U
```

If the function changes its local parameter:

```c
voltage = 950U;
```

the original variable in `main.c` does not change.

---

# 19. Pass by Reference Using Pointers

C does not have true pass-by-reference parameters. However, we can pass the address of a variable to a function using pointers.

Example:

```c
void update_voltage(
    uint16_t *voltage,
    uint16_t new_voltage
)
{
    *voltage = new_voltage;
}
```

Function call:

```c
update_voltage(
    &voltage,
    700U
);
```

Explanation:

```text
&voltage
    ↓
Passes the address of voltage

uint16_t *voltage
    ↓
Receives the address

*voltage
    ↓
Accesses the original value stored at that address
```

The function changes the original variable.

Before:

```text
voltage = 650U
```

After:

```text
voltage = 700U
```

---

# 20. Pointer Operators

## Address-of Operator: `&`

Example:

```c
&voltage
```

Meaning:

> Get the memory address of `voltage`.

---

## Dereference Operator: `*`

Example:

```c
*voltage = 700U;
```

Meaning:

> Access the value stored at the address and update it to `700U`.

---

# 21. Why Pass an Address?

Passing an address allows a function to update the original variable.

This is useful in embedded software for:

* Updating sensor values
* Updating charger status
* Updating fault information
* Updating CAN message data
* Filling communication buffers
* Returning multiple output values
* Avoiding copies of large structures

Example:

```c
void read_sensor(
    uint16_t *sensor_value
)
{
    *sensor_value = 500U;
}
```

Call:

```c
read_sensor(
    &sensor_value
);
```

---

# 22. Reusable Voltage Update Function

Less reusable version:

```c
void update_voltage(
    uint16_t *voltage
)
{
    *voltage = 700U;
}
```

This always updates the voltage to `700U`.

More reusable version:

```c
void update_voltage(
    uint16_t *voltage,
    uint16_t new_voltage
)
{
    *voltage = new_voltage;
}
```

Call:

```c
update_voltage(
    &voltage,
    700U
);
```

Later, the same function can be used with another value:

```c
update_voltage(
    &voltage,
    850U
);
```

Benefits:

* Reusable
* Flexible
* Easy to maintain
* No need to modify the function implementation for every new voltage value

---

# 23. Over-Voltage Detection

Function declaration:

```c
uint8_t check_over_voltage(
    uint16_t voltage
);
```

Function definition:

```c
uint8_t check_over_voltage(
    uint16_t voltage
)
{
    if (voltage > 900U)
    {
        return 1U;
    }

    return 0U;
}
```

Logic:

```text
If voltage > 900 V:
    Return 1 → Over-voltage fault

Otherwise:
    Return 0 → Normal
```

Test cases:

| Voltage | Expected Result |
| ------: | --------------- |
|  `650U` | Normal          |
|  `700U` | Normal          |
|  `900U` | Normal          |
|  `901U` | Over-voltage    |
|  `950U` | Over-voltage    |

---

# 24. Final Project Flow

```text
Start
  ↓
charger_init()
  ↓
EV Charger Initialized
  ↓
read_voltage()
  ↓
Voltage = 650 V
  ↓
update_voltage(&voltage, 700U)
  ↓
Voltage = 700 V
  ↓
check_over_voltage(700)
  ↓
Fault Status = NORMAL
  ↓
calculate_power(700, 120)
  ↓
Power = 84000 W
  ↓
Display charger data
  ↓
End
```

---

# 25. Final Project Output

```text
EV Charger Multi-File Project

EV Charger Initialized
Voltage: 700 V
Current: 120 A
Power: 84000 W
Fault Status: NORMAL
```

---

# 26. GCC Compilation

Compile both source files:

```powershell
gcc main.c charger.c -o charger_monitor.exe
```

Explanation:

```text
gcc
    → GNU C Compiler

main.c
    → Main application source file

charger.c
    → Charger module source file

-o
    → Specifies the output file name

charger_monitor.exe
    → Generated executable
```

Run:

```powershell
.\charger_monitor.exe
```

---

# 27. Important Compilation Rule

If `main.c` calls a function implemented in `charger.c`, both source files must be compiled and linked together:

Correct:

```powershell
gcc main.c charger.c -o charger_monitor.exe
```

Incorrect:

```powershell
gcc main.c -o charger_monitor.exe
```

The incorrect command may produce an **undefined reference** linker error because the function definitions in `charger.c` are not included.

---

# 28. Day 9 Final Project Summary

The Day 9 project demonstrates:

* Functions
* Function prototypes
* Function declarations
* Function definitions
* Function parameters
* Return values
* Modular programming
* Multi-file project structure
* Header files
* Header guards
* Pass by value
* Pointer-based updates
* Pass-by-address technique
* Power calculation
* Over-voltage fault detection
* GCC compilation
* Functional testing

---

# 29. Interview Questions and Answers

## Q1. What is the difference between a function declaration and a function definition?

**Answer:**

A function declaration tells the compiler the function name, return type, and parameter types. A function definition contains the actual code that performs the function’s task.

---

## Q2. Why do we use header files?

**Answer:**

Header files store shared declarations, data types, macros, constants, and function prototypes. They allow multiple source files to use the same declarations.

---

## Q3. Why are header guards used?

**Answer:**

Header guards prevent the same header file from being included and processed multiple times, avoiding multiple declaration or redefinition errors.

---

## Q4. What is the role of `main.c`?

**Answer:**

`main.c` contains the `main()` function, which is the entry point of the application. It controls the application flow and calls the required functions.

---

## Q5. What is pass by value?

**Answer:**

In pass by value, a copy of the variable’s value is passed to the function. Changes to the function parameter do not affect the original variable.

---

## Q6. How can a function modify the original variable in C?

**Answer:**

A function can modify the original variable by receiving its address through a pointer and using the dereference operator to access and update the value.

---

## Q7. What does `&voltage` mean?

**Answer:**

`&voltage` returns the memory address of the `voltage` variable.

---

## Q8. What does `*voltage` mean?

**Answer:**

`*voltage` dereferences the pointer and accesses the value stored at the memory address.

---

## Q9. Why is `uint32_t` used for power?

**Answer:**

Power can exceed the maximum value of `uint16_t`. For example, `650 × 120 = 78000`, which is greater than `65535`. Therefore, `uint32_t` is used.

---

## Q10. Why is modular programming useful in embedded systems?

**Answer:**

Modular programming improves code organization, readability, testing, debugging, maintainability, reusability, and team development.

---

# 30. Day 9 Completion Checklist

| Topic                    | Status |
| ------------------------ | -----: |
| Functions                |      ✅ |
| Function prototypes      |      ✅ |
| Parameters               |      ✅ |
| Return values            |      ✅ |
| Four function types      |      ✅ |
| Modular programming      |      ✅ |
| `main.c`                 |      ✅ |
| `charger.c`              |      ✅ |
| `charger.h`              |      ✅ |
| Header guards            |      ✅ |
| Multi-file compilation   |      ✅ |
| Pass by value            |      ✅ |
| Pointer-based update     |      ✅ |
| Over-voltage detection   |      ✅ |
| Power calculation        |      ✅ |
| Normal test              |      ✅ |
| Over-voltage test        |      ✅ |
| Final integrated project |      ✅ |

**Day 9 Status: COMPLETED ✅**
