# Day 22 — Embedded C Error Handling

## 1. What is Error Handling?

Error handling is the process of:

1. Detecting an error
2. Representing the error
3. Propagating the error
4. Handling the error
5. Recovering when possible
6. Falling back to a safe state when recovery fails

Typical embedded flow:

```text
Detect → Represent → Propagate → Handle → Recover/Fallback
```

---

## 2. Error vs Fault

### Error

An error means something went wrong during operation.

Examples:

* Communication timeout
* Invalid data
* Sensor read failure

Some errors may be temporary and recoverable.

### Fault

A fault is a condition where the system should no longer continue normal operation.

Examples:

* Overvoltage
* Critical overtemperature
* Serious hardware failure

Example:

```text
ERROR_SENSOR
     ↓
Retry
     ↓
Recovery
     ↓
Recovery fails
     ↓
FAULTED state
```

---

# 3. Representing Errors with `enum`

A common embedded approach is:

```c
typedef enum
{
    ERROR_NONE,
    ERROR_TIMEOUT,
    ERROR_INVALID_DATA,
    ERROR_SENSOR,
    ERROR_COMMUNICATION,
    ERROR_OVERVOLTAGE,
    ERROR_OVERTEMPERATURE
} ErrorCode;
```

The caller can now understand exactly what went wrong.

For example:

```c
ErrorCode result;

result = read_temperature(&temperature, sensor_ok);

if(result != ERROR_NONE)
{
    /* Error occurred */
}
```

---

# 4. Why `ERROR_NONE` is Zero

By default, the first enum member has value `0`.

Therefore:

```text
ERROR_NONE             = 0
ERROR_TIMEOUT          = 1
ERROR_INVALID_DATA     = 2
ERROR_SENSOR           = 3
...
```

`0` is commonly used to represent **no error / success**.

This makes checks natural:

```c
if(result != ERROR_NONE)
{
    /* Something went wrong */
}
```

The numerical values themselves are less important than the symbolic names.

---

# 5. Error Handling API Pattern

A function can return an `ErrorCode` while using a pointer to return data.

Example:

```c
ErrorCode read_temperature(uint16_t *temperature);
```

The function has two outputs:

```text
Return value       → status/error
temperature pointer → actual data
```

Example:

```c
ErrorCode result;

result = read_temperature(&temperature);

if(result == ERROR_NONE)
{
    printf("Temperature = %u\n", temperature);
}
```

---

# 6. Detecting an Error

Example:

```c
ErrorCode read_temperature(uint16_t *temperature,
                           uint8_t sensor_ok)
{
    if(sensor_ok == 1U)
    {
        *temperature = 45U;
        return ERROR_NONE;
    }
    else
    {
        return ERROR_SENSOR;
    }
}
```

The lower-level function detects the sensor problem.

It does **not** decide the entire system response.

---

# 7. Error Propagation

A lower-level function detects an error and returns it to its caller.

Example:

```text
read_temperature()
       ↓
ERROR_SENSOR
       ↓
read_temperature_with_retry()
       ↓
ERROR_SENSOR
       ↓
check_temperature()
       ↓
ERROR_SENSOR
       ↓
process_temperature()
```

Important principle:

> The function that detects the error does not necessarily have to decide what the entire system should do.

Higher-level logic can make the final decision.

---

# 8. Checking Returned Errors

Correct:

```c
ErrorCode result;

result = read_temperature(temperature, sensor_ok);

if(result != ERROR_NONE)
{
    return result;
}
```

This is error propagation.

Avoid ignoring important error return values.

---

# 9. Temperature Checking

The sensor read must succeed before using the temperature.

Example:

```c
ErrorCode check_temperature(uint16_t *temperature,
                            uint8_t sensor_ok)
{
    ErrorCode result;

    result = read_temperature_with_retry(temperature, sensor_ok);

    if(result != ERROR_NONE)
    {
        return result;
    }
    else if(*temperature > 80U)
    {
        return ERROR_OVERTEMPERATURE;
    }
    else
    {
        return ERROR_NONE;
    }
}
```

The sequence is:

```text
Read sensor
    ↓
Did read succeed?
    ↓
   YES
    ↓
Check temperature
    ↓
Temperature > 80?
    ↓
ERROR_OVERTEMPERATURE
```

Never check the temperature before confirming that the sensor supplied valid data.

---

# 10. Error Handling Strategies

Different errors require different responses.

| Error                   | Typical response                   |
| ----------------------- | ---------------------------------- |
| `ERROR_TIMEOUT`         | Retry                              |
| `ERROR_COMMUNICATION`   | Reinitialize/recover communication |
| `ERROR_INVALID_DATA`    | Reject data                        |
| `ERROR_SENSOR`          | Retry/recover sensor               |
| `ERROR_OVERVOLTAGE`     | Stop operation / safe state        |
| `ERROR_OVERTEMPERATURE` | Stop charging / fault state        |

Important:

> Do not blindly retry every error.

