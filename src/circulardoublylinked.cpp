#include "circulardoublylinked.h"

/// @brief Constructor for the linked-list
template <typename NodeStorageType, typename... StorageArgs>
circulardoublylist<NodeStorageType, StorageArgs...>::circulardoublylist()
    : head(nullptr), tail(nullptr), list_nodes(0)
{
}

/// @brief Destructor for the linked-list
template <typename NodeStorageType, typename... StorageArgs>
circulardoublylist<NodeStorageType, StorageArgs...>::~circulardoublylist()
{
    clear();
}

/// @brief Constructor for circlynode
/// @param storage_pointer pointer of type NodeStorageType
template <typename NodeStorageType, typename... StorageArgs>
circulardoublylist<NodeStorageType, StorageArgs...>::circlynode::circlynode(NodeStorageType *storage_pointer)
    : sp(storage_pointer), n(nullptr), p(nullptr)
{
}

/// @brief inserts a node at the beginning of the list
/// @param args Arguments to construct the storage
/// @return node that was inserted
template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::insertAtBeginning(StorageArgs... args)
{
    circlynode *temp = create_node(args...);
    if (list_nodes == 0) // empty list
    {
        head = tail = temp;
        head->n = tail;
        head->p = tail;
        tail->n = head;
        tail->p = head;
    }
    else
    {
        temp->n = head;
        temp->p = tail;
        head->p = temp;
        tail->n = temp;
        head = temp;
    }
    list_nodes++;
    return head;
}

/// @brief clears the list and deallocates memory
template <typename NodeStorageType, typename... StorageArgs>
void circulardoublylist<NodeStorageType, StorageArgs...>::clear()
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
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::insertAtEnd(StorageArgs... args)
{
    circlynode *temp = create_node(args...);
    if (temp == nullptr || (head == tail && head == nullptr))
    {
        head = tail = temp;
        head->n = tail->n = nullptr;
        head->p = tail->p = nullptr;
        return tail;
    }
    else
    {
        tail->n = temp;
        temp->p = tail;
        tail = temp;
        head->p = tail;
        tail->n = head;
        return tail;
    }
    return nullptr;
}

/// @brief inserts node at position
/// @param position 1-... position in the array to insert
/// @param args Arguments to construct the storage
/// @return null node on error
template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::insertAtPosition(int position, StorageArgs... args)
{
    if (head == tail && head == nullptr) // empty list
    {
        if (position == 1 || list_nodes < position)
        {
            return insertAtBeginning(args...);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        circlynode *ptr, *forenode, *temp;
        temp = create_node(args...);
        if (temp == nullptr)
        {
            deallocate_node(temp);
            return nullptr;
        }
        forenode = getNode(position - 1);
        if (forenode == nullptr)
        {
            deallocate_node(temp);
            return nullptr;
        }
        ptr = forenode;
        forenode = forenode->n;

        ptr->n = temp;
        temp->p = ptr;
        temp->n = forenode;
        forenode->p = temp;
        node_ptr = temp;
        return node_ptr;
    }
}

/// @brief deletes the node at position
/// @param position 1-... position of node to delete
/// @return true if the node was deleted, false otherwise
template <typename NodeStorageType, typename... StorageArgs>
bool circulardoublylist<NodeStorageType, StorageArgs...>::deleteFromPosition(int position)
{
    if (head == tail && head == nullptr || list_nodes < position || position < 1)
    {
        return false;
    }
    else
    {
        delete_node(position);
        return true;
    }
    return false;
}

/// @brief gets the storage pointer at position
/// @param position 1-... position in the list
/// @return pointer to storage at position
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

/// @brief gets the node at position
/// @param position 1-... position in the list
/// @return pointer to node at position
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
        return node_ptr;
    }
    else
    {
        node_ptr = head;
        for (int i = 0; i < position - 1; ++i)
        {
            node_ptr = node_ptr->n;
        }
        return node_ptr;
    }
    return nullptr;
}

/// @brief removes a node from the beginning of the list
template <typename NodeStorageType, typename... StorageArgs>
void circulardoublylist<NodeStorageType, StorageArgs...>::removeAtBeginning()
{
    if (list_nodes == 0)
        return;

    circlynode *temp = head;
    if (list_nodes == 1)
    {
        head = tail = nullptr;
    }
    else
    {
        head = head->n;
        head->p = tail;
        tail->n = head;
    }
    deallocate_node(temp);
}

/// @brief creates a new node
/// @param args Arguments to construct the storage
/// @return pointer to the created node
template <typename NodeStorageType, typename... StorageArgs>
typename circulardoublylist<NodeStorageType, StorageArgs...>::circlynode *
circulardoublylist<NodeStorageType, StorageArgs...>::create_node(StorageArgs... args)
{
    storage_ptr = new NodeStorageType(args...);
    node_ptr = new circlynode(storage_ptr);
    if (node_ptr == nullptr || storage_ptr == nullptr)
    {
        delete storage_ptr;
        delete node_ptr;
        return node_ptr;
    }
    list_nodes++;
    node_ptr->n = nullptr;
    node_ptr->p = nullptr;
    return node_ptr;
}

/// @brief deallocates a node
/// @param dealloc_node pointer to the node to deallocate
template <typename NodeStorageType, typename... StorageArgs>
void circulardoublylist<NodeStorageType, StorageArgs...>::deallocate_node(circlynode *dealloc_node)
{
    if (dealloc_node != nullptr)
    {
        delete dealloc_node->sp;
        delete dealloc_node;
        list_nodes--;
        if (list_nodes == 0)
        {
            head = nullptr;
            tail = nullptr;
            node_ptr = nullptr;
        }
    }
}

/// @brief deletes a node at position
/// @param position 1-... position of node to delete
template <typename NodeStorageType, typename... StorageArgs>
void circulardoublylist<NodeStorageType, StorageArgs...>::delete_node(int position)
{
    if (head == tail == nullptr)
    {
        return; // empty list
    }
    if (list_nodes < position || position < 1)
    {
        return; // position out of list range
    }

    circlynode *ptr;
    if (position == 1)
    {
        node_ptr = head;
        tail->n = node_ptr->n;
        if (node_ptr->n != nullptr)
        {
            node_ptr->n->p = tail;
        }
        head = node_ptr->n;
        deallocate_node(node_ptr);
        if (list_nodes == 0)
        {
            head = tail = nullptr;
        }
        return; // delete head
    }
    if (position > 1 && position <= list_nodes)
    {
        node_ptr = getNode(position);
        ptr = node_ptr->p;
        ptr->n = node_ptr->n;
        if (node_ptr->n != nullptr)
        {
            node_ptr->n->p = ptr;
        }
        if (position == list_nodes) // delete other than tail but not head if not true
        {
            tail = ptr; // deleted tail
        }
        deallocate_node(node_ptr);
        return;
    }
    return; // node does not exist
}

// Explicit template instantiation
template class circulardoublylist<int>; // Example instantiation, replace with actual types as needed