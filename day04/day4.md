# Embedded C Learning — Day 4

## Topics Covered

1. Conditional Statements

   * `if`
   * `if-else`
   * `if-else if-else`

2. `switch-case`

3. Loops

   * `for` loop
   * `while` loop
   * `do-while` loop

4. Functions

   * Function declaration
   * Function definition
   * Function call
   * Function parameters
   * Function arguments
   * `void` functions
   * Functions with return values

5. EV Charger Applications

   * Charger state handling
   * Charging permission
   * Temperature monitoring
   * Charger safety monitoring

---

# 1. Conditional Statements

Conditional statements allow a program to execute different code blocks based on whether a condition is true or false.

## 1.1 `if` Statement

Syntax:

```c
if (condition)
{
    /* Code executes when the condition is true */
}
```

Example:

```c
uint8_t charging_active = 1U;

if (charging_active == 1U)
{
    printf("Charging is active\n");
}
```

The code inside the `if` block executes only when the condition evaluates to true.

---

## 1.2 `if-else` Statement

Syntax:

```c
if (condition)
{
    /* Executes when the condition is true */
}
else
{
    /* Executes when the condition is false */
}
```

Example:

```c
uint8_t fault_status = 0U;

if (fault_status == 0U)
{
    printf("No fault detected\n");
}
else
{
    printf("Fault active\n");
}
```

---

## 1.3 `if-else if-else` Statement

This structure is used when multiple conditions must be checked.

Example:

```c
uint8_t temperature = 65U;

if (temperature > 70U)
{
    printf("Overtemperature fault\n");
}
else if (temperature > 50U)
{
    printf("High-temperature warning\n");
}
else
{
    printf("Temperature normal\n");
}
```

Temperature logic:

| Temperature | Status  |
| ----------- | ------- |
| `≤ 50°C`    | Normal  |
| `51–70°C`   | Warning |
| `> 70°C`    | Fault   |

When conditions overlap, the highest-priority or most restrictive condition should be checked first.

For example, if `temperature = 75U`, both conditions below are true:

```c
temperature > 50U
temperature > 70U
```

Therefore, `temperature > 70U` must be checked first to correctly identify the condition as a fault.

---

# 2. `switch-case`

The `switch-case` statement is useful when one variable is compared with multiple fixed values.

Syntax:

```c
switch (variable)
{
    case value1:
        /* Code */
        break;

    case value2:
        /* Code */
        break;

    default:
        /* Code */
        break;
}
```

Example:

```c
uint8_t charger_state = 3U;

switch (charger_state)
{
    case 0U:
        printf("Power ON\n");
        break;

    case 1U:
        printf("Self-test\n");
        break;

    case 2U:
        printf("Charger ready\n");
        break;

    case 3U:
        printf("Charging in progress\n");
        break;

    case 4U:
        printf("Charging complete\n");
        break;

    case 5U:
        printf("Fault active\n");
        break;

    default:
        printf("Invalid charger state\n");
        break;
}
```

### Purpose of `break`

The `break` statement exits the `switch` block after a matching case is executed.

Without `break`, execution may continue into the next case. This behavior is called **fall-through**.

### Purpose of `default`

The `default` block executes when none of the defined cases match.

---

# 3. Loops

Loops are used to execute a block of code repeatedly.

---

## 3.1 `for` Loop

A `for` loop is commonly used when the number of iterations is known.

Syntax:

```c
for (initialization; condition; update)
{
    /* Repeated code */
}
```

Example:

```c
uint8_t sample;

for (sample = 1U; sample <= 5U; sample++)
{
    printf("Voltage sample: %u\n", sample);
}
```

The loop contains:

1. **Initialization** — executed once before the loop starts
2. **Condition** — checked before every iteration
3. **Update** — executed after each iteration

---

## 3.2 `while` Loop

A `while` loop checks the condition before executing the loop body.

Syntax:

```c
while (condition)
{
    /* Repeated code */
}
```

Example:

```c
uint8_t charging_active = 1U;
uint8_t reading = 1U;

while (charging_active == 1U)
{
    printf("Reading: %u\n", reading);

    reading++;

    if (reading == 6U)
    {
        charging_active = 0U;
    }
}
```

The loop continues while the condition is true.

A `while` loop may execute **zero times** if the condition is false initially.

---

## 3.3 `do-while` Loop

A `do-while` loop executes the loop body first and checks the condition afterward.

Syntax:

```c
do
{
    /* Code executes at least once */
}
while (condition);
```

Example:

```c
uint8_t retry = 1U;

do
{
    printf("Server connection attempt %u\n", retry);
    retry++;
}
while (retry <= 3U);
```

A `do-while` loop executes **at least once**, even when the condition is initially false.

The semicolon after the `while` condition is required:

```c
while (condition);
```

---

# 4. Functions

A function is a reusable block of code designed to perform a specific task.

Functions improve:

* Code readability
* Code organization
* Reusability
* Maintainability
* Testing

A function should ideally have one clear responsibility.

---

## 4.1 Function Declaration

A function declaration informs the compiler about:

* Function return type
* Function name
* Input parameters

Example:

```c
uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
);
```

The declaration is also called a **function prototype**.

