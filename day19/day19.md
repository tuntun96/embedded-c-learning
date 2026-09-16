# Day 19 — Circular Buffer

**Roadmap position:** Embedded C → Advanced → **Circular Buffer**. Your official roadmap places Circular Buffer immediately after Interrupt Concepts and before Ring Buffer. 

---

## 1. What is a Circular Buffer?

A **circular buffer** is a fixed-size array used as a FIFO (**First In, First Out**) queue where the end of the array logically connects back to the beginning.

Normal array:

```text
Index:   0   1   2   3   4
         ↓   ↓   ↓   ↓   ↓
Buffer: [10][20][30][40][50]
```

When we reach index `4`, instead of stopping, the next position becomes index `0`.

```text
        ┌───────────────────┐
        ↓                   │
   [0] [1] [2] [3] [4]      │
    ↑                   │    │
    └───────────────────┘
```

That is the **circular** behavior.

---

# 2. Why do we need it?

Suppose we have a UART receiving data.

UART may receive bytes quickly:

```text
UART
 ↓
10
20
30
40
50
```

But the main application may process them later.

We need somewhere to temporarily store the received bytes.

A circular buffer is ideal:

```text
UART Hardware
      ↓
     ISR
      ↓
Circular Buffer
      ↓
 Main Application
      ↓
 Process Data
```

This is one of the most important practical uses of a circular buffer in embedded systems.

---

# 3. Basic Circular Buffer Structure

Our implementation uses:

```c
#define BUFFER_SIZE 5U

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
    uint8_t count;
} Circular_Buffer;
```

There are four important components.

| Member     | Purpose                                       |
| ---------- | --------------------------------------------- |
| `buffer[]` | Stores the data                               |
| `head`     | Position where the next data will be inserted |
| `tail`     | Position where the next data will be removed  |
| `count`    | Number of valid elements currently stored     |

Think:

```text
             INSERT
               ↓
             HEAD
               ↓
        ┌───────────────┐
        │ 10 20 30 40  │
        └───────────────┘
          ↑
         TAIL
          ↓
        REMOVE
```

---

# 4. FIFO Principle

Circular buffers normally behave as FIFO.

If we insert:

```text
10
20
30
40
```

the order is:

```text
10 → 20 → 30 → 40
```

Therefore, removal must produce:

```text
10
20
30
40
```

not:

```text
40
30
20
10
```

---

# 5. Understanding `head`

`head` tells us:

> **Where should the next received data be written?**

Initially:

```text
head = 0
```

Insert `10`:

```text
buffer[0] = 10
head = 1
```

Insert `20`:

```text
buffer[1] = 20
head = 2
```

Insert `30`:

```text
buffer[2] = 30
head = 3
```

So:

```text
Index:   0   1   2   3   4
         ↓   ↓   ↓
Buffer: [10][20][30][  ][  ]
                     ↑
                    head
```

---

# 6. Understanding `tail`

`tail` tells us:

> **From where should the next data be removed?**

Initially:

```text
tail = 0
```

After inserting:

```text
10, 20, 30
```

we have:

```text
Index:   0   1   2   3   4
         ↓   ↓   ↓
Buffer: [10][20][30][  ][  ]

tail → 0
head → 3
```

Get operation reads:

```text
buffer[tail]
```

So first:

```text
buffer[0] → 10
```

then:

```text
tail = 1
```

Next:

```text
buffer[1] → 20
```

then:

```text
tail = 2
```

---

# 7. Empty Buffer

The simplest way we tracked empty state was:

```c
if (cb->count == 0U)
```

Example:

```text
count = 0
```

means:

```text
Buffer Empty
```

So `Get()` should fail.

---

# 8. Full Buffer

For:

```c
#define BUFFER_SIZE 5U
```

maximum stored elements:

```text
count = 5
```

Therefore:

```c
if (cb->count == BUFFER_SIZE)
```

means the buffer is full.

