//
// Created by dangel on 21.09.2026.
//

#include "ast.hpp"
#include <sstream>

namespace to_lang {

    class ASTDotVisitor {
    private:
        std::ostringstream ss_;
        int node_id_ = 0;

        int next_id() { return ++node_id_; }

        void emit_node(int id, const std::string& label, const std::string& shape = "box", const std::string& color = "black") {
            ss_ << "  n" << id << " [label=\"" << escape(label) << "\", shape=" << shape << ", color=\"" << color << "\"];\n";
        }

        void emit_edge(int from, int to, const std::string& label = "") {
            ss_ << "  n" << from << " -> n" << to;
            if (!label.empty()) {
                ss_ << " [label=\"" << escape(label) << "\"]";
            }
            ss_ << ";\n";
        }

        std::string escape(const std::string& s) {
            std::string res;
            for (char c : s) {
                if (c == '"') res += "\\\"";
                else if (c == '\\') res += "\\\\";
                else if (c == '\n') res += "\\n";
                else res += c;
            }
            return res;
        }

    public:
        std::string export_dot(const ASTNodePtr& root) {
            ss_ << "digraph AST {\n";
            ss_ << "  node [fontname=\"Consolas\"];\n";
            ss_ << "  edge [fontname=\"Consolas\"];\n";
            if (root) {
                visit(root);
            }
            ss_ << "}\n";
            return ss_.str();
        }

        int visit(const ASTNodePtr& node) {
            if (!node) return 0;

            if (auto p = std::dynamic_pointer_cast<ProgramNode>(node)) {
                int id = next_id();
                emit_node(id, "Program", "doubleoctagon", "darkgreen");
                for (const auto& fn : p->functions) {
                    int c_id = visit(fn);
                    if (c_id) emit_edge(id, c_id, "func");
                }
                return id;
            }

            if (auto fn = std::dynamic_pointer_cast<FunctionDecl>(node)) {
                int id = next_id();
                std::string label = "fn " + fn->name + "(): " + (fn->return_type ? fn->return_type->to_string() : "void");
                emit_node(id, label, "component", "blue");

                for (const auto& param : fn->params) {
                    int p_id = next_id();
                    emit_node(p_id, "param: " + (param.type ? param.type->to_string() : "type") + " " + param.name, "ellipse", "gray");
                    emit_edge(id, p_id, "param");
                }

                if (fn->body) {
                    int b_id = visit(fn->body);
                    if (b_id) emit_edge(id, b_id, "body");
                }
                return id;
            }

            if (auto blk = std::dynamic_pointer_cast<BlockStmt>(node)) {
                int id = next_id();
                emit_node(id, "Block", "box", "purple");
                for (size_t i = 0; i < blk->statements.size(); ++i) {
                    int s_id = visit(blk->statements[i]);
                    if (s_id) emit_edge(id, s_id, std::to_string(i));
                }
                return id;
            }

            if (auto vd = std::dynamic_pointer_cast<VarDeclStmt>(node)) {
                int id = next_id();
                std::string label = "VarDecl: " + (vd->type ? vd->type->to_string() : "") + " " + vd->name;
                emit_node(id, label, "rect", "darkblue");
                if (vd->init) {
                    int i_id = visit(vd->init);
                    if (i_id) emit_edge(id, i_id, "init");
                }
                return id;
            }

            if (auto as = std::dynamic_pointer_cast<AssignStmt>(node)) {
                int id = next_id();
                emit_node(id, "Assign (" + as->op + ")", "rect", "brown");
                if (as->target) {
                    int t_id = visit(as->target);
                    if (t_id) emit_edge(id, t_id, "target");
                }
                if (as->value) {
                    int v_id = visit(as->value);
                    if (v_id) emit_edge(id, v_id, "value");
                }
                return id;
            }

            if (auto ifs = std::dynamic_pointer_cast<IfStmt>(node)) {
                int id = next_id();
                emit_node(id, "IfStmt", "diamond", "darkorange");
                if (ifs->condition) {
                    int c_id = visit(ifs->condition);
                    if (c_id) emit_edge(id, c_id, "cond");
                }
                if (ifs->then_branch) {
                    int t_id = visit(ifs->then_branch);
                    if (t_id) emit_edge(id, t_id, "then");
                }
                for (size_t i = 0; i < ifs->elif_branches.size(); ++i) {
                    if (ifs->elif_branches[i].condition) {
                        int el_c = visit(ifs->elif_branches[i].condition);
                        if (el_c) emit_edge(id, el_c, "elif[" + std::to_string(i) + "].cond");
                    }
                    if (ifs->elif_branches[i].body) {
                        int el_b = visit(ifs->elif_branches[i].body);
                        if (el_b) emit_edge(id, el_b, "elif[" + std::to_string(i) + "].body");
                    }
                }
                if (ifs->else_branch) {
                    int e_id = visit(ifs->else_branch);
                    if (e_id) emit_edge(id, e_id, "else");
                }
                return id;
            }

            if (auto wh = std::dynamic_pointer_cast<WhileStmt>(node)) {
                int id = next_id();
                emit_node(id, "WhileStmt", "hexagon", "darkcyan");
                if (wh->condition) {
                    int c_id = visit(wh->condition);
                    if (c_id) emit_edge(id, c_id, "cond");
                }
                if (wh->body) {
                    int b_id = visit(wh->body);
                    if (b_id) emit_edge(id, b_id, "body");
                }
                return id;
            }

            if (auto ret = std::dynamic_pointer_cast<ReturnStmt>(node)) {
                int id = next_id();
                emit_node(id, "Return", "trapezium", "red");
                if (ret->value) {
                    int v_id = visit(ret->value);
                    if (v_id) emit_edge(id, v_id, "value");
                }
                return id;
            }

            if (auto ex = std::dynamic_pointer_cast<ExprStmt>(node)) {
                return visit(ex->expr);
            }

            if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(node)) {
                int id = next_id();
                emit_node(id, lit->value_string(), "ellipse", "green");
                return id;
            }

