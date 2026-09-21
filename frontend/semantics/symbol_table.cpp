//
// Created by dangel on 28.09.2026.
//

#include "symbol_table.hpp"

namespace to_lang {

    SymbolTable::SymbolTable() {
        reset();
    }

    void SymbolTable::reset() {
        scope_stack_.clear();
        function_table_.clear();
        // Глобальная область видимости
        scope_stack_.emplace_back();
        register_builtins();
    }

    void SymbolTable::register_builtins() {
        // Встроенная функция prints(ptr$ char) -> void
        auto void_t = TypeNode::make_base(BaseTypeKind::VOID);
        auto char_ptr_t = TypeNode::make_pointer(TypeNode::make_base(BaseTypeKind::CHAR));
        auto i32_t = TypeNode::make_base(BaseTypeKind::I32);

        std::string err;
        declare_function("prints", void_t, { char_ptr_t }, { "s" }, 0, err);
        declare_function("printi", void_t, { i32_t }, { "n" }, 0, err);
    }

    void SymbolTable::push_scope() {
        scope_stack_.emplace_back();
    }

    void SymbolTable::pop_scope() {
        if (scope_stack_.size() > 1) {
            scope_stack_.pop_back();
        }
    }

    bool SymbolTable::is_declared_in_current_scope(const std::string& name) const {
        if (scope_stack_.empty()) return false;
        return scope_stack_.back().find(name) != scope_stack_.back().end();
    }

    bool SymbolTable::declare_variable(const std::string& name, const TypeNodePtr& type, int line, std::string& err_msg) {
        if (scope_stack_.empty()) push_scope();

        if (is_declared_in_current_scope(name)) {
            err_msg = "Redefinition of variable '" + name + "'. Previously declared at line " +
                      std::to_string(scope_stack_.back()[name].decl_line);
            return false;
        }

        VariableSymbol sym;
        sym.name = name;
        sym.type = type;
        sym.decl_line = line;
        scope_stack_.back()[name] = sym;
        return true;
    }

    const VariableSymbol* SymbolTable::lookup_variable(const std::string& name) const {
        // Поиск от текущего локального блока к глобальному
        for (auto it = scope_stack_.rbegin(); it != scope_stack_.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return &found->second;
            }
        }
        return nullptr;
    }

    bool SymbolTable::declare_function(const std::string& name, const TypeNodePtr& ret_type,
                                       const std::vector<TypeNodePtr>& param_types,
                                       const std::vector<std::string>& param_names,
                                       int line, std::string& err_msg) {
        auto it = function_table_.find(name);
        if (it != function_table_.end()) {
            err_msg = "Redefinition of function '" + name + "'. Previously declared at line " +
                      std::to_string(it->second.decl_line);
            return false;
        }

        FunctionSymbol fn;
        fn.name = name;
        fn.return_type = ret_type;
        fn.param_types = param_types;
        fn.param_names = param_names;
        fn.decl_line = line;
        function_table_[name] = fn;
        return true;
    }

    const FunctionSymbol* SymbolTable::lookup_function(const std::string& name) const {
        auto it = function_table_.find(name);
        if (it != function_table_.end()) {
            return &it->second;
        }
        return nullptr;
    }

} // namespace to_lang

