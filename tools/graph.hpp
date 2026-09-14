//
// Created by dangel on 14.09.2026.
//

#ifndef TOC_GRAPH_H
#define TOC_GRAPH_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Макрос дебаг
#ifdef DEBUG
#define TOC_ASSERT(cond, msg)                                                                    \
    do {                                                                                         \
        if (!(cond)) {                                                                           \
            throw std::runtime_error(std::string("Assertion failed: ") + (msg) + " (" +         \
                                     __FILE__ + ":" + std::to_string(__LINE__) + ")");           \
        }                                                                                        \
    } while (false)
#else
#define TOC_ASSERT(cond, msg) ((void)0)
#endif

namespace graph {

    // Проверка допустимости типов данных узлов и дуг
    template <typename T>
    struct is_valid_graph_data : std::bool_constant<
        std::is_void_v<T> || (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>)
    > {};
    template <typename T>
    inline constexpr bool is_valid_graph_data_v = is_valid_graph_data<T>::value;





    // Класс ячейки данных
    template <typename T>
    struct memory_cell {
        T data_{};

        memory_cell() = default;

        template <typename First, typename... Rest, typename = std::enable_if_t<
            !std::is_same_v<std::decay_t<First>, memory_cell<T>>
        >>
        explicit memory_cell(First&& first, Rest&&... rest)
            : data_(std::forward<First>(first), std::forward<Rest>(rest)...) {}

        T& get() noexcept { return data_; }
        const T& get() const noexcept { return data_; }

        std::string print() const {
            if constexpr (std::is_convertible_v<T, std::string>) {
                return std::string(data_);
            } else if constexpr (std::is_arithmetic_v<T>) {
                return std::to_string(data_);
            } else {
                return "";
            }
        }
    };
    // void
    template <>
    struct memory_cell<void> {
        memory_cell() = default;
        template <typename... Args>
        explicit memory_cell(Args&&...) {}
    };




    // Класс Графа std::list
    template <
        typename Tnode_data,
        typename Tedge_data,
        typename = std::enable_if_t<
            is_valid_graph_data_v<Tnode_data> && is_valid_graph_data_v<Tedge_data>
        >
    >

    class orgraph_t {
    public:
        class node_t;
        class edge_t;

        using node_list_t = std::list<node_t>;
        using edge_list_t = std::list<edge_t>;

        using node_iterator       = typename node_list_t::iterator;
        using const_node_iterator = typename node_list_t::const_iterator;

        using edge_iterator       = typename edge_list_t::iterator;
        using const_edge_iterator = typename edge_list_t::const_iterator;

    protected:
        node_list_t nodes_;
        edge_list_t edges_;

        // Проверка базовых связей в графе
        void check_graph_invariants() const {
#ifdef DEBUG
            for (auto edge_it = edges_.begin(); edge_it != edges_.end(); ++edge_it) {
                TOC_ASSERT(edge_it->from_ != nodes_.end(), "Edge from is end iterator");
                TOC_ASSERT(edge_it->to_ != nodes_.end(), "Edge to is end iterator");

                const auto& out_list = edge_it->from_->edgesOUT_;
                TOC_ASSERT(std::find(out_list.begin(), out_list.end(), edge_it) != out_list.end(),
                           "Edge not found in pred out_edges");

                const auto& in_list = edge_it->to_->edgesIN_;
                TOC_ASSERT(std::find(in_list.begin(), in_list.end(), edge_it) != in_list.end(),
                           "Edge not found in succ in_edges");
            }

            for (auto node_it = nodes_.begin(); node_it != nodes_.end(); ++node_it) {
                for (auto e_it : node_it->edgesIN_) {
                    TOC_ASSERT(e_it->to_ == node_it, "Incoming edge destination mismatch");
                }
                for (auto e_it : node_it->edgesOUT_) {
                    TOC_ASSERT(e_it->from_ == node_it, "Outgoing edge source mismatch");
                }
            }
#endif
        }

    public:
        virtual ~orgraph_t() = default;

        orgraph_t() = default;
        orgraph_t(const orgraph_t&) = delete;
        orgraph_t& operator=(const orgraph_t&) = delete;
        orgraph_t(orgraph_t&&) = default;
        orgraph_t& operator=(orgraph_t&&) = default;

        // Итераторы узлов
        node_iterator nodes_begin() noexcept { return nodes_.begin(); }
        node_iterator nodes_end() noexcept { return nodes_.end(); }
        const_node_iterator nodes_begin() const noexcept { return nodes_.begin(); }
        const_node_iterator nodes_end() const noexcept { return nodes_.end(); }

        // Итераторы дуг
        edge_iterator edges_begin() noexcept { return edges_.begin(); }
        edge_iterator edges_end() noexcept { return edges_.end(); }
        const_edge_iterator edges_begin() const noexcept { return edges_.begin(); }
        const_edge_iterator edges_end() const noexcept { return edges_.end(); }

        size_t nodes_count() const noexcept { return nodes_.size(); }
        size_t edges_count() const noexcept { return edges_.size(); }
        bool empty() const noexcept { return nodes_.empty(); }


        // void
        node_iterator add_node() {
            nodes_.emplace_back();
            auto it = std::prev(nodes_.end());
            check_graph_invariants();
            return it;
        }
        template <
            typename... Args,
            typename = std::enable_if_t<!std::is_void_v<Tnode_data> && (sizeof...(Args) > 0)>
        >
        node_iterator add_node(Args&&... args) {
            nodes_.emplace_back(std::forward<Args>(args)...);
            auto it = std::prev(nodes_.end());
            check_graph_invariants();
            return it;
        }


        // void
        edge_iterator add_edge(node_iterator from, node_iterator to) {
            if (from == nodes_.end() || to == nodes_.end()) {
                throw std::runtime_error("invalid node iterator");
            }

            edges_.emplace_back(from, to);
            auto edge = std::prev(edges_.end());

            to->edgesIN_.emplace_back(edge);
            from->edgesOUT_.emplace_back(edge);

            check_graph_invariants();
            return edge;
        }

        template <
            typename... Args,
            typename = std::enable_if_t<!std::is_void_v<Tedge_data> && (sizeof...(Args) > 0) >
        >
        edge_iterator add_edge(node_iterator from, node_iterator to, Args&&... args) {
            if (from == nodes_.end() || to == nodes_.end()) {
                throw std::runtime_error("invalid node iterator");
            }

            edges_.emplace_back(from, to, std::forward<Args>(args)...);
            auto edge = std::prev(edges_.end());

            to->edgesIN_.emplace_back(edge);
            from->edgesOUT_.emplace_back(edge);

            check_graph_invariants();
            return edge;
        }




        void delete_node(node_iterator node) {
            if (node == nodes_.end()) {
                throw std::runtime_error("invalid node iterator");
            }

            while (!node->edgesOUT_.empty()) {
                delete_edge(node->edgesOUT_.front());
            }
            while (!node->edgesIN_.empty()) {
                delete_edge(node->edgesIN_.front());
            }

            nodes_.erase(node);
            check_graph_invariants();
        }

        // Удаление одной дуги
        void delete_edge(edge_iterator edge) {
            if (edge == edges_.end()) {
                throw std::runtime_error("invalid edge iterator");
            }

            auto& out_list = edge->from_->edgesOUT_;
            auto& to_list  = edge->to_->edgesIN_;

            auto o = std::find(out_list.begin(), out_list.end(), edge);
            if (o != out_list.end()) {
                out_list.erase(o);
            }
            auto i = std::find(to_list.begin(), to_list.end(), edge);
            if (i != to_list.end()) {
                to_list.erase(i);
            }

            edges_.erase(edge);
            check_graph_invariants();
        }

        // Замена узла-предка
        void set_from(edge_iterator edge, node_iterator new_from) {
            if (new_from == nodes_.end() || edge == edges_.end()) {
                throw std::runtime_error("invalid node or edge iterators");
            }

            auto& out_list = edge->from_->edgesOUT_;
            auto o = std::find(out_list.begin(), out_list.end(), edge);
            if (o != out_list.end()) {
                out_list.erase(o);
            }

            new_from->edgesOUT_.emplace_back(edge);
            edge->from_ = new_from;
            check_graph_invariants();
        }

        // Замена узла-потомка
        void set_to(edge_iterator edge, node_iterator new_to) {
            if (new_to == nodes_.end() || edge == edges_.end()) {
                throw std::runtime_error("invalid node or edge iterators");
            }

            auto& to_list = edge->to_->edgesIN_;
            auto i = std::find(to_list.begin(), to_list.end(), edge);
            if (i != to_list.end()) {
                to_list.erase(i);
            }

            new_to->edgesIN_.emplace_back(edge);
            edge->to_ = new_to;
            check_graph_invariants();
        }


        // Проверка
        virtual void check() const {
            check_graph_invariants();
        }

        // Проверка без исключений
        bool validate() const noexcept {
            try {
                check();
                return true;
            } catch (...) {
                return false;
            }
        }

