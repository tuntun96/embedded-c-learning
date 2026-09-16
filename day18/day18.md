# Professional Notes — Day 18

## Interrupts, EXTI, NVIC, ISR, `volatile` & Debouncing — STM32F072

### 1. Interrupt — Core Concept

An **interrupt** is a hardware/software mechanism that temporarily stops normal CPU execution so the processor can respond to an important event.

Instead of continuously checking whether an event occurred, the CPU is **notified when the event occurs**.

```text
Normal execution
      ↓
Interrupt occurs
      ↓
CPU services interrupt
      ↓
ISR executes
      ↓
Return to normal execution
```

### Interrupt vs Polling

| Polling                               | Interrupt                                 |
| ------------------------------------- | ----------------------------------------- |
| CPU continuously checks an event      | Hardware notifies CPU                     |
| CPU wastes execution time checking    | CPU can perform other work                |
| Response depends on polling frequency | Event can be serviced immediately         |
| Simple                                | More efficient but requires configuration |

Example:

```c
while (1)
{
    if (button_pressed)
    {
        // handle button
    }
}
```

This is polling.

With an interrupt:

```text
Button → EXTI → NVIC → CPU → ISR
```

the CPU doesn't need to continuously check the physical pin.

---

# 2. ISR — Interrupt Service Routine

An **ISR** is the function executed when an interrupt occurs.

Important ISR characteristics:

* Should execute quickly.
* Avoid long calculations.
* Avoid blocking delays.
* Avoid unnecessary `printf()`.
* Avoid complex communication processing.
* Usually capture the event and return.

### Good pattern

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13)
    {
        button_event = 1U;
    }
}
```

The ISR records the event and exits quickly.

---

# 3. STM32 EXTI

**EXTI = External Interrupt/Event Controller**

It allows GPIO pins to generate interrupts when their electrical state changes according to the configured trigger.

For our NUCLEO-F072RB:

```text
B1 User Button
      ↓
    PC13
      ↓
   EXTI13
```

PC13 was configured as:

```text
GPIO_EXTI13
```

---

# 4. NVIC

**NVIC = Nested Vectored Interrupt Controller**

It is part of the ARM Cortex-M processor architecture.

Its responsibilities include:

* enabling/disabling interrupts
* managing interrupt priority
* selecting which pending interrupt gets serviced
* supporting interrupt nesting/preemption

For our STM32F072:

```c
HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
```

enables the EXTI4–15 interrupt group.

---

# 5. Why `EXTI4_15_IRQHandler()`?

The STM32F072 groups EXTI lines 4 through 15 into a common IRQ.

Therefore PC13 does not have a unique:

```c
PC13_IRQHandler()
```

Instead:

```c
void EXTI4_15_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
```

The flow is:

```text
PC13
 ↓
EXTI13
 ↓
EXTI4_15 IRQ
 ↓
EXTI4_15_IRQHandler()
```

---

# 6. HAL Interrupt Handling

CubeIDE generates:

```c
void EXTI4_15_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
```

The HAL then processes the EXTI event and eventually invokes:

```c
HAL_GPIO_EXTI_Callback(GPIO_PIN_13);
```

Your application logic goes into the callback.

This separates:

```text
Hardware/driver infrastructure
        ↓
Application logic
```

---

# 7. Interrupt Priority

CubeIDE generated:

```c
HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
```

### `HAL_NVIC_SetPriority()`

```c
HAL_NVIC_SetPriority(IRQn, priority, subpriority);
```

Your configuration:

```c
HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
```

means:

```text
IRQ              = EXTI4_15
Preemption       = 0
Subpriority      = 0
```

For Cortex-M priority numbering, **lower numerical values represent higher urgency**.

So:

```text
Priority 0 → highest
Priority 1 → lower
Priority 2 → lower
Priority 3 → lower
```

### `HAL_NVIC_EnableIRQ()`

```c
HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
```

allows the NVIC to service that interrupt.

Remember:

> **Priority determines urgency; Enable determines whether the interrupt is allowed to execute.**

---

# 8. Interrupt Preemption

A higher-priority interrupt can interrupt a lower-priority ISR.

Example:

```text
UART ISR — Priority 2
       ↓
UART ISR running
       ↓
Fault interrupt — Priority 0
       ↓
Fault ISR executes
       ↓
Fault ISR returns
       ↓
UART ISR resumes
```

This is called **interrupt preemption/nesting**.

### EV charger example

A possible priority strategy could be:

```text
Critical protection/fault → High priority
CAN communication        → Medium-high
UART communication       → Medium
User button              → Lower
Periodic background task → Lower
```

Actual priorities must be designed according to system requirements.

---

# 9. `volatile` in Interrupt Programming

When a variable is modified in an ISR and accessed from normal program execution, it should generally be declared `volatile`.

Our example:

```c
volatile uint8_t button_event = 0U;
```

ISR:

```c
button_event = 1U;
```

Main loop:

```c
if (button_event != 0U)
{
    button_event = 0U;
    // process event
}
```

### Why `volatile`?

Without `volatile`, the compiler may optimize accesses based on the assumption that the variable doesn't unexpectedly change.

`volatile` tells the compiler:

> "This variable can change outside the normal flow of this code. Always perform the required memory access."

### Important

`volatile` does **not** automatically make an operation atomic or thread-safe.

It only controls compiler optimization/visibility of accesses.

---

# 10. ISR + Event Flag Architecture

A professional embedded pattern is:

```text
             Hardware Event
                   ↓
                 ISR
                   ↓
        Set volatile event flag
                   ↓
              ISR returns
                   ↓
              Main loop
                   ↓
          Process the event
