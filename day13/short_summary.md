# Day 13 — Quick Revision

```text
struct
→ Each member has separate memory

union
→ All members share the same memory

union size
→ Generally based on the largest member

enum
→ Gives names to integer constants

enum + union
→ Tagged union

enum/tag
→ Tells which union member is currently active

switch
→ Selects the correct union member

struct + union
→ Combines the type information and actual data

Function
→ Keeps processing logic modular
```

### EV Charger Example

```text
Voltage
→ DATA_VOLTAGE
→ data.voltage

Current
→ DATA_CURRENT
→ data.current

Temperature
→ DATA_TEMPERATURE
→ data.temperature
```

### Remember

```text
STRUCT → Store ALL
UNION  → Store ONE
ENUM   → Identify WHICH
SWITCH → Process HOW
```

### Interview Answer

```text
enum + union
→ enum identifies the active data
→ union stores the data using shared memory
→ useful when data is mutually exclusive
```
