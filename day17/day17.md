## Hardware Registers & Advanced Bit Manipulation

### 1. Hardware Register

A **hardware register** is a memory location associated with a microcontroller peripheral or hardware block.

Firmware uses registers to:

* configure hardware
* enable/disable features
* read hardware status
* control peripheral behavior

Think of a register as:

> **A hardware-connected variable.**

---

## 2. Register Address vs Register Value

These are different concepts.

```text
Register Address → WHERE the register exists
Register Value   → WHAT data/bits are currently stored
```

Example:

```text
Address = 0x40001000
Value   = 0x00000009
```

The address identifies the register's location, while the value represents its current configuration or status.

---

## 3. Memory-Mapped Registers

Microcontrollers commonly map peripheral registers into their memory address space.

Conceptually:

```c
#define CONTROL_REG (*(volatile uint32_t *)0x40001000U)
```

Then:

```c
CONTROL_REG = 0x01U;
```

means the firmware writes `0x01` to that memory-mapped register.

Reading:

```c
uint32_t value = CONTROL_REG;
```

reads the register.

This combines three important Embedded C concepts:

```text
Pointer
   +
Memory address
   +
volatile
   ↓
Hardware register access
```

---

## 4. Why `volatile`?

Hardware can change a register independently of normal program execution.

For example:

```text
Peripheral
    ↓
updates status register
    ↓
CPU reads register
```

`volatile` tells the compiler that the value can change externally and that accesses must not simply be optimized away or assumed unchanged.

Typical form:

```c
volatile uint32_t *reg;
```

or:

```c
#define STATUS_REG (*(volatile uint32_t *)ADDRESS)
```

---

# 5. Control Register

A control register is generally used by firmware to **configure or control hardware**.

Example EV charger:

```text
CONTROL REGISTER

Bit 0 → Charger Enable
Bit 1 → Gun 1 Enable
Bit 2 → Gun 2 Enable
Bit 3 → Fan Enable
```

Example:

```text
0000 1011
```

means:

```text
Charger → ON
Gun 1   → ON
Gun 2   → OFF
Fan     → ON
```

---

# 6. Status Register

A status register generally reports **what the hardware is currently experiencing**.

Example:

```text
STATUS REGISTER

Bit 0 → Charging
Bit 1 → Fault
Bit 2 → Over Temperature
Bit 3 → Communication Error
```

Example:

```text
0000 0001
```

means:

```text
Charging          → ON
Fault             → OFF
Over Temperature  → OFF
Communication     → OK
```

### Key distinction

```text
CONTROL REGISTER
Firmware → Hardware

STATUS REGISTER
Hardware → Firmware
```

---

# 7. Bit Masks

Instead of using unexplained hexadecimal values, define meaningful masks.

```c
#define CHARGER_ENABLE_MASK (1U << 0U)
#define GUN1_ENABLE_MASK    (1U << 1U)
#define GUN2_ENABLE_MASK    (1U << 2U)
#define FAN_ENABLE_MASK     (1U << 3U)
```

This is better than:

```c
control_reg |= 0x04U;
```

because the purpose of Bit 2 is immediately clear.

---

# 8. Four Core Bit Operations

These are fundamental Embedded C register operations.

### Set a bit

```c
reg |= MASK;
```

Changes:

```text
0 → 1
```

without changing unrelated bits.

---

### Clear a bit

```c
reg &= ~MASK;
```

Changes:

```text
1 → 0
```

without changing unrelated bits.

---

### Toggle a bit

```c
reg ^= MASK;
```

Changes:

```text
0 → 1
1 → 0
```

---

### Test a bit

```c
if (reg & MASK)
{
    /* Bit is set */
}
```

For clearer embedded code:

```c
if ((reg & MASK) != 0U)
{
    /* Bit is set */
}
```

---

# 9. Read-Modify-Write

**Read-Modify-Write (RMW)** means:

```text
Read register
      ↓
Modify selected bit(s)
      ↓
Write register back
```

Example:

```c
control_reg |= GUN2_ENABLE_MASK;
```

Conceptually:

```text
READ
0000 1011

MODIFY
Set Bit 2

WRITE
0000 1111
```

The important point is:

> **Only the required bits are changed; existing unrelated bits are preserved.**

---

# 10. Why Direct Assignment Can Be Dangerous

Suppose:

```text
CONTROL_REG = 0000 1011
```

You want to enable Gun 2.

### ❌ Incorrect

```c
CONTROL_REG = GUN2_ENABLE_MASK;
```

Result:

```text
0000 0100
```

You accidentally cleared the other configuration bits.

### ✅ Correct

```c
CONTROL_REG |= GUN2_ENABLE_MASK;
```

Result:

```text
0000 1111
```

Only Bit 2 was added.

---

# 11. EV Charger Example

A simplified control register:

```c
#define CHARGER_ENABLE_MASK (1U << 0U)
#define GUN1_ENABLE_MASK    (1U << 1U)
#define GUN2_ENABLE_MASK    (1U << 2U)
#define FAN_ENABLE_MASK     (1U << 3U)
```

Starting value:

```c
uint8_t control_reg = 0x0BU;
```

Binary:

```text
0000 1011
```

Enable Gun 2:

```c
control_reg |= GUN2_ENABLE_MASK;
```

Result:

```text
0000 1111
```

Disable Gun 1:

```c
control_reg &= ~GUN1_ENABLE_MASK;
```

Result:

```text
0000 1101
```

Toggle Fan:

```c
control_reg ^= FAN_ENABLE_MASK;
```

Result:

```text
0000 0101
```

Final state:

```text
Charger → ON
Gun 1   → OFF
Gun 2   → ON
Fan     → OFF
```

---

# 12. Status Register Decision Logic

A charger should allow charging only when all required conditions are satisfied.

For example:

```c
if ((control_reg & CHARGER_ENABLE_MASK) &&
    (control_reg & GUN1_ENABLE_MASK) &&
    (status_reg & CHARGING_STATUS_MASK) &&
    !(status_reg & FAULT_STATUS_MASK) &&
    !(status_reg & OVER_TEMPERATURE_MASK) &&
    !(status_reg & COMM_ERROR_MASK))
{
    /* Charging allowed */
}
```

Read it as:

```text
Charger enabled
AND
Gun 1 enabled
AND
Charging active
AND
No fault
AND
No over-temperature
AND
No communication error
```

→ **Charging allowed**

This is a practical example of converting individual hardware flags into a firmware safety decision.

---

# 13. Important Status-Register Principle

If:

```c
uint8_t status_reg = 0U;
```

then all bits start as zero.

If we only want to simulate Charging = ON:

```c
status_reg |= CHARGING_STATUS_MASK;
```

We don't need to explicitly clear the other bits.

More importantly, with a **real hardware status register**, firmware normally **reads and interprets the bits** rather than arbitrarily clearing them.

```text
Control register → usually modify
Status register  → usually read/test
```

The exact behavior depends on the peripheral specification; some status registers have special clear-on-write or write-one-to-clear semantics.

---

# 14. Common Embedded Mistakes

### Mistake 1 — Wrong mask

```c
if (status_reg & CHARGING_STATUS_MASK)
{
    printf("Communication OK");
}
```

The code is testing the wrong bit.

Always ensure:

```text
Bit → Correct mask → Correct meaning
```

---

### Mistake 2 — Overwriting the register

```c
reg = MASK;
```

when you intended to set one bit.

Prefer:

```c
reg |= MASK;
```

---

### Mistake 3 — Forgetting `~` while clearing

Incorrect:

```c
reg &= MASK;
```

Correct:

```c
reg &= ~MASK;
```

---

### Mistake 4 — Confusing status with enable

For example:

```c
#define CHARGING_ENABLE_MASK
```

for a status bit can be misleading.

Better:

```c
#define CHARGING_STATUS_MASK
```

when the bit represents whether charging is currently active.

---

# 15. Professional Embedded C Pattern

A clean register interface often looks like:

```c
#define FEATURE_ENABLE_MASK   (1U << 0U)
#define FAULT_STATUS_MASK     (1U << 1U)

reg |= FEATURE_ENABLE_MASK;       /* Set */
reg &= ~FEATURE_ENABLE_MASK;     /* Clear */
reg ^= FEATURE_ENABLE_MASK;      /* Toggle */

if ((reg & FAULT_STATUS_MASK) != 0U)
{
    /* Fault detected */
}
```

This style is:

* readable
* maintainable
* hardware-oriented
* less error-prone
* easy to review

---

# Very Important Takeaways

```text
Register address → WHERE
Register value   → WHAT
```

```text
Set    → |= MASK
Clear  → &= ~MASK
Toggle → ^= MASK
Test   → & MASK
```

```text
Control register → Firmware controls hardware
Status register  → Firmware reads hardware state
```

```text
Read → Modify → Write
```

And the most important practical rule:

> **When modifying a register, change only the bits you intend to change and preserve unrelated bits.**


