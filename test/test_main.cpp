// TemplatedLinkedList test suite
// Compile: g++ -std=c++11 -Wall -Wextra -I../src -I. -o test_bin test_main.cpp
// Or use the provided run_tests.ps1 / run_tests.sh scripts.

#include <iostream>
#include <cstdlib>
#include "TemplatedLinkedList.h"

// ─── Minimal test framework ───────────────────────────────────────────────────

static int g_run    = 0;
static int g_passed = 0;
static int g_failed = 0;
static const char *g_suite = "";

static void begin_suite(const char *name)
{
    g_suite = name;
    std::cout << "\n[" << name << "]\n";
}

static void check_impl(bool ok, const char *file, int line, const char *expr)
{
    ++g_run;
    if (ok)
    {
        ++g_passed;
        std::cout << "  PASS  " << expr << "\n";
    }
    else
    {
        ++g_failed;
        std::cerr << "  FAIL  " << file << ":" << line << "  " << expr << "\n";
    }
}

#define CHECK(expr) check_impl(!!(expr), __FILE__, __LINE__, #expr)

// ─── Storage type used across all tests ──────────────────────────────────────

struct Vec3
{
    int x, y, z;
};

// ─── singlylist tests ─────────────────────────────────────────────────────────

void test_singly_empty_list()
{
    begin_suite("singlylist: empty list");
    singlylist<Vec3, int, int, int> list;

    CHECK(list.getNode(1)                     == nullptr);
    CHECK(list.getNode(0)                     == nullptr);
    CHECK(list.getNode(-1)                    == nullptr);
    CHECK(list.getStoragePtr(1)               == nullptr);
    CHECK(list.getLastAccessedNode()          == nullptr);
    CHECK(list.getLastAccessedNodeStoragePtr()== nullptr);
    CHECK(list.deleteFromPosition(1)          == false);
    CHECK(list.deleteFromPosition(0)          == false);
}

void test_singly_insert_beginning()
{
    begin_suite("singlylist: insertAtBeginning");
    singlylist<Vec3, int, int, int> list;

    // Single insert
    auto *n1 = list.insertAtBeginning(1, 2, 3);
    CHECK(n1          != nullptr);
    CHECK(n1->sp      != nullptr);
    CHECK(n1->sp->x   == 1);
    CHECK(n1->sp->y   == 2);
    CHECK(n1->sp->z   == 3);
    CHECK(n1->n       == nullptr); // only element

    // Second insert becomes new head
    auto *n2 = list.insertAtBeginning(4, 5, 6);
    CHECK(n2          != nullptr);
    CHECK(n2->sp->x   == 4);
    CHECK(n2->n       == n1);  // new head->n is old head
    // lastnode reflects the most recent insert before any getStoragePtr call
    CHECK(list.getLastAccessedNode() == n2);

    // Positional access (these update lastnode)
    CHECK(list.getStoragePtr(1)->x == 4);
    CHECK(list.getStoragePtr(2)->x == 1);
    CHECK(list.getStoragePtr(3)    == nullptr);
}

void test_singly_insert_end()
{
    begin_suite("singlylist: insertAtEnd");
    singlylist<Vec3, int, int, int> list;

    auto *n1 = list.insertAtEnd(10, 20, 30);
    CHECK(n1->sp->x == 10);
    CHECK(n1->n     == nullptr);

    auto *n2 = list.insertAtEnd(40, 50, 60);
    CHECK(n2->sp->x == 40);
    CHECK(n1->n     == n2);   // first->n links to second
    CHECK(n2->n     == nullptr);

    CHECK(list.getStoragePtr(1)->x == 10);
    CHECK(list.getStoragePtr(2)->x == 40);
    CHECK(list.getStoragePtr(3)    == nullptr);

    // lastnode tracks latest insert
    CHECK(list.getLastAccessedNode()           == n2);
    CHECK(list.getLastAccessedNodeStoragePtr() == n2->sp);
}