In our **reject mode**, new data is rejected.

Example:

```text
Buffer:

[10][20][30][40][50]

count = 5
```

Now UART receives:

```text
60
```

There is no free slot, so:

```text
60 → rejected
```

The existing data remains unchanged.

---

# 9. Insert Function

Our final `Put()` implementation:

```c
uint8_t CircularBuffer_Put(Circular_Buffer *cb, uint8_t data)
{
    if (cb->count == BUFFER_SIZE)
    {
        return 0U;
    }

    cb->buffer[cb->head] = data;

    cb->head = (cb->head + 1U) % BUFFER_SIZE;

    cb->count++;

    return 1U;
}
```

Important sequence:

```text
Check full
    ↓
Write data at head
    ↓
Move head
    ↓
Increase count
    ↓
Return success
```

---

# 10. Why `% BUFFER_SIZE`?

This is what creates the circular behavior.

For:

```c
BUFFER_SIZE = 5U;
```

we get:

```text
head = 0
head = 1
head = 2
head = 3
head = 4
head = 0
head = 1
...
```

Because:

```text
(4 + 1) % 5 = 0
```

So the index wraps around.

---

# 11. Worked Example — Wrap Around

Start:

```text
BUFFER_SIZE = 5

head = 0
tail = 0
count = 0
```

Insert:

```text
10, 20, 30, 40
```

Result:

```text
Index:   0   1   2   3   4
         ┌───┬───┬───┬───┬───┐
Buffer:  │10 │20 │30 │40 │   │
         └───┴───┴───┴───┴───┘
          ↑               ↑
         tail            head
          0               4
```

Now remove two:

```text
Get → 10
Get → 20
```

Now:

```text
tail = 2
head = 4
count = 2
```

Logically:

```text
30 → 40
```

Now insert:

```text
50
60
```

`50` goes to index `4`.

Then:

```text
head = (4 + 1) % 5
     = 0
```

So `60` goes to index `0`.

Physical array becomes:

```text
Index:   0   1   2   3   4
         ┌───┬───┬───┬───┬───┐
Buffer:  │60 │   │30 │40 │50 │
         └───┴───┴───┴───┴───┘
              ↑   ↑       ↑
             ... tail    ...
```

But **logical FIFO order** is:

```text
30 → 40 → 50 → 60
```

This distinction between **physical array order** and **logical FIFO order** is extremely important.

---

# 12. Get Function

Our improved `Get()` function:

```c
uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data)
{
    if (cb->count == 0U)
    {
        return 0U;
    }

    *data = cb->buffer[cb->tail];

    cb->tail = (cb->tail + 1U) % BUFFER_SIZE;

    cb->count--;

    return 1U;
}
```

The function returns:

```text
1 → data successfully received
0 → buffer empty
```

This is better than returning the data itself because `0` can be a valid UART byte.

---

# 13. Why use a pointer in `Get()`?

We need to return **two pieces of information**:

```text
Was data available?
+
What was the data?
```

C functions have one normal return value, so we use:

```c
uint8_t CircularBuffer_Get(Circular_Buffer *cb, uint8_t *data)
```

Example:

```c
uint8_t data;

if (CircularBuffer_Get(&cb, &data))
{
    printf("Data = %u\n", data);
}
```

If buffer contains `0`:

```text
Data = 0
```

the function can still return:

```text
1
```

meaning:

> Yes, `0` is valid data.

---

# 14. Producer and Consumer

This is the most important embedded concept behind the buffer.

### Producer

The producer **adds data**.

For UART:

```text
UART → ISR → Put()
```

### Consumer

The consumer **removes data**.

Usually:

```text
Main Loop → Get()
```

So:

```text
             PRODUCER
                ↓
              UART
                ↓
               ISR
                ↓
              head
                ↓
        ┌─────────────────┐
        │ Circular Buffer │
        └─────────────────┘
                ↓
              tail
                ↓
          Main Application
                ↓
             CONSUMER
```

