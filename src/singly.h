#ifndef CIRCULAR_SINGLY_LINKED_H
#define CIRCULAR_SINGLY_LINKED_H

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

#include "singly.cpp"

#endif // CIRCULAR_SINGLY_LINKED_H