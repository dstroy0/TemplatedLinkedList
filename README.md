# TemplatedLinkedList

An Arduino library providing templated linked-list data structures: a singly linked list and a circular doubly linked list. Both are header-only and work with any struct or primitive type via template parameters.

## Features

- **`singlylist`** — singly linked list with O(1) insert at head/tail, positional insert/delete, and last-accessed-node tracking.
- **`circulardoublylist`** — circular doubly linked list with bidirectional traversal, O(1) insert at head/tail, optimized positional lookup (traverses from whichever end is closer), and last-accessed-node tracking.

## Installation

1. Download or clone this repository.
2. Place the `TemplatedLinkedList` folder in your Arduino libraries directory (typically `Documents/Arduino/libraries`).
3. Restart the Arduino IDE.

## Usage

### Including the Library

```cpp
#include <TemplatedLinkedList.h>
```

### Template Parameters

Both classes share the same two-parameter template signature:

```cpp
template <typename NodeStorageType, typename... StorageArgs>
```

- `NodeStorageType` — the struct (or type) stored at each node.
- `StorageArgs...` — the constructor argument types used to build each `NodeStorageType` instance.

The storage object is heap-allocated by the list; callers receive raw pointers to it via `getStoragePtr()` or through the node's `.sp` member.

### Singly Linked List

```cpp
#include <TemplatedLinkedList.h>

struct Point {
    int x, y;
};

// NodeStorageType = Point, StorageArgs = int, int
singlylist<Point, int, int> points;

void setup() {
    points.insertAtBeginning(1, 2);    // head: Point{1, 2}
    points.insertAtEnd(3, 4);          // tail: Point{3, 4}
    points.insertAtPosition(2, 5, 6);  // position 2: Point{5, 6}

    Point *p = points.getStoragePtr(1); // p->x == 1, p->y == 2

    points.deleteFromPosition(2);
    points.clear();
}
```

### Circular Doubly Linked List

```cpp
#include <TemplatedLinkedList.h>

struct Color {
    uint8_t r, g, b;
};

// NodeStorageType = Color, StorageArgs = uint8_t, uint8_t, uint8_t
circulardoublylist<Color, uint8_t, uint8_t, uint8_t> palette;

void setup() {
    palette.insertAtBeginning(255, 0, 0);    // red at head
    palette.insertAtEnd(0, 255, 0);          // green at tail
    palette.insertAtPosition(2, 0, 0, 255);  // blue at position 2

    Color *c = palette.getStoragePtr(1);     // c->r == 255

    palette.deleteFromPosition(1);
    palette.clear();
}
```

## API Reference

### `singlylist<NodeStorageType, StorageArgs...>`

| Method | Description |
|--------|-------------|
| `singlynode *insertAtBeginning(StorageArgs... args)` | Inserts a new node at the head. Returns the inserted node. |
| `singlynode *insertAtEnd(StorageArgs... args)` | Inserts a new node at the tail. Returns the inserted node. |
| `singlynode *insertAtPosition(int position, StorageArgs... args)` | Inserts at 1-based position. Returns `nullptr` if out of range. |
| `bool deleteFromPosition(int position)` | Deletes the node at 1-based position. Returns `false` if out of range. |
| `NodeStorageType *getStoragePtr(int position)` | Returns pointer to storage at position, or `nullptr`. Updates `lastnode`. |
| `singlynode *getNode(int position)` | Returns pointer to the node at position, or `nullptr`. Updates `lastnode`. |
| `void removeAtBeginning()` | Removes the head node. No-op on an empty list. |
| `void clear()` | Removes all nodes and frees memory. |
| `singlynode *getLastAccessedNode()` | Returns the last node touched by any insert, `getNode`, or `getStoragePtr`. Returns `nullptr` if none. |
| `NodeStorageType *getLastAccessedNodeStoragePtr()` | Returns the storage pointer of the last accessed node, or `nullptr`. |

#### `singlynode` members

| Member | Description |
|--------|-------------|
| `NodeStorageType *sp` | Pointer to the stored data. |
| `singlynode *n` | Pointer to the next node, or `nullptr` at the tail. |

---

### `circulardoublylist<NodeStorageType, StorageArgs...>`

| Method | Description |
|--------|-------------|
| `circlynode *insertAtBeginning(StorageArgs... args)` | Inserts a new node at the head. Returns the inserted node. |
| `circlynode *insertAtEnd(StorageArgs... args)` | Inserts a new node at the tail. Returns the inserted node. |
| `circlynode *insertAtPosition(int position, StorageArgs... args)` | Inserts at 1-based position. Returns `nullptr` if out of range. |
| `bool deleteFromPosition(int position)` | Deletes the node at 1-based position. Returns `false` if out of range. |
| `void clear()` | Removes all nodes and frees memory. |
| `NodeStorageType *getStoragePtr(int position)` | Returns pointer to storage at position, or `nullptr`. Updates `lastnode`. |
| `circlynode *getLastAccessedNode()` | Returns the last node touched by any insert or `getStoragePtr`. Returns `nullptr` if none. |
| `NodeStorageType *getLastAccessedNodeStoragePtr()` | Returns the storage pointer of the last accessed node, or `nullptr`. |

#### `circlynode` members

| Member | Description |
|--------|-------------|
| `NodeStorageType *sp` | Pointer to the stored data. |
| `circlynode *n` | Pointer to the next node (wraps from tail back to head). |
| `circlynode *p` | Pointer to the previous node (wraps from head back to tail). |

---

## Testing

A native C++ test suite lives in [`test/`](test/). It compiles and runs with any desktop g++ (no Arduino hardware required) via a minimal `Arduino.h` stub.

**Requirements:** g++ with C++11 support. On Windows, [MSYS2](https://www.msys2.org/) (`pacman -S mingw-w64-ucrt-x86_64-gcc`) or [MinGW-w64](https://www.mingw-w64.org/). On Linux/macOS, `build-essential` or Xcode Command Line Tools.

```powershell
# Windows (PowerShell)
pwsh test/run_tests.ps1

# Linux / macOS / WSL (Bash)
bash test/run_tests.sh
```

Both scripts locate g++ automatically, compile with `-Wall -Wextra -Wpedantic`, run the binary, and exit non-zero on any failure. Pass `--clean` / `-Clean` to remove the previous binary before building.

The suite covers 27 test functions and 269 assertions:

- Empty-list behavior for all query and delete operations
- `insertAtBeginning`, `insertAtEnd`, `insertAtPosition` — return values, data correctness, link integrity
- `deleteFromPosition` — head, middle, tail, single-element (regression), sequential drain, and all out-of-range inputs
- `removeAtBeginning` — including no-op on empty list
- `getNode` / `getStoragePtr` — every valid position in a 5- and 6-element list, all out-of-range inputs
- Backward traversal regression — 10-element `circulardoublylist` verifying positions 6–9 (the previously broken path)
- Circular link integrity — forward and backward pointer chain after insertions and after head deletion
- `lastnode` tracking — updated correctly by inserts and reads, queried via `getLastAccessedNode()` and `getLastAccessedNodeStoragePtr()`
- `clear()` and destructor — no crash, list is fully reusable after clearing

## License

This library is released under the GNU Affero General Public License v3.0. See the [LICENSE](LICENSE) file for the full text.