---

# 15. UART Practical Example

Imagine the UART receives:

```text
H
E
L
L
O
```

The UART interrupt should not try to process the entire command.

Instead:

```text
UART receives 'H'
        ↓
ISR
        ↓
Put('H')
        ↓
Return quickly
```

Then:

```text
UART receives 'E'
        ↓
ISR
        ↓
Put('E')
        ↓
Return
```

Eventually:

```text
Circular Buffer:

H E L L O
```

The main loop can later do:

```text
Get → H
Get → E
Get → L
Get → L
Get → O
```

This allows the UART reception and application processing to operate independently.

---

# 16. Why ISR Should Be Short

Avoid doing this inside a UART ISR:

```c
printf("Received command...\n");
```

or:

```c
parse_entire_command();
```

or:

```c
perform_long_calculation();
```

Instead:

```text
ISR
 ↓
Read byte
 ↓
Put byte into circular buffer
 ↓
Restart UART reception
 ↓
Exit ISR
```

Then:

```text
Main loop
 ↓
Get byte
 ↓
Parse/process
```

This keeps interrupt latency low.

---

# 17. Full Buffer Policy — Reject Mode

Our implementation uses **reject mode**.

When full:

```text
New data
   ↓
Buffer Full?
   ↓
 YES
   ↓
Reject new data
```

Example:

```text
[10][20][30][40][50]
```

Receive:

```text
60
```

Result:

```text
60 rejected
```

This is usually safer for a UART command/message buffer because silently deleting old bytes could corrupt a command.

---

# 18. Overwrite Mode

Another design is **overwrite mode**.

When full:

```text
New data arrives
       ↓
Buffer full
       ↓
Discard oldest data
       ↓
Store new data
```

Example:

```text
Before:

[10][20][30][40][50]
 ↑
oldest
```

Receive:

```text
60
```

The oldest `10` is discarded.

Logical contents become:

```text
20 → 30 → 40 → 50 → 60
```

A typical overwrite operation:

```c
uint8_t CircularBuffer_PutOverwrite(Circular_Buffer *cb, uint8_t data)
{
    cb->buffer[cb->head] = data;

    cb->head = (cb->head + 1U) % BUFFER_SIZE;

    if (cb->count == BUFFER_SIZE)
    {
        cb->tail = (cb->tail + 1U) % BUFFER_SIZE;
    }
    else
    {
        cb->count++;
    }

    return 1U;
}
```

### When overwrite is useful

Good examples:

```text
Sensor history
ADC samples
Diagnostic history
Temperature history
Latest N measurements
```

For a command-oriented UART buffer, **reject mode is generally safer**.

---

# 19. Power-of-Two Optimization

If:

```c
#define BUFFER_SIZE 8U
```

then:

```c
index = (index + 1U) % BUFFER_SIZE;
```

can be optimized to:

```c
index = (index + 1U) & (BUFFER_SIZE - 1U);
```

Because:

```text
BUFFER_SIZE = 8
BUFFER_SIZE - 1 = 7
```

and:

```text
8 & 7 = 0
```

So:

```text
6 → 7 → 0 → 1 → 2
```

Your test produced:

```text
Head = 7
Head = 0
Head = 1
Head = 2
```

which correctly demonstrated the wrap-around.

### Important restriction

This optimization works only when the buffer size is a power of two:

```text
2
4
8
16
32
64
128
...
```

Not:

```text
5
10
12
20
```

---

# 20. `volatile` in ISR + Main Context

You used:

```c
volatile uint8_t head;
volatile uint8_t tail;
```

Why?

Because these variables can be accessed by different execution contexts, for example:

```text
Main code
    ↕
ISR
```

`volatile` tells the compiler:

> Do not assume this variable remains unchanged between accesses; actually perform the required memory access.

Without it, compiler optimization could cause problems when a variable can change outside the normal flow being compiled.

### But remember:

**`volatile` does NOT make code thread-safe.**

It does not automatically make:

```c
count++;
```

atomic.

`count++` is effectively a read-modify-write operation:

```text
read count
   ↓
add 1
   ↓
write count
```

If ISR and main both modify the same variable at the wrong time, a race can occur.

For a robust embedded circular buffer, we therefore think about:

```text
Who owns head?
Who owns tail?
Who modifies count?
Can an operation be interrupted?
Do we need atomic access or a critical section?
```

This is an important interview-level concept.

---

# 21. Best Mental Model

Remember these four rules:

```text
HEAD  → where new data goes

TAIL  → where old data comes out

COUNT → how many valid elements exist

% SIZE → makes the index wrap around
```

And:

```text
PUT  → Producer → increases count

GET  → Consumer → decreases count
```

---

# 22. Common Mistakes

### Mistake 1 — Using `tail` for insertion

Wrong:

```c
cb->buffer[cb->tail] = data;
```

Insertion belongs at:

```c
cb->buffer[cb->head] = data;
```

---

### Mistake 2 — Forgetting to move `head`

After writing:

```c
cb->buffer[cb->head] = data;
```

we must advance:

```c
cb->head = (cb->head + 1U) % BUFFER_SIZE;
```

---

### Mistake 3 — Forgetting `return 1U`

If the function returns status:

```c
uint8_t CircularBuffer_Get(...)
```

successful operation must return:

```c
return 1U;
```

Otherwise the caller may think the operation failed.

---

### Mistake 4 — Returning `0` for empty buffer when `0` is valid data

This design:

```c
return 0U;
```

cannot distinguish:

```text
empty buffer
```

from:

```text
received data = 0
```

That's why the status + output pointer design is better.

---

### Mistake 5 — Forgetting wrap-around

Wrong:

```c
head++;
```

Correct:

```c
head = (head + 1U) % BUFFER_SIZE;
```

or, for a power-of-two size:

```c
head = (head + 1U) & (BUFFER_SIZE - 1U);
```

---

# 23. Day 19 Interview Questions

You should be able to answer these without looking at the notes:

1. What is a circular buffer?
2. Why is it called circular?
3. What is the purpose of `head`?
4. What is the purpose of `tail`?
5. What does `count` represent?
6. What happens when the buffer becomes full?
7. What happens when the buffer is empty?
8. Why is `% BUFFER_SIZE` used?
9. Why is a circular buffer useful with UART?
10. Who is the producer in a UART circular-buffer architecture?
11. Who is the consumer?
12. Why should a UART ISR be short?
13. What is the difference between reject mode and overwrite mode?
14. When would overwrite mode be useful?
15. Why can overwrite mode be dangerous for UART commands?
16. Why can power-of-two buffer sizes be optimized using `&`?
17. Why do we use `volatile` for ISR-shared variables?
18. Does `volatile` make `count++` atomic?
19. What is the difference between physical array order and logical FIFO order?
20. Why is returning status + output pointer better than returning the received byte directly?

---

# 24. Day 19 Practical Architecture

The complete concept you should now have in your mind is:

```text
                 UART HARDWARE
                      │
                      │ byte received
                      ↓
                  UART ISR
                      │
                      │ Put(byte)
                      ↓
              ┌─────────────────┐
              │ Circular Buffer │
              │                 │
              │ head → write    │
              │ tail → read     │
              │ count           │
              └─────────────────┘
                      │
                      │ Get()
                      ↓
                 MAIN LOOP
                      │
                      ↓
                Process Data
```

That is the **real embedded-software reason** for learning Circular Buffer after Interrupts.

Your Day 19 implementation has already covered the core FIFO, wrap-around, full/empty handling, status-return design, UART producer/consumer model, overwrite mode, and power-of-two optimization. The remaining conceptual checkpoint is being able to clearly explain **why `volatile` helps with ISR/main shared state but does not make the buffer thread-safe**.
