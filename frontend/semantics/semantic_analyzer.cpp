//
// Created by dangel on 28.09.2026.
//

#include "semantic_analyzer.hpp"
#include "type_utils.hpp"
#include <climits>

namespace to_lang {

    void SemanticAnalyzer::error(const std::string& msg, int line, int col) {
        errors_.push_back(SemanticError{ msg, line, col });
    }

    bool SemanticAnalyzer::analyze(const ProgramNodePtr& program) {
        errors_.clear();
        symbols_.reset();

        if (!program) {
            error("Null program AST root");
            return false;
        }

        // 1-й проход: предварительная регистрация заголовков всех функций
        for (const auto& fn : program->functions) {
            if (!fn) continue;
            std::vector<TypeNodePtr> p_types;
            std::vector<std::string> p_names;
            for (const auto& param : fn->params) {
                p_types.push_back(param.type);
                p_names.push_back(param.name);
            }

            std::string err;
            if (!symbols_.declare_function(fn->name, fn->return_type, p_types, p_names, fn->line, err)) {
                error(err, fn->line, fn->col);
            }
        }

        // 2-й проход: детальный анализ тел всех функций
        for (const auto& fn : program->functions) {
            if (fn) visit_function(fn);
        }

        return errors_.empty();
    }

    void SemanticAnalyzer::visit_function(const FunctionDeclPtr& fn) {
        current_func_ret_type_ = fn->return_type;

        // Вход в область видимости функции
        symbols_.push_scope();

        // Объявление параметров в локальной области видимости
        for (const auto& param : fn->params) {
            std::string err;
            if (!symbols_.declare_variable(param.name, param.type, fn->line, err)) {
                error("Parameter " + err, fn->line, fn->col);
            }
        }

        // Анализ тела функции
        if (fn->body) {
            // Тело функции - это BlockStmt. Чтобы не плодить лишний уровень скоупа для тела:
            for (const auto& s : fn->body->statements) {
                if (s) visit_stmt(s);
            }
        }

        symbols_.pop_scope();
        current_func_ret_type_ = nullptr;
    }

    void SemanticAnalyzer::visit_block(const BlockStmtPtr& block) {
        if (!block) return;
        symbols_.push_scope();
        for (const auto& s : block->statements) {
            if (s) visit_stmt(s);
        }
        symbols_.pop_scope();
    }

    void SemanticAnalyzer::visit_stmt(const StmtNodePtr& stmt) {
        if (!stmt) return;

        if (auto b = std::dynamic_pointer_cast<BlockStmt>(stmt)) {
            visit_block(b);
            return;
        }

        if (auto vd = std::dynamic_pointer_cast<VarDeclStmt>(stmt)) {
            if (vd->init) {
                auto init_t = infer_type(vd->init);
                bool is_null_lit = false;
                if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(vd->init)) {
                    if (lit->lit_kind == LiteralExpr::LitKind::INT && lit->int_val == 0) {
                        is_null_lit = true;
                    }
                }

                if (init_t && !can_implicitly_convert(init_t, vd->type) &&
                    !(is_null_lit && is_pointer_type(vd->type))) {
                    error("Cannot initialize variable '" + vd->name + "' of type " +
                          vd->type->to_string() + " with value of type " + init_t->to_string(),
                          vd->line, vd->col);
                }
            }

            std::string err;
            if (!symbols_.declare_variable(vd->name, vd->type, vd->line, err)) {
                error(err, vd->line, vd->col);
            }
            return;
        }

        if (auto as = std::dynamic_pointer_cast<AssignStmt>(stmt)) {
            infer_type(as);
            return;
        }

