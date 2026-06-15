#ifndef CIRCULAR_DOUBLY_LINKED_LIST_H
#define CIRCULAR_DOUBLY_LINKED_LIST_H

#include <Arduino.h>

/// @brief inheritable circular doubly linked-list framework
template <typename NodeStorageType, typename... StorageArgs>
class circulardoublylist
{
public:
    /// @brief Constructor for the linked-list
    circulardoublylist();

    /// @brief Destructor for the linked-list
    ~circulardoublylist();

    /// @brief list node with pointers to next, previous, and storage
    struct circlynode
    {
        /// @brief list node structure, contains pointers to next, previous, and storage
        /// @param storage_pointer pointer of type NodeStorageType
        circlynode(NodeStorageType *storage_pointer);
        NodeStorageType *sp; ///< pointer to storage
        circlynode *n;       ///< pointer to next list member (wraps to head)
        circlynode *p;       ///< pointer to previous list member (wraps to tail)
    };

    /// @brief inserts a node at the beginning of the list
    /// @param args Arguments to construct the storage
    /// @return node that was inserted
    circlynode *insertAtBeginning(StorageArgs... args);

    /// @brief inserts a node at the end of the list
    /// @param args Arguments to construct the storage
    /// @return node that was inserted
    circlynode *insertAtEnd(StorageArgs... args);

    /// @brief inserts a node at position
    /// @param position 1-... position in the list to insert
    /// @param args Arguments to construct the storage
    /// @return null on error
    circlynode *insertAtPosition(int position, StorageArgs... args);

    /// @brief deletes the node at position
    /// @param position 1-... position of node to delete
    /// @return true on success, false if position is out of range
    bool deleteFromPosition(int position);

    /// @brief clears the list and deallocates memory
    void clear();

    /// @brief gets the storage pointer at position
    /// @param position 1-... position in the list
    /// @return pointer to storage at position, or null if out of range
    NodeStorageType *getStoragePtr(int position);

    /// @brief gets the last accessed or inserted node
    /// @return pointer to the last accessed node, or null if none
    circlynode *getLastAccessedNode();

    /// @brief gets the storage pointer of the last accessed or inserted node
    /// @return pointer to storage of the last accessed node, or null if none
    NodeStorageType *getLastAccessedNodeStoragePtr();

private:
    /// @brief gets the node at position, updating node_ptr cache
    /// @param position 1-... position in the list
    /// @return pointer to node at position, or null if out of range
    circlynode *getNode(int position);
    circlynode *head;         ///< list head
    circlynode *tail;         ///< list tail
    circlynode *node_ptr;     ///< traversal cache pointer
    circlynode *lastnode;     ///< last accessed or inserted node
    NodeStorageType *storage_ptr; ///< temporary storage pointer used by getStoragePtr
    int list_nodes;           ///< number of list members
};

template <typename NodeStorageType, typename... StorageArgs>
circulardoublylist<NodeStorageType, StorageArgs...>::circulardoublylist() : head(nullptr), tail(nullptr), node_ptr(nullptr), lastnode(nullptr), storage_ptr(nullptr), list_nodes(0) {}

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
    lastnode = new_node;
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
    lastnode = new_node;
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
    lastnode = new_node;
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

    if (list_nodes == 1)
    {
        head = tail = nullptr;
    }
    else
    {
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
    }

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
        lastnode = access;
        return storage_ptr;
    }
    return storage_ptr;
}

template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::getNode(int position)
{
    if (list_nodes == 0 || position < 1)
    {
        return nullptr;
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
        for (int i = 0; i < list_nodes - position; ++i)
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

template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::getLastAccessedNode()
{
    return lastnode;
}

template <typename NodeStorageType, typename... StorageArgs>
NodeStorageType *circulardoublylist<NodeStorageType, StorageArgs...>::getLastAccessedNodeStoragePtr()
{
    return lastnode ? lastnode->sp : nullptr;
}

#endif // CIRCULAR_DOUBLY_LINKED_LIST_H