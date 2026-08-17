
# Day 3 Notes — C Operators, Type Conversion & Bitwise Operations

## 1. C Operators

Operators are symbols used to perform operations on variables and values.

### Arithmetic operators

| Operator | Operation      | Example       |
| -------- | -------------- | ------------- |
| `+`      | Addition       | `10 + 5 = 15` |
| `-`      | Subtraction    | `10 - 5 = 5`  |
| `*`      | Multiplication | `10 * 5 = 50` |
| `/`      | Division       | `10 / 5 = 2`  |
| `%`      | Remainder      | `10 % 3 = 1`  |

### Relational operators

These compare two values and return true or false.

| Operator | Meaning                  |
| -------- | ------------------------ |
| `==`     | Equal to                 |
| `!=`     | Not equal to             |
| `>`      | Greater than             |
| `<`      | Less than                |
| `>=`     | Greater than or equal to |
| `<=`     | Less than or equal to    |

Example:

```c
if (voltage > 500U)
{
    printf("Voltage is high\n");
}
```

---

## 2. Logical Operators

| Operator | Meaning     |   |            |
| -------- | ----------- | - | ---------- |
| `&&`     | Logical AND |   |            |
| `        |             | ` | Logical OR |
| `!`      | Logical NOT |   |            |

Example:

```c
if ((voltage > 400U) && (current > 0U))
{
    printf("Charging conditions are valid\n");
}
```

For `&&`, both conditions must be true.

---

## 3. Increment and Decrement

```c
count++;
```

Adds `1`.

```c
count--;
```

Subtracts `1`.

### Prefix and postfix

```c
++count;
```

Increment first, then use the value.

```c
count++;
```

Use the current value first, then increment.

---

# 4. Type Conversion

Type conversion changes a value from one data type to another.

## Implicit conversion

The compiler automatically performs the conversion.

```c
int voltage = 600;

float voltage_float = voltage;
```

Conversion:

```text
int → float
```

## Explicit conversion

The programmer manually requests the conversion using a cast.

```c
float result = (float)7 / 2;
```

Result:

```text
3.5
```

Without casting:

```c
float result = 7 / 2;
```

Result:

```text
3.0
```

The division happens as integer division before the result is stored in `float`.

---

## EV charger example: Wh to kWh

```c
uint16_t energy_wh = 1500U;

float energy_kwh;

energy_kwh = (float)energy_wh / 1000.0f;
```

Result:

```text
1.5 kWh
```

`1000.0f` is a float constant.

---

# 5. Binary Basics

Binary uses only:

```text
0 and 1
```

Each binary position has a value:

| Bit position | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---:|---:|---:|---:|---:|---:|---:|
| Value | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |

Example:

```text
00000110
```

Active bits:

```text
Bit 2 = 4

Bit 1 = 2
```

Therefore:

```text
4 + 2 = 6
```

---

# 6. Hexadecimal Basics

Hexadecimal is base 16.

```text
0 1 2 3 4 5 6 7 8 9 A B C D E F
```

| Hex | Decimal | Binary |
| --- | ------: | ------ |
| `A` |      10 | `1010` |
| `B` |      11 | `1011` |
| `C` |      12 | `1100` |
| `D` |      13 | `1101` |
| `E` |      14 | `1110` |
| `F` |      15 | `1111` |

Examples:

```text
0x0A = 10 decimal = 00001010 binary

0x0F = 15 decimal = 00001111 binary

0xF0 = 240 decimal = 11110000 binary

0xFF = 255 decimal = 11111111 binary
```

### Meaning of `0x0FU`

```text
0x → hexadecimal

0F → hexadecimal value

U → unsigned integer constant
```

Therefore:

```text
0x0FU = 15 decimal
```

---

# 7. Bitwise Operators

| Operator | Name        | Main use          |           |
| -------- | ----------- | ----------------- | --------- |
| `&`      | Bitwise AND | Check a bit       |           |
| `        | `           | Bitwise OR        | Set a bit |
| `^`      | Bitwise XOR | Toggle a bit      |           |
| `~`      | Bitwise NOT | Invert a mask     |           |
| `<<`     | Left shift  | Create a bit mask |           |
| `>>`     | Right shift | Shift bits right  |           |

