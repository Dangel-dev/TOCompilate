//
// Created by dangel on 11.11.2026.
//

#include "generator.hpp"
#include "../frontend/semantics/type_utils.hpp"

namespace to_lang::hir {

    void HIRGenerator::push_scope() {
        scope_slots_.emplace_back();
    }

    void HIRGenerator::pop_scope() {
        if (!scope_slots_.empty()) {
            scope_slots_.pop_back();
        }
    }

    HIRObjectPtr HIRGenerator::lookup_slot(const std::string& name) const {
        for (auto it = scope_slots_.rbegin(); it != scope_slots_.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second;
            }
        }
        return nullptr;
    }

    void HIRGenerator::register_slot(const std::string& name, HIRObjectPtr slot) {
        if (scope_slots_.empty()) push_scope();
        scope_slots_.back()[name] = std::move(slot);
    }

    HIRObjectPtr HIRGenerator::emit_cast_if_needed(HIRObjectPtr obj, TypeNodePtr target_type) {
        if (!obj || !target_type || are_types_equal(obj->type, target_type)) {
            return obj;
        }
        auto cast_dst = current_fn_->create_temp(target_type);
        current_fn_->emit(HIROperation::make_cast(cast_dst, obj));
        return cast_dst;
    }

    HIRTranslationUnitPtr HIRGenerator::generate(const ProgramNodePtr& program) {
        unit_ = std::make_shared<HIRTranslationUnit>("translation_unit");
        if (!program) return unit_;

        for (const auto& fn : program->functions) {
            if (fn) {
                translate_function(fn);
            }
        }

        return unit_;
    }

    void HIRGenerator::translate_function(const FunctionDeclPtr& fn_node) {
        current_fn_ = std::make_shared<HIRFunction>(fn_node->name, fn_node->return_type);
        unit_->add_function(current_fn_);
        scope_slots_.clear();
        push_scope();

        // 1. Обработка параметров: выделение слота alloca и сохранение
        for (const auto& p : fn_node->params) {
            auto param_obj = current_fn_->add_param(p.name, p.type);
            auto ptr_type = TypeNode::make_pointer(p.type);
            auto slot = current_fn_->create_temp(ptr_type);

            current_fn_->emit(HIROperation::make_alloca(slot, fn_node->line, fn_node->col));
            current_fn_->emit(HIROperation::make_store(slot, param_obj, fn_node->line, fn_node->col));
            register_slot(p.name, slot);
        }

        // 2. Тело функции
        if (fn_node->body) {
            translate_block(fn_node->body);
        }

        // 3. Завершение функции: для void-функций гарантируем return
        if (is_void_type(fn_node->return_type)) {
            if (current_fn_->operations.empty() || !current_fn_->operations.back()->is_terminator()) {
                current_fn_->emit(HIROperation::make_return(nullptr, fn_node->line, fn_node->col));
            }
        }

        pop_scope();
        current_fn_ = nullptr;
    }

    void HIRGenerator::translate_block(const BlockStmtPtr& block) {
        if (!block) return;
        push_scope();
        for (const auto& stmt : block->statements) {
            if (stmt) {
                translate_stmt(stmt);
            }
        }
        pop_scope();
    }

    void HIRGenerator::translate_stmt(const StmtNodePtr& stmt) {
        if (auto vd = std::dynamic_pointer_cast<VarDeclStmt>(stmt)) {
            translate_var_decl(vd);
        } else if (auto if_s = std::dynamic_pointer_cast<IfStmt>(stmt)) {
            translate_if(if_s);
        } else if (auto while_s = std::dynamic_pointer_cast<WhileStmt>(stmt)) {
            translate_while(while_s);
        } else if (auto ret_s = std::dynamic_pointer_cast<ReturnStmt>(stmt)) {
            translate_return(ret_s);
        } else if (auto b = std::dynamic_pointer_cast<BlockStmt>(stmt)) {
            translate_block(b);
        } else if (auto expr_s = std::dynamic_pointer_cast<ExprStmt>(stmt)) {
            translate_expr(expr_s->expr);
        } else if (auto as = std::dynamic_pointer_cast<AssignStmt>(stmt)) {
            translate_assign(as);
        }
    }

    void HIRGenerator::translate_var_decl(const std::shared_ptr<VarDeclStmt>& vd) {
        auto ptr_t = TypeNode::make_pointer(vd->type);
        auto slot = current_fn_->create_temp(ptr_t);
        current_fn_->emit(HIROperation::make_alloca(slot, vd->line, vd->col));
        register_slot(vd->name, slot);

        if (vd->init) {
            auto val = translate_expr(vd->init);
            val = emit_cast_if_needed(val, vd->type);
            current_fn_->emit(HIROperation::make_store(slot, val, vd->line, vd->col));
        }
    }

    void HIRGenerator::translate_if(const std::shared_ptr<IfStmt>& if_stmt) {
        std::string then_lbl = current_fn_->create_label("if_then");
        std::string exit_lbl = current_fn_->create_label("if_exit");

        bool has_elifs_or_else = !if_stmt->elif_branches.empty() || if_stmt->else_branch != nullptr;
        std::string next_lbl = has_elifs_or_else ? current_fn_->create_label("if_next") : exit_lbl;

        auto cond_val = translate_expr(if_stmt->condition);
        current_fn_->emit(HIROperation::make_branch(cond_val, then_lbl, next_lbl, if_stmt->line, if_stmt->col));

        // Ветка then
        current_fn_->emit(HIROperation::make_label(then_lbl, if_stmt->line, if_stmt->col));
        translate_block(if_stmt->then_branch);
        if (current_fn_->operations.empty() || !current_fn_->operations.back()->is_terminator()) {
            current_fn_->emit(HIROperation::make_jump(exit_lbl, if_stmt->line, if_stmt->col));
        }

        // Ветки elif
        for (size_t i = 0; i < if_stmt->elif_branches.size(); ++i) {
            current_fn_->emit(HIROperation::make_label(next_lbl, if_stmt->line, if_stmt->col));
            std::string elif_then = current_fn_->create_label("elif_then");

            bool is_last = (i + 1 == if_stmt->elif_branches.size()) && (if_stmt->else_branch == nullptr);
            next_lbl = is_last ? exit_lbl : current_fn_->create_label("elif_next");

            auto elif_cond = translate_expr(if_stmt->elif_branches[i].condition);
            current_fn_->emit(HIROperation::make_branch(elif_cond, elif_then, next_lbl, if_stmt->line, if_stmt->col));

            current_fn_->emit(HIROperation::make_label(elif_then, if_stmt->line, if_stmt->col));
            translate_block(if_stmt->elif_branches[i].body);
            if (current_fn_->operations.empty() || !current_fn_->operations.back()->is_terminator()) {
                current_fn_->emit(HIROperation::make_jump(exit_lbl, if_stmt->line, if_stmt->col));
            }
        }

        // Ветка else
        if (if_stmt->else_branch) {
            current_fn_->emit(HIROperation::make_label(next_lbl, if_stmt->line, if_stmt->col));
            translate_block(if_stmt->else_branch);
            if (current_fn_->operations.empty() || !current_fn_->operations.back()->is_terminator()) {
                current_fn_->emit(HIROperation::make_jump(exit_lbl, if_stmt->line, if_stmt->col));
            }
        }

        current_fn_->emit(HIROperation::make_label(exit_lbl, if_stmt->line, if_stmt->col));
    }

    void HIRGenerator::translate_while(const std::shared_ptr<WhileStmt>& while_stmt) {
        std::string cond_lbl = current_fn_->create_label("while_cond");
        std::string body_lbl = current_fn_->create_label("while_body");
        std::string exit_lbl = current_fn_->create_label("while_exit");

        current_fn_->emit(HIROperation::make_jump(cond_lbl, while_stmt->line, while_stmt->col));

        // Проверка условия
        current_fn_->emit(HIROperation::make_label(cond_lbl, while_stmt->line, while_stmt->col));
        auto cond_val = translate_expr(while_stmt->condition);
        current_fn_->emit(HIROperation::make_branch(cond_val, body_lbl, exit_lbl, while_stmt->line, while_stmt->col));

        // Тело цикла
        current_fn_->emit(HIROperation::make_label(body_lbl, while_stmt->line, while_stmt->col));
        translate_block(while_stmt->body);
        if (current_fn_->operations.empty() || !current_fn_->operations.back()->is_terminator()) {
            current_fn_->emit(HIROperation::make_jump(cond_lbl, while_stmt->line, while_stmt->col));
        }

        current_fn_->emit(HIROperation::make_label(exit_lbl, while_stmt->line, while_stmt->col));
    }

    void HIRGenerator::translate_return(const std::shared_ptr<ReturnStmt>& ret_stmt) {
        if (ret_stmt->value) {
            auto val = translate_expr(ret_stmt->value);
            val = emit_cast_if_needed(val, current_fn_->return_type);
            current_fn_->emit(HIROperation::make_return(val, ret_stmt->line, ret_stmt->col));
        } else {
            current_fn_->emit(HIROperation::make_return(nullptr, ret_stmt->line, ret_stmt->col));
        }
    }

    HIRObjectPtr HIRGenerator::translate_expr(const ExprNodePtr& expr) {
        if (!expr) return nullptr;

        if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
            return translate_literal(lit);
        } else if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
            return translate_variable(var);
        } else if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
            return translate_binary(bin);
        } else if (auto un = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
            return translate_unary(un);
        } else if (auto call = std::dynamic_pointer_cast<CallExpr>(expr)) {
            return translate_call(call);
        } else if (auto as = std::dynamic_pointer_cast<AssignStmt>(expr)) {
            return translate_assign(as);
        } else if (auto idx = std::dynamic_pointer_cast<IndexExpr>(expr)) {
            return translate_index(idx, false);
        }

        return nullptr;
    }

    HIRObjectPtr HIRGenerator::translate_literal(const std::shared_ptr<LiteralExpr>& lit) {
        HIRConstantPtr c = nullptr;
        TypeNodePtr t = nullptr;

        switch (lit->lit_kind) {
            case LiteralExpr::LitKind::INT:
                t = TypeNode::make_base(BaseTypeKind::I32);
                c = HIRConstant::make_int(lit->int_val, t);
                break;
            case LiteralExpr::LitKind::BOOL:
                t = TypeNode::make_base(BaseTypeKind::BOOL);
                c = HIRConstant::make_bool(lit->bool_val);
                break;
            case LiteralExpr::LitKind::CHAR:
                t = TypeNode::make_base(BaseTypeKind::CHAR);
                c = HIRConstant::make_char(lit->char_val);
                break;
            case LiteralExpr::LitKind::STRING:
                t = TypeNode::make_pointer(TypeNode::make_base(BaseTypeKind::CHAR));
                c = HIRConstant::make_string(lit->str_val);
                break;
        }

        auto dst = current_fn_->create_temp(t);
        current_fn_->emit(HIROperation::make_const(dst, c, lit->line, lit->col));
        return dst;
    }

    HIRObjectPtr HIRGenerator::translate_variable(const std::shared_ptr<VariableExpr>& var) {
        auto slot = lookup_slot(var->name);
        if (!slot) return nullptr;

        // slot — это alloca-указатель (ptr$ T), загружаем значение
        auto pointee_t = slot->type->subtype;
        auto val = current_fn_->create_temp(pointee_t);
        current_fn_->emit(HIROperation::make_load(val, slot, var->line, var->col));
        return val;
    }

    HIRObjectPtr HIRGenerator::translate_binary(const std::shared_ptr<BinaryExpr>& bin) {
        auto l_val = translate_expr(bin->lhs);
        auto r_val = translate_expr(bin->rhs);
        if (!l_val || !r_val) return nullptr;

        Opcode opc = Opcode::ADD;
        TypeNodePtr res_t = l_val->type;

        if (bin->op == "+")  opc = Opcode::ADD;
        else if (bin->op == "-")  opc = Opcode::SUB;
        else if (bin->op == "*")  opc = Opcode::MUL;
        else if (bin->op == "/")  opc = Opcode::DIV;
        else if (bin->op == "%")  opc = Opcode::MOD;
        else if (bin->op == "&")  opc = Opcode::AND;
        else if (bin->op == "|")  opc = Opcode::OR;
        else if (bin->op == "^")  opc = Opcode::XOR;
        else if (bin->op == "<<") opc = Opcode::SHL;
        else if (bin->op == ">>") opc = Opcode::SHR;
        else if (bin->op == "&&") { opc = Opcode::AND; res_t = TypeNode::make_base(BaseTypeKind::BOOL); }
        else if (bin->op == "||") { opc = Opcode::OR;  res_t = TypeNode::make_base(BaseTypeKind::BOOL); }
        else if (bin->op == "==") { opc = Opcode::EQ;  res_t = TypeNode::make_base(BaseTypeKind::BOOL); }
        else if (bin->op == "!=") { opc = Opcode::NE;  res_t = TypeNode::make_base(BaseTypeKind::BOOL); }
        else if (bin->op == "<")  { opc = Opcode::LT;  res_t = TypeNode::make_base(BaseTypeKind::BOOL); }
        else if (bin->op == "<=") { opc = Opcode::LE;  res_t = TypeNode::make_base(BaseTypeKind::BOOL); }
        else if (bin->op == ">")  { opc = Opcode::GT;  res_t = TypeNode::make_base(BaseTypeKind::BOOL); }
        else if (bin->op == ">=") { opc = Opcode::GE;  res_t = TypeNode::make_base(BaseTypeKind::BOOL); }

        auto dst = current_fn_->create_temp(res_t);
        current_fn_->emit(HIROperation::make_binary(opc, dst, l_val, r_val, bin->line, bin->col));
        return dst;
    }

    HIRObjectPtr HIRGenerator::translate_unary(const std::shared_ptr<UnaryExpr>& un) {
        if (un->op == "&") {
            // Взятие адреса переменной: &var
            if (auto var = std::dynamic_pointer_cast<VariableExpr>(un->operand)) {
                return lookup_slot(var->name);
            }
            return nullptr;
        }

        auto op_val = translate_expr(un->operand);
        if (!op_val) return nullptr;

        if (un->op == "-") {
            auto dst = current_fn_->create_temp(op_val->type);
            current_fn_->emit(HIROperation::make_unary(Opcode::NEG, dst, op_val, un->line, un->col));
            return dst;
        } else if (un->op == "!" || un->op == "~") {
            auto dst = current_fn_->create_temp(op_val->type);
            current_fn_->emit(HIROperation::make_unary(Opcode::NOT, dst, op_val, un->line, un->col));
            return dst;
        } else if (un->op == "*") {
            // Разыменование: *ptr
            if (is_pointer_type(op_val->type)) {
                auto pointee_t = op_val->type->subtype;
                auto dst = current_fn_->create_temp(pointee_t);
                current_fn_->emit(HIROperation::make_load(dst, op_val, un->line, un->col));
                return dst;
            }
        }

        return nullptr;
    }

    HIRObjectPtr HIRGenerator::translate_call(const std::shared_ptr<CallExpr>& call) {
        std::vector<HIRObjectPtr> arg_vals;
        for (const auto& a : call->args) {
            arg_vals.push_back(translate_expr(a));
        }

        // Поиск сигнатуры или определение возвращаемого типа
        TypeNodePtr ret_t = call->inferred_type ? call->inferred_type : TypeNode::make_base(BaseTypeKind::I32);
        if (call->callee == "prints" || call->callee == "printi") {
            ret_t = TypeNode::make_base(BaseTypeKind::VOID);
        }

        HIRObjectPtr dst = nullptr;
        if (!is_void_type(ret_t)) {
            dst = current_fn_->create_temp(ret_t);
        }

        current_fn_->emit(HIROperation::make_call(dst, call->callee, std::move(arg_vals), call->line, call->col));
        return dst;
    }

    HIRObjectPtr HIRGenerator::translate_assign(const std::shared_ptr<AssignStmt>& as) {
        auto val = translate_expr(as->value);
        if (!val) return nullptr;

        HIRObjectPtr target_ptr = nullptr;

        if (auto var = std::dynamic_pointer_cast<VariableExpr>(as->target)) {
            target_ptr = lookup_slot(var->name);
        } else if (auto idx = std::dynamic_pointer_cast<IndexExpr>(as->target)) {
            target_ptr = translate_index(idx, true); // получить адрес элемента
        } else if (auto un = std::dynamic_pointer_cast<UnaryExpr>(as->target)) {
            if (un->op == "*") {
                target_ptr = translate_expr(un->operand); // разыменование lvalue
            }
        }

        if (!target_ptr) return nullptr;

        if (as->op == "=") {
            current_fn_->emit(HIROperation::make_store(target_ptr, val, as->line, as->col));
        } else {
            // Составное присваивание: +=, -=, *=, /=
            auto old_val = current_fn_->create_temp(target_ptr->type->subtype);
            current_fn_->emit(HIROperation::make_load(old_val, target_ptr, as->line, as->col));

            Opcode opc = Opcode::ADD;
            if (as->op == "+=") opc = Opcode::ADD;
            else if (as->op == "-=") opc = Opcode::SUB;
            else if (as->op == "*=") opc = Opcode::MUL;
            else if (as->op == "/=") opc = Opcode::DIV;

            auto new_val = current_fn_->create_temp(target_ptr->type->subtype);
            current_fn_->emit(HIROperation::make_binary(opc, new_val, old_val, val, as->line, as->col));
            current_fn_->emit(HIROperation::make_store(target_ptr, new_val, as->line, as->col));
            val = new_val;
        }

        return val;
    }

    HIRObjectPtr HIRGenerator::translate_index(const std::shared_ptr<IndexExpr>& idx, bool as_lvalue) {
        auto base = translate_expr(idx->target);
        auto index = translate_expr(idx->index);
        if (!base || !index) return nullptr;

        TypeNodePtr elem_t = nullptr;
        if (is_pointer_type(base->type) || is_array_type(base->type)) {
            elem_t = base->type->subtype;
        } else {
            elem_t = TypeNode::make_base(BaseTypeKind::I32);
        }

        auto elem_ptr = current_fn_->create_temp(TypeNode::make_pointer(elem_t));
        current_fn_->emit(HIROperation::make_gep(elem_ptr, base, index, idx->line, idx->col));

        if (as_lvalue) {
            return elem_ptr;
        }

        auto val = current_fn_->create_temp(elem_t);
        current_fn_->emit(HIROperation::make_load(val, elem_ptr, idx->line, idx->col));
        return val;
    }

} // namespace to_lang::hir

