Absolutely. Here are your **Day 20 — Ring Buffer notes**, written in the same practical style we've been using.

# Day 20 — Ring Buffer

## 1. Learning Objectives

By the end of Day 20, you should understand:

* What a Ring Buffer is
* Ring Buffer vs Circular Buffer
* `head` and `tail`
* FIFO behavior
* Empty condition
* Full condition
* One-empty-slot technique
* Wrap-around
* Reusing freed buffer positions
* Producer and Consumer
* UART + Ring Buffer architecture
* ISR + Main Loop architecture
* `volatile`
* Why `volatile` does **not** mean thread-safe
* SPSC — Single Producer, Single Consumer
* Implementing `RingBuffer_Put()`
* Implementing `RingBuffer_Get()`

This follows the official roadmap sequence: **Circular Buffer → Ring Buffer → State Machines**. 

---

# 2. What is a Ring Buffer?

A **Ring Buffer** is a fixed-size memory buffer where the end connects back to the beginning.

Normal array:

```text
0 → 1 → 2 → 3 → 4 → END
```

Ring buffer:

```text
0 → 1 → 2 → 3 → 4
↑               ↓
└───────────────┘
```

When the index reaches the last position, it wraps back to `0`.

For example, with:

```c
#define BUFFER_SIZE 5U
```

the indexes are:

```text
0 → 1 → 2 → 3 → 4 → 0 → 1 → 2 ...
```

---

# 3. Ring Buffer vs Circular Buffer

In practice, the terms are often used interchangeably.

The important thing is the **implementation and behavior**, not the name.

A Ring Buffer is commonly used for:

* UART reception
* CAN message buffering
* Sensor data
* ADC samples
* Logging
* Communication protocols
* ISR-to-main-loop communication

---

# 4. Basic Structure

Our Ring Buffer uses:

```c
typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} Ring_Buffer;
```

### Meaning

| Variable   | Purpose                    |
| ---------- | -------------------------- |
| `buffer[]` | Stores data                |
| `head`     | Where producer writes next |
| `tail`     | Where consumer reads next  |

Think:

```text
Producer → HEAD → [BUFFER] → TAIL → Consumer
```

---

# 5. What is `head`?

`head` tells us:

> **Where the next data item will be written.**

Example:

```text
head = 2
```

means the producer will write at:

```text
buffer[2]
```

After writing:

```c
rb->buffer[rb->head] = data;
rb->head = next_head;
```

the head moves forward.

---

# 6. What is `tail`?

`tail` tells us:

> **Where the next data item will be read.**

Example:

```text
tail = 1
```

means the consumer reads:

```c
buffer[1]
```

After reading, tail moves forward.

---

# 7. FIFO Behavior

Ring buffers normally implement:

> **FIFO = First In, First Out**

Example:

```text
Put:  10
Put:  20
Put:  30
Put:  40
```

Then:

```text
Get → 10
Get → 20
Get → 30
Get → 40
```

The first item inserted is the first item removed.

---

# 8. Empty Condition

With our implementation:

```c
if (rb->head == rb->tail)
```

the buffer is empty.

Example:

```text
head = 0
tail = 0
```

Therefore:

```text
Buffer Empty
```

Important:

> **`head == tail` means EMPTY.**

---

# 9. Full Condition — One Empty Slot Method

This is the most important part of today's implementation.

We deliberately keep **one slot empty**.

For:

```c
#define BUFFER_SIZE 5U
```

we can store only:

```text
4 items
```

not 5.

Why?

Because we use:

```c
next_head = (head + 1U) % BUFFER_SIZE;
```

and declare the buffer full when:

```c
next_head == tail
```

So:

```c
if (next_head == rb->tail)
{
    return 0U;
}
```

means:

> There is no free slot.

---

# 10. Why Do We Leave One Slot Empty?

Because otherwise these two conditions would look identical:

### Empty

```text
head = 0
tail = 0
```

### Full

If all five positions were occupied:

```text
head = 0
tail = 0
```

Both would give:

```text
head == tail
```

So we couldn't distinguish empty from full.

The one-empty-slot technique solves this.

---

# 11. Capacity

If:

```c
BUFFER_SIZE = 5U
```

then:

```text
Actual capacity = BUFFER_SIZE - 1
               = 5 - 1
               = 4
```

Therefore:

| Buffer size | Usable elements |
| ----------: | --------------: |
|           4 |               3 |
|           5 |               4 |
|           8 |               7 |
|          16 |              15 |

This is an important interview point.

---

# 12. `RingBuffer_Put()`

Your implementation:

```c
uint8_t RingBuffer_Put(Ring_Buffer *rb, uint8_t data)
{
    uint8_t next_head = (rb->head + 1U) % BUFFER_SIZE;

    if (next_head == rb->tail)
    {
        return 0U;
    }

    rb->buffer[rb->head] = data;
    rb->head = next_head;

    return 1U;
}
```

### Step-by-step

First calculate:

