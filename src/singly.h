#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <Arduino.h>

/// @brief inheritable linked-list framework
template <typename NodeStorageType, typename... StorageArgs>
class singlylist
{
public:
    /// @brief Constructor for the linked-list
    singlylist();

    /// @brief Destructor for the linked-list
    ~singlylist();

    /// @brief list node with pointer to next and void* to storage
    struct singlynode
    {
        /// @brief list node structure, contains pointer to next and storage
        /// @param storage_pointer pointer of type NodeStorageType
        singlynode(NodeStorageType *storage_pointer);
        NodeStorageType *sp; ///< pointer to storage
        singlynode *n;       ///< pointer to next list member or null
    };

    /// @brief inserts a node at the beginning of the list
    /// @param args Arguments to construct the storage
    /// @return node that was inserted
    singlynode *insertAtBeginning(StorageArgs... args);

    /// @brief clears the list and deallocates memory
    void clear();

    /// @brief inserts a node at the end of the list
    /// @param args Arguments to construct the storage
    /// @return node that was inserted
    singlynode *insertAtEnd(StorageArgs... args);

    /// @brief inserts node at position
    /// @param position 1-... position in the array to insert
    /// @param args Arguments to construct the storage
    /// @return null node on error
    singlynode *insertAtPosition(int position, StorageArgs... args);

    /// @brief deletes the node at position
    /// @param position 1-... position of node to delete
    bool deleteFromPosition(int position);

    /// @brief gets the storage pointer at position
    /// @param position 1-... position in the list
    /// @return pointer to storage at position
    NodeStorageType *getStoragePtr(int position);

    /// @brief gets the node at position
    /// @param position 1-... position in the list
    /// @return pointer to node at position
    singlynode *getNode(int position);

    /// @brief removes a node from the beginning of the list
    void removeAtBeginning();

private:
    NodeStorageType *storage_ptr; ///< pointer to storage
    singlynode *head;             ///< list head
    singlynode *tail;             ///< list tail
    size_t list_nodes;            ///< number of list members

    /// @brief creates a new node
    /// @param args Arguments to construct the storage
    /// @return pointer to the created node
    singlynode *create_node(StorageArgs... args);

    /// @brief deallocates a node
    /// @param dealloc_node pointer to the node to deallocate
    void deallocate_node(singlynode *dealloc_node);

    /// @brief deletes a node at position
    /// @param position 1-... position of node to delete
    void delete_node(int position);
};

// Implementation of template functions

template <typename NodeStorageType, typename... StorageArgs>
singlylist<NodeStorageType, StorageArgs...>::singlylist()
    : head(nullptr), tail(nullptr), list_nodes(0)
{
}

template <typename NodeStorageType, typename... StorageArgs>
singlylist<NodeStorageType, StorageArgs...>::~singlylist()
{
    clear();
}

template <typename NodeStorageType, typename... StorageArgs>
singlylist<NodeStorageType, StorageArgs...>::singlynode::singlynode(NodeStorageType *storage_pointer)
    : sp(storage_pointer), n(nullptr)
{
}

template <typename NodeStorageType, typename... StorageArgs>
typename singlylist<NodeStorageType, StorageArgs...>::singlynode *
singlylist<NodeStorageType, StorageArgs...>::insertAtBeginning(StorageArgs... args)
{
    singlynode *temp = create_node(args...);
    if (list_nodes == 0) // empty list
    {
        head = tail = temp;
    }
    else
    {
        temp->n = head;
        head = temp;
    }
    list_nodes++;
    return head;
}

template <typename NodeStorageType, typename... StorageArgs>
void singlylist<NodeStorageType, StorageArgs...>::clear()
{
    while (list_nodes > 0)
    {
        removeAtBeginning();
    }
}

template <typename NodeStorageType, typename... StorageArgs>
typename singlylist<NodeStorageType, StorageArgs...>::singlynode *
singlylist<NodeStorageType, StorageArgs...>::insertAtEnd(StorageArgs... args)
{
    singlynode *temp = create_node(args...);
    if (list_nodes == 0) // empty list
    {
        head = tail = temp;
    }
    else
    {
        tail->n = temp;
        tail = temp;
    }
    list_nodes++;
    return tail;
}

template <typename NodeStorageType, typename... StorageArgs>
typename singlylist<NodeStorageType, StorageArgs...>::singlynode *
singlylist<NodeStorageType, StorageArgs...>::insertAtPosition(int position, StorageArgs... args)
{
    if (position < 1 || position > list_nodes + 1)
    {
        return nullptr; // position out of range
    }
    if (position == 1)
    {
        return insertAtBeginning(args...);
    }
    if (position == list_nodes + 1)
    {
        return insertAtEnd(args...);
    }

    singlynode *prev = getNode(position - 1);
    singlynode *temp = create_node(args...);
    if (prev == nullptr || temp == nullptr)
    {
        return nullptr;
    }
    temp->n = prev->n;
    prev->n = temp;
    list_nodes++;
    return temp;
}

template <typename NodeStorageType, typename... StorageArgs>
bool singlylist<NodeStorageType, StorageArgs...>::deleteFromPosition(int position)
{
    if (position < 1 || position > list_nodes)
    {
        return false; // position out of range
    }
    delete_node(position);
    return true;
}

template <typename NodeStorageType, typename... StorageArgs>
NodeStorageType *singlylist<NodeStorageType, StorageArgs...>::getStoragePtr(int position)
{
    singlynode *access = getNode(position);
    return access ? access->sp : nullptr;
}

template <typename NodeStorageType, typename... StorageArgs>
typename singlylist<NodeStorageType, StorageArgs...>::singlynode *
singlylist<NodeStorageType, StorageArgs...>::getNode(int position)
{
    if (position < 1 || position > list_nodes)
    {
        return nullptr; // position out of range
    }
    singlynode *current = head;
    for (int i = 1; i < position; ++i)
    {
        current = current->n;
    }
    return current;
}

template <typename NodeStorageType, typename... StorageArgs>
void singlylist<NodeStorageType, StorageArgs...>::removeAtBeginning()
{
    if (list_nodes == 0)
        return;

    singlynode *temp = head;
    head = head->n;
    deallocate_node(temp);
    if (--list_nodes == 0)
    {
        tail = nullptr;
    }
}

template <typename NodeStorageType, typename... StorageArgs>
typename singlylist<NodeStorageType, StorageArgs...>::singlynode *
singlylist<NodeStorageType, StorageArgs...>::create_node(StorageArgs... args)
{
    storage_ptr = new NodeStorageType{args...};
    singlynode *new_node = new singlynode(storage_ptr);
    if (new_node == nullptr || storage_ptr == nullptr)
    {
        delete storage_ptr;
        delete new_node;
        return nullptr;
    }
    return new_node;
}

template <typename NodeStorageType, typename... StorageArgs>
void singlylist<NodeStorageType, StorageArgs...>::deallocate_node(singlynode *dealloc_node)
{
    if (dealloc_node != nullptr)
    {
        delete dealloc_node->sp;
        delete dealloc_node;
    }
}

template <typename NodeStorageType, typename... StorageArgs>
void singlylist<NodeStorageType, StorageArgs...>::delete_node(int position)
{
    if (position == 1)
    {
        removeAtBeginning();
        return;
    }

    singlynode *prev = getNode(position - 1);
    singlynode *current = prev->n;
    prev->n = current->n;
    if (current == tail)
    {
        tail = prev;
    }
    deallocate_node(current);
    list_nodes--;
}

#endif // SINGLY_LINKED_LIST_H