void test_singly_insert_position()
{
    begin_suite("singlylist: insertAtPosition");
    singlylist<Vec3, int, int, int> list;

    list.insertAtEnd(1, 0, 0); // pos 1
    list.insertAtEnd(3, 0, 0); // pos 2

    // Middle insert
    auto *mid = list.insertAtPosition(2, 2, 0, 0);
    CHECK(mid            != nullptr);
    CHECK(mid->sp->x     == 2);
    CHECK(list.getStoragePtr(1)->x == 1);
    CHECK(list.getStoragePtr(2)->x == 2);
    CHECK(list.getStoragePtr(3)->x == 3);

    // New head (position 1)
    auto *front = list.insertAtPosition(1, 0, 0, 0);
    CHECK(front              != nullptr);
    CHECK(list.getStoragePtr(1)->x == 0);

    // Append at end (position == list_nodes + 1 == 5)
    auto *back = list.insertAtPosition(5, 9, 0, 0);
    CHECK(back               != nullptr);
    CHECK(list.getStoragePtr(5)->x == 9);

    // Out-of-range positions
    CHECK(list.insertAtPosition(0,  0, 0, 0) == nullptr);
    CHECK(list.insertAtPosition(7,  0, 0, 0) == nullptr);
    CHECK(list.insertAtPosition(-1, 0, 0, 0) == nullptr);
}

void test_singly_delete_position()
{
    begin_suite("singlylist: deleteFromPosition");
    singlylist<Vec3, int, int, int> list;

    // Out of range on empty list
    CHECK(list.deleteFromPosition(1) == false);
    CHECK(list.deleteFromPosition(0) == false);

    list.insertAtEnd(1, 0, 0); // pos 1
    list.insertAtEnd(2, 0, 0); // pos 2
    list.insertAtEnd(3, 0, 0); // pos 3

    // Delete middle (pos 2: value 2)
    CHECK(list.deleteFromPosition(2) == true);
    CHECK(list.getStoragePtr(1)->x   == 1);
    CHECK(list.getStoragePtr(2)->x   == 3);
    CHECK(list.getStoragePtr(3)      == nullptr);

    // Delete head
    CHECK(list.deleteFromPosition(1) == true);
    CHECK(list.getStoragePtr(1)->x   == 3);

    // Delete only remaining element
    CHECK(list.deleteFromPosition(1) == true);
    CHECK(list.getStoragePtr(1)      == nullptr);

    // Out of range on now-empty list
    CHECK(list.deleteFromPosition(1) == false);
}

void test_singly_delete_tail()
{
    begin_suite("singlylist: deleteFromPosition tail");
    singlylist<Vec3, int, int, int> list;

    list.insertAtEnd(1, 0, 0);
    list.insertAtEnd(2, 0, 0);
    list.insertAtEnd(3, 0, 0);

    CHECK(list.deleteFromPosition(3) == true);
    CHECK(list.getStoragePtr(1)->x   == 1);
    CHECK(list.getStoragePtr(2)->x   == 2);
    CHECK(list.getStoragePtr(3)      == nullptr);
}

void test_singly_remove_at_beginning()
{
    begin_suite("singlylist: removeAtBeginning");
    singlylist<Vec3, int, int, int> list;

    // No-op on empty list — must not crash
    list.removeAtBeginning();
    CHECK(true); // reaching here means no crash

    list.insertAtEnd(10, 0, 0);
    list.insertAtEnd(20, 0, 0);

    list.removeAtBeginning();
    CHECK(list.getStoragePtr(1)->x == 20);

    list.removeAtBeginning();
    CHECK(list.getStoragePtr(1) == nullptr);

    // Another remove on empty list — must not crash
    list.removeAtBeginning();
    CHECK(true);
}

void test_singly_get_node_and_storage()
{
    begin_suite("singlylist: getNode / getStoragePtr");
    singlylist<Vec3, int, int, int> list;

    // Empty list edge cases
    CHECK(list.getNode(0)       == nullptr);
    CHECK(list.getNode(1)       == nullptr);
    CHECK(list.getNode(-1)      == nullptr);
    CHECK(list.getStoragePtr(0) == nullptr);
    CHECK(list.getStoragePtr(1) == nullptr);

    for (int i = 1; i <= 5; i++)
        list.insertAtEnd(i * 10, i * 20, i * 30);

    // Every valid position
    for (int i = 1; i <= 5; i++)
    {
        Vec3 *sp = list.getStoragePtr(i);
        CHECK(sp        != nullptr);
        CHECK(sp->x     == i * 10);
        CHECK(sp->y     == i * 20);
        CHECK(sp->z     == i * 30);

        auto *node = list.getNode(i);
        CHECK(node      != nullptr);
        CHECK(node->sp  == sp);
    }

    // Out-of-range
    CHECK(list.getStoragePtr(0)       == nullptr);
    CHECK(list.getStoragePtr(6)       == nullptr);
    CHECK(list.getNode(0)             == nullptr);
    CHECK(list.getNode(6)             == nullptr);
}

