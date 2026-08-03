# Day 5 — Arrays and Strings in Embedded C

## Learning Objectives

By the end of Day 5, the following topics were covered:

* Understand arrays and their use in Embedded C
* Declare and initialize arrays
* Access array elements using indexes
* Use loops with arrays
* Read and update array values
* Calculate array size
* Calculate sum and average
* Find minimum and maximum values
* Understand strings as character arrays
* Use the null terminator `'\0'`
* Print strings using `%s`
* Access characters using `%c`
* Update characters in a string
* Calculate string length using `strlen()`
* Understand the difference between `strlen()` and `sizeof()`
* Understand the purpose of `size_t`
* Compare string contents using `strcmp()`

---

# 1. Arrays

## 1.1 What Is an Array?

An array is a collection of multiple values of the same data type stored under one variable name.

Example:

```c
uint16_t voltage_samples[5] =
{
    650U,
    655U,
    648U,
    652U,
    649U
};
```

The array contains five voltage values.

---

## 1.2 Array Indexing

Array indexes start from `0`.

| Index | Value |
| ----: | ----: |
|     0 |   650 |
|     1 |   655 |
|     2 |   648 |
|     3 |   652 |
|     4 |   649 |

Example:

```c
voltage_samples[0]
```

returns:

```text
650
```

The last valid index is:

```text
Number of elements - 1
```

For an array containing five elements:

```text
Last index = 5 - 1 = 4
```

---

## 1.3 Accessing Array Values Using a Loop

A `for` loop can be used to read every array element.

```c
uint8_t index;

for (index = 0U; index < 5U; index++)
{
    printf(
        "Sample %u: %u V\n",
        (unsigned int)index,
        (unsigned int)voltage_samples[index]
    );
}
```

The condition must use:

```c
index < 5U
```

and not:

```c
index <= 5U
```

because index `5` is outside the valid array range.

---

## 1.4 Updating an Array Element

An individual array value can be changed using its index.

```c
voltage_samples[2] = 660U;
```

The value at index `2` changes from:

```text
648 → 660
```

---

## 1.5 Calculating Array Size

The total array size can be calculated using:

```c
sizeof(voltage_samples)
```

The size of one element can be calculated using:

```c
sizeof(voltage_samples[0])
```

The number of elements can be calculated using:

```c
sizeof(voltage_samples) /
sizeof(voltage_samples[0])
```

Example:

```c
uint16_t voltage_samples[] =
{
    650U,
    655U,
    648U,
    652U,
    649U
};
```

Calculation:

```text
Total array size = 5 × 2 bytes = 10 bytes

Number of elements =
10 / 2 = 5
```

---

## 1.6 Sum of Array Values

Example:

```c
uint32_t total_current = 0U;

for (index = 0U; index < 5U; index++)
{
    total_current += current_samples[index];
}
```

The accumulator must be initialized to:

```c
0U
```

Otherwise, an uninitialized value may affect the result.

---

## 1.7 Average of Array Values

Formula:

```text
Average = Total / Number of Samples
```

Example:

```c
average_current =
    (uint16_t)(
        total_current /
        CURRENT_SAMPLE_COUNT
    );
```

---

## 1.8 Finding the Maximum Value

Initialize the maximum value with the first array element:

```c
maximum_current = current_samples[0];
```

Then compare the remaining elements:

```c
for (
    index = 1U;
    index < CURRENT_SAMPLE_COUNT;
    index++
)
{
    if (
        current_samples[index] >
        maximum_current
    )
    {
        maximum_current =
            current_samples[index];
    }
}
```

---

## 1.9 Finding the Minimum Value

Initialize the minimum value with the first array element:

```c
minimum_current = current_samples[0];
```

Then compare the remaining elements:

```c
for (
    index = 1U;
    index < CURRENT_SAMPLE_COUNT;
    index++
)
{
    if (
        current_samples[index] <
        minimum_current
    )
    {
        minimum_current =
            current_samples[index];
    }
}
```

---

## 1.10 EV Charger Application

Arrays can be used to store:

* Voltage samples
* Current samples
* Temperature samples
* Battery SOC values
* Power measurements
* Fault history
* Communication data

Example:

```c
uint16_t current_samples[5] =
{
    100U,
    120U,
    110U,
    130U,
    140U
};
```

The program can calculate:

```text
Total Current: 600 A
Average Current: 120 A
Maximum Current: 140 A
Minimum Current: 100 A
```

---

# 2. Strings

## 2.1 What Is a String?

In C, a string is a sequence of characters stored in a character array.

Example:

```c
char charger_status[] = "CHARGING";
```

The compiler stores:

```text
C H A R G I N G \0
```

The final character:

```c
'\0'
```

is called the null terminator.

It indicates the end of the string.

---

## 2.2 Character and String Literals

A single character uses single quotes:

```c
'C'
```

A string uses double quotes:

```c
"CHARGING"
```

Example:

