#include <Arduino.h>
#include <TemplatedLinkedList.h>

// Define a container type
struct Container {
    int a, b, c, d;
};

// Create instances of the lists
singlylist<Container, int, int, int, int> int_list;
circulardoublylist<Container, int, int, int, int> int_dlist;

void setup() {
    Serial.begin(115200);

    // Insert elements into the singly linked list
    int_list.insertAtBeginning(1, 2, 3, 4);
    int_list.insertAtEnd(5, 6, 7, 8);

    // Insert elements into the circular doubly linked list
    int_dlist.insertAtBeginning(9, 10, 11, 12);
    int_dlist.insertAtEnd(13, 14, 15, 16);

    // Print elements from the singly linked list
    Container* container = int_list.getStoragePtr(1);
    if (container) {
        Serial.print("Singly list first element: ");
        Serial.print(container->a);
        Serial.print(", ");
        Serial.print(container->b);
        Serial.print(", ");
        Serial.print(container->c);
        Serial.println(container->d);
    }

    // Print elements from the circular doubly linked list
    container = int_dlist.getStoragePtr(1);
    if (container) {
        Serial.print("Circular doubly list first element: ");
        Serial.print(container->a);
        Serial.print(", ");
        Serial.print(container->b);
        Serial.print(", ");
        Serial.print(container->c);
        Serial.println(container->d);
    }
}

void loop() {
    // Your loop code here
}