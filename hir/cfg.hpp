//
// Created by dangel on 11.11.2026.
//

#ifndef TOC_HIR_CFG_HPP
#define TOC_HIR_CFG_HPP

#include "hir.hpp"
#include "../tools/graph.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace to_lang::hir {

    // Базовый блок линейного IR
    struct BasicBlock {
        std::string label;
        std::vector<HIROperationPtr> instructions;
        bool is_entry = false;
        bool is_exit = false;

        std::string to_string() const;
    };

    // Граф потока управления функции на базе orgraph_t из Лабораторной №1
    class ControlFlowGraph {
    public:
        using CFGGraph = graph::orgraph_t<BasicBlock, std::string>;
        using node_iterator = typename CFGGraph::node_iterator;
        using const_node_iterator = typename CFGGraph::const_node_iterator;

    private:
        std::string func_name_;
        CFGGraph graph_;
        node_iterator entry_node_;
        std::unordered_map<std::string, node_iterator> label_to_node_;

    public:
        ControlFlowGraph() = default;

        const std::string& get_function_name() const noexcept { return func_name_; }
        CFGGraph& get_graph() noexcept { return graph_; }
        const CFGGraph& get_graph() const noexcept { return graph_; }
        node_iterator get_entry() noexcept { return entry_node_; }

        // Построение CFG из функции линейного IR
        static std::shared_ptr<ControlFlowGraph> build(const HIRFunctionPtr& fn);

        // Экспорт CFG в Graphviz DOT
        std::string to_dot() const;
    };

    using ControlFlowGraphPtr = std::shared_ptr<ControlFlowGraph>;

} // namespace to_lang::hir

#endif // TOC_HIR_CFG_HPP

