# Day 17 — Hardware Registers & Advanced Bit Manipulation

We already covered basic bitwise operators and masks on **Day 3**, and `volatile` on **Day 11**. So today we'll connect those concepts to **real embedded firmware** instead of repeating them.

### Today's target

By the end of Day 17, we should understand:

* What a hardware register is
* Register address vs register value
* Control vs status registers
* Set/clear/toggle/test bits in registers
* Register masks
* Read-modify-write
* Why `volatile` is important for hardware registers
* How firmware controls hardware through registers


---

# 1. What is a Hardware Register?

A hardware register is a small memory location inside a microcontroller/peripheral that firmware can read or write to control or monitor hardware.

Think of it as:

> **A variable that is directly connected to hardware.**

For example, imagine an EV charger controller has:

CONTROL_REGISTER

with:

Bit 0 → Charger Enable
Bit 1 → Gun 1 Enable
Bit 2 → Gun 2 Enable
Bit 3 → Fan Enable

The firmware might write:

0000 0001

This means:

Charger Enable = ON
Gun 1          = OFF
Gun 2          = OFF
Fan            = OFF

If firmware writes:
0000 1001


then:
Bit 0 = 1 → Charger ON
Bit 3 = 1 → Fan ON

---

# 2. Register Address vs Register Value

This is extremely important.

Suppose:

CONTROL_REGISTER address = 0x40001000

The **address** tells us:

> Where is the register?

The **value** tells us:

> What is currently stored/configured in that register?

For example:

Address: 0x40001000
Value:   0x00000009

So:

0x40001000 → location
0x00000009 → value

This connects directly to our pointer knowledge.

---

# 3. Hardware Register Using a Pointer

In C, a memory-mapped register can conceptually be accessed using a pointer:

```c
#define CONTROL_REG (*(volatile uint32_t *)0x40001000U)
```

Now:

```c
CONTROL_REG = 0x01U;
```

means:

> Write `0x01` to memory address `0x40001000`.

And:

```c
uint32_t status = CONTROL_REG;
```

means:

> Read the value from that hardware register.

This is one of the most important connections between:

```text
Pointers
+
volatile
+
bit manipulation
+
memory addresses
```

---

# 4. Why `volatile`?

You already learned `volatile` on Day 11.

Now apply it to hardware.

Consider:

```c
#define STATUS_REG (*(volatile uint32_t *)0x40002000U)
```

The `volatile` tells the compiler:

> This memory can change outside the normal program flow, so every read/write must actually access the memory.

Hardware can change a register without your C code explicitly modifying it.

For example:

```text
ADC conversion
      ↓
Hardware updates register
      ↓
Firmware reads register
```

Without proper `volatile` usage, compiler optimization can cause incorrect assumptions about the value.

---

# 5. Control Register vs Status Register

A common embedded pattern is:

### Control register

Firmware **writes** it to control hardware.

```text
CONTROL_REG

Bit 0 → Enable
Bit 1 → Reset
Bit 2 → Fan
Bit 3 → Relay
```

### Status register

Firmware **reads** it to determine hardware state.

```text
STATUS_REG

Bit 0 → Ready
Bit 1 → Charging
Bit 2 → Fault
Bit 3 → Over Temperature
```

Think:

```text
CONTROL → Firmware → Hardware

STATUS  ← Firmware ← Hardware
```

---

# 6. Set a Bit

Suppose:

```c
uint8_t control = 0x00U;
```

We want to enable Bit 0.

Mask:

```c
#define CHARGER_ENABLE_MASK (1U << 0)
```

Then:

```c
control |= CHARGER_ENABLE_MASK;
```

Result:

```text
Before:
0000 0000

Mask:
0000 0001

After:
0000 0001
```

### Meaning

```text
OR with 1 → set bit
```

---

# 7. Set Another Bit

Fan is Bit 3:

```c
#define FAN_ENABLE_MASK (1U << 3)
```

Now:

```c
control |= FAN_ENABLE_MASK;
```

If control was:

```text
0000 0001
```

it becomes:

```text
0000 1001
```

We changed Bit 3 without disturbing Bit 0.

This is the power of masking.

---

# 8. Clear a Bit

Suppose:

```text
0000 1001
```

We want to turn OFF the fan, Bit 3.

Use:

```c
control &= ~(1U << 3);
```

Step by step:

```text
1U << 3

0000 1000
```

Invert:

```text
1111 0111
```

AND:

```text
0000 1001
1111 0111
---------
0000 0001
```

Bit 3 is cleared.

### Rule

```text
Set   → |= mask
Clear → &= ~mask
```

---

# 9. Toggle a Bit

Toggle means:

```text
0 → 1
1 → 0
```

Use XOR:

```c
control ^= (1U << 3);
```

For example:

```text
Before:
0000 0001

Toggle Bit 3:

0000 1001
```

Again:

```c
control ^= (1U << 3);
```

Result:

```text
0000 0001
```

### Rule

```text
Toggle → ^= mask
```

---

# 10. Test / Read a Bit

Suppose:

```c
uint8_t status = 0x05U;
```

Binary:

```text
0000 0101
```

We want to check Bit 2:

```c
if (status & (1U << 2))
{
    printf("Fault detected\n");
}
```

Because:

```text
0000 0101
0000 0100
---------
0000 0100
```

Result is non-zero → Bit 2 is set.

### Rule

```text
Test bit → value & mask
```

---

# 11. The Four Core Operations

This is worth memorizing:

```text
SET
reg |= mask;

CLEAR
reg &= ~mask;

TOGGLE
reg ^= mask;

TEST
reg & mask;
```

These four operations appear constantly in embedded firmware.

---

# 12. Read-Modify-Write

This is a very important embedded concept.

Suppose:

```text
CONTROL_REG = 0000 1001
```

We want to set Bit 1.

We should **not** simply write:

```c
CONTROL_REG = (1U << 1);
```

Because that produces:

```text
0000 0010
```

and destroys the existing Bit 0 and Bit 3 configuration.

Instead:

```c
CONTROL_REG |= (1U << 1);
```

Conceptually:

```text
READ
  ↓
0000 1001

MODIFY
  ↓
set Bit 1

WRITE
  ↓
0000 1011
```

This is called:

> **Read-Modify-Write (RMW)**

---

# 13. Why RMW Matters

Imagine:

```text
CONTROL_REGISTER

Bit 0 → Charger Enable
Bit 1 → Gun 1 Enable
Bit 2 → Gun 2 Enable
Bit 3 → Fan Enable
```

Current:

```text
0000 1001
```

Meaning:

```text
Charger = ON
Gun 1   = OFF
Gun 2   = OFF
Fan     = ON
```

If you want to enable Gun 1:

### Wrong

```c
CONTROL_REG = 0x02U;
```

Result:

```text
0000 0010
```

You've accidentally turned OFF the charger and fan.

### Correct

```c
CONTROL_REG |= (1U << 1);
```

Result:

```text
0000 1011
```

Only Gun 1 changed.

---

# 14. Practical EV Charger Register Model

We'll simulate a hardware register using a normal variable today.

```c
uint8_t control_reg = 0x00U;
```

Define masks:

```c
#define CHARGER_ENABLE   (1U << 0)
#define GUN1_ENABLE       (1U << 1)
#define GUN2_ENABLE       (1U << 2)
#define FAN_ENABLE        (1U << 3)
```

Then:

```c
control_reg |= CHARGER_ENABLE;
control_reg |= GUN1_ENABLE;
control_reg |= FAN_ENABLE;
```

Result:

```text
0000 1011
```

---

# 15. Why We Are Simulating the Register

We don't want to access a random hardware address on your PC.

For example:

```c
*(volatile uint32_t *)0x40001000U
```

is appropriate only when that address actually belongs to a peripheral on the target MCU.

On your Windows PC, that address doesn't represent your EV charger's hardware register.

So today we'll use:

```c
uint8_t control_reg;
```

to simulate the register.

Later, when we work with STM32, we'll see actual peripheral registers and memory-mapped addresses.

---

# Your understanding check

Answer these in your own words:

### Q1

What is the difference between a **register address** and a **register value**?

### Q2

Why is `volatile` commonly used with hardware registers?

### Q3

How do you **set Bit 3**?

### Q4

How do you **clear Bit 3**?

### Q5

How do you **toggle Bit 3**?

### Q6

How do you test whether **Bit 2 is set**?

### Q7

Why is this dangerous?

```c
CONTROL_REG = (1U << 1);
```

when other bits already contain important settings?

### Q8

What does **Read-Modify-Write** mean?

After these, we'll do the **EV Charger Control Register hands-on exercise**.