For example, repeatedly retrying an overvoltage condition is not a safe recovery strategy.

---

# 11. Bounded Retry

A retry mechanism should have a maximum number of attempts.

Example:

```c
#define MAX_ATTEMPTS 3U
```

Concept:

```text
Attempt 1
   ↓
Failed
   ↓
Attempt 2
   ↓
Failed
   ↓
Attempt 3
   ↓
Failed
   ↓
Stop retrying
```

This prevents an infinite retry loop.

---

# 12. Retry Implementation

Example:

```c
ErrorCode read_temperature_with_retry(uint16_t *temperature,
                                      uint8_t sensor_ok)
{
    uint8_t attempt_count = 0U;

    for(attempt_count = 1U;
        attempt_count <= MAX_ATTEMPTS;
        attempt_count++)
    {
        ErrorCode result;

        result = read_temperature(temperature, sensor_ok);

        if(result == ERROR_NONE)
        {
            return ERROR_NONE;
        }
        else if(result != ERROR_SENSOR)
        {
            return result;
        }
        else
        {
            printf("Attempt = %u\n", attempt_count);

            if(attempt_count == MAX_ATTEMPTS)
            {
                return result;
            }
        }
    }

    return ERROR_SENSOR;
}
```

For the exercise, only `ERROR_SENSOR` is retried.

Other errors are immediately propagated.

---

# 13. Recovery

Recovery means attempting to restore normal operation after an error.

Example:

```c
ErrorCode recover_temperature_sensor(uint8_t sensor_ok)
{
    if(sensor_ok == 1U)
    {
        return ERROR_NONE;
    }
    else
    {
        return ERROR_SENSOR;
    }
}
```

Meaning:

```text
sensor_ok = 1
    ↓
Recovery successful
    ↓
ERROR_NONE
```

or:

```text
sensor_ok = 0
    ↓
Recovery failed
    ↓
ERROR_SENSOR
```

---

# 14. Fallback

Fallback is what the system does when normal recovery cannot restore operation.

Example:

```c
ErrorCode fallback_temperature_sensor(ErrorCode recovery_result)
{
    if(recovery_result == ERROR_NONE)
    {
        printf("Sensor recovery successful\n");
        return ERROR_NONE;
    }
    else
    {
        printf("Sensor recovery failed\n");

        state = STATE_FAULTED;

        return ERROR_SENSOR;
    }
}
```

The important idea:

```text
Recovery failed
      ↓
Normal operation cannot safely continue
      ↓
Enter safe state
      ↓
STATE_FAULTED
```

---

# 15. Recovery vs Fallback

These are different concepts.

### Recovery

Try to restore normal operation.

```text
Sensor error
    ↓
Retry
    ↓
Reinitialize sensor
    ↓
Sensor works
    ↓
Continue
```

### Fallback

Normal operation cannot be restored, so choose a safe alternative.

```text
Sensor error
    ↓
Retry
    ↓
Recovery
    ↓
Still failed
    ↓
Fallback
    ↓
FAULTED
```

---

# 16. Error Injection / Fault Injection

Error injection means deliberately creating an error during testing.

Example:

```c
uint8_t inject_sensor_error = 1U;
```

Then:

```c
ErrorCode read_temperature(uint16_t *temperature)
{
    if(inject_sensor_error == 1U)
    {
        return ERROR_SENSOR;
    }
    else
    {
        *temperature = 45U;
        return ERROR_NONE;
    }
}
```

Test cases:

```text
inject_sensor_error = 0
        ↓
Normal sensor operation
```

```text
inject_sensor_error = 1
        ↓
Simulated sensor failure
```

This allows the developer/test engineer to test failure paths without waiting for an actual hardware failure.

---

# 17. Why Error Injection is Important

A program can look correct during normal operation but still contain broken recovery logic.

Therefore test:

```text
Normal path
Failure path
Recovery path
Fallback path
Fault path
```

For an EV charger:

```text
CHARGING
   ↓
Temperature sensor fails
   ↓
Retry × 3
   ↓
Recovery attempt
   ↓
Recovery fails
   ↓
STATE_FAULTED
```

This complete path must be tested.

---

# 18. Uninitialized Output Data

One important bug encountered during Day 22:

```c
uint16_t temperature;
```

If the sensor fails before writing to `temperature`, the variable does not contain a valid temperature.

Printing it can produce an unpredictable value, such as:

```text
Temperature = 6056
```

The correct protection is:

```c
if(result == ERROR_NONE)
{
    printf("Temperature = %u\n", temperature);
}
```

Optionally initialize the variable:

```c
uint16_t temperature = 0U;
```

But initialization does **not** make `0U` a valid sensor reading.

The real protection is to check the error status before using the data.

---

# 19. Pointer Levels — Important Review Point

If a function receives:

```c
uint16_t *temperature
```

then `temperature` is already a pointer.

Therefore:

```c
check_temperature(temperature, sensor_ok);
```

