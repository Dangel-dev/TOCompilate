//
// Created by dangel on 11.11.2026.
//

#include "cfg.hpp"
#include <iomanip>
#include <sstream>

namespace to_lang::hir {

    std::string BasicBlock::to_string() const {
        std::ostringstream ss;
        ss << label << ":\n";
        for (const auto& op : instructions) {
            if (op && op->opcode != Opcode::LABEL) {
                ss << "  " << op->to_string() << "\n";
            }
        }
        return ss.str();
    }

    std::shared_ptr<ControlFlowGraph> ControlFlowGraph::build(const HIRFunctionPtr& fn) {
        auto cfg = std::make_shared<ControlFlowGraph>();
        if (!fn) return cfg;

        cfg->func_name_ = fn->name;

        // 1. Нормализация потока инструкций: вставка меток в начало и после терминаторов
        std::vector<HIROperationPtr> normalized_ops;
        size_t auto_lbl_cnt = 0;
        auto make_unique_lbl = [&](const std::string& pfx) {
            return pfx + "_" + std::to_string(auto_lbl_cnt++);
        };

        if (fn->operations.empty() || fn->operations.front()->opcode != Opcode::LABEL) {
            normalized_ops.push_back(HIROperation::make_label(make_unique_lbl("entry")));
        }

        for (size_t i = 0; i < fn->operations.size(); ++i) {
            auto op = fn->operations[i];
            normalized_ops.push_back(op);

            if (op->is_terminator()) {
                // Если после терминатора идут инструкции, и следующая не метка — вставляем метку
                if (i + 1 < fn->operations.size() && fn->operations[i + 1]->opcode != Opcode::LABEL) {
                    normalized_ops.push_back(HIROperation::make_label(make_unique_lbl("split")));
                }
            }
        }

        // 2. Нарезка на базовые блоки по меткам и переходам
        std::vector<BasicBlock> raw_blocks;
        BasicBlock current_block;
        bool in_block = false;

        for (const auto& op : normalized_ops) {
            if (op->opcode == Opcode::LABEL) {
                if (in_block) {
                    raw_blocks.push_back(std::move(current_block));
                    current_block = BasicBlock{};
                }
                current_block.label = op->label_target;
                current_block.instructions.push_back(op);
                in_block = true;
            } else {
                if (!in_block) {
                    current_block.label = make_unique_lbl("bb");
                    in_block = true;
                }
                current_block.instructions.push_back(op);
                if (op->is_terminator()) {
                    raw_blocks.push_back(std::move(current_block));
                    current_block = BasicBlock{};
                    in_block = false;
                }
            }
        }

        if (in_block && !current_block.instructions.empty()) {
            raw_blocks.push_back(std::move(current_block));
        }

        if (raw_blocks.empty()) return cfg;

        // Помечаем входной блок
        raw_blocks.front().is_entry = true;

        // 3. Создание узлов графа
        std::vector<node_iterator> node_iters;
        for (auto& bb : raw_blocks) {
            auto it = cfg->graph_.add_node(std::move(bb));
            node_iters.push_back(it);
            cfg->label_to_node_[it->get().label] = it;
        }

        cfg->entry_node_ = node_iters.front();

        // 4. Построение ориентированных дуг между базовыми блоками
        for (size_t i = 0; i < node_iters.size(); ++i) {
            auto from_it = node_iters[i];
            const auto& bb = from_it->get();
            if (bb.instructions.empty()) continue;

            auto last_op = bb.instructions.back();

            if (last_op->opcode == Opcode::JUMP) {
                auto target_it = cfg->label_to_node_.find(last_op->label_target);
                if (target_it != cfg->label_to_node_.end()) {
                    cfg->graph_.add_edge(from_it, target_it->second, "");
                }
            } else if (last_op->opcode == Opcode::BRANCH) {
                auto true_it = cfg->label_to_node_.find(last_op->label_target);
                if (true_it != cfg->label_to_node_.end()) {
                    cfg->graph_.add_edge(from_it, true_it->second, "true");
                }
                auto false_it = cfg->label_to_node_.find(last_op->label_false);
                if (false_it != cfg->label_to_node_.end()) {
                    cfg->graph_.add_edge(from_it, false_it->second, "false");
                }
            } else if (last_op->opcode == Opcode::RETURN) {
                // Выходной узел, дуг управления наружу нет
                from_it->get().is_exit = true;
            } else {
                // Fallthrough переход в следующий блок
                if (i + 1 < node_iters.size()) {
                    cfg->graph_.add_edge(from_it, node_iters[i + 1], "fallthrough");
                }
            }
        }

        cfg->graph_.check();
        return cfg;
    }

    std::string ControlFlowGraph::to_dot() const {
        std::ostringstream ss;
        ss << "digraph CFG_" << func_name_ << " {\n";
        ss << "  node [shape=box, fontname=\"Courier New\", fontsize=10];\n";
        ss << "  edge [fontname=\"Courier New\", fontsize=9];\n\n";

        // Сериализация узлов
        for (auto it = graph_.nodes_begin(); it != graph_.nodes_end(); ++it) {
            const auto& bb = it->get();
            ss << "  \"" << bb.label << "\" [label=\"" << bb.label << ":\\l";
            for (const auto& op : bb.instructions) {
                if (op && op->opcode != Opcode::LABEL) {
                    std::string line = op->to_string();
                    // Экранирование для DOT
                    std::string escaped;
                    for (char ch : line) {
                        if (ch == '\"') escaped += "\\\"";
                        else if (ch == '\n') escaped += "\\l";
                        else if (ch == '\\') escaped += "\\\\";
                        else escaped += ch;
                    }
                    ss << "  " << escaped << "\\l";
                }
            }
            ss << "\", style=filled, fillcolor=";
            if (bb.is_entry) ss << "\"#e8f4f8\"";
            else if (bb.is_exit) ss << "\"#fff0f0\"";
            else ss << "\"#fcfcfc\"";
            ss << "];\n";
        }

        ss << "\n";

        // Сериализация дуг
        for (auto e = graph_.edges_begin(); e != graph_.edges_end(); ++e) {
            auto from_bb = e->from()->get().label;
            auto to_bb   = e->to()->get().label;
            ss << "  \"" << from_bb << "\" -> \"" << to_bb << "\"";
            if (!e->get().empty()) {
                ss << " [label=\"" << e->get() << "\", color=";
                if (e->get() == "true") ss << "darkgreen";
                else if (e->get() == "false") ss << "crimson";
                else ss << "gray40";
                ss << "]";
            }
            ss << ";\n";
        }

        ss << "}\n";
        return ss.str();
    }

} // namespace to_lang::hir

