# Templated Linked List Library

This library provides implementations of templated linked lists in Arduino, including both singly linked lists and circular doubly linked lists. 

## Features

- **Singly Linked List**: A simple implementation of a singly linked list with methods for inserting, deleting, and accessing nodes.
- **Circular Doubly Linked List**: An advanced implementation of a circular doubly linked list that allows traversal in both directions.

## Installation

1. Download the library files.
2. Place the `TemplatedLinkedList` folder in your Arduino libraries directory (usually found in `Documents/Arduino/libraries`).
3. Restart the Arduino IDE.

## Usage

### Including the Library

To use the library in your Arduino sketch, include the main header file:

```cpp
#include <TemplatedLinkedList.h>
```

### Creating a Singly Linked List

```cpp
singlylist<int> mySinglyList;
mySinglyList.insertAtBeginning(10);
mySinglyList.insertAtEnd(20);
```

### Creating a Circular Doubly Linked List

```cpp
circulardoublylist<int> myCircularDoublyList;
myCircularDoublyList.insertAtBeginning(30);
myCircularDoublyList.insertAtEnd(40);
```

## Documentation

Refer to the header files for detailed documentation on the available methods and their usage.

## License

This library is released under the MIT License. See the LICENSE file for more details.