```

Example:

```c
volatile uint8_t button_event = 0U;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13)
    {
        button_event = 1U;
    }
}
```

Then:

```c
while (1)
{
    if (button_event != 0U)
    {
        button_event = 0U;

        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}
```

This is preferable to doing heavy work directly inside the ISR.

---

# 11. Mechanical Button Debouncing

A physical push button does not always transition cleanly between logic states.

When pressed:

```text
Ideal:

HIGH ─────────┐
              └──────── LOW
```

Actual mechanical signal may temporarily bounce:

```text
HIGH ─────┐ ┌─┐ ┌──┐
          └─┘ └─┘  └──── LOW
```

One physical press can therefore produce multiple transitions.

Without debouncing:

```text
1 physical press
       ↓
multiple interrupts
       ↓
multiple LED toggles
```

---

# 12. Software Debouncing

A common approach is to ignore events occurring within a short time window.

For example:

```c
#define BUTTON_DEBOUNCE_MS 50U
```

Use the STM32 HAL tick:

```c
HAL_GetTick()
```

which provides a millisecond time base.

Typical logic:

```c
if ((current_time - last_button_time) >= BUTTON_DEBOUNCE_MS)
{
    // accept event
}
```

### Why 50 ms?

There is **no universal debounce value**.

The value should be based on:

1. Button datasheet
2. Actual hardware measurement
3. Oscilloscope/logic-analyzer observation
4. Application response requirements

Typical starting values for mechanical buttons may be around **10–50 ms**.

For our NUCLEO experiment:

```c
50U
```

is a reasonable starting value.

---

# 13. Why `uint32_t` for `HAL_GetTick()`?

`HAL_GetTick()` returns a 32-bit millisecond counter.

Therefore:

```c
uint32_t current_time;
uint32_t last_button_time;
```

is appropriate.

A 32-bit unsigned counter can represent:

```text
0 → 4,294,967,295 ms
```

which is approximately:

**49.7 days**

before wrapping around.

This is why embedded code commonly uses:

```c
uint32_t
```

for millisecond tick calculations.

---

# 14. Safe Timer Difference Pattern

Prefer:

```c
if ((current_time - last_button_time) >= timeout)
```

rather than:

```c
if (current_time >= last_button_time + timeout)
```

The subtraction form works correctly across normal unsigned timer rollover, assuming the elapsed interval is within the valid range of the counter.

This pattern is extremely common in embedded firmware:

```c
if ((HAL_GetTick() - start_time) >= timeout)
{
    // timeout occurred
}
```

You will use this for:

* communication timeout
* sensor timeout
* button debounce
* periodic tasks
* charger timeout
* state-machine timing

---

# 15. Final Practical Architecture

Your Day 18 implementation now looks like:

```text
             NUCLEO-F072RB
                  │
             B1 User Button
                  │
                PC13
                  │
               EXTI13
                  │
              NVIC IRQ
                  │
        EXTI4_15_IRQHandler()
                  │
       HAL_GPIO_EXTI_IRQHandler()
                  │
       HAL_GPIO_EXTI_Callback()
                  │
        button_event = 1
                  │
             ISR returns
                  │
              main()
                  │
        debounce/time check
                  │
        button_event = 0
                  │
       HAL_GPIO_TogglePin()
                  │
                PA5
                  │
                LD2
```

You physically verified this entire chain on your **NUCLEO-F072RB**. That's the most important part of Day 18.

---

## Day 18 — What you should be able to explain

By the end of this day, you should confidently explain:

* What an interrupt is
* Interrupt vs polling
* What EXTI does
* What NVIC does
* Why PC13 uses `EXTI4_15_IRQn`
* What an ISR is
* What `EXTI4_15_IRQHandler()` does
* What `HAL_GPIO_EXTI_IRQHandler()` does
* What `HAL_GPIO_EXTI_Callback()` is used for
* Why ISR code should be short
* Why `volatile` is used with ISR-shared variables
* Why event flags are useful
* What interrupt priority means
* What interrupt preemption means
* Why mechanical buttons need debouncing
* How `HAL_GetTick()` can be used for timing
* Why `uint32_t` is used for the HAL tick
* Why unsigned subtraction is used for timeout calculations
* How CubeIDE-generated HAL initialization fits into the overall firmware architecture

### Day 18 practical achievement

**PC13 button → EXTI interrupt → NVIC → ISR → `volatile` event flag → debounce → main loop → PA5/LD2**

**Status: Day 18 COMPLETE ✅**