void test_singly_last_accessed()
{
    begin_suite("singlylist: last accessed node tracking");
    singlylist<Vec3, int, int, int> list;

    // Initially null
    CHECK(list.getLastAccessedNode()           == nullptr);
    CHECK(list.getLastAccessedNodeStoragePtr() == nullptr);

    // Updated by insertAtBeginning
    auto *n1 = list.insertAtBeginning(1, 2, 3);
    CHECK(list.getLastAccessedNode()           == n1);
    CHECK(list.getLastAccessedNodeStoragePtr() == n1->sp);

    // Updated by insertAtEnd
    auto *n2 = list.insertAtEnd(4, 5, 6);
    CHECK(list.getLastAccessedNode()           == n2);

    // Updated by getNode
    auto *got = list.getNode(1);
    CHECK(list.getLastAccessedNode()           == got);
    CHECK(got->sp->x                           == 1);

    // Updated by getStoragePtr
    list.getStoragePtr(2);
    CHECK(list.getLastAccessedNodeStoragePtr() != nullptr);
    CHECK(list.getLastAccessedNodeStoragePtr()->x == 4);

    // Updated by insertAtPosition
    auto *n3 = list.insertAtPosition(2, 7, 8, 9);
    CHECK(list.getLastAccessedNode()           == n3);
}

void test_singly_clear()
{
    begin_suite("singlylist: clear");
    singlylist<Vec3, int, int, int> list;

    // Clear empty list — must not crash
    list.clear();
    CHECK(list.getStoragePtr(1) == nullptr);

    for (int i = 0; i < 10; i++)
        list.insertAtEnd(i, 0, 0);

    list.clear();
    CHECK(list.getStoragePtr(1) == nullptr);
    CHECK(list.getNode(1)       == nullptr);

    // Usable after clear
    list.insertAtEnd(99, 0, 0);
    CHECK(list.getStoragePtr(1)->x == 99);
}

void test_singly_destructor()
{
    begin_suite("singlylist: destructor (no crash or leak)");
    {
        singlylist<Vec3, int, int, int> list;
        for (int i = 0; i < 20; i++)
            list.insertAtEnd(i, i, i);
    } // destructor called here
    CHECK(true); // reaching here == no crash
}

// ─── circulardoublylist tests ─────────────────────────────────────────────────

void test_cdl_empty_list()
{
    begin_suite("circulardoublylist: empty list");
    circulardoublylist<Vec3, int, int, int> list;

    CHECK(list.getStoragePtr(1)                == nullptr);
    CHECK(list.getStoragePtr(0)                == nullptr);
    CHECK(list.getLastAccessedNode()           == nullptr);
    CHECK(list.getLastAccessedNodeStoragePtr() == nullptr);
    CHECK(list.deleteFromPosition(1)           == false);
    CHECK(list.deleteFromPosition(0)           == false);
}

void test_cdl_insert_beginning()
{
    begin_suite("circulardoublylist: insertAtBeginning");
    circulardoublylist<Vec3, int, int, int> list;

    // Single element: circular, n and p must point to itself
    auto *n1 = list.insertAtBeginning(1, 2, 3);
    CHECK(n1            != nullptr);
    CHECK(n1->sp->x     == 1);
    CHECK(n1->n         == n1);
    CHECK(n1->p         == n1);
    CHECK(list.getLastAccessedNode() == n1);

    // Second element at head
    auto *n2 = list.insertAtBeginning(4, 5, 6);
    CHECK(n2->sp->x     == 4);
    // lastnode reflects the insert before any getStoragePtr call
    CHECK(list.getLastAccessedNode() == n2);
    // n2 is head, n1 is tail
    // forward: n2->n == n1, n1->n == n2  (wraps)
    // backward: n2->p == n1 (tail), n1->p == n2 (head)
    CHECK(n2->n         == n1);
    CHECK(n1->n         == n2);
    CHECK(n2->p         == n1);
    CHECK(n1->p         == n2);

    CHECK(list.getStoragePtr(1)->x == 4); // head
    CHECK(list.getStoragePtr(2)->x == 1); // tail
    CHECK(list.getStoragePtr(3)    == nullptr);
}

