#ifndef CIRCULAR_DOUBLY_LINKED_LIST_H
#define CIRCULAR_DOUBLY_LINKED_LIST_H

#include <Arduino.h>

template <typename NodeStorageType, typename... StorageArgs>
class circulardoublylist
{
public:
    circulardoublylist();
    ~circulardoublylist();

    struct circlynode
    {
        circlynode(NodeStorageType *storage_pointer);
        NodeStorageType *sp;
        circlynode *n;
        circlynode *p;
    };

    circlynode *insertAtBeginning(StorageArgs... args);
    circlynode *insertAtEnd(StorageArgs... args);
    circlynode *insertAtPosition(int position, StorageArgs... args);
    bool deleteFromPosition(int position);
    void clear();
    NodeStorageType *getStoragePtr(int position);

private:
    circlynode *getNode(int position);
    circlynode *head;
    circlynode *tail;
    circlynode *node_ptr;
    NodeStorageType *storage_ptr;
    int list_nodes;
};

template <typename NodeStorageType, typename... StorageArgs>
circulardoublylist<NodeStorageType, StorageArgs...>::circulardoublylist() : head(nullptr), tail(nullptr), node_ptr(nullptr), storage_ptr(nullptr), list_nodes(0) {}

template <typename NodeStorageType, typename... StorageArgs>
circulardoublylist<NodeStorageType, StorageArgs...>::~circulardoublylist()
{
    clear();
}

template <typename NodeStorageType, typename... StorageArgs>
circulardoublylist<NodeStorageType, StorageArgs...>::circlynode::circlynode(NodeStorageType *storage_pointer) : sp(storage_pointer), n(nullptr), p(nullptr) {}

template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::insertAtBeginning(StorageArgs... args)
{
    NodeStorageType *storage = new NodeStorageType{args...};
    circlynode *new_node = new circlynode(storage);
    if (head == nullptr)
    {
        head = tail = new_node;
        new_node->n = new_node->p = new_node;
    }
    else
    {
        new_node->n = head;
        new_node->p = tail;
        head->p = new_node;
        tail->n = new_node;
        head = new_node;
    }
    list_nodes++;
    return new_node;
}

template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::insertAtEnd(StorageArgs... args)
{
    NodeStorageType *storage = new NodeStorageType{args...};
    circlynode *new_node = new circlynode(storage);
    if (tail == nullptr)
    {
        head = tail = new_node;
        new_node->n = new_node->p = new_node;
    }
    else
    {
        new_node->n = head;
        new_node->p = tail;
        tail->n = new_node;
        head->p = new_node;
        tail = new_node;
    }
    list_nodes++;
    return new_node;
}

template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::insertAtPosition(int position, StorageArgs... args)
{
    if (position < 1 || position > list_nodes + 1)
    {
        return nullptr;
    }
    if (position == 1)
    {
        return insertAtBeginning(args...);
    }
    if (position == list_nodes + 1)
    {
        return insertAtEnd(args...);
    }

    NodeStorageType *storage = new NodeStorageType{args...};
    circlynode *new_node = new circlynode(storage);
    circlynode *current = getNode(position);

    new_node->n = current;
    new_node->p = current->p;
    current->p->n = new_node;
    current->p = new_node;

    list_nodes++;
    return new_node;
}

template <typename NodeStorageType, typename... StorageArgs>
bool circulardoublylist<NodeStorageType, StorageArgs...>::deleteFromPosition(int position)
{
    if (position < 1 || position > list_nodes)
    {
        return false;
    }

    circlynode *node_to_delete = getNode(position);

    if (node_to_delete == head)
    {
        head = head->n;
    }
    if (node_to_delete == tail)
    {
        tail = tail->p;
    }

    node_to_delete->p->n = node_to_delete->n;
    node_to_delete->n->p = node_to_delete->p;

    delete node_to_delete->sp;
    delete node_to_delete;

    list_nodes--;
    return true;
}

template <typename NodeStorageType, typename... StorageArgs>
void circulardoublylist<NodeStorageType, StorageArgs...>::clear()
{
    while (list_nodes > 0)
    {
        deleteFromPosition(1);
    }
}

template <typename NodeStorageType, typename... StorageArgs>
NodeStorageType *circulardoublylist<NodeStorageType, StorageArgs...>::getStoragePtr(int position)
{
    storage_ptr = nullptr;
    circlynode *access = getNode(position);
    if (access != nullptr)
    {
        storage_ptr = access->sp;
        return storage_ptr;
    }
    return storage_ptr;
}

template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::getNode(int position)
{
    if (list_nodes == 0)
    {
        return nullptr;
    }
    if (head == tail && head == nullptr)
    {
        return nullptr; // empty list
    }
    if (list_nodes < position)
    {
        return nullptr; // position out of list range
    }
    if (position == 1)
    {
        return node_ptr = head;
    }
    if (position == list_nodes)
    {
        return node_ptr = tail;
    }

    if ((list_nodes - position) < (list_nodes / 2))
    {
        node_ptr = tail;
        for (int i = 0; i < position - 1; ++i)
        {
            node_ptr = node_ptr->p;
        }
    }
    else
    {
        node_ptr = head;
        for (int i = 1; i < position; ++i)
        {
            node_ptr = node_ptr->n;
        }
    }
    return node_ptr;
}

#endif // CIRCULAR_DOUBLY_LINKED_LIST_H