```c
next_head = (head + 1U) % BUFFER_SIZE;
```

Then check:

```c
if (next_head == tail)
```

If yes:

```text
Buffer full
```

Return:

```c
0U
```

Otherwise:

```c
buffer[head] = data;
```

and:

```c
head = next_head;
```

Finally:

```c
return 1U;
```

means successful insertion.

---

# 13. `RingBuffer_Get()`

Your implementation:

```c
uint8_t RingBuffer_Get(Ring_Buffer *rb, uint8_t *data)
{
    if (rb->head == rb->tail)
    {
        return 0U;
    }

    *data = rb->buffer[rb->tail];

    rb->tail = (rb->tail + 1U) % BUFFER_SIZE;

    return 1U;
}
```

### Step-by-step

First:

```c
if (rb->head == rb->tail)
```

checks whether the buffer is empty.

If empty:

```c
return 0U;
```

Otherwise:

```c
*data = rb->buffer[rb->tail];
```

reads the oldest data.

Then:

```c
rb->tail = (rb->tail + 1U) % BUFFER_SIZE;
```

moves the tail.

Finally:

```c
return 1U;
```

means successful read.

---

# 14. Return Convention

We established a clean convention:

### Put

```text
1U → success
0U → failed/full
```

### Get

```text
1U → success
0U → failed/empty
```

This allows:

```c
if (RingBuffer_Put(&rb, 10U))
{
    printf("Success\n");
}
```

and:

```c
if (RingBuffer_Get(&rb, &data))
{
    printf("Data: %u\n", data);
}
```

This is much cleaner than reversing the meaning between functions.

---

# 15. Wrap-Around

This is what makes the buffer a **ring**.

Suppose:

```text
BUFFER_SIZE = 5
```

and:

```text
head = 4
```

Then:

```c
head = (head + 1U) % BUFFER_SIZE;
```

becomes:

```text
(4 + 1) % 5
= 5 % 5
= 0
```

So:

```text
4 → 0
```

The index wraps around.

---

# 16. Practical Wrap-Around Example

Start:

```text
BUFFER:

Index:   0   1   2   3   4
         -------------------
Data:    10  20  30  40  -
```

After reading:

```text
Get 10
Get 20
```

the consumer has freed positions.

Then:

```text
Put 50
Put 60
```

The physical array may look unusual, but logically the FIFO sequence is:

```text
30 → 40 → 50 → 60
```

Your test produced exactly:

```text
Data: 30
Data: 40
Data: 50
Data: 60
```

So your wrap-around implementation is working correctly.

---

# 17. Physical Order vs Logical Order

This is important.

The physical array might look like:

```text
Index:
0   1   2   3   4

50  60  30  40  -
```

That does **not** mean the FIFO order is:

```text
50 → 60 → 30 → 40
```

The FIFO order is determined by:

```text
tail
 ↓
30 → 40 → 50 → 60
              ↑
             head
```

So:

> **Never judge FIFO order just by looking at the array from index 0.**

Follow `tail → ... → head`.

---

# 18. Producer and Consumer

A Ring Buffer usually has two sides.

### Producer

Produces/inserts data.

```text
Producer → Put()
```

### Consumer

Consumes/removes data.

```text
Consumer → Get()
```

Example:

```text
UART ISR
   ↓
Producer
   ↓
Ring Buffer
   ↓
Consumer
   ↓
Main Loop
```

---

# 19. UART Application

A very common embedded application is:

```text
UART Hardware
      ↓
Byte received
      ↓
UART ISR / Callback
      ↓
RingBuffer_Put()
      ↓
Ring Buffer
      ↓
Main Loop
      ↓
RingBuffer_Get()
      ↓
Process byte
```

For example, UART receives:

```text
'C'
'A'
'N'
' '
'1'
'2'
'3'
'\n'
```

The ISR quickly stores these bytes.

The main loop later processes them.

---

# 20. Why Use a Ring Buffer with UART?

UART hardware may receive data faster than your main program can process it.

For example:

```text
UART:
10 20 30 40 50 60 ...
```

If your main loop is busy doing something else, the incoming bytes need somewhere to wait.

The Ring Buffer provides temporary storage.

Without it:

```text
UART → Main
```

may lose data.

With it:

```text
UART → Ring Buffer → Main
```

the producer and consumer are decoupled.

---

# 21. ISR + Ring Buffer

This is especially useful with interrupts.

A good architecture is:

```text
       UART Hardware
             ↓
          Interrupt
             ↓
        ISR/Callback
             ↓
      RingBuffer_Put()
             ↓
       Return quickly
             ↓
         Main Loop
             ↓
      RingBuffer_Get()
             ↓
       Process data
```

### ISR should be short

Avoid doing this inside the ISR:

```text
printf()
large calculations
complete command parsing
long loops
delays
```

Instead:

```text
Receive byte
     ↓
Store byte
     ↓
Return
```

The main loop does the heavier processing.

---

# 22. `volatile`

