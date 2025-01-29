#include "singly.h"

/// @brief Constructor for singlylist
template <typename NodeStorageType, typename... StorageArgs>
singlylist<NodeStorageType, StorageArgs...>::singlylist()
    : head(nullptr), tail(nullptr), list_nodes(0)
{
}

/// @brief Destructor for singlylist
template <typename NodeStorageType, typename... StorageArgs>
singlylist<NodeStorageType, StorageArgs...>::~singlylist()
{
    clear();
}

/// @brief Constructor for singlynode
/// @param storage_pointer pointer of type NodeStorageType
template <typename NodeStorageType, typename... StorageArgs>
singlylist<NodeStorageType, StorageArgs...>::singlynode::singlynode(NodeStorageType *storage_pointer)
    : sp(storage_pointer), n(nullptr)
{
}

/// @brief inserts a node at the beginning of the list
/// @param args Arguments to construct the storage
/// @return node that was inserted
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

/// @brief clears the list and deallocates memory
template <typename NodeStorageType, typename... StorageArgs>
void singlylist<NodeStorageType, StorageArgs...>::clear()
{
    while (list_nodes > 0)
    {
        removeAtBeginning();
    }
}

/// @brief inserts a node at the end of the list
/// @param args Arguments to construct the storage
/// @return node that was inserted
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

/// @brief inserts node at position
/// @param position 1-... position in the array to insert
/// @param args Arguments to construct the storage
/// @return null node on error
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

/// @brief deletes the node at position
/// @param position 1-... position of node to delete
/// @return true if the node was deleted, false otherwise
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

/// @brief gets the storage pointer at position
/// @param position 1-... position in the list
/// @return pointer to storage at position
template <typename NodeStorageType, typename... StorageArgs>
NodeStorageType *singlylist<NodeStorageType, StorageArgs...>::getStoragePtr(int position)
{
    singlynode *access = getNode(position);
    return access ? access->sp : nullptr;
}

/// @brief gets the node at position
/// @param position 1-... position in the list
/// @return pointer to node at position
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

/// @brief removes a node from the beginning of the list
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

/// @brief creates a new node
/// @param args Arguments to construct the storage
/// @return pointer to the created node
template <typename NodeStorageType, typename... StorageArgs>
typename singlylist<NodeStorageType, StorageArgs...>::singlynode *
singlylist<NodeStorageType, StorageArgs...>::create_node(StorageArgs... args)
{
    storage_ptr = new NodeStorageType(args...);
    singlynode *new_node = new singlynode(storage_ptr);
    if (new_node == nullptr || storage_ptr == nullptr)
    {
        delete storage_ptr;
        delete new_node;
        return nullptr;
    }
    return new_node;
}

/// @brief deallocates a node
/// @param dealloc_node pointer to the node to deallocate
template <typename NodeStorageType, typename... StorageArgs>
void singlylist<NodeStorageType, StorageArgs...>::deallocate_node(singlynode *dealloc_node)
{
    if (dealloc_node != nullptr)
    {
        delete dealloc_node->sp;
        delete dealloc_node;
    }
}

/// @brief deletes a node at position
/// @param position 1-... position of node to delete
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

// Explicit template instantiation
template class singlylist<int>; // Example instantiation, replace with actual types as needed