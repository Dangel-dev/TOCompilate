#include "tree.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

void test_tree_construction_and_root() {
    tree::tree_t<int> t;
    assert(t.empty());
    assert(t.root() == t.nodes_end());

    auto r = t.create_root(10);
    assert(t.root() == r);
    assert(r->get() == 10);

    auto c1 = t.add_child(r, 20);
    auto c2 = t.add_child(r, 30);
    auto c1_1 = t.add_child(c1, 40);

    t.check(); // Не должно бросать исключений

    assert(t.nodes_count() == 4);
    assert(c1->get_in_edges().size() == 1);
    assert(c2->get_in_edges().size() == 1);
    assert(c1_1->get_in_edges().size() == 1);
    assert(r->get_in_edges().empty());
}

void test_tree_manual_valid() {
    tree::tree_t<std::string> t;
    auto n1 = t.add_node("Root");
    auto n2 = t.add_node("Child1");
    auto n3 = t.add_node("Child2");

    t.add_edge(n1, n2);
    t.add_edge(n1, n3);

    t.check();
    assert(t.root() == n1);
}

void test_tree_cycle_detection() {
    tree::tree_t<int> t;
    auto n1 = t.add_node(1);
    auto n2 = t.add_node(2);
    auto n3 = t.add_node(3);

    t.add_edge(n1, n2);
    t.add_edge(n2, n3);
    t.add_edge(n3, n1); // Замыкаем цикл 1 -> 2 -> 3 -> 1

    bool caught = false;
    try {
        t.check();
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);
}

void test_tree_multiple_roots() {
    tree::tree_t<int> t;
    t.add_node(1);
    t.add_node(2);

    bool caught = false;
    try {
        t.check();
    } catch (const std::runtime_error&) {
        caught = true;
    }
    assert(caught);
}

void test_tree_multiple_incoming_edges() {
    tree::tree_t<int> t;
    auto r = t.create_root(1);
    auto c1 = t.add_child(r, 2);
    auto c2 = t.add_child(r, 3);

    // Добавляем вторую входящую дугу в c2 (от c1)
    t.add_edge(c1, c2);

    bool caught = false;
    try {
        t.check();
    } catch (const std::runtime_error&) {
        caught = true;
    }
    assert(caught);
}

void test_tree_disconnected() {
    tree::tree_t<int> t;
    auto r = t.create_root(1);
    t.add_child(r, 2);

    // Изолированный остров из двух узлов с циклом
    auto x1 = t.add_node(10);
    auto x2 = t.add_node(20);
    t.add_edge(x1, x2);
    t.add_edge(x2, x1);

    bool caught = false;
    try {
        t.check();
    } catch (const std::runtime_error&) {
        caught = true;
    }
    assert(caught);
}

void test_tree_deletion_and_recheck() {
    tree::tree_t<int> t;
    auto r = t.create_root(1);
    auto c1 = t.add_child(r, 2);
    auto c2 = t.add_child(r, 3);
    auto c1_sub = t.add_child(c1, 4);

    t.check();
    assert(t.nodes_count() == 4);

    // Удаляем листовой узел c1_sub
    t.delete_node(c1_sub);
    assert(t.nodes_count() == 3);
    t.check();

    // Удаляем c2
    t.delete_node(c2);
    assert(t.nodes_count() == 2);
    t.check();
    assert(t.root() == r);
}

void test_tree_deep_hierarchy() {
    tree::tree_t<int> t;
    auto curr = t.create_root(0);
    for (int i = 1; i <= 25; ++i) {
        curr = t.add_child(curr, i);
    }
    assert(t.nodes_count() == 26);
    t.check();
    assert(t.root()->get() == 0);
    assert(curr->get() == 25);
}

void test_tree_const_access() {
    tree::tree_t<std::string> t;
    auto r = t.create_root("Root");
    t.add_child(r, "Child");

    const auto& ct = t;
    assert(ct.nodes_count() == 2);
    assert(!ct.empty());
    assert(ct.root()->get() == "Root");
    ct.check();
}

void test_tree_two_node_cycle() {
    tree::tree_t<int> t;
    auto n1 = t.add_node(1);
    auto n2 = t.add_node(2);
    t.add_edge(n1, n2);
    t.add_edge(n2, n1); // Двухузловой взаимный цикл

    bool caught = false;
    try {
        t.check();
    } catch (const std::runtime_error&) {
        caught = true;
    }
    assert(caught);
}

int main() {
    test_tree_construction_and_root();
    test_tree_manual_valid();
    test_tree_cycle_detection();
    test_tree_two_node_cycle();
    test_tree_multiple_roots();
    test_tree_multiple_incoming_edges();
    test_tree_disconnected();
    test_tree_deletion_and_recheck();
    test_tree_deep_hierarchy();
    test_tree_const_access();

    std::cout << "All tree tests successfully passed!" << std::endl;
    return 0;
}