        if (auto ifs = std::dynamic_pointer_cast<IfStmt>(stmt)) {
            auto cond_t = infer_type(ifs->condition);
            if (cond_t && !is_boolean_type(cond_t) && !is_integer_type(cond_t)) {
                error("If condition must be of boolean or integer type, got " + cond_t->to_string(),
                      ifs->condition->line, ifs->condition->col);
            }

            visit_block(ifs->then_branch);

            for (const auto& elif_b : ifs->elif_branches) {
                auto el_cond_t = infer_type(elif_b.condition);
                if (el_cond_t && !is_boolean_type(el_cond_t) && !is_integer_type(el_cond_t)) {
                    error("Elif condition must be of boolean or integer type, got " + el_cond_t->to_string(),
                          elif_b.condition->line, elif_b.condition->col);
                }
                visit_block(elif_b.body);
            }

            if (ifs->else_branch) {
                visit_block(ifs->else_branch);
            }
            return;
        }

        if (auto wh = std::dynamic_pointer_cast<WhileStmt>(stmt)) {
            auto cond_t = infer_type(wh->condition);
            if (cond_t && !is_boolean_type(cond_t) && !is_integer_type(cond_t)) {
                error("While condition must be of boolean or integer type, got " + cond_t->to_string(),
                      wh->condition->line, wh->condition->col);
            }
            visit_block(wh->body);
            return;
        }

        if (auto ret = std::dynamic_pointer_cast<ReturnStmt>(stmt)) {
            if (ret->value) {
                auto val_t = infer_type(ret->value);
                if (!current_func_ret_type_) {
                    error("Return statement outside function", ret->line, ret->col);
                } else if (current_func_ret_type_->kind == TypeNode::Kind::BASE &&
                           current_func_ret_type_->base_kind == BaseTypeKind::VOID) {
                    error("Void function cannot return a value", ret->line, ret->col);
                } else if (val_t && !can_implicitly_convert(val_t, current_func_ret_type_)) {
                    error("Return type mismatch: expected " + current_func_ret_type_->to_string() +
                          ", got " + val_t->to_string(), ret->line, ret->col);
                }
            } else {
                if (current_func_ret_type_ &&
                    !(current_func_ret_type_->kind == TypeNode::Kind::BASE &&
                      current_func_ret_type_->base_kind == BaseTypeKind::VOID)) {
                    error("Non-void function must return a value of type " +
                          current_func_ret_type_->to_string(), ret->line, ret->col);
                }
            }
            return;
        }

