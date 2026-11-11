//
// Created by dangel on 11.11.2026.
//

#ifndef TOC_HIR_GENERATOR_HPP
#define TOC_HIR_GENERATOR_HPP

#include "hir.hpp"
#include "../frontend/ast/ast.hpp"
#include <unordered_map>

namespace to_lang::hir {

    class HIRGenerator {
    private:
        HIRTranslationUnitPtr unit_ = nullptr;
        HIRFunctionPtr current_fn_ = nullptr;

        // Таблица локальных слотов аллокации (var_name -> alloca pointer object)
        std::vector<std::unordered_map<std::string, HIRObjectPtr>> scope_slots_;

        void push_scope();
        void pop_scope();
        HIRObjectPtr lookup_slot(const std::string& name) const;
        void register_slot(const std::string& name, HIRObjectPtr slot);

        // Генерация сущностей AST
        void translate_function(const FunctionDeclPtr& fn_node);
        void translate_block(const BlockStmtPtr& block);
        void translate_stmt(const StmtNodePtr& stmt);
        void translate_var_decl(const std::shared_ptr<VarDeclStmt>& vd);
        void translate_if(const std::shared_ptr<IfStmt>& if_stmt);
        void translate_while(const std::shared_ptr<WhileStmt>& while_stmt);
        void translate_return(const std::shared_ptr<ReturnStmt>& ret_stmt);

        // Генерация выражений (возвращают HIRObjectPtr со значением результата)
        HIRObjectPtr translate_expr(const ExprNodePtr& expr);
        HIRObjectPtr translate_literal(const std::shared_ptr<LiteralExpr>& lit);
        HIRObjectPtr translate_variable(const std::shared_ptr<VariableExpr>& var);
        HIRObjectPtr translate_binary(const std::shared_ptr<BinaryExpr>& bin);
        HIRObjectPtr translate_unary(const std::shared_ptr<UnaryExpr>& un);
        HIRObjectPtr translate_call(const std::shared_ptr<CallExpr>& call);
        HIRObjectPtr translate_assign(const std::shared_ptr<AssignStmt>& as);
        HIRObjectPtr translate_index(const std::shared_ptr<IndexExpr>& idx, bool as_lvalue = false);

        // Вспомогательное приведение типов
        HIRObjectPtr emit_cast_if_needed(HIRObjectPtr obj, TypeNodePtr target_type);

    public:
        HIRGenerator() = default;

        HIRTranslationUnitPtr generate(const ProgramNodePtr& program);
    };

} // namespace to_lang::hir

#endif // TOC_HIR_GENERATOR_HPP

