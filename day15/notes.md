# 🚀 Day 15 — Memory Layout in Embedded C

# 1. What is Memory Layout?

When we compile a C program, the variables and functions don't simply exist in "memory."

The compiler and linker organize the program into different memory sections.

A simplified view is:


                 MCU MEMORY
        ┌─────────────────────────┐
        │         FLASH           │
        │                         │
        │  .text                  │
        │  Program instructions   │
        │                         │
        │  .rodata                │
        │  Read-only constants    │
        │                         │
        │  Initial values         │
        │  for .data              │
        ├─────────────────────────┤
        │          RAM            │
        │                         │
        │  .data                  │
        │  .bss                   │
        │  Heap                   │
        │                         │
        │          ↓              │
        │        Stack            │
        │          ↑              │
        └─────────────────────────┘
```

This is a conceptual model. The exact layout depends on the MCU, compiler, linker script and startup configuration.

---

# 2. Flash vs RAM


### Flash

Flash is typically:

* non-volatile
* retains data after power off
* used to store firmware
* generally where program code resides

Example:

```c
void start_charging(void)
{
    /* charging logic */
}
```

The machine instructions generated for this function are typically stored in Flash.

### RAM

RAM is typically:

* volatile
* loses contents after power off
* used for runtime variables
* used by stack
* may be used by heap

For example:

```c
uint16_t current = 150U;
```

The runtime value needs RAM if it is a normal writable object.

---

# 3. `.text`

The `.text` section generally contains the executable machine code.

Example:

void start_charging(void)
{
    printf("Charging started\n");
}


The instructions generated for `start_charging()` belong to the executable code area, typically `.text`.

Conceptually:

.text
┌─────────────────────────┐
│ main() instructions     │
│ start_charging()        │
│ stop_charging()         │
│ read_can_frame()        │
│ process_fault()         │
└─────────────────────────┘

In an MCU, this is normally located in Flash.

### Important interview point

> `.text` primarily contains executable program instructions.

---

# 4. `.rodata`

`.rodata` means read-only data.

Typical examples include string literals and constant read-only data.

Example:

const char message[] = "Charging Started";


and:
printf("Voltage = %u\n", voltage);

The string literal:

"Voltage = %u\n"

is commonly placed in a read-only section such as `.rodata`.

Conceptually:
.rodata
┌──────────────────────────┐
│ "Charging Started"       │
│ "Voltage = %u"           │
│ constant lookup tables   │
│ other read-only data     │
└──────────────────────────┘
On many embedded systems this is stored in Flash.

### Important distinction

Don't think:

> `const` always means `.rodata`.

That's too simplistic.

`const` means the object is not modifiable through its declared type. Where it is placed is ultimately determined by the toolchain/linker and how the object is defined/used.

For our learning model, however, read-only constants are commonly associated with `.rodata`.

---

# 5. `.data`

Now we reach one of the most important sections.

`.data` contains **initialized writable global/static objects**.

Example:

```c
uint16_t voltage = 650U;
```

at file scope.

Or:

```c
static uint16_t current = 150U;
```

at file scope.

These variables:

* have an initial value
* are writable
* have static storage duration

So they commonly belong to `.data`.

Conceptually:

```text
.data
┌─────────────────────────┐
│ voltage = 650           │
│ current = 150           │
│ charging_mode = 1       │
└─────────────────────────┘
```

But there's an interesting embedded detail.

If RAM is volatile, how does the initial value `650` survive power-on?

It doesn't.

The **initial value is stored in Flash**, and startup code copies it into RAM.

We'll come back to this.

---

# 6. `.bss`

`.bss` contains **uninitialized or zero-initialized global/static objects**.

Example:

```c
uint16_t voltage;
```

or:

```c
static uint16_t current;
```

Because these variables have static storage duration and no explicit initializer, they are typically placed in `.bss`.

At startup, `.bss` is initialized to zero.

For example:

```c
uint16_t voltage;
```

will normally begin as:

```text
voltage = 0
```

You don't need to manually write:

```c
voltage = 0;
```

for a static-storage-duration object.

---

# 7. `.data` vs `.bss`

This is a very common interview question.

| Section | Initialized? | Writable? | Typical location |
| ------- | ------------ | --------- | ---------------- |
| `.data` | Yes          | Yes       | RAM at runtime   |
| `.bss`  | No / zero    | Yes       | RAM at runtime   |

Example:

```c
uint16_t voltage = 650U;
```

→ `.data`

```c
uint16_t current;
```

→ `.bss`

The exact section assignment can depend on toolchain details, but this is the standard model.

---

# 8. Why `.data` Needs Flash + RAM

Consider:

```c
uint16_t voltage = 650U;
```

We need:

```text
After reset:

RAM:
voltage = 650
```

But RAM loses its contents after power-off.

Therefore the initial value must be preserved somewhere non-volatile.

Conceptually:

```text
FLASH
┌──────────────────────┐
│ initial .data values │
│ voltage = 650        │
└──────────┬───────────┘
           │ startup copy
           ↓
RAM
┌──────────────────────┐
│ .data                │
│ voltage = 650        │
└──────────────────────┘
```

This copying is performed by **startup code** before `main()` executes.

---

# 9. What Happens to `.bss`?

Suppose:

```c
uint16_t current;
```

The variable must start as zero.

Startup code typically clears the `.bss` region:

```text
RAM before initialization
      ↓
   unknown

Startup code
      ↓
clear .bss

RAM after initialization
      ↓
current = 0
```

Then the program eventually enters:

```c
int main(void)
{
    ...
}
```

So conceptually:

```text
Reset
  ↓
Startup code
  ↓
Initialize .data
  ↓
Clear .bss
  ↓
System initialization
  ↓
main()
```

This is a very important Embedded C concept.

---

# 10. Stack

The **stack** is used for function execution and automatic/local objects.

Example:

```c
void process_voltage(void)
{
    uint16_t voltage = 650U;
}
```

`voltage` is a local automatic variable.

It is commonly stored on the stack.

When the function starts:

```text
process_voltage()
       ↓
stack frame created
       ↓
voltage exists
```

When the function returns:

```text
function ends
       ↓
stack frame removed
       ↓
voltage no longer exists
```

Conceptually:

```text
Stack
┌──────────────────────┐
│ process_voltage()    │
│ voltage              │
│ return information   │
├──────────────────────┤
│ main()               │
│ local variables      │
└──────────────────────┘
```

---

# 11. Stack Frame

Every function call can require a **stack frame**.

For example:

```c
void calculate(uint16_t voltage)
{
    uint16_t current = 150U;

    ...
}
```

The stack may contain things associated with the function call, such as:

* local automatic variables
* function arguments depending on ABI/compiler
* saved registers
* return information

The exact details depend on the CPU architecture and calling convention.

For embedded interviews, understand the concept rather than assuming every argument always resides on the stack.

---

# 12. Stack Overflow

Embedded systems often have **limited RAM**.

Suppose an MCU has only:

```text
20 KB RAM
```

and your firmware uses too much stack.

Eventually the stack may overwrite another memory region.

This can cause:

* corrupted variables
* random crashes
* HardFaults on ARM Cortex-M
* unpredictable behavior

Example of a dangerous pattern:

```c
void bad_function(void)
{
    uint8_t huge_buffer[10000];
}
```

On a small MCU, this can consume a large portion of the available stack.

---

# 13. Recursion and Stack

Consider:

```c
void test(void)
{
    test();
}
```

Each call creates another function-call context.

Conceptually:

```text
test()
 ↓
test()
 ↓
test()
 ↓
test()
 ↓
...
```

Eventually:

```text
STACK OVERFLOW
```

For resource-constrained embedded firmware, uncontrolled recursion is generally avoided.

---

# 14. Heap

The heap is the region used for **dynamic memory allocation**.

Example:

```c
uint16_t *ptr;

ptr = malloc(sizeof(uint16_t));
```

Memory is obtained dynamically.

Later:

```c
free(ptr);
```

returns it.

In embedded systems, dynamic allocation must be used carefully because of:

* limited RAM
* fragmentation
* unpredictable allocation time
* allocation failure
* difficult debugging

For many safety-critical or deterministic embedded systems, developers prefer **static allocation**.

---

# 15. Stack vs Heap

| Stack                   | Heap                               |
| ----------------------- | ---------------------------------- |
| Automatic management    | Programmer-managed                 |
| Function/local context  | Dynamic allocation                 |
| Fast and deterministic  | Allocation can be less predictable |
| Limited                 | Limited                            |
| Automatically released  | Requires `free()`                  |
| Stack overflow possible | Fragmentation possible             |

Example stack:

```c
void test(void)
{
    uint16_t voltage;
}
```

Example heap:

```c
uint16_t *ptr = malloc(sizeof(uint16_t));
```

---

# 16. Global, Static and Local Variables

This is where your previous learning connects directly to memory layout.

Consider:

```c
uint16_t voltage = 650U;

static uint16_t current = 150U;

uint16_t temperature;
```

At file scope, these typically map as:

```text
voltage
→ .data

current
→ .data