You used:

```c
volatile uint8_t head;
volatile uint8_t tail;
```

`volatile` tells the compiler:

> "This variable can change outside the normal code flow, so don't assume its value stays unchanged."

This is useful when variables are accessed by:

* Interrupt routines
* Hardware
* Main loop
* DMA
* Other execution contexts

---

# 23. Very Important: `volatile` Is Not Thread Safety

Remember this:

> **`volatile` does not make code thread-safe.**

For example:

```c
count++;
```

is conceptually:

```text
Read count
   ↓
Add 1
   ↓
Write count
```

It is not necessarily one indivisible operation.

So:

```text
volatile ≠ atomic
volatile ≠ mutex
volatile ≠ thread-safe
```

This is a very common embedded interview question.

---

# 24. SPSC Ring Buffer

Our design is particularly suitable for:

> **SPSC = Single Producer, Single Consumer**

Example:

```text
UART ISR
   ↓
Producer
   ↓
head
   ↓
Ring Buffer
   ↓
tail
   ↓
Main Loop
   ↓
Consumer
```

The ownership is:

```text
ISR / Producer → owns head

Main / Consumer → owns tail
```

This avoids both sides modifying the same index.

---

# 25. Why Avoid `count` Here?

Our earlier circular buffer had:

```c
uint8_t count;
```

Ring Buffer can avoid it.

We determine:

### Empty

```c
head == tail
```

### Full

```c
next_head == tail
```

Therefore we don't need a shared `count`.

This can simplify ISR/main-loop communication.

---

# 26. Important Interview Comparison

### Circular Buffer with `count`

```text
buffer
head
tail
count
```

Capacity can use all slots, but `count` becomes shared state.

### Ring Buffer with one empty slot

```text
buffer
head
tail
```

One slot is sacrificed, but empty/full detection becomes simple.

---

# 27. Common Mistakes

### Mistake 1 — Thinking size 5 means capacity 5

With our one-empty-slot design:

```text
BUFFER_SIZE = 5
capacity = 4
```

---

### Mistake 2 — Confusing head and tail

Remember:

```text
HEAD → write
TAIL → read
```

---

### Mistake 3 — Forgetting wrap-around

Wrong:

```c
rb->head++;
```

Correct:

```c
rb->head = (rb->head + 1U) % BUFFER_SIZE;
```

---

### Mistake 4 — Forgetting the return value

A function declared:

```c
uint8_t RingBuffer_Put(...)
```

must return a value on every relevant path.

Your corrected implementation does this correctly.

---

### Mistake 5 — Thinking `volatile` provides synchronization

It doesn't.

```text
volatile → compiler visibility

Synchronization → access/ownership/atomicity mechanisms
```

---

# 28. Your Final Ring Buffer API

The version you practiced is:

```c
uint8_t RingBuffer_Put(Ring_Buffer *rb, uint8_t data);

uint8_t RingBuffer_Get(Ring_Buffer *rb, uint8_t *data);
```

The API behavior is:

```text
RingBuffer_Put()
        ↓
success → 1U
full    → 0U

RingBuffer_Get()
        ↓
success → 1U
empty   → 0U
```

---

# 29. Quick Revision

Remember these five lines:

```text
HEAD  → next write position
TAIL  → next read position

HEAD == TAIL
→ EMPTY

NEXT_HEAD == TAIL
→ FULL

HEAD/Tail wrap around
→ Ring behavior
```

And:

```text
volatile
→ tells compiler value may change unexpectedly

volatile does NOT
→ make operations atomic/thread-safe
```

---

# 30. EV Charger Example

In an EV charger, imagine the UART receives diagnostic data:

```text
CAN
UART
Modbus
HMI
```

Data can arrive continuously.

A Ring Buffer can temporarily store received bytes:

```text
UART RX
   ↓
ISR
   ↓
Ring Buffer
   ↓
Parser
   ↓
Charger Logic
   ↓
Fault / Status / HMI
```

This is a very realistic embedded firmware architecture.

---

# 31. Day 20 Completion

You have successfully demonstrated:

* ✅ Ring Buffer concept
* ✅ `head`
* ✅ `tail`
* ✅ FIFO
* ✅ Empty detection
* ✅ Full detection
* ✅ One-empty-slot design
* ✅ Wrap-around
* ✅ Reusing freed slots
* ✅ Producer/Consumer
* ✅ UART application
* ✅ ISR + Ring Buffer architecture
* ✅ `volatile`
* ✅ `volatile` limitation
* ✅ SPSC ownership
* ✅ Working `Put()` and `Get()` implementation

### Interview one-liner

> **A Ring Buffer is a fixed-size FIFO data structure that uses head and tail indexes with wrap-around, commonly used in embedded systems for efficiently buffering asynchronous data such as UART bytes between an ISR producer and a main-loop consumer.**

Your next official topic is **Day 21 — State Machines**. The official roadmap places State Machines immediately after Ring Buffer. 