        if (auto ex = std::dynamic_pointer_cast<ExprStmt>(stmt)) {
            infer_type(ex->expr);
            return;
        }
    }

    TypeNodePtr SemanticAnalyzer::infer_type(const ExprNodePtr& expr) {
        if (!expr) return nullptr;

        // 1. Литералы
        if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
            TypeNodePtr t = nullptr;
            switch (lit->lit_kind) {
                case LiteralExpr::LitKind::INT:
                    t = TypeNode::make_base(BaseTypeKind::I32);
                    break;
                case LiteralExpr::LitKind::BOOL:
                    t = TypeNode::make_base(BaseTypeKind::BOOL);
                    break;
                case LiteralExpr::LitKind::CHAR:
                    t = TypeNode::make_base(BaseTypeKind::CHAR);
                    break;
                case LiteralExpr::LitKind::STRING:
                    // Строка - это ptr$ char
                    t = TypeNode::make_pointer(TypeNode::make_base(BaseTypeKind::CHAR));
                    break;
            }
            expr->inferred_type = t;
            return t;
        }

        // 2. Имена переменных
        if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
            auto sym = symbols_.lookup_variable(var->name);
            if (!sym) {
                error("Use of undeclared variable '" + var->name + "'", var->line, var->col);
                return nullptr;
            }
            expr->inferred_type = sym->type;
            return sym->type;
        }

        // 3. Бинарные операции
        if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
            auto l_t = infer_type(bin->lhs);
            auto r_t = infer_type(bin->rhs);
            if (!l_t || !r_t) return nullptr;

            std::string op = bin->op;

            // Сравнения
            if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") {
                if (!can_implicitly_convert(l_t, r_t) && !can_implicitly_convert(r_t, l_t)) {
                    error("Comparison between incompatible types: " + l_t->to_string() +
                          " and " + r_t->to_string(), bin->line, bin->col);
                }
                auto res_t = TypeNode::make_base(BaseTypeKind::BOOL);
                bin->inferred_type = res_t;
                return res_t;
            }

            // Логические операции:
            if (op == "&&" || op == "||") {
                if (!is_boolean_type(l_t) && !is_integer_type(l_t)) {
                    error("Left operand of '" + op + "' must be boolean/integer, got " + l_t->to_string(),
                          bin->lhs->line, bin->lhs->col);
                }
                if (!is_boolean_type(r_t) && !is_integer_type(r_t)) {
                    error("Right operand of '" + op + "' must be boolean/integer, got " + r_t->to_string(),
                          bin->rhs->line, bin->rhs->col);
                }
                auto res_t = TypeNode::make_base(BaseTypeKind::BOOL);
                bin->inferred_type = res_t;
                return res_t;
            }

            // Арифметика
            if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
                // Арифметика указателей: ptr + int или int + ptr -> ptr
                if (op == "+") {
                    if (is_pointer_type(l_t) && is_integer_type(r_t)) {
                        bin->inferred_type = l_t;
                        return l_t;
                    }
                    if (is_integer_type(l_t) && is_pointer_type(r_t)) {
                        bin->inferred_type = r_t;
                        return r_t;
                    }
                }
                if (op == "-") {
                    if (is_pointer_type(l_t) && is_integer_type(r_t)) {
                        bin->inferred_type = l_t;
                        return l_t;
                    }
                    if (is_pointer_type(l_t) && is_pointer_type(r_t)) {
                        auto diff_t = TypeNode::make_base(BaseTypeKind::I64);
                        bin->inferred_type = diff_t;
                        return diff_t;
                    }
                }

                if (!is_integer_type(l_t) && !is_char_type(l_t)) {
                    error("Left operand of '" + op + "' must be integer, got " + l_t->to_string(),
                          bin->lhs->line, bin->lhs->col);
                    return nullptr;
                }
                if (!is_integer_type(r_t) && !is_char_type(r_t)) {
                    error("Right operand of '" + op + "' must be integer, got " + r_t->to_string(),
                          bin->rhs->line, bin->rhs->col);
                    return nullptr;
                }

                // Общий целый тип (i64 побеждает i32)
                if ((l_t->kind == TypeNode::Kind::BASE && l_t->base_kind == BaseTypeKind::I64) ||
                    (r_t->kind == TypeNode::Kind::BASE && r_t->base_kind == BaseTypeKind::I64)) {
                    auto res_t = TypeNode::make_base(BaseTypeKind::I64);
                    bin->inferred_type = res_t;
                    return res_t;
                }
                auto res_t = TypeNode::make_base(BaseTypeKind::I32);
                bin->inferred_type = res_t;
                return res_t;
            }

            // Побитовые операции: &, |, ^, <<, >>
            if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>") {
                if (!is_integer_type(l_t) || !is_integer_type(r_t)) {
                    error("Operands of bitwise '" + op + "' must be integer types, got " +
                          l_t->to_string() + " and " + r_t->to_string(), bin->line, bin->col);
                    return nullptr;
                }
                bin->inferred_type = l_t;
                return l_t;
            }

            error("Unknown binary operator '" + op + "'", bin->line, bin->col);
            return nullptr;
        }

        // 4. Унарные операции
        if (auto un = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
            auto op_t = infer_type(un->operand);
            if (!op_t) return nullptr;

            if (un->op == "-") {
                if (!is_integer_type(op_t)) {
                    error("Operand of unary minus must be integer, got " + op_t->to_string(), un->line, un->col);
                }
                un->inferred_type = op_t;
                return op_t;
            }
            if (un->op == "!") {
                if (!is_boolean_type(op_t) && !is_integer_type(op_t)) {
                    error("Operand of '!' must be boolean or integer, got " + op_t->to_string(), un->line, un->col);
                }
                auto res_t = TypeNode::make_base(BaseTypeKind::BOOL);
                un->inferred_type = res_t;
                return res_t;
            }
            if (un->op == "~") {
                if (!is_integer_type(op_t)) {
                    error("Operand of bitwise NOT must be integer, got " + op_t->to_string(), un->line, un->col);
                }
                un->inferred_type = op_t;
                return op_t;
            }
            if (un->op == "*") {
                // Разыменование указателя
                if (!is_pointer_type(op_t)) {
                    error("Cannot dereference non-pointer type " + op_t->to_string(), un->line, un->col);
                    return nullptr;
                }
                un->inferred_type = op_t->subtype;
                return op_t->subtype;
            }

            error("Unknown unary operator '" + un->op + "'", un->line, un->col);
            return nullptr;
        }

        // 5. Индексация: target[index]
        if (auto idx = std::dynamic_pointer_cast<IndexExpr>(expr)) {
            auto t_t = infer_type(idx->target);
            auto i_t = infer_type(idx->index);

            if (i_t && !is_integer_type(i_t) && !is_char_type(i_t)) {
                error("Array subscript must be integer type, got " + i_t->to_string(), idx->index->line, idx->index->col);
            }

            if (!t_t) return nullptr;

            if (is_array_type(t_t) || is_pointer_type(t_t)) {
                idx->inferred_type = t_t->subtype;
                return t_t->subtype;
            }

            error("Subscripted value is not an array or pointer, got " + t_t->to_string(), idx->line, idx->col);
            return nullptr;
        }

        // 6. Вызов функции
        if (auto call = std::dynamic_pointer_cast<CallExpr>(expr)) {
            auto fn = symbols_.lookup_function(call->callee);
            if (!fn) {
                error("Call to undeclared function '" + call->callee + "'", call->line, call->col);
                return nullptr;
            }

            if (call->args.size() != fn->param_types.size()) {
                error("Function '" + call->callee + "' expects " +
                      std::to_string(fn->param_types.size()) + " arguments, got " +
                      std::to_string(call->args.size()), call->line, call->col);
            } else {
                for (size_t i = 0; i < call->args.size(); ++i) {
                    auto a_t = infer_type(call->args[i]);
                    bool is_null_lit = false;
                    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(call->args[i])) {
                        if (lit->lit_kind == LiteralExpr::LitKind::INT && lit->int_val == 0) is_null_lit = true;
                    }

                    if (a_t && !can_implicitly_convert(a_t, fn->param_types[i]) &&
                        !(is_null_lit && is_pointer_type(fn->param_types[i]))) {
                        error("Argument " + std::to_string(i + 1) + " of '" + call->callee +
                              "' type mismatch: expected " + fn->param_types[i]->to_string() +
                              ", got " + a_t->to_string(), call->args[i]->line, call->args[i]->col);
                    }
                }
            }

            call->inferred_type = fn->return_type;
            return fn->return_type;
        }

        // 7. Присваивание
        if (auto as = std::dynamic_pointer_cast<AssignStmt>(expr)) {
            auto t_t = infer_type(as->target);
            auto v_t = infer_type(as->value);

            if (t_t && v_t) {
                bool is_null_lit = false;
                if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(as->value)) {
                    if (lit->lit_kind == LiteralExpr::LitKind::INT && lit->int_val == 0) is_null_lit = true;
                }

                if (!can_implicitly_convert(v_t, t_t) && !(is_null_lit && is_pointer_type(t_t))) {
                    error("Cannot assign value of type " + v_t->to_string() + " to target of type " +
                          t_t->to_string(), as->line, as->col);
                }
            }

            as->inferred_type = t_t;
            return t_t;
        }

        return nullptr;
    }

} // namespace to_lang
