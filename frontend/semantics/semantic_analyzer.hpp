//
// Created by dangel on 28.09.2026.
//

#ifndef TOC_SEMANTIC_ANALYZER_HPP
#define TOC_SEMANTIC_ANALYZER_HPP

#include "../ast/ast.hpp"
#include "symbol_table.hpp"
#include <string>
#include <vector>

namespace to_lang {

    // Описание семантической ошибки
    struct SemanticError {
        std::string message;
        int line = 1;
        int col = 1;

        std::string format() const {
            return "Semantic error at line " + std::to_string(line) + ", col " +
                   std::to_string(col) + ": " + message;
        }
    };

    // Класс семантического анализатора
    class SemanticAnalyzer {
    private:
        SymbolTable symbols_;
        std::vector<SemanticError> errors_;
        TypeNodePtr current_func_ret_type_ = nullptr;

        void error(const std::string& msg, int line = 1, int col = 1);

        void visit_function(const FunctionDeclPtr& fn);
        void visit_block(const BlockStmtPtr& block);
        void visit_stmt(const StmtNodePtr& stmt);

    public:
        SemanticAnalyzer() = default;

        // Запуск полного семантического анализа AST
        bool analyze(const ProgramNodePtr& program);

        // Проверка и вывод типов выражений
        TypeNodePtr infer_type(const ExprNodePtr& expr);

        const std::vector<SemanticError>& get_errors() const noexcept { return errors_; }
        bool has_errors() const noexcept { return !errors_.empty(); }

        const SymbolTable& get_symbol_table() const noexcept { return symbols_; }
    };

} // namespace to_lang

#endif // TOC_SEMANTIC_ANALYZER_HPP

