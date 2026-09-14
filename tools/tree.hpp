//
// Created by dangel on 14.09.2026.
//

#ifndef TOC_TREE_HPP
#define TOC_TREE_HPP

#include "graph.hpp"
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <utility>

namespace tree {
    template <typename Tnode_data, typename Tedge_data = void>
    class tree_t : public graph::orgraph_t<Tnode_data, Tedge_data> {
    public:
        using base_t              = graph::orgraph_t<Tnode_data, Tedge_data>;
        using node_iterator       = typename base_t::node_iterator;
        using const_node_iterator = typename base_t::const_node_iterator;
        using edge_iterator       = typename base_t::edge_iterator;
        using const_edge_iterator = typename base_t::const_edge_iterator;

        tree_t() = default;

        // Корень дерева
        node_iterator root() noexcept {
            for (auto it = this->nodes_begin(); it != this->nodes_end(); ++it) {
                if (it->get_in_edges().empty()) {
                    return it;
                }
            }
            return this->nodes_end();
        }

        const_node_iterator root() const noexcept {
            for (auto it = this->nodes_begin(); it != this->nodes_end(); ++it) {
                if (it->get_in_edges().empty()) {
                    return it;
                }
            }
            return this->nodes_end();
        }

        // Создание корня
        template <typename... Args>
        node_iterator create_root(Args&&... args) {
            if (root() != this->nodes_end()) {
                throw std::runtime_error("Root already exists in tree");
            }
            return this->add_node(std::forward<Args>(args)...);
        }

        // Добавление дочернего узла
        template <typename... Args>
        node_iterator add_child(node_iterator parent, Args&&... args) {
            if (parent == this->nodes_end()) {
                throw std::runtime_error("Invalid parent node iterator");
            }

            auto child = this->add_node(std::forward<Args>(args)...);
            this->add_edge(parent, child);
            return child;
        }

        // Проверка дерева на ацикличность и структуру
        void check() const override {
#ifdef DEBUG
            base_t::check();

            if (this->empty()) {
                return;
            }

            size_t root_count = 0;
            const_node_iterator root_it = this->nodes_end();

            for (auto it = this->nodes_begin(); it != this->nodes_end(); ++it) {
                size_t in_deg = it->get_in_edges().size();
                if (in_deg == 0) {
                    ++root_count;
                    root_it = it;
                } else if (in_deg > 1) {
                    throw std::runtime_error("Tree check failed: Node has more than one incoming edge");
                }
            }

            if (root_count == 0) {
                throw std::runtime_error("Tree check failed: Tree has cycle and no root");
            }
            if (root_count > 1) {
                throw std::runtime_error("Tree check failed: Multiple roots found in tree");
            }

            std::unordered_set<const void*> visited;
            std::queue<const_node_iterator> q;

            q.push(root_it);
            visited.insert(&(*root_it));

            while (!q.empty()) {
                auto curr = q.front();
                q.pop();

                for (auto edge_it : curr->get_out_edges()) {
                    auto succ_it = edge_it->to();
                    if (visited.find(&(*succ_it)) != visited.end()) {
                        throw std::runtime_error("Tree check failed: Cycle detected in tree");
                    }
                    visited.insert(&(*succ_it));
                    q.push(succ_it);
                }
            }

            if (visited.size() != this->nodes_count()) {
                throw std::runtime_error("Tree check failed: Disconnected nodes found in tree");
            }
#endif
        }
    };

} // namespace tree

#endif // TOC_TREE_HPP
