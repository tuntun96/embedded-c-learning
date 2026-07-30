# Embedded C Learning Journey

This repository documents my hands-on Embedded C learning journey, from C programming fundamentals to embedded systems, microcontrollers, communication protocols, firmware development, and embedded system testing.

The learning programs and mini-projects are organized day-wise and include practical examples based on EV charging systems.

## Learning Goal

To build strong skills in:

* Embedded C programming
* Embedded systems fundamentals
* STM32 microcontroller development
* Firmware development and debugging
* Embedded communication protocols
* Embedded system testing and validation
* Python-based test automation

## Completed Topics

### Day 1 — Embedded Systems Fundamentals

* Introduction to embedded systems
* Embedded system examples
* Hardware and firmware
* Microcontroller vs. microprocessor
* CPU, Flash, and RAM
* Stack and heap
* Embedded software build process
* Source code, compiler, linker, and executable files

### Day 2 — Variables and Data Types

* Variables and constants
* Declaration, initialization, and assignment
* Signed and unsigned data types
* Fixed-width integer types
* `uint8_t`, `uint16_t`, and `uint32_t`
* `int8_t`, `int16_t`, and `int32_t`
* `sizeof()` operator
* EV charger parameter mini-project

### Day 3 — C Operators and Bitwise Operations

* Arithmetic operators
* Relational operators
* Logical operators
* Increment and decrement operators
* Implicit and explicit type conversion
* Binary and hexadecimal number systems
* Bitwise AND, OR, XOR, and NOT
* Left and right shift operations
* Bit masks
* Set, clear, toggle, and check a bit
* Register-control concepts
* EV charger fault-status mini-project

## Projects

### EV Charger Fault Manager

A simple Embedded C program that uses bitwise operations to manage EV charger fault states.

Fault mapping:

|   Bit | Fault           |
| ----: | --------------- |
| Bit 0 | Overvoltage     |
| Bit 1 | Overcurrent     |
| Bit 2 | Overtemperature |
| Bit 3 | Emergency Stop  |

The project demonstrates:

* Creating bit masks
* Setting fault bits
* Clearing fault bits
* Checking fault status
* Managing multiple faults using a single `uint8_t` variable

## Repository Structure

```text
embedded-c-learning/
├── README.md
├── .gitignore
├── day1/
│   ├── day1.md
│   └── day1_memory.c
├── day2/
│   ├── day2.md
│   ├── day2_data_type.c
│   └── day2_mini_projects.c
└── day3/
    ├── notes.md
    ├── arithmetic_operations.c
    ├── comparison_operators.c
    ├── logical_operators.c
    ├── increment_decrement.c
    ├── implicit_conversion.c
    ├── explicit_conversion.c
    ├── bitwise_and.c
    ├── bitwise_or.c
    ├── bitwise_xor.c
    ├── left_shift.c
    ├── right_shift.c
    ├── set_bit.c
    ├── clear_bit.c
    ├── check_bit.c
    └── ev_charger_fault_manager.c
```

## Tools Used

* Visual Studio Code
* GCC
* Git
* GitHub
* Windows PowerShell

## Learning Approach

Each learning day follows this structure:

1. Theory and concepts
2. Simple C examples
3. Embedded-system examples
4. EV charger-based examples
5. Practice questions
6. Coding exercises
7. Mini-project
8. Notes and revision
9. GitHub update

## Current Progress

* Day 1: Completed
* Day 2: Completed
* Day 3: Completed
* Day 4: In progress

## Future Topics

* Control flow and functions
* Arrays and strings
* Pointers
* Structures, unions, and enums
* Storage classes and qualifiers
* Memory management
* Preprocessor and modular programming
* Embedded C programming
* STM32 development
* GPIO, UART, SPI, I2C, CAN, and ADC
* Interrupts and timers
* RTOS fundamentals
* Embedded Linux basics
* Embedded system test automation using Python

## Author

**Tuntun Sah**

Embedded Systems | Firmware Validation | EV Charging | CAN | Modbus | OCPP
