#include "graph.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ComplexData {
    int id;
    std::string name;
    double weight;

    ComplexData(int i, std::string n, double w)
        : id(i), name(std::move(n)), weight(w) {}
};

void test_basic() {
    graph::orgraph_t<std::string, int> g;
    auto n1 = g.add_node("Node 1");
    auto n2 = g.add_node("Node 2");

    assert(n1->get() == "Node 1");
    assert(n2->data() == "Node 2"); // проверка синонима data()

    auto e1 = g.add_edge(n1, n2, 100);
    assert(e1->get() == 100);
    assert(e1->from() == n1);
    assert(e1->to() == n2);
    assert(e1->pred() == n1);
    assert(e1->succ() == n2);

    assert(n1->get_out_edges().size() == 1);
    assert(n1->get_in_edges().empty());
    assert(n2->get_in_edges().size() == 1);
    assert(n2->get_out_edges().empty());

    assert(g.nodes_count() == 2);
    assert(g.edges_count() == 1);
}

void test_variadic_emplace() {
    graph::orgraph_t<ComplexData, ComplexData> g;

    // Создание узлов с передачей 3 аргументов конструктора напрямую
    auto n1 = g.add_node(1, "Alpha", 1.5);
    auto n2 = g.add_node(2, "Beta", 2.5);

    assert(n1->get().id == 1);
    assert(n1->get().name == "Alpha");
    assert(n1->get().weight == 1.5);

    auto e = g.add_edge(n1, n2, 99, "EdgeAlphaBeta", 0.75);
    assert(e->get().id == 99);
    assert(e->get().name == "EdgeAlphaBeta");
}

void test_void_types() {
    graph::orgraph_t<void, void> g;
    auto n1 = g.add_node();
    auto n2 = g.add_node();
    auto e1 = g.add_edge(n1, n2);

    assert(n1->get_out_edges().size() == 1);
    assert(n2->get_in_edges().size() == 1);
    assert(e1->from() == n1);
    assert(e1->to() == n2);

    graph::orgraph_t<int, void> g_node_only;
    auto na = g_node_only.add_node(42);
    auto nb = g_node_only.add_node(84);
    auto e_nv = g_node_only.add_edge(na, nb);
    assert(na->get() == 42);
    assert(e_nv->from() == na);
}

void test_replace() {
    graph::orgraph_t<int, int> g;
    auto n1 = g.add_node(1);
    auto n2 = g.add_node(2);
    auto n3 = g.add_node(3);

    auto e = g.add_edge(n1, n2, 10);

    // Замена потомка (n2 -> n3)
    g.set_to(e, n3);
    assert(e->to() == n3);
    assert(n2->get_in_edges().empty());
    assert(n3->get_in_edges().size() == 1);

    // Замена предка (n1 -> n2)
    g.set_from(e, n2);
    assert(e->from() == n2);
    assert(n1->get_out_edges().empty());
    assert(n2->get_out_edges().size() == 1);
}

void test_delete_and_iterator_stability() {
    graph::orgraph_t<int, int> g;
    auto n1 = g.add_node(1);
    auto n2 = g.add_node(2);
    auto n3 = g.add_node(3);

    auto e12 = g.add_edge(n1, n2, 12);
    auto e23 = g.add_edge(n2, n3, 23);

    // Удаление дуги
    g.delete_edge(e12);
    assert(n1->get_out_edges().empty());
    assert(n2->get_in_edges().empty());

    // Итератор n2 и e23 должны оставаться валидными
    assert(n2->get() == 2);
    assert(e23->get() == 23);

    // Удаление узла n2 должно удалить инцидентную дугу e23
    g.delete_node(n2);
    assert(g.nodes_count() == 2);
    assert(g.edges_count() == 0);
    assert(n3->get_in_edges().empty());
}

void test_find() {
    graph::orgraph_t<std::string, int> g;
    auto n1 = g.add_node("Apple");
    auto n2 = g.add_node("Banana");
    auto n3 = g.add_node("Cherry");
    auto e12 = g.add_edge(n1, n2, 100);
    auto e23 = g.add_edge(n2, n3, 200);
    assert(e12 != g.edges_end() && e23 != g.edges_end());

    // Поиск узла
    auto found_node = g.find_node_if([](const std::string& name) {
        return name == "Cherry";
    });
    assert(found_node != g.nodes_end());
    assert(found_node == n3);

    auto not_found_node = g.find_node_if([](const std::string& name) {
        return name == "Orange";
    });
    assert(not_found_node == g.nodes_end());

    // Поиск дуги
    auto found_edge = g.find_edge_if([](int weight) {
        return weight == 200;
    });
    assert(found_edge != g.edges_end());
    assert(found_edge == e23);

    auto not_found_edge = g.find_edge_if([](int weight) {
        return weight == 999;
    });
    assert(not_found_edge == g.edges_end());
}