temperature
→ .bss
```

Now:

```c
void monitor(void)
{
    uint16_t fault_code = 10U;
}
```

`fault_code` is an automatic local variable and is typically associated with the stack.

---

# 17. Static Local Variable

You already learned `static`.

Now connect it to memory layout:

```c
void counter(void)
{
    static uint32_t count = 0U;

    count++;
}
```

`count` is:

```text
Local scope
+
Static storage duration
```

Therefore it does **not behave like a normal stack local variable**.

Its value survives between function calls.

It is typically placed in `.bss` if initialized to zero:

```c
static uint32_t count = 0U;
```

or `.data` if initialized to a nonzero value:

```c
static uint32_t count = 10U;
```

This connection is extremely important.

---

# 18. Typical Embedded Memory Map

A simplified MCU memory map might look like:

```text
Higher Address
┌───────────────────────────┐
│           Stack           │
│             ↓             │
├───────────────────────────┤
│                           │
│       Free RAM            │
│                           │
├───────────────────────────┤
│             ↑             │
│           Heap            │
├───────────────────────────┤
│          .bss             │
├───────────────────────────┤
│          .data            │
├───────────────────────────┤
│                           │
│          RAM              │
└───────────────────────────┘


FLASH
┌───────────────────────────┐
│          .text            │
├───────────────────────────┤
│         .rodata           │
├───────────────────────────┤
│ Initial .data image       │
└───────────────────────────┘
```

**Important:** The exact ordering and direction of stack/heap depends on architecture/linker configuration. This is a conceptual model, not a universal physical memory map.

---

# 19. Where Does a Function Live?

Example:

```c
void start_charging(void)
{
    ...
}
```

The compiled machine instructions are normally stored in:

```text
.text
  ↓
Flash
```

When the CPU executes:

```c
start_charging();
```

the processor fetches instructions from the code memory.

---

# 20. Where Does a String Literal Live?

Example:

```c
printf("Charging Started\n");
```

The string literal is generally placed in a read-only section such as:

```text
.rodata
   ↓
Flash
```

This is one reason embedded firmware can store many constant strings in Flash without consuming equivalent runtime RAM, although library/runtime behavior still matters.

---

# 21. EV Charger Example

Imagine our charger firmware has:

```c
#include <stdint.h>

uint16_t charger_voltage = 750U;

static uint16_t charger_current = 150U;

uint16_t fault_count;

const char charger_name[] = "DC_FAST_CHARGER";

void start_charging(void)
{
    uint16_t session_voltage = 700U;
}
```

Typical conceptual placement:

```text
FLASH
────────────────────────────
.text
    start_charging()

.rodata
    "DC_FAST_CHARGER"

RAM
────────────────────────────
.data
    charger_voltage = 750
    charger_current = 150

.bss
    fault_count = 0

Stack
    session_voltage
```

This is exactly the kind of connection we want you to be able to make in an Embedded C interview.

---

# 22. The Most Important Mental Model

Don't memorize sections independently.

Think:

```text
                 C PROGRAM
                     │
        ┌────────────┴────────────┐
        ↓                         ↓
      CODE                       DATA
        │                         │
     .text                    ┌───┴────┐
        │                     │        │
      Flash                 Read     Write
                              │        │
                           .rodata   .data/.bss
                                      │
                                      RAM
                                      │
                             ┌────────┴────────┐
                             ↓                 ↓
                           Stack             Heap
```

---

# 23. Day 15 — Current Progress

We've now covered the **core theory** of:

* Flash vs RAM
* `.text`
* `.rodata`
* `.data`
* `.bss`
* Stack
* Heap
* stack frames
* stack overflow
* recursion and stack
* global/static/local variables
* startup initialization
* typical embedded memory map
* function and string placement
* EV charger memory example

We still need to learn **linker/linker script basics and the practical GCC memory-section experiment** before the learning phase is complete.

### Next topic: Linker and Linker Script

This is where the pieces connect:

```text
C source
   ↓
Compiler
   ↓
Object file
   ↓
Linker
   ↓
Linker script
   ↓
Final ELF / firmware image
   ↓
Flash + RAM
```

We'll go through that next, and then finish the remaining Day 15 learning topics **before any quiz or hands-on phase**.



Great. 👍 Let's continue **Day 15 — Memory Layout**.

## 23. Linker: Connecting Your C Code to Actual Memory

So far we have:

```text
C source
   ↓
Compiler
   ↓
Object file
```

But the compiler doesn't decide the complete final MCU memory layout by itself.

The **linker** combines object files and determines where the program's sections go.

For example:

```text
main.c
charger.c
can.c
   │
   ↓
