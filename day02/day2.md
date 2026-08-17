# Embedded C Roadmap — Day 2 Notes

## Topic: Variables, Data Types, Signed/Unsigned, `sizeof()`, and Integer Overflow

---

## 1. Variable

A variable is a named memory location used to store data that can change while a program is running.

Example:

```c
int voltage = 600;
```

Here:

* `int` → data type
* `voltage` → variable name
* `600` → initial value

---

## 2. Declaration

Declaration tells the compiler the variable’s name and data type.

Example:

```c
int voltage;
```

The variable is declared, but no value is assigned in this statement.

---

## 3. Initialization

Initialization means assigning the first value to a variable when it is declared.

Example:

```c
int voltage = 600;
```

This is:

* Declaration
* Initialization

---

## 4. Assignment

Assignment means changing or assigning a value after the variable has already been declared.

Example:

```c
voltage = 750;
```

This is assignment only.

---

## 5. Common C Data Types

| Data Type | Purpose                        |
| --------- | ------------------------------ |
| `char`    | Character or small integer     |
| `int`     | Integer value                  |
| `float`   | Decimal value                  |
| `double`  | Higher-precision decimal value |
| `void`    | No value or no return value    |

---

## 6. Signed Data Types

Signed data types can store:

* Negative values
* Zero
* Positive values

Example:

```c
int8_t temperature = -5;
```

`int8_t` range:

```text
-128 to +127
```

---

## 7. Unsigned Data Types

Unsigned data types can store:

* Zero
* Positive values

They do not store negative values.

Example:

```c
uint8_t soc = 56;
```

`uint8_t` range:

```text
0 to 255
```

---

## 8. Fixed-Width Integer Types

Include:

```c
#include <stdint.h>
```

Common fixed-width types:

| Data Type  |    Size |                           Range |
| ---------- | ------: | ------------------------------: |
| `uint8_t`  |  1 byte |                        0 to 255 |
| `int8_t`   |  1 byte |                    -128 to +127 |
| `uint16_t` | 2 bytes |                     0 to 65,535 |
| `int16_t`  | 2 bytes |               -32,768 to 32,767 |
| `uint32_t` | 4 bytes |              0 to 4,294,967,295 |
| `int32_t`  | 4 bytes | -2,147,483,648 to 2,147,483,647 |

---

## 9. Why Fixed-Width Types Are Used in Embedded Systems

Fixed-width types provide a known and predictable size.

Example:

```c
uint16_t voltage;
```

This always represents a 16-bit unsigned integer when the type is available.

These types are useful for:

* CAN messages
* Modbus registers
* Hardware registers
* Communication packets
* Memory-mapped peripherals
* Embedded drivers

---

## 10. `sizeof()` Operator

`sizeof()` returns the memory size in bytes.

Example:

```c
uint16_t voltage = 600;

printf("%zu\n", sizeof(voltage));
```

Output:

```text
2
```

The format specifier `%zu` is used because `sizeof()` returns a value of type `size_t`.

---

## 11. Integer Overflow

Integer overflow occurs when a value exceeds the range supported by its data type.

Example:

```c
uint8_t value = 255;

value = value + 1;
```

Output:

```text
0
```

Reason:

```text
uint8_t range:

0 to 255

After 255:

256 wraps to 0
```

Another example:

```c
uint8_t value = 250;

value = value + 10;
```

Calculation:

```text
250 + 10 = 260

260 - 256 = 4
```

Output:

```text
4
```

---

## 12. How to Select a Data Type

Ask:

1. Can the value be negative?
2. What is the minimum value?
3. What is the maximum value?
4. Is a fixed size required?
5. How much memory is available?

Examples:

```c
uint8_t soc = 80;
```

Reason:

```text
SOC normally ranges from 0 to 100.
No negative value is required.
uint8_t uses only 1 byte.
```

```c
int8_t temperature = -5;
```

Reason:

```text
Temperature can be negative or positive.
The expected range must remain between -128 and +127.
```

```c
uint16_t voltage = 600;
```

Reason:

```text
Voltage is positive.
The value fits within 0 to 65,535.
```

```c
uint32_t energy = 132444;
```

Reason:

```text
Energy can become a large positive value.
uint32_t provides a large range.
```

---

## 13. Day 2 EV Charger Example

```c
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint16_t voltage = 600;
    uint16_t current = 150;
    uint8_t soc = 56;
    int8_t temperature = -5;
    uint32_t energy = 132444;

    printf("Charger Voltage : %u V\n", voltage);
    printf("Charger Current : %u A\n", current);
    printf("SOC : %u %%\n", soc);
    printf("Temperature : %d C\n", temperature);
    printf("Energy : %u Wh\n", energy);

    return 0;
}
```

---

## 14. Day 2 Key Points

```text
Variable
→ Named memory location used to store data

Declaration
→ int voltage;

Initialization
→ int voltage = 600;

Assignment
→ voltage = 750;

uint8_t
→ 1 byte
→ 0 to 255

int8_t
→ 1 byte
→ -128 to +127

uint16_t
→ 2 bytes
→ 0 to 65,535

uint32_t
→ 4 bytes
→ Large positive values

sizeof()
→ Returns the memory size in bytes

Integer overflow
→ Occurs when a value exceeds the supported range
```