void test_cdl_insert_end()
{
    begin_suite("circulardoublylist: insertAtEnd");
    circulardoublylist<Vec3, int, int, int> list;

    // Single element: circular
    auto *n1 = list.insertAtEnd(10, 0, 0);
    CHECK(n1->n == n1);
    CHECK(n1->p == n1);

    // Second element at tail
    auto *n2 = list.insertAtEnd(20, 0, 0);
    // lastnode reflects the insert before any getStoragePtr call
    CHECK(list.getLastAccessedNode() == n2);
    // n1 is head, n2 is tail
    CHECK(n1->n == n2);
    CHECK(n2->p == n1);
    CHECK(n2->n == n1); // circular: tail->n == head
    CHECK(n1->p == n2); // circular: head->p == tail

    CHECK(list.getStoragePtr(1)->x == 10);
    CHECK(list.getStoragePtr(2)->x == 20);

    // Third element
    auto *n3 = list.insertAtEnd(30, 0, 0);
    CHECK(n2->n == n3);
    CHECK(n3->p == n2);
    CHECK(n3->n == n1);
    CHECK(n1->p == n3);

    CHECK(list.getStoragePtr(3)->x == 30);
}

void test_cdl_insert_position()
{
    begin_suite("circulardoublylist: insertAtPosition");
    circulardoublylist<Vec3, int, int, int> list;

    list.insertAtEnd(1, 0, 0);
    list.insertAtEnd(3, 0, 0);

    // Middle insert
    auto *mid = list.insertAtPosition(2, 2, 0, 0);
    CHECK(mid            != nullptr);
    CHECK(mid->sp->x     == 2);
    // lastnode reflects the insert before any getStoragePtr call
    CHECK(list.getLastAccessedNode() == mid);
    CHECK(list.getStoragePtr(1)->x == 1);
    CHECK(list.getStoragePtr(2)->x == 2);
    CHECK(list.getStoragePtr(3)->x == 3);

    // New head
    auto *front = list.insertAtPosition(1, 0, 0, 0);
    CHECK(front               != nullptr);
    CHECK(list.getStoragePtr(1)->x == 0);

    // Append at end (list has 4 elements, position 5)
    auto *back = list.insertAtPosition(5, 9, 0, 0);
    CHECK(back                != nullptr);
    CHECK(list.getStoragePtr(5)->x == 9);

    // Out-of-range positions
    CHECK(list.insertAtPosition(0,  0, 0, 0) == nullptr);
    CHECK(list.insertAtPosition(7,  0, 0, 0) == nullptr);
    CHECK(list.insertAtPosition(-1, 0, 0, 0) == nullptr);
}

void test_cdl_delete_single_element()
{
    begin_suite("circulardoublylist: deleteFromPosition single element (regression)");
    circulardoublylist<Vec3, int, int, int> list;

    list.insertAtEnd(42, 0, 0);

    // Deleting the only element must not leave dangling head/tail
    CHECK(list.deleteFromPosition(1) == true);
    CHECK(list.getStoragePtr(1)      == nullptr);
    CHECK(list.deleteFromPosition(1) == false); // list is now empty

    // Must be fully usable after
    list.insertAtEnd(99, 0, 0);
    CHECK(list.getStoragePtr(1)->x == 99);
    list.clear();
}

void test_cdl_delete_head()
{
    begin_suite("circulardoublylist: deleteFromPosition head");
    circulardoublylist<Vec3, int, int, int> list;

    list.insertAtEnd(1, 0, 0);
    list.insertAtEnd(2, 0, 0);
    list.insertAtEnd(3, 0, 0);

    CHECK(list.deleteFromPosition(1) == true);
    CHECK(list.getStoragePtr(1)->x   == 2);
    CHECK(list.getStoragePtr(2)->x   == 3);
    CHECK(list.getStoragePtr(3)      == nullptr);
}

