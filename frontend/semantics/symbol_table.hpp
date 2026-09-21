//
// Created by dangel on 28.09.2026.
//

#ifndef TOC_SYMBOL_TABLE_HPP
#define TOC_SYMBOL_TABLE_HPP

#include "../ast/ast.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace to_lang {

    // Описание символа переменной
    struct VariableSymbol {
        std::string name;
        TypeNodePtr type;
        int decl_line = 0;
    };

    // Описание символа функции
    struct FunctionSymbol {
        std::string name;
        TypeNodePtr return_type;
        std::vector<TypeNodePtr> param_types;
        std::vector<std::string> param_names;
        int decl_line = 0;
    };

    // Таблица символов со стеком областей видимости
    class SymbolTable {
    private:
        std::vector<std::unordered_map<std::string, VariableSymbol>> scope_stack_;
        std::unordered_map<std::string, FunctionSymbol> function_table_;

    public:
        SymbolTable();

        // Управление областями видимости
        void push_scope();
        void pop_scope();
        size_t scope_depth() const { return scope_stack_.size(); }

        // Переменные
        bool declare_variable(const std::string& name, const TypeNodePtr& type, int line, std::string& err_msg);
        const VariableSymbol* lookup_variable(const std::string& name) const;
        bool is_declared_in_current_scope(const std::string& name) const;

        // Функции
        bool declare_function(const std::string& name, const TypeNodePtr& ret_type,
                              const std::vector<TypeNodePtr>& param_types,
                              const std::vector<std::string>& param_names,
                              int line, std::string& err_msg);
        const FunctionSymbol* lookup_function(const std::string& name) const;

        // Сброс и регистрация встроенных функций
        void reset();
        void register_builtins();
    };

} // namespace to_lang

#endif // TOC_SYMBOL_TABLE_HPP

