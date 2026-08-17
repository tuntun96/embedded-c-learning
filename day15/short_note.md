## Day 15 — Very Short Revision Sheet

### Memory Layout

```text
FLASH
├── .text       → program code
├── .rdata      → read-only/const data
└── .data image → initial values

RAM
├── .data       → initialized writable data
├── .bss        → zero/uninitialized global/static data
├── heap        → dynamic allocation
└── stack       → local variables/function calls
```

### Sections

| Section          | Stores                             | Memory                    |
| ---------------- | ---------------------------------- | ------------------------- |
| `.text`          | Code                               | Flash                     |
| `.rdata/.rodata` | `const` data                       | Flash                     |
| `.data`          | Initialized writable global/static | RAM + initial Flash image |
| `.bss`           | Zero/uninitialized global/static   | RAM                       |
| Stack            | Local/automatic variables          | RAM                       |
| Heap             | `malloc()`/dynamic memory          | RAM                       |

### Key rules

```text
initialized global/static → .data
uninitialized global/static → .bss
const data → .rdata/.rodata
function/code → .text
local variable → stack
malloc() → heap
```

### Critical interview points

* `static` → lifetime throughout program execution, **not** power-cycle persistence.
* Large local arrays → **stack overflow risk**.
* Dynamic allocation → **fragmentation/predictability concerns**.
* `.bss` is normally cleared to zero during startup.
* `.data` initial values are stored in the firmware image and copied to RAM during startup.
* Use `objdump -h` to inspect object-file sections.