void test_cdl_delete_tail()
{
    begin_suite("circulardoublylist: deleteFromPosition tail");
    circulardoublylist<Vec3, int, int, int> list;

    list.insertAtEnd(1, 0, 0);
    list.insertAtEnd(2, 0, 0);
    list.insertAtEnd(3, 0, 0);

    CHECK(list.deleteFromPosition(3) == true);
    CHECK(list.getStoragePtr(1)->x   == 1);
    CHECK(list.getStoragePtr(2)->x   == 2);
    CHECK(list.getStoragePtr(3)      == nullptr);
}

void test_cdl_delete_middle()
{
    begin_suite("circulardoublylist: deleteFromPosition middle");
    circulardoublylist<Vec3, int, int, int> list;

    for (int i = 1; i <= 5; i++)
        list.insertAtEnd(i * 10, 0, 0);

    // Delete position 3 (value 30)
    CHECK(list.deleteFromPosition(3) == true);
    CHECK(list.getStoragePtr(1)->x   == 10);
    CHECK(list.getStoragePtr(2)->x   == 20);
    CHECK(list.getStoragePtr(3)->x   == 40);
    CHECK(list.getStoragePtr(4)->x   == 50);
    CHECK(list.getStoragePtr(5)      == nullptr);
}

void test_cdl_delete_all_sequentially()
{
    begin_suite("circulardoublylist: delete all elements one by one");
    circulardoublylist<Vec3, int, int, int> list;

    for (int i = 1; i <= 4; i++)
        list.insertAtEnd(i, 0, 0);

    for (int remaining = 4; remaining >= 1; remaining--)
    {
        CHECK(list.deleteFromPosition(1) == true);
        if (remaining > 1)
            CHECK(list.getStoragePtr(1) != nullptr);
    }

    CHECK(list.getStoragePtr(1)      == nullptr);
    CHECK(list.deleteFromPosition(1) == false);
}

void test_cdl_delete_invalid()
{
    begin_suite("circulardoublylist: deleteFromPosition invalid");
    circulardoublylist<Vec3, int, int, int> list;

    CHECK(list.deleteFromPosition(0)  == false);
    CHECK(list.deleteFromPosition(1)  == false);
    CHECK(list.deleteFromPosition(-1) == false);

    list.insertAtEnd(1, 0, 0);
    CHECK(list.deleteFromPosition(0)  == false);
    CHECK(list.deleteFromPosition(2)  == false);
    CHECK(list.deleteFromPosition(-1) == false);
}

void test_cdl_get_storage_ptr()
{
    begin_suite("circulardoublylist: getStoragePtr");
    circulardoublylist<Vec3, int, int, int> list;

    // Empty
    CHECK(list.getStoragePtr(0) == nullptr);
    CHECK(list.getStoragePtr(1) == nullptr);

    for (int i = 1; i <= 6; i++)
        list.insertAtEnd(i * 100, i * 200, i * 300);

    for (int i = 1; i <= 6; i++)
    {
        Vec3 *sp = list.getStoragePtr(i);
        CHECK(sp        != nullptr);
        CHECK(sp->x     == i * 100);
        CHECK(sp->y     == i * 200);
        CHECK(sp->z     == i * 300);
    }

    CHECK(list.getStoragePtr(0) == nullptr);
    CHECK(list.getStoragePtr(7) == nullptr);
}

void test_cdl_traversal_backward_regression()
{
    begin_suite("circulardoublylist: getNode backward traversal (regression)");
    // With 10 elements, positions 6-9 use the backward path.
    // The old code went back (position-1) steps from tail instead of
    // (list_nodes-position) steps, returning the wrong nodes.
    circulardoublylist<Vec3, int, int, int> list;

    for (int i = 1; i <= 10; i++)
        list.insertAtEnd(i, 0, 0);

    for (int i = 1; i <= 10; i++)
    {
        Vec3 *sp = list.getStoragePtr(i);
        CHECK(sp        != nullptr);
        CHECK(sp->x     == i);
    }
}

