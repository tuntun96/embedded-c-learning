# Day 16 — Very Short Revision Sheet

### Dynamic Memory

```text
Heap → runtime memory allocation
```

| Function    | Purpose                       |
| ----------- | ----------------------------- |
| `malloc()`  | Allocate memory               |
| `calloc()`  | Allocate + initialize to zero |
| `realloc()` | Resize existing allocation    |
| `free()`    | Release allocated memory      |

### `malloc()`

```c
ptr = malloc(n * sizeof(type));
```

* Returns address
* Contents are indeterminate
* Failure → `NULL`

### `calloc()`

```c
ptr = calloc(n, sizeof(type));
```

* Allocates `n` elements
* Initializes allocated memory to zero
* Failure → `NULL`

### `realloc()`

```c
temp = realloc(ptr, new_size);
```

* Resizes existing allocation
* Existing data is preserved
* Address may change
* Failure → `NULL`
* Original allocation remains valid on failure

### `free()`

```c
free(ptr);
ptr = NULL;
```

* Releases heap memory
* Does **not** automatically make pointer `NULL`

### Critical Embedded-C rule

```text
Known size at compile time
        ↓
Prefer static allocation

Runtime/variable size
        ↓
Dynamic allocation may be useful
```

### Remember

> **`malloc` = allocate | `calloc` = allocate + zero | `realloc` = resize | `free` = release**

**Day 16 revision sheet ✅**

Next → **Tracker update**, then **Git commit + push** to close Day 16.
