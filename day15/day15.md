# Day 15 — Professional Notes: Memory Layout

## 1. Embedded System Memory

An embedded system generally uses two major memory categories:

* **Flash** — non-volatile; typically stores program code and read-only data.
* **RAM** — volatile; stores writable runtime data.

```text
MCU Memory
│
├── Flash
│   ├── .text
│   ├── .rdata / .rodata
│   └── .data initial-value image
│
└── RAM
    ├── .data
    ├── .bss
    ├── Heap
    └── Stack
```

The exact arrangement depends on the MCU, compiler, linker script, and startup configuration.

---

## 2. `.text`

Contains executable machine code.

```c
void charger_task(void)
{
    /* ... */
}
```

Typically:

```text
Function → .text → Flash
```

---

## 3. `.rdata` / `.rodata`

Contains read-only data.

Example:

```c
const uint16_t max_voltage = 1000U;
const char message[] = "Charging Started";
```

On your Windows/GCC environment, you observed:

```text
.rdata
```

instead of `.rodata`.

Typically:

```text
const data → .rdata/.rodata → Flash
```

---

## 4. `.data`

Contains initialized, writable global/static data.

Example:

```c
uint16_t voltage = 750U;
static uint16_t current = 150U;
```

Runtime location:

```text
RAM → .data
```

However, the initial values must be stored in the firmware image in Flash and copied to RAM during startup.

```text
Flash
  │
  │ initial value
  ↓
RAM .data
```

---

## 5. `.bss`

Contains zero-initialized or uninitialized global/static data.

Examples:

```c
uint16_t current;

static uint8_t can_buffer[512];
```

Typically:

```text
.bss → RAM
```

Startup code normally clears this region to zero.

---

## 6. Stack

The stack is used for automatic runtime data such as local variables and function-call information.

Example:

```c
void charger_task(void)
{
    uint16_t voltage = 750U;
}
```

`voltage` normally uses stack storage.

Stack usage changes as functions are called and return.

Large local arrays can cause **stack overflow** on memory-constrained MCUs.

---

## 7. Heap

The heap is used for dynamic allocation.

Example:

```c
uint8_t *buffer = malloc(1024);
```

Dynamic allocation can introduce:

* fragmentation
* allocation failure
* less predictable memory usage
* debugging complexity

For predictable embedded systems, fixed/static allocation is often preferred when practical.

---

## 8. Static vs Automatic Storage

### Automatic local

```c
void task(void)
{
    uint8_t buffer[2048];
}
```

Typically:

```text
buffer → Stack
```

The storage is associated with the function invocation.

### Static

```c
static uint8_t buffer[2048];
```

Typically:

```text
buffer → .bss
```

The object exists throughout program execution.

**Important:**

```text
static ≠ persistent across power OFF
```

Static RAM loses its contents when power is removed unless the system specifically uses retained/non-volatile memory.

---

## 9. Initialized vs Uninitialized Arrays

```c
uint8_t buffer[300];
```

→ `.bss`

```c
uint8_t buffer[300] = {1U, 2U, 3U};
```

→ `.data`

```c
const uint8_t lookup[300] = {1U, 2U, 3U};
```

→ `.rdata/.rodata`

Typical resource impact:

| Declaration                   | Runtime location | Typical resource                |
| ----------------------------- | ---------------- | ------------------------------- |
| `uint8_t a[300];`             | `.bss`           | 300 B RAM                       |
| `uint8_t a[300] = {1};`       | `.data`          | 300 B RAM + initial Flash image |
| `const uint8_t a[300] = {1};` | `.rdata/.rodata` | ~300 B Flash                    |

---

## 10. Memory Map Analysis

For an MCU with:

```text
RAM   = 16 KB
.data = 2 KB
.bss  = 5 KB
Stack = 6 KB
Heap  = 1 KB
```

Total runtime RAM consumption:

```text
2 + 5 + 6 + 1 = 14 KB
```

Remaining:

```text
16 - 14 = 2 KB
```

Adding a 2 KB local buffer at maximum stack depth can therefore create a stack/RAM capacity problem.

---

## 11. GCC Practical Verification

You verified memory sections using:

```text
gcc -c memory_layout.c -o memory_layout.o
```

and:

```text
objdump -h memory_layout.o
```

You observed sections including:

```text
.text
.data
.bss
.rdata
```

This confirmed the theoretical memory-placement rules using an actual compiler toolchain.

---

## 12. EV Charger Example

For firmware such as an EV charger:

```c
uint16_t voltage = 750U;
uint16_t current;
static uint8_t can_buffer[512];
const uint16_t max_voltage = 1000U;

void charger_task(void)
{
    uint8_t state = 2U;
}
```

Typical placement:

```text
voltage       → .data
current       → .bss
can_buffer    → .bss
max_voltage   → .rdata/.rodata
state         → Stack
charger_task  → .text
```

### Core mental model

> **C declaration → storage duration/type → memory section → RAM/Flash → startup/runtime behavior**

This is the key Day 15 concept to retain.

---