void test_loops_and_parallel_edges() {
    graph::orgraph_t<int, std::string> g;
    auto n1 = g.add_node(1);
    auto n2 = g.add_node(2);

    // Петля: 1 -> 1
    auto e_loop = g.add_edge(n1, n1, "loop");
    assert(e_loop->from() == n1);
    assert(e_loop->to() == n1);
    assert(n1->get_in_edges().size() == 1);
    assert(n1->get_out_edges().size() == 1);

    // Параллельные дуги: 1 -> 2 (дуга A) и 1 -> 2 (дуга B)
    auto e_par1 = g.add_edge(n1, n2, "par1");
    auto e_par2 = g.add_edge(n1, n2, "par2");
    assert(e_par1 != g.edges_end() && e_par2 != g.edges_end());

    assert(g.edges_count() == 3);
    assert(n1->get_out_edges().size() == 3);
    assert(n2->get_in_edges().size() == 2);

    g.check(); // валидация инвариантов

    g.delete_edge(e_loop);
    assert(g.edges_count() == 2);
    assert(n1->get_in_edges().empty());
}

void test_dfs() {
    graph::orgraph_t<int, void> g;
    auto n1 = g.add_node(1);
    auto n2 = g.add_node(2);
    auto n3 = g.add_node(3);
    auto n4 = g.add_node(4);

    g.add_edge(n1, n2);
    g.add_edge(n2, n3);
    g.add_edge(n3, n1); // цикл 1 -> 2 -> 3 -> 1
    g.add_edge(n2, n4); // ответвление в 4

    std::vector<int> visited_order;
    g.dfs(n1, [&visited_order](auto it) {
        visited_order.push_back(it->get());
    });

    // Все 4 узла должны быть посещены ровно по 1 разу
    assert(visited_order.size() == 4);
    assert(visited_order[0] == 1);
}

void test_dot_export() {
    graph::orgraph_t<std::string, int> g;
    auto n1 = g.add_node("Src");
    auto n2 = g.add_node("Dst");
    g.add_edge(n1, n2, 777);

    std::ostringstream ss;
    g.write_dot(ss,
        [](const std::string& s) { return "Node: " + s; },
        [](int weight) { return "Cost: " + std::to_string(weight); }
    );

    std::string dot = ss.str();
    assert(dot.find("label=\"Node: Src\"") != std::string::npos);
    assert(dot.find("label=\"Node: Dst\"") != std::string::npos);
    assert(dot.find("label=\"Cost: 777\"") != std::string::npos);

    // Проверка to_dot() по умолчанию
    std::string default_dot = g.to_dot();
    assert(!default_dot.empty());
    assert(default_dot.find("digraph G") != std::string::npos);
}

void test_const_traversal() {
    graph::orgraph_t<std::string, int> g;
    auto n1 = g.add_node("Alpha");
    auto n2 = g.add_node("Beta");
    auto e1 = g.add_edge(n1, n2, 42);
    (void)e1;

    // Проверка работы с константной ссылкой на граф
    const auto& const_g = g;
    assert(const_g.nodes_count() == 2);
    assert(const_g.edges_count() == 1);

    int count = 0;
    for (auto it = const_g.nodes_begin(); it != const_g.nodes_end(); ++it) {
        assert(!it->get().empty());
        count++;
    }
    assert(count == 2);

    for (auto it = const_g.edges_begin(); it != const_g.edges_end(); ++it) {
        assert(it->get() == 42);
        assert(it->from()->get() == "Alpha");
        assert(it->to()->get() == "Beta");
    }

    auto found = const_g.find_node_if([](const std::string& s) { return s == "Beta"; });
    assert(found != const_g.nodes_end());
    assert(found->get() == "Beta");
}

void test_in_place_modification() {
    graph::orgraph_t<int, std::string> g;
    auto n1 = g.add_node(10);
    auto n2 = g.add_node(20);
    auto e = g.add_edge(n1, n2, "old_edge");

    // Модификация через get()
    n1->get() = 100;
    n2->get() = 200;
    e->get() = "new_edge";

    assert(n1->get() == 100);
    assert(n2->get() == 200);
    assert(e->get() == "new_edge");
}

void test_sfinae_type_traits() {
    static_assert(graph::is_valid_graph_data_v<int>);
    static_assert(graph::is_valid_graph_data_v<std::string>);
    static_assert(graph::is_valid_graph_data_v<void>);
    static_assert(graph::is_valid_graph_data_v<ComplexData>);

    struct NonCopyable {
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
    static_assert(!graph::is_valid_graph_data_v<NonCopyable>);
}

int main() {
    test_basic();
    test_variadic_emplace();
    test_void_types();
    test_replace();
    test_delete_and_iterator_stability();
    test_find();
    test_loops_and_parallel_edges();
    test_dfs();
    test_dot_export();
    test_const_traversal();
    test_in_place_modification();
    test_sfinae_type_traits();

    std::cout << "All graph tests successfully passed!" << std::endl;
    return 0;
}