            if (auto var = std::dynamic_pointer_cast<VariableExpr>(node)) {
                int id = next_id();
                emit_node(id, var->name, "ellipse", "black");
                return id;
            }

            if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(node)) {
                int id = next_id();
                emit_node(id, bin->op, "circle", "teal");
                if (bin->lhs) {
                    int l_id = visit(bin->lhs);
                    if (l_id) emit_edge(id, l_id, "L");
                }
                if (bin->rhs) {
                    int r_id = visit(bin->rhs);
                    if (r_id) emit_edge(id, r_id, "R");
                }
                return id;
            }

            if (auto un = std::dynamic_pointer_cast<UnaryExpr>(node)) {
                int id = next_id();
                emit_node(id, un->op, "circle", "teal");
                if (un->operand) {
                    int o_id = visit(un->operand);
                    if (o_id) emit_edge(id, o_id, "op");
                }
                return id;
            }

            if (auto idx = std::dynamic_pointer_cast<IndexExpr>(node)) {
                int id = next_id();
                emit_node(id, "[]", "circle", "indigo");
                if (idx->target) {
                    int t_id = visit(idx->target);
                    if (t_id) emit_edge(id, t_id, "target");
                }
                if (idx->index) {
                    int i_id = visit(idx->index);
                    if (i_id) emit_edge(id, i_id, "index");
                }
                return id;
            }

            if (auto call = std::dynamic_pointer_cast<CallExpr>(node)) {
                int id = next_id();
                emit_node(id, "call: " + call->callee + "()", "rect", "magenta");
                for (size_t i = 0; i < call->args.size(); ++i) {
                    int a_id = visit(call->args[i]);
                    if (a_id) emit_edge(id, a_id, "arg[" + std::to_string(i) + "]");
                }
                return id;
            }

            int id = next_id();
            emit_node(id, node->get_node_type(), "box", "gray");
            return id;
        }
    };

    std::string export_ast_to_dot(const ASTNodePtr& root) {
        ASTDotVisitor visitor;
        return visitor.export_dot(root);
    }

} // namespace to_lang

