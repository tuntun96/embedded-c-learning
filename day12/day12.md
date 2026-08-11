# Day 12 -- Scope, Lifetime & `extern`

## Learning Objectives

-   Scope and Lifetime
-   Local vs Global variables
-   `extern`
-   Declaration vs Definition
-   Multi-file programming
-   Linker and undefined reference
-   EV Charger example
-   Interview questions

# 1. Scope

Scope is the region of a program where a variable/function can be
accessed.

## Local Scope

Declared inside a function.

``` c
void monitor(void)
{
    uint16_t voltage = 650U;
}
```

-   Visible only inside `monitor()`
-   Cannot be accessed by other functions.

## Global Scope

Declared outside all functions.

``` c
uint16_t charger_voltage = 650U;
```

-   Accessible by all functions in the same source file.
-   Can be shared with other files using `extern`.

# 2. Lifetime

Lifetime tells how long a variable exists.

  Variable       Scope             Lifetime
  -------------- ----------------- ------------------------
  Local          Function          Until function returns
  Global         Program           Entire program
  Static Local   Function          Entire program
  File Static    Current .c file   Entire program

Memory trick:

> Scope = WHO can access it.
>
> Lifetime = HOW LONG it exists.

# 3. Declaration vs Definition

Definition creates storage.

``` c
uint16_t charger_voltage = 650U;
```

Declaration only informs the compiler.

``` c
extern uint16_t charger_voltage;
```

  Definition             Declaration
  ---------------------- ---------------------
  Allocates memory       No memory allocated
  Initializes possible   No initialization
  One definition         Many declarations

# 4. extern

Purpose:

Share one global variable across multiple source files.

## charger.h

``` c
#ifndef CHARGER_H
#define CHARGER_H

#include <stdint.h>

extern uint16_t charger_voltage;

void display_charger_status(void);

#endif
```

## charger.c

``` c
#include <stdio.h>
#include "charger.h"

uint16_t charger_voltage = 650U;

void display_charger_status(void)
{
    printf("Charger Voltage: %u V\n",
           (unsigned int)charger_voltage);
}
```

## main.c

``` c
#include <stdio.h>
#include "charger.h"

int main(void)
{
    printf("EV Charger Started\n\n");

    display_charger_status();

    printf("Voltage from main(): %u V\n",
           (unsigned int)charger_voltage);

    return 0;
}
```

Expected Output

    EV Charger Started

    Charger Voltage: 650 V
    Voltage from main(): 650 V

If:

``` c
charger_voltage = 720U;
display_charger_status();
```

Output:

    Charger Voltage: 720 V
    Voltage from main(): 720 V

Reason: Both files access the same global variable.

# 5. Linker & Undefined Reference

Common error:

    undefined reference to charger_voltage

Causes: - Spelling mismatch - Variable defined nowhere - Missing source
file during compilation

Compile:

``` bash
gcc main.c charger.c -o charger.exe
```

Run:

``` bash
./charger.exe
```

# 6. EV Charger Use Cases

-   Charger voltage
-   Charger current limit
-   Configuration shared across modules
-   CAN communication
-   Display module

# 7. Common Mistakes

-   Initializing an extern declaration.

Wrong:

``` c
extern uint16_t charger_voltage = 650U;
```

Correct:

``` c
extern uint16_t charger_voltage;
```

-   Too many global variables.
-   Confusing scope and lifetime.

# 8. Interview Questions

1.  What is scope?
2.  Difference between scope and lifetime?
3.  Local vs global variable?
4.  What is extern?
5.  Declaration vs definition?
6.  Why use header files?
7.  Why undefined reference occurs?
8.  Difference between compiler and linker?
9.  Can extern allocate memory?
10. Can an extern variable be initialized in a header?

# Quick Revision

-   Scope → Visibility
-   Lifetime → Existence
-   extern → Variable defined elsewhere
-   Definition → Creates memory
-   Declaration → Refers to memory
-   One global variable can be shared across files.

# Day Summary

Today you learned: - Scope - Lifetime - Local & Global variables -
extern - Declaration vs Definition - Multi-file programming - Linker
debugging - Real EV charger implementation