Compiler
   │
   ↓
main.o
charger.o
can.o
   │
   ↓
   LINKER
   │
   ↓
Final firmware
```

The linker combines these pieces into the final executable image.

---

# 24. What Does the Linker Actually Do?

Suppose you have:

```c
void start_charging(void)
{
    ...
}
```

and:

```c
uint16_t voltage = 750U;
```

The compiler can generate machine code and object information, but the final program still needs to know things such as:

```text
Where should start_charging() be located?

Where should voltage be located?

Where does RAM start?

Where does Flash start?

Where should .text go?

Where should .data go?

Where should .bss go?
```

The linker handles this.

---

# 25. Linker Script

In embedded systems, a **linker script** describes the target memory regions and how sections should be placed.

A simplified example might look conceptually like:

```text
FLASH
ORIGIN = 0x08000000
LENGTH = 512K

RAM
ORIGIN = 0x20000000
LENGTH = 128K
```

Then it can specify something like:

```text
.text   → FLASH
.rodata → FLASH
.data   → RAM
.bss    → RAM
```

The actual linker script syntax depends on the toolchain.

For example, GNU ARM toolchains commonly use `.ld` linker scripts.

---

# 26. Why This Matters in Embedded Systems

Suppose your STM32 has:

```text
Flash = 512 KB
RAM   = 128 KB
```

But your firmware requires:

```text
Flash required = 600 KB
```

The program cannot fit.

You may get a linker error such as:

```text
region `FLASH' overflowed
```

Similarly:

```text
RAM required = 150 KB
RAM available = 128 KB
```

can cause a RAM overflow.

This is why memory layout is not just theoretical.

---

# 27. `.map` File

One very useful output from the linker is the **map file**.

It can show information such as:

```text
.text
.rodata
.data
.bss
```

and how much memory they consume.

For example, conceptually:

```text
.text       82 KB
.rodata     12 KB
.data        4 KB
.bss        20 KB
```

This helps an embedded developer answer:

> "Why is my firmware using so much Flash/RAM?"

---

# 28. Startup Code

Now connect everything together.

Suppose:

```c
uint16_t voltage = 650U;
uint16_t current;
```

We know:

```text
voltage → .data
current → .bss
```

But when the MCU resets, RAM doesn't magically contain:

```text
voltage = 650
current = 0
```

Startup code prepares RAM before `main()`.

Conceptually:

```text
                MCU RESET
                    ↓
              Startup code
                    ↓
        ┌───────────┴───────────┐
        ↓                       ↓
 Copy .data                  Clear .bss
 Flash → RAM                  RAM → 0
        │                       │
        └───────────┬───────────┘
                    ↓
             System initialization
                    ↓
                  main()
```

This is a very important embedded concept.

---

# 29. Let's Connect Everything

Consider this code:

```c
#include <stdint.h>

uint16_t voltage = 750U;

uint16_t current;

static uint16_t temperature = 25U;

static uint16_t fault_count;

const char charger_name[] = "DC_CHARGER";

void start_charging(void)
{
    uint16_t session_voltage = 700U;
}
```

Typical conceptual layout:

```text
FLASH
────────────────────────────
.text
    start_charging()

.rodata
    "DC_CHARGER"

initial .data image
    voltage = 750
    temperature = 25


RAM
────────────────────────────
.data
    voltage
    temperature

.bss
    current
    fault_count

Stack
    session_voltage
```

Notice something important:

```text
static uint16_t temperature = 25U;
```

and:

```text
uint16_t voltage = 750U;
```

can both be in `.data`.

But `static` has a different purpose:

* `temperature` has internal linkage because it is file-level `static`
* `voltage` has external linkage

Memory section and linkage are **separate concepts**.

---

# 30. Practical GCC Experiment

Now let's actually verify memory sections instead of only memorizing them.

Create a file:

```c
#include <stdio.h>
#include <stdint.h>

uint16_t global_init = 100U;
uint16_t global_uninit;

static uint16_t static_init = 200U;
static uint16_t static_uninit;

const char message[] = "EV Charger";

void test_function(void)
{
    uint16_t local = 50U;

    printf("%u\n", local);
}

int main(void)
{
    test_function();

    return 0;
}
```

Compile it with:

```powershell
gcc -c memory_test.c -o memory_test.o
```

Then inspect the object file:

```powershell
objdump -h memory_test.o
```

You should see sections similar to:

```text
.text
.data
.bss
.rodata
```

The exact sizes and additional sections depend on the compiler/toolchain.

---

# 31. Why We're Using `-c`

This command:

```powershell
gcc -c memory_test.c -o memory_test.o
```