---

# 8. Bit Mask

A bit mask selects a specific bit.

General formula:

```c
1U << bit_number
```

Examples:

```c
1U << 0U  /* 00000001 = 1 */

1U << 1U  /* 00000010 = 2 */

1U << 2U  /* 00000100 = 4 */

1U << 3U  /* 00001000 = 8 */

1U << 4U  /* 00010000 = 16 */
```

---

# 9. Set a Bit

Use bitwise OR:

```c
status |= (1U << bit_number);
```

Example:

```c
status |= (1U << 2U);
```

This sets Bit 2 to `1` and keeps the other bits unchanged.

---

# 10. Clear a Bit

Use bitwise AND with NOT:

```c
status &= ~(1U << bit_number);
```

Example:

```c
status &= ~(1U << 2U);
```

This clears Bit 2 to `0` and keeps the other bits unchanged.

---

# 11. Toggle a Bit

Use XOR:

```c
status ^= (1U << bit_number);
```

Example:

```c
status ^= (1U << 2U);
```

Effect:

```text
0 → 1

1 → 0
```

Toggling the same bit twice returns it to the original state.

---

# 12. Check a Bit

Use AND:

```c
if ((status & (1U << bit_number)) != 0U)
{
    /* Bit is active */
}
```

Example:

```c
if ((fault_status & (1U << 2U)) != 0U)
{
    printf("Overtemperature fault is active\n");
}
```

Important:

```text
Result = 0 → false/inactive

Result ≠ 0 → true/active
```

The result may be `2`, `4`, `8`, or another nonzero value. It does not need to be exactly `1`.

---

# 13. Embedded Register Control

A microcontroller has hardware registers for:

* GPIO
* UART
* SPI
* I2C
* CAN
* ADC
* Timers

Bitwise operations control individual bits in these registers.

Example:

```c
GPIO_OUTPUT |= (1U << 0U);
```

Sets Bit 0.

```c
GPIO_OUTPUT &= ~(1U << 0U);
```

Clears Bit 0.

```c
GPIO_OUTPUT ^= (1U << 0U);
```

Toggles Bit 0.

---

# 14. EV Charger Fault Example

```c
#define FAULT_OVERVOLTAGE     (1U << 0U)
#define FAULT_OVERCURRENT     (1U << 1U)
#define FAULT_OVERTEMPERATURE (1U << 2U)
#define FAULT_ESTOP           (1U << 3U)
```

Fault mapping:

|   Bit | Fault           | Decimal mask |
| ----: | --------------- | -----------: |
| Bit 0 | Overvoltage     |            1 |
| Bit 1 | Overcurrent     |            2 |
| Bit 2 | Overtemperature |            4 |
| Bit 3 | E-stop          |            8 |

Example:

```text
fault_status = 00000110
```

Active faults:

```text
Bit 1 → Overcurrent

Bit 2 → Overtemperature
```

Decimal:

```text
2 + 4 = 6
```

---

# 15. Important Embedded C Rules

1. Use fixed-width types:

```c
uint8_t
uint16_t
uint32_t
int8_t
int16_t
int32_t
```

2. Use unsigned constants where appropriate:

```c
0U
1U
100U
```

3. Use parentheses in bitwise expressions:

```c
status |= (1U << 2U);
```

4. Use named macros instead of magic numbers:

```c
#define FAULT_OVERCURRENT (1U << 1U)
```

Better than:

```c
fault_status |= 2U;
```

5. Use `!= 0U` when checking a bit:

```c
if ((fault_status & FAULT_OVERCURRENT) != 0U)
```

This is clear and professional.

---

# Day 3 Final Summary

You learned:

```text
C operators
↓
Type conversion
↓
Binary and hexadecimal
↓
Bitwise operators
↓
Bit masks
↓
Set, clear, toggle, and check bits
↓
Register-control concept
↓
EV charger fault manager