is correct.

But:

```c
check_temperature(&temperature, sensor_ok);
```

would pass a pointer to a pointer:

```text
temperature       → uint16_t *
&temperature      → uint16_t **
```

General rule:

> Use `&variable` when the function expects a pointer and you currently have the actual variable.

Example:

```c
uint16_t temperature;

read_temperature(&temperature);
```

Inside another function where you already have:

```c
uint16_t *temperature;
```

use:

```c
read_temperature(temperature);
```

---

# 20. High-Level Error Flow

The final Day 22 architecture:

```text
                process_temperature()
                         ↓
                 check_temperature()
                         ↓
              read_temperature_with_retry()
                         ↓
                  read_temperature()
                         ↓
                    ERROR_SENSOR
                         ↓
                   Retry × 3
                         ↓
                  Recovery attempt
                         ↓
                 ┌───────┴───────┐
                 ↓               ↓
             SUCCESS           FAILED
                 ↓               ↓
              Continue        Fallback
                                 ↓
                           STATE_FAULTED
```

---

# 21. EV Charger Example

Suppose an EV charger is charging at 50 kW.

The temperature sensor suddenly stops responding.

A safe firmware strategy can be:

```text
CHARGING
   ↓
Sensor read fails
   ↓
ERROR_SENSOR
   ↓
Retry 1
   ↓
Retry 2
   ↓
Retry 3
   ↓
Recovery
   ↓
Failed
   ↓
Fallback
   ↓
Stop charging
   ↓
STATE_FAULTED
```

The system should not simply invent a temperature such as:

```c
temperature = 25U;
```

unless the product specification explicitly defines such a fallback.

---

# 22. Main Coordinator Function

The final high-level function was:

```c
ErrorCode process_temperature(uint16_t *temperature,
                              uint8_t sensor_ok)
{
    ErrorCode result = check_temperature(temperature, sensor_ok);

    if(result == ERROR_SENSOR)
    {
        ErrorCode recovery_result;

        recovery_result = recover_temperature_sensor(sensor_ok);

        result = fallback_temperature_sensor(recovery_result);
    }

    return result;
}
```

Its job is coordination.

It does not duplicate:

* sensor reading
* retry logic
* recovery logic
* fallback logic

Each function has a focused responsibility.

---

# 23. Main Function Concept

The main application can call:

```c
result = process_temperature(&temperature, sensor_ok);
```

Then handle the final result:

```c
result = handle_error(result);
```

Only use the temperature when the final result indicates success:

```c
if(result == ERROR_NONE)
{
    printf("Temperature = %u\n", temperature);
}
```

---

# 24. Important Embedded C Lessons from Day 22

### Lesson 1

An error return is information.

```c
ErrorCode result;
```

Do not ignore it.

### Lesson 2

Errors should be propagated upward.

```c
return result;
```

### Lesson 3

Different errors require different handling.

### Lesson 4

Retries must be bounded.

```c
#define MAX_ATTEMPTS 3U
```

### Lesson 5

Recovery and fallback are different.

```text
Recovery → restore normal operation
Fallback → safely handle inability to restore
```

### Lesson 6

Never use invalid output data.

### Lesson 7

Error injection is essential for testing failure paths.

### Lesson 8

Error handling and state machines work together.

```text
Error → Decision → State transition
```

---

# 25. Day 22 Interview Questions

### Q1. What is error propagation?

Passing an error from a lower-level function to a higher-level function so the higher level can decide the appropriate response.

### Q2. Why use an `ErrorCode` enum instead of a Boolean?

A Boolean only indicates success/failure. An `ErrorCode` can identify the specific problem.

### Q3. Why is bounded retry important?

It prevents the system from getting stuck indefinitely retrying an operation.

### Q4. Should every error be retried?

No. Only errors for which retry/recovery is appropriate should be retried.

### Q5. What is the difference between recovery and fallback?

Recovery attempts to restore normal operation. Fallback provides a safe alternative when recovery fails.

### Q6. Why shouldn't an EV charger blindly continue after a temperature-sensor failure?

Because the controller may no longer have trustworthy thermal information needed to operate safely.

### Q7. What is error injection?

Deliberately generating an error during testing to verify error-handling and recovery behavior.

### Q8. Why should output data not be used after a failed function call?

Because the function may not have written valid data to the output variable.

---

# Day 22 Completion Checklist

* [x] Error codes using `enum`
* [x] `ERROR_NONE`
* [x] Error detection
* [x] Error propagation
* [x] Error handling
* [x] Error vs fault
* [x] Error severity
* [x] Bounded retry
* [x] Recovery
* [x] Fallback
* [x] Safe `FAULTED` state
* [x] Error injection
* [x] Failure-path testing
* [x] Output-data safety
* [x] Pointer-level understanding
* [x] Error handling + state machine integration
* [x] Final end-to-end implementation

## Day 22 Status: COMPLETE ✅

Next official topic: **Day 23 — MISRA C**