means:

```text
source code
   ↓
compiler
   ↓
object file
```

It does **not** perform the final linking step.

That's useful because we can directly inspect the object file's sections.

---

# 32. Inspect the Final Executable

Now compile normally:

```powershell
gcc memory_test.c -o memory_test.exe
```

Then:

```powershell
objdump -h memory_test.exe
```

You'll see a much larger set of sections because the final executable contains additional runtime/linker information.

For embedded development, the exact executable format may instead be ELF, and tools such as:

```text
arm-none-eabi-size
arm-none-eabi-objdump
arm-none-eabi-nm
```

are commonly used.

---

# 33. `size` Command

For an embedded ELF file, one of the most useful commands is:

```powershell
arm-none-eabi-size firmware.elf
```

A typical output looks conceptually like:

```text
text    data    bss    dec    hex
50000   3000   12000   ...
```

Meaning approximately:

```text
text → code + read-only program content
data → initialized writable data
bss  → zero/uninitialized static data
```

This gives a quick view of Flash/RAM requirements.

Exact accounting can vary with toolchain and how sections are categorized, so treat this as a practical summary rather than a universal byte-for-byte memory map.

---

# 34. Important Memory Problems

Now let's connect memory layout to real firmware bugs.

### Problem 1 — Stack overflow

```c
void function(void)
{
    uint8_t buffer[20000];
}
```

Potentially dangerous on a small MCU.

---

### Problem 2 — Heap fragmentation

Repeated:

```c
malloc()
free()
malloc()
free()
```

can fragment available heap memory.

---

### Problem 3 — RAM overflow

Too many:

```c
global variables
static variables
large buffers
stack usage
heap allocations
```

can exceed available RAM.

---

### Problem 4 — Flash overflow

Large:

```c
code
constant tables
strings
```

can make `.text` + read-only content too large for Flash.

---

### Problem 5 — Large local arrays

This:

```c
void process(void)
{
    uint8_t buffer[4096];
}
```

may consume significant stack.

Instead, depending on the application's requirements, an embedded developer might use a carefully managed static/global buffer:

```c
static uint8_t buffer[4096];
```

But that moves the memory burden to static RAM, so it doesn't make the memory requirement disappear.

---

# 35. Very Important Interview Question

### Where does this variable go?

```c
uint16_t a = 10;
```

At file scope:

**Typically `.data`**

---

```c
uint16_t b;
```

At file scope:

**Typically `.bss`**

---

```c
static uint16_t c = 20;
```

At file scope:

**Typically `.data`**

---

```c
static uint16_t d;
```

At file scope:

**Typically `.bss`**

---

```c
void test(void)
{
    uint16_t e = 30;
}
```

`e` is an automatic local variable and is typically associated with the **stack**.

---

```c
const uint16_t f = 40;
```

At file scope, it is commonly placed in a read-only section such as **`.rodata`**, depending on toolchain/linker behavior.

---

# 🧠 The Most Important Summary

Don't memorize:

```text
.text = Flash
.data = RAM
.bss = RAM
```

only.

Understand the complete flow:

```text
                    SOURCE CODE
                         ↓
                     COMPILER
                         ↓
                    OBJECT FILE
                         ↓
                      LINKER
                         ↓
                  LINKER SCRIPT
                         ↓
              FINAL FIRMWARE IMAGE
                         ↓
          ┌──────────────┴──────────────┐
          ↓                             ↓
        FLASH                          RAM
          │                             │
       .text                         .data
       .rodata                       .bss
       data init image               Stack
                                     Heap
```

And at reset:

```text
RESET
  ↓
Startup code
  ↓
.data: Flash → RAM
  ↓
.bss: clear to zero
  ↓
System initialization
  ↓
main()
```

---


Good. Let's move to the next important Day 15 concept: **initialized arrays vs uninitialized arrays**.

### 1. Uninitialized global/static array

```c
uint8_t buffer1[300];
```

This goes to:

```text
.bss
```

At startup, the system clears it to zero:

```text
buffer1[0] = 0
buffer1[1] = 0
...
buffer1[299] = 0
```

It does **not** need to store 300 meaningful initial bytes in the Flash image.

---

### 2. Initialized global array

Now:

```c
uint8_t buffer2[300] = {1U, 2U, 3U};
```

This goes to:

```text
.data
```

Conceptually:

```text
FLASH
└── Initial .data image
      └── 300-byte buffer2 contents

             ↓ startup copy

RAM
└── .data
      └── buffer2[300]
```

So this array consumes approximately:

* **300 bytes of RAM**
* **300 bytes of Flash** for its initial contents