---

## 4.2 Function Definition

A function definition contains the implementation of the function.

Example:

```c
uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
)
{
    uint32_t power;

    power = (uint32_t)voltage * (uint32_t)current;

    return power;
}
```

---

## 4.3 Function Call

A function is executed when it is called.

Example:

```c
power = calculate_power(voltage, current);
```

Execution flow:

```text
voltage and current
        ↓
calculate_power()
        ↓
Calculated power
        ↓
Stored in power
```

---

## 4.4 Function Parameters and Arguments

A **parameter** is a variable defined in a function declaration or definition.

Example:

```c
uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
);
```

Parameters:

```text
voltage
current
```

An **argument** is the value or variable passed when the function is called.

Example:

```c
calculate_power(650U, 120U);
```

Arguments:

```text
650U
120U
```

---

## 4.5 `void` Functions

A `void` function does not return a value.

Example:

```c
void display_charger_ready(void)
{
    printf("Charger is READY\n");
}
```

Function call:

```c
display_charger_ready();
```

---

## 4.6 Functions with Return Values

A non-`void` function returns a value to the calling function.

Example:

```c
uint32_t calculate_power(
    uint16_t voltage,
    uint16_t current
)
{
    uint32_t power;

    power = (uint32_t)voltage * (uint32_t)current;

    return power;
}
```

Function call:

```c
power = calculate_power(650U, 120U);
```

Calculation:

```text
650 × 120 = 78,000 W
```

`uint32_t` is used because:

```text
uint16_t maximum value = 65,535
Calculated power = 78,000
```

The calculated value does not fit in `uint16_t`.

---

## 4.7 Purpose of `return`

A `return` statement:

1. Sends a value back to the calling function
2. Immediately exits the current function

Example:

```c
return 2U;
```

After `return` executes, the remaining statements in the function are not executed.

A non-`void` function should return a value matching its declared return type.

---

# 5. EV Charger Temperature Monitoring

Temperature status mapping:

| Return value | Status  |
| -----------: | ------- |
|         `0U` | Normal  |
|         `1U` | Warning |
|         `2U` | Fault   |

Example:

```c
uint8_t check_temperature(uint8_t temperature)
{
    if (temperature > 70U)
    {
        return 2U;
    }
    else if (temperature > 50U)
    {
        return 1U;
    }
    else
    {
        return 0U;
    }
}
```

Example results:

| Temperature | Returned status |
| ----------: | --------------: |
|       `40U` |            `0U` |
|       `65U` |            `1U` |
|       `75U` |            `2U` |

---

# 6. EV Charger Charging Permission

Charging is allowed when:

```text
fault_status == 0U
AND
temperature_status != 2U
```

Example:

```c
uint8_t is_charging_allowed(
    uint8_t fault_status,
    uint8_t temperature_status
)
{
    if (
        (fault_status == 0U) &&
        (temperature_status != 2U)
    )
    {
        return 1U;
    }
    else
    {
        return 0U;
    }
}
```

Return values:

| Return value | Charging status |
| -----------: | --------------- |
|         `1U` | Allowed         |
|         `0U` | Blocked         |

---

# 7. Function Separation

The EV charger safety monitor uses separate functions for separate tasks:

```c
uint8_t check_temperature(uint8_t temperature);
```

Purpose:

> Checks temperature and returns a status code.

```c
uint8_t is_charging_allowed(
    uint8_t fault_status,
    uint8_t temperature_status
);
```

Purpose:

> Determines whether charging is allowed.

```c
void display_charger_status(
    uint8_t temperature_status,
    uint8_t charging_allowed
);
```

Purpose:

> Displays the temperature and charging status.

This approach improves code organization and allows individual functions to be reused and tested independently.

---

# 8. EV Charger Safety Monitor Logic

Program flow:

```text
Temperature input
       ↓
check_temperature()
       ↓
Temperature status
       ↓
is_charging_allowed()
       ↓
Charging permission
       ↓
display_charger_status()
       ↓
Final charger status
```

Test cases:

| Temperature | Fault status | Temperature status | Charging status |
| ----------: | -----------: | ------------------ | --------------- |
|       `40U` |         `0U` | Normal             | Allowed         |
|       `65U` |         `0U` | Warning            | Allowed         |
|       `75U` |         `0U` | Fault              | Blocked         |
|       `40U` |         `1U` | Normal             | Blocked         |

The final test confirms that temperature status and charger fault status are evaluated independently.

---

# Key Takeaways

* Use `if-else` for conditional decision-making.
* Use `switch-case` when comparing one variable with multiple fixed values.
* Use a `for` loop when the number of iterations is known.
* Use a `while` loop when execution depends on a condition.
* Use a `do-while` loop when the code must execute at least once.
* Use functions to organize code into reusable units.
* A function declaration informs the compiler about a function.
* A function definition contains the function implementation.
* Parameters are defined in the function; arguments are passed during the function call.
* A `void` function does not return a value.
* A non-`void` function returns a value.
* `return` sends a value back and exits the function.
* Check higher-priority conditions before lower-priority overlapping conditions.
* Keep independent checks separate to improve reliability and maintainability.
* Use clear and consistent naming for variables and functions.