        // DOT
        template <typename NodeNameFn, typename EdgeNameFn>
        void write_dot(std::ostream& os, NodeNameFn node_name_fn, EdgeNameFn edge_name_fn) const {
            os << "digraph G {\n";

            std::unordered_map<const node_t*, size_t> node_indices;
            node_indices.reserve(nodes_.size());

            size_t idx = 0;
            for (auto it = nodes_.begin(); it != nodes_.end(); ++it, ++idx) {
                node_indices[&(*it)] = idx;
                std::string id = "n" + std::to_string(idx);

                std::string label;
                if constexpr (!std::is_void_v<Tnode_data>) {
                    if constexpr (std::is_invocable_v<NodeNameFn, const Tnode_data&>) {
                        label = node_name_fn(it->get());
                    } else if constexpr (std::is_invocable_v<NodeNameFn>) {
                        label = node_name_fn();
                    }
                } else {
                    if constexpr (std::is_invocable_v<NodeNameFn>) {
                        label = node_name_fn();
                    } else {
                        label = id;
                    }
                }

                os << "    " << id << " [label=\"" << label << "\"];\n";
            }

            for (auto it = edges_.begin(); it != edges_.end(); ++it) {
                auto from_it = node_indices.find(&(*(it->from())));
                auto to_it   = node_indices.find(&(*(it->to())));

                if (from_it != node_indices.end() && to_it != node_indices.end()) {
                    os << "    n" << from_it->second << " -> n" << to_it->second;

                    std::string label;
                    if constexpr (!std::is_void_v<Tedge_data>) {
                        if constexpr (std::is_invocable_v<EdgeNameFn, const Tedge_data&>) {
                            label = edge_name_fn(it->get());
                        } else if constexpr (std::is_invocable_v<EdgeNameFn>) {
                            label = edge_name_fn();
                        }
                    } else {
                        if constexpr (std::is_invocable_v<EdgeNameFn>) {
                            label = edge_name_fn();
                        }
                    }

                    if (!label.empty()) {
                        os << " [label=\"" << label << "\"]";
                    }
                    os << ";\n";
                }
            }

            os << "}\n";
        }

        // Выгрузка в DOT строку
        std::string to_dot() const {
            std::ostringstream ss;
            auto default_node_fn = [](auto&&... args) -> std::string {
                if constexpr (sizeof...(args) == 1) {
                    auto&& data = (args, ...);
                    if constexpr (std::is_convertible_v<std::decay_t<decltype(data)>, std::string>) {
                        return std::string(data);
                    } else if constexpr (std::is_arithmetic_v<std::decay_t<decltype(data)>>) {
                        return std::to_string(data);
                    }
                }
                return "";
            };

            auto default_edge_fn = [](auto&&... args) -> std::string {
                if constexpr (sizeof...(args) == 1) {
                    auto&& data = (args, ...);
                    if constexpr (std::is_convertible_v<std::decay_t<decltype(data)>, std::string>) {
                        return std::string(data);
                    } else if constexpr (std::is_arithmetic_v<std::decay_t<decltype(data)>>) {
                        return std::to_string(data);
                    }
                }
                return "";
            };

            write_dot(ss, default_node_fn, default_edge_fn);
            return ss.str();
        }

        // Поиск узла по предикату
        template <typename Predicate>
        node_iterator find_node_if(Predicate pred) {
            for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
                if constexpr (std::is_invocable_r_v<bool, Predicate, const node_t&>) {
                    if (pred(*it)) return it;
                } else if constexpr (!std::is_void_v<Tnode_data> && std::is_invocable_r_v<bool, Predicate, const Tnode_data&>) {
                    if (pred(it->get())) return it;
                }
            }
            return nodes_.end();
        }

        template <typename Predicate>
        const_node_iterator find_node_if(Predicate pred) const {
            for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
                if constexpr (std::is_invocable_r_v<bool, Predicate, const node_t&>) {
                    if (pred(*it)) return it;
                } else if constexpr (!std::is_void_v<Tnode_data> && std::is_invocable_r_v<bool, Predicate, const Tnode_data&>) {
                    if (pred(it->get())) return it;
                }
            }
            return nodes_.end();
        }

        // Поиск дуги по предикату
        template <typename Predicate>
        edge_iterator find_edge_if(Predicate pred) {
            for (auto it = edges_.begin(); it != edges_.end(); ++it) {
                if constexpr (std::is_invocable_r_v<bool, Predicate, const edge_t&>) {
                    if (pred(*it)) return it;
                } else if constexpr (!std::is_void_v<Tedge_data> && std::is_invocable_r_v<bool, Predicate, const Tedge_data&>) {
                    if (pred(it->get())) return it;
                }
            }
            return edges_.end();
        }

        template <typename Predicate>
        const_edge_iterator find_edge_if(Predicate pred) const {
            for (auto it = edges_.begin(); it != edges_.end(); ++it) {
                if constexpr (std::is_invocable_r_v<bool, Predicate, const edge_t&>) {
                    if (pred(*it)) return it;
                } else if constexpr (!std::is_void_v<Tedge_data> && std::is_invocable_r_v<bool, Predicate, const Tedge_data&>) {
                    if (pred(it->get())) return it;
                }
            }
            return edges_.end();
        }

        // Обход графа в глубину
        template <typename VisitorFn>
        void dfs(node_iterator start, VisitorFn visitor) {
            if (start == nodes_.end()) return;
            std::unordered_set<const node_t*> visited;
            dfs_impl(start, visitor, visited);
        }

        template <typename VisitorFn>
        void dfs(const_node_iterator start, VisitorFn visitor) const {
            if (start == nodes_.end()) return;
            std::unordered_set<const node_t*> visited;
            dfs_impl(start, visitor, visited);
        }

    private:
        template <typename Iter, typename VisitorFn>
        void dfs_impl(Iter current, VisitorFn& visitor, std::unordered_set<const node_t*>& visited) const {
            visited.insert(&(*current));
            visitor(current);

            for (auto edge_it : current->get_out_edges()) {
                auto succ_it = edge_it->to();
                if (visited.find(&(*succ_it)) == visited.end()) {
                    dfs_impl(succ_it, visitor, visited);
                }
            }
        }
    };

    // Класс узла node_t
    template < typename Tnode_data, typename Tedge_data, typename SFINAE >
    class orgraph_t <Tnode_data, Tedge_data, SFINAE>::node_t {
        friend class orgraph_t;
    private:
        memory_cell<Tnode_data> data_;
        std::list<edge_iterator> edgesIN_;
        std::list<edge_iterator> edgesOUT_;

    public:
        node_t() = default;

        template <typename... Args, typename = std::enable_if_t<
            !std::is_void_v<Tnode_data> && (sizeof...(Args) > 0)
        >>
        explicit node_t(Args&&... args) : data_(std::forward<Args>(args)...) {}

        // Пользовательские данные
        template <typename U = Tnode_data>
        std::enable_if_t<!std::is_void_v<U>, U&> get() noexcept { return data_.get(); }

        template <typename U = Tnode_data>
        std::enable_if_t<!std::is_void_v<U>, const U&> get() const noexcept { return data_.get(); }

        template <typename U = Tnode_data>
        std::enable_if_t<!std::is_void_v<U>, U&> data() noexcept { return data_.get(); }

        template <typename U = Tnode_data>
        std::enable_if_t<!std::is_void_v<U>, const U&> data() const noexcept { return data_.get(); }

        // Списки дуг
        const std::list<edge_iterator>& get_in_edges() const noexcept { return edgesIN_; }
        const std::list<edge_iterator>& get_out_edges() const noexcept { return edgesOUT_; }

        const std::list<edge_iterator>& in_edges() const noexcept { return edgesIN_; }
        const std::list<edge_iterator>& out_edges() const noexcept { return edgesOUT_; }
    };

    // Класс дуги edge_t
    template < typename Tnode_data, typename Tedge_data, typename SFINAE >
    class orgraph_t <Tnode_data, Tedge_data, SFINAE>::edge_t {
        friend class orgraph_t;
    private:
        node_iterator from_;
        node_iterator to_;
        memory_cell<Tedge_data> data_;

    public:
        edge_t(node_iterator from, node_iterator to) : from_(from), to_(to) {}

        template <typename... Args, typename = std::enable_if_t<
            !std::is_void_v<Tedge_data> && (sizeof...(Args) > 0)
        >>
        explicit edge_t(node_iterator from, node_iterator to, Args&&... args)
            : from_(from), to_(to), data_(std::forward<Args>(args)...) {}

        // Пользовательские данные
        template <typename U = Tedge_data>
        std::enable_if_t<!std::is_void_v<U>, U&> get() noexcept { return data_.get(); }

        template <typename U = Tedge_data>
        std::enable_if_t<!std::is_void_v<U>, const U&> get() const noexcept { return data_.get(); }

        template <typename U = Tedge_data>
        std::enable_if_t<!std::is_void_v<U>, U&> data() noexcept { return data_.get(); }

        template <typename U = Tedge_data>
        std::enable_if_t<!std::is_void_v<U>, const U&> data() const noexcept { return data_.get(); }

        // Узлы дуги
        node_iterator from() const noexcept { return from_; }
        node_iterator to() const noexcept { return to_; }

        node_iterator pred() const noexcept { return from_; }
        node_iterator succ() const noexcept { return to_; }
        node_iterator prev() const noexcept { return from_; }
    };

} // namespace graph

#endif // TOC_GRAPH_H