---

### 3. Compare them

```c
uint8_t buffer1[300];
```

```text
.bss
RAM → 300 bytes
Flash → no 300-byte initial data image
```

Whereas:

```c
uint8_t buffer2[300] = {1U, 2U, 3U};
```

```text
.data
RAM   → 300 bytes
Flash → 300 bytes initial image
```

This is an important embedded-memory tradeoff.

---

## 4. What about a `const` array?

```c
const uint8_t lookup[300] = {1U, 2U, 3U};
```

Typically:

```text
.rdata / .rodata
```

So:

```text
Flash → ~300 bytes
RAM   → normally no writable RAM copy
```

This is why lookup tables that never change are often declared `const`.

---

## 🧠 Remember this pattern

| Declaration                   | Typical section  |                      RAM | Flash initial data |
| ----------------------------- | ---------------- | -----------------------: | -----------------: |
| `uint8_t a[300];`             | `.bss`           |                    300 B |                 No |
| `uint8_t a[300] = {1};`       | `.data`          |                    300 B |             ~300 B |
| `const uint8_t a[300] = {1};` | `.rdata/.rodata` | Usually no writable copy |             ~300 B |

**One subtle point:** exact placement and whether a particular MCU/toolchain makes a RAM copy can depend on the compiler/linker/startup configuration. The table above is the normal model we're learning.

---

You're right. 👍 **We discussed the memory sections and Stack/Heap, but we did not properly teach the “Memory Map” itself.**

So let's **not mark Day 15 complete yet**. We should finish Memory Map first.

# Day 15 — Memory Map

A **memory map** is the arrangement of different memory regions of an embedded system and what each region is used for.

A simplified MCU RAM/Flash model looks like this:

```text
                MCU MEMORY
┌─────────────────────────────────┐
│             FLASH               │
│                                 │
│  .text                          │
│  Program instructions           │
│                                 │
│  .rodata / .rdata               │
│  Constant/read-only data        │
│                                 │
│  Initial values for .data       │
│                                 │
├─────────────────────────────────┤
│              RAM                │
│                                 │
│  .data                          │
│  Initialized writable variables │
│                                 │
│  .bss                           │
│  Zero/uninitialized variables   │
│                                 │
│  Heap                           │
│  Dynamic allocation             │
│                                 │
│             ↓                   │
│           free                  │
│             ↑                   │
│  Stack                          │
│  Local variables/function calls │
│                                 │
└─────────────────────────────────┘
```

The **exact physical arrangement** can differ between MCUs/linker scripts, but this is the model you should understand.

---

## 1. Flash vs RAM

Think of it first at a high level:

```text
FLASH
├── Code
├── Constants
└── Initial data image

RAM
├── Writable variables
├── Runtime buffers
├── Stack
└── Heap
```

### Flash

Usually contains things that need to remain available when the MCU starts:

```c
const uint16_t lookup_table[100];
```

and:

```c
void charger_task(void)
{
    // code
}
```

The code itself is normally placed in `.text`.

---

## 2. `.text`

Example:

```c
void read_voltage(void)
{
    printf("Reading voltage");
}
```

The **machine instructions** generated for this function normally reside in:

```text
.text → Flash
```

So:

```text
Function/code
     ↓
.text
     ↓
Flash
```

---

## 3. `.rodata` / `.rdata`

Example:

```c
const uint16_t max_voltage = 750U;
```

or:

```c
const char message[] = "Charging Started";
```

These are read-only objects.

Depending on your toolchain:

```text
.rodata
```

or, as you observed on Windows/GCC:

```text
.rdata
```

may be used.

---

## 4. `.data`

Example:

```c
uint16_t voltage = 650U;
```

It needs to be writable:

```text
voltage = 700U;
```

but it also has an initial value.

Therefore conceptually:

```text
FLASH
└── initial value: 650
          ↓ startup
RAM
└── .data
    voltage = 650
```

After startup, your program modifies the RAM copy.

---

## 5. `.bss`

Example:

```c
uint16_t current;
```

or:

```c
static uint8_t buffer[500];
```

These don't require meaningful initial values.

Startup normally initializes them to zero:

```text
.bss
   ↓
RAM
   ↓
startup clears it
   ↓
all zeros
```

---

# 6. Stack

The stack handles things such as:

```c
void charger_task(void)
{
    uint16_t voltage = 750U;
    uint16_t current = 150U;
}
```

These automatic local variables normally have stack storage.

When the function is called:

```text
Stack grows
```

When the function returns:

```text
Stack frame released
```

That's why large local arrays can be dangerous.

---