```c
char status_character = 'R';

char charger_status[] = "READY";
```

---

## 2.3 Printing Strings

Use:

```c
%s
```

to print a complete string.

Example:

```c
printf(
    "Status: %s\n",
    charger_status
);
```

Output:

```text
Status: CHARGING
```

---

## 2.4 Printing Individual Characters

Use:

```c
%c
```

to print one character.

Example:

```c
printf(
    "First Character: %c\n",
    charger_status[0]
);
```

---

## 2.5 String Indexing

For:

```c
char status[] = "READY";
```

the indexes are:

| Index | Character |
| ----: | :-------- |
|     0 | `R`       |
|     1 | `E`       |
|     2 | `A`       |
|     3 | `D`       |
|     4 | `Y`       |
|     5 | `'\0'`    |

Therefore:

```c
status[0]
```

is:

```text
R
```

and:

```c
status[4]
```

is:

```text
Y
```

---

## 2.6 Updating a Character

A character array can be modified.

Example:

```c
char charger_status[] = "READY";

charger_status[0] = 'L';
```

The result is:

```text
LEADY
```

Another update:

```c
charger_status[4] = '!';
```

The final result is:

```text
LEAD!
```

---

## 2.7 String Length Using `strlen()`

The `strlen()` function is declared in:

```c
#include <string.h>
```

Example:

```c
strlen(charger_status)
```

For:

```c
char charger_status[] = "CHARGING";
```

the result is:

```text
8
```

`strlen()` counts visible characters and does not count the null terminator.

---

## 2.8 Difference Between `strlen()` and `sizeof()`

For:

```c
char charger_status[] = "CHARGING";
```

| Expression               | Result |
| ------------------------ | -----: |
| `strlen(charger_status)` |      8 |
| `sizeof(charger_status)` |      9 |

Reason:

```text
CHARGING = 8 visible characters

'\0' = 1 null terminator

Total array size = 9 bytes
```

---

## 2.9 `size_t`

`size_t` is an unsigned integer type used to represent:

* Object sizes
* Number of bytes
* Array lengths
* String lengths
* Memory-related counts

`sizeof()` returns a value of type `size_t`.

`strlen()` also returns a value of type `size_t`.

Use:

```c
%zu
```

to print a `size_t` value.

Example:

```c
printf(
    "Length: %zu\n",
    strlen(charger_status)
);
```

---

## 2.10 Accessing the Last Character Dynamically

Instead of manually writing the last index:

```c
charger_status[7]
```

use:

```c
charger_status[
    strlen(charger_status) - 1U
]
```

This automatically works when the string length changes.

---

## 2.11 String Comparison Using `strcmp()`

`strcmp()` compares the contents of two strings.

Include:

```c
#include <string.h>
```

Example:

```c
if (
    strcmp(command, "START") == 0
)
{
    printf(
        "Charging Session Started\n"
    );
}
```

When `strcmp()` returns:

```text
0
```

the strings are equal.

Do not use:

```c
command == "START"
```

because this does not compare the string contents.

Use:

```c
strcmp(command, "START") == 0
```

to compare the characters in both strings.

---

# 3. Data-Type Selection

Use fixed-width integer types for fixed-size embedded data.

| Data                         | Example type |
| ---------------------------- | ------------ |
| Small status or flag         | `uint8_t`    |
| Voltage or current           | `uint16_t`   |
| Energy or large counter      | `uint32_t`   |
| Object size or string length | `size_t`     |

Examples:

```c
uint16_t voltage = 650U;

uint16_t current = 120U;

uint8_t fault_status = 0U;

uint32_t energy_wh = 250000U;

size_t status_length =
    strlen(charger_status);
```

Important rule:

```text
Sensor, protocol, register, or firmware data
→ Use a fixed-width type based on the required range

Array size, string length, or memory size
→ Use size_t
```

---

# 4. Day 5 Programs

The following programs were created:

```text
array_voltage.c
array_update.c
array_size.c
current_analyzer.c
string_basic.c
string_index.c
string_update.c
string_length.c
charger_status_message.c
charger_command.c
```

---

# 5. Key Takeaways

* Array indexes start from `0`.
* The last valid array index is `number of elements - 1`.
* Do not access an array outside its valid range.
* Use loops to process multiple array values.
* Use `sizeof()` to calculate array storage size.
* A C string is stored in a character array.
* Every C string ends with `'\0'`.
* Use `%s` for a complete string.
* Use `%c` for one character.
* Use `strlen()` to count visible characters.
* `sizeof()` includes the null terminator when used on a character array.
* Use `size_t` for sizes and lengths.
* Use `strcmp()` to compare string contents.
* Use fixed-width integer types for embedded data and communication values.

---

# Day 5 Completion

**Status: Completed**

Topics completed:

* Arrays
* Array processing
* EV charger current analysis
* Strings
* String indexing
* String updates
* String length
* `size_t`
* EV charger status messages
* String comparison