void test_cdl_circular_link_integrity()
{
    begin_suite("circulardoublylist: circular link integrity");
    circulardoublylist<Vec3, int, int, int> list;

    auto *n1 = list.insertAtEnd(1, 0, 0);
    auto *n2 = list.insertAtEnd(2, 0, 0);
    auto *n3 = list.insertAtEnd(3, 0, 0);
    auto *n4 = list.insertAtEnd(4, 0, 0);

    // Forward links
    CHECK(n1->n == n2);
    CHECK(n2->n == n3);
    CHECK(n3->n == n4);
    CHECK(n4->n == n1); // tail->n wraps to head

    // Backward links
    CHECK(n1->p == n4); // head->p wraps to tail
    CHECK(n2->p == n1);
    CHECK(n3->p == n2);
    CHECK(n4->p == n3);

    // After deleting head, circular links must remain valid
    CHECK(list.deleteFromPosition(1) == true);
    // new head is n2
    CHECK(n2->p == n4); // new head->p == tail
    CHECK(n4->n == n2); // tail->n == new head
}

void test_cdl_last_accessed()
{
    begin_suite("circulardoublylist: last accessed node tracking");
    circulardoublylist<Vec3, int, int, int> list;

    // Initially null
    CHECK(list.getLastAccessedNode()           == nullptr);
    CHECK(list.getLastAccessedNodeStoragePtr() == nullptr);

    // Updated by insertAtBeginning
    auto *n1 = list.insertAtBeginning(1, 2, 3);
    CHECK(list.getLastAccessedNode()           == n1);
    CHECK(list.getLastAccessedNodeStoragePtr() == n1->sp);

    // Updated by insertAtEnd
    auto *n2 = list.insertAtEnd(4, 5, 6);
    CHECK(list.getLastAccessedNode()           == n2);

    // Updated by insertAtPosition
    auto *n3 = list.insertAtPosition(2, 7, 8, 9);
    CHECK(list.getLastAccessedNode()           == n3);

    // Updated by getStoragePtr
    list.getStoragePtr(1);
    CHECK(list.getLastAccessedNodeStoragePtr() != nullptr);
    CHECK(list.getLastAccessedNodeStoragePtr()->x == 1);

    list.getStoragePtr(2);
    CHECK(list.getLastAccessedNodeStoragePtr()->x == 7);
}

void test_cdl_clear()
{
    begin_suite("circulardoublylist: clear");
    circulardoublylist<Vec3, int, int, int> list;

    // Clear empty list — must not crash
    list.clear();
    CHECK(list.getStoragePtr(1)      == nullptr);

    for (int i = 0; i < 10; i++)
        list.insertAtEnd(i, 0, 0);

    list.clear();
    CHECK(list.getStoragePtr(1)      == nullptr);
    CHECK(list.deleteFromPosition(1) == false);

    // Fully usable after clear
    list.insertAtEnd(7, 0, 0);
    CHECK(list.getStoragePtr(1)->x   == 7);
}

void test_cdl_destructor()
{
    begin_suite("circulardoublylist: destructor (no crash or leak)");
    {
        circulardoublylist<Vec3, int, int, int> list;
        for (int i = 0; i < 20; i++)
            list.insertAtEnd(i, i, i);
    } // destructor called here
    CHECK(true); // reaching here == no crash
}

// ─── main ─────────────────────────────────────────────────────────────────────

int main()
{
    std::cout << "TemplatedLinkedList test suite\n";
    std::cout << "==============================\n";

    // singlylist
    test_singly_empty_list();
    test_singly_insert_beginning();
    test_singly_insert_end();
    test_singly_insert_position();
    test_singly_delete_position();
    test_singly_delete_tail();
    test_singly_remove_at_beginning();
    test_singly_get_node_and_storage();
    test_singly_last_accessed();
    test_singly_clear();
    test_singly_destructor();

    // circulardoublylist
    test_cdl_empty_list();
    test_cdl_insert_beginning();
    test_cdl_insert_end();
    test_cdl_insert_position();
    test_cdl_delete_single_element();
    test_cdl_delete_head();
    test_cdl_delete_tail();
    test_cdl_delete_middle();
    test_cdl_delete_all_sequentially();
    test_cdl_delete_invalid();
    test_cdl_get_storage_ptr();
    test_cdl_traversal_backward_regression();
    test_cdl_circular_link_integrity();
    test_cdl_last_accessed();
    test_cdl_clear();
    test_cdl_destructor();

    std::cout << "\n==============================\n";
    std::cout << "Results: " << g_passed << "/" << g_run << " passed";
    if (g_failed > 0)
        std::cout << "  (" << g_failed << " FAILED)";
    std::cout << "\n";

    return g_failed > 0 ? 1 : 0;
}