# 7. Heap

Heap is used for dynamic allocation:

```c
uint8_t *buffer = malloc(1024);
```

The memory comes from the heap.

But embedded systems often prefer predictable static allocation when possible because dynamic allocation can introduce:

* fragmentation
* allocation failure
* less deterministic behavior
* harder debugging

---

# 8. The most important Memory Map relationship

This is what I want you to remember:

```text
              PROGRAM
                 │
       ┌─────────┴─────────┐
       ↓                   ↓
     FLASH                 RAM
       │                    │
 ┌─────┼──────┐       ┌────┼──────────┐
 ↓     ↓      ↓       ↓    ↓    ↓     ↓
.text .rdata .data   .data .bss heap  stack
```

But there is an important detail:

### `.data` has a connection to both Flash and RAM.

```text
              FLASH
                │
        initial .data values
                │
             startup
                ↓
              RAM
                │
              .data
```

This is one of the most important things for embedded interviews.

---

# 9. Example — EV Charger

Imagine your firmware has:

```c
uint16_t voltage = 750U;

uint16_t current;

static uint8_t can_buffer[256];

const uint16_t max_voltage = 1000U;

void charger_control(void)
{
    uint8_t state = 2U;
}
```

Think like this:

| Object                   | Typical location         |
| ------------------------ | ------------------------ |
| `voltage`                | `.data` → RAM            |
| `current`                | `.bss` → RAM             |
| `can_buffer`             | `.bss` → RAM             |
| `max_voltage`            | `.rdata/.rodata` → Flash |
| `state`                  | Stack                    |
| `charger_control()` code | `.text` → Flash          |

Now you can look at a firmware and start estimating **Flash and RAM requirements**.

---

# 10. Why Memory Map matters in Embedded Systems

Suppose your MCU has:

```text
Flash = 256 KB
RAM   = 64 KB
```

Your firmware might use:

```text
Flash:
.text   = 120 KB
.rdata  = 20 KB
.data initial image = 4 KB

RAM:
.data = 4 KB
.bss  = 20 KB
stack = 12 KB
heap  = 2 KB
```

You need to think about **two separate resource limits**:

### Flash consumption

```text
.text + .rdata + initial .data + other Flash sections
```

### RAM consumption

```text
.data + .bss + stack + heap + runtime buffers
```

This distinction is critical.

---

## 🎯 Now I want you to test your understanding

Consider:

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

Great. 👍 Let's continue **Day 15 — Stack & Heap Analysis**.

This is especially important for your transition toward **Embedded Software Developer**, because interviewers often ask memory questions using practical examples.

## 1. Stack vs Heap

You already know that local variables normally use the stack:

```c
void charger_task(void)
{
    uint16_t voltage = 750U;
    uint16_t current = 150U;
}
```

Conceptually:

```text
RAM
┌─────────────────────┐
│       Stack         │
│                     │
│ voltage             │
│ current             │
├─────────────────────┤
│                     │
│       Heap          │
│                     │
├─────────────────────┤
│ .bss                │
├─────────────────────┤
│ .data               │
└─────────────────────┘
```

The exact RAM arrangement depends on the MCU/linker script, but this is the conceptual model.

---

# 2. Stack grows/shrinks with function calls

Consider:

```c
void function_a(void)
{
    uint16_t voltage = 750U;

    function_b();
}

void function_b(void)
{
    uint16_t current = 150U;
}
```

When `main()` calls `function_a()`:

```text
Stack
┌──────────────────┐
│ function_a frame │
│ voltage          │
├──────────────────┤
```

Then `function_a()` calls `function_b()`:

```text
Stack
┌──────────────────┐
│ function_b frame │
│ current          │
├──────────────────┤
│ function_a frame │
│ voltage          │
└──────────────────┘
```

When `function_b()` returns:

```text
Stack
┌──────────────────┐
│ function_a frame │
│ voltage          │
└──────────────────┘
```

When `function_a()` returns:

```text
Stack
┌──────────────────┐
│      empty       │
└──────────────────┘
```

This is why automatic local variables have a lifetime associated with the function call.

---

# 3. Static local is different

Compare:

```c
void counter(void)
{
    uint16_t count = 0U;
    count++;
}
```

with:

```c
void counter(void)
{
    static uint16_t count = 0U;
    count++;
}
```

### Normal local

```text
Call 1 → stack → count
Return → destroyed

Call 2 → new count
```

### Static local

```text
Call 1 → static memory → count = 1
Return → remains

Call 2 → same count → 2
```

So:

```text
automatic local → stack
static local    → .data/.bss
```

typically.

---

# 4. Why large local arrays are dangerous

Consider:

```c
void read_can_message(void)
{
    uint8_t buffer[1024];
}
```

Every call needs approximately:

```text
1024 bytes
```

of stack just for `buffer`.

Now imagine:

```c
void process(void)
{
    uint8_t buffer[4096];
}
```

On a small MCU with only a few KB of RAM, this can be dangerous.

---

# 5. Embedded Example

Suppose an MCU has:

```text
RAM = 32 KB
```

Your firmware uses:

```text
.data = 4 KB
.bss  = 10 KB
```

So before considering stack/heap:

```text
Used = 4 + 10
     = 14 KB
```

Remaining:

```text
32 - 14
= 18 KB
```

That remaining RAM must accommodate things such as:

```text
Stack
Heap
RTOS objects
DMA buffers
other runtime memory
```

You cannot simply say:

> "I have 18 KB free, so I can use an 18 KB local array."

Because the stack also needs space.

---

# 6. Heap

The heap is used for dynamic memory allocation.

Example:

```c
uint8_t *buffer;

buffer = malloc(1000);
```

Conceptually:

```text
Heap
┌─────────────────┐
│ 1000-byte block │ ← buffer
├─────────────────┤
│ free space      │
└─────────────────┘
```

Then:

```c
free(buffer);
```

returns the allocated block to the heap.

---

# 7. Why Embedded Developers Often Avoid `malloc()`

Dynamic allocation can introduce:

### Fragmentation

Imagine:

```text
Heap:

[A][B][C][D]
```

Free B and D:

```text
[A][free][C][free]
```

You may have enough **total free memory**, but not enough **contiguous memory** for a large request.

For example:

```text
free = 100 + 100 bytes
```

Total:

```text
200 bytes
```

But:

```c
malloc(180);
```

may fail if the allocator requires a contiguous 180-byte block.

This is called **fragmentation**.

---

# 8. Embedded Example: EV Charger

Imagine you need a CAN receive buffer:

```c
uint8_t can_rx_buffer[256];
```

You have several choices.

### Local:

```c
void can_receive(void)
{
    uint8_t can_rx_buffer[256];
}
```

Uses stack.

### Static:

```c
static uint8_t can_rx_buffer[256];
```

Uses static storage, typically `.bss`.

### Dynamic:

```c
uint8_t *can_rx_buffer = malloc(256);
```

Uses heap.

For a predictable embedded system, a statically allocated buffer is often preferable when the required size is known.

---

# 9. Stack Overflow

Suppose:

```text
Stack available = 2 KB
```

And your function chain requires:

```text
main              200 B
charger_task      400 B
CAN processing    500 B
diagnostic        600 B
local buffer      800 B
```

Total:

```text
200 + 400 + 500 + 600 + 800
= 2500 B
```

But only:

```text
2048 B
```

is available.

Therefore:

```text
2500 > 2048
```

→ **Stack overflow**

This can cause extremely difficult-to-debug firmware failures.

---

# 10. Recursion

Recursion is another stack-risk example:

```c
void function(void)
{
    function();
}
```

This keeps creating stack frames:

```text
function()
function()
function()
function()
function()
...
```

Eventually:

```text
STACK OVERFLOW
```

This is one reason recursion is often avoided or carefully controlled in resource-constrained embedded systems.

---

# 11. Stack vs Static Buffer

Suppose you need a 2 KB communication buffer.

### Option A

```c
void communication_task(void)
{
    uint8_t buffer[2048];
}
```

Consumes stack while the function is active.

### Option B

```c
static uint8_t buffer[2048];
```

Consumes static RAM continuously.

Neither option magically saves RAM.

The important question becomes:

> **Where should the memory be allocated based on lifetime, sharing requirements, and RAM constraints?**

---

# 🔥 Your practical challenge

Assume an MCU has:

```text
RAM = 16 KB
```

Firmware has:

```text
.data = 2 KB
.bss  = 5 KB
```

During the deepest function call, stack usage is:

```text
Stack = 6 KB
```

Heap usage:

```text
Heap = 1 KB
```

### Answer these:

**Q1.** How much RAM is already consumed by `.data + .bss`?

**Q2.** How much RAM is consumed after including stack and heap?

**Q3.** How much RAM remains?

**Q4.** If you add a local:

```c
uint8_t buffer[2048];
```

during that deepest function call, what problem could occur?

**Q5.** For a permanent 2 KB CAN communication buffer, which would you generally prefer in a predictable embedded system:

```c
void task(void)
{
    uint8_t buffer[2048];
}
```

or

```c
static uint8_t buffer[2048];
```

