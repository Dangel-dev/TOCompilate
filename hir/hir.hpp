//
// Created by dangel on 11.11.2026.
//

#ifndef TOC_HIR_HPP
#define TOC_HIR_HPP

#include "../frontend/ast/ast.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace to_lang::hir {

    // Впередсмотрящие объявления
    class HIRObject;
    class HIRConstant;
    class HIROperation;
    class HIRFunction;
    class HIRTranslationUnit;

    using HIRObjectPtr          = std::shared_ptr<HIRObject>;
    using HIRConstantPtr        = std::shared_ptr<HIRConstant>;
    using HIROperationPtr       = std::shared_ptr<HIROperation>;
    using HIRFunctionPtr        = std::shared_ptr<HIRFunction>;
    using HIRTranslationUnitPtr = std::shared_ptr<HIRTranslationUnit>;

    // 1. Константа (Constant)
    class HIRConstant {
    public:
        enum class Kind { INT, BOOL, CHAR, STRING, NULL_PTR };

        Kind kind;
        TypeNodePtr type;
        int64_t int_val = 0;
        bool bool_val = false;
        char char_val = '\0';
        std::string str_val;

        static HIRConstantPtr make_int(int64_t v, TypeNodePtr t = nullptr) {
            auto c = std::make_shared<HIRConstant>();
            c->kind = Kind::INT;
            c->int_val = v;
            c->type = t ? t : TypeNode::make_base(BaseTypeKind::I32);
            return c;
        }

        static HIRConstantPtr make_bool(bool v) {
            auto c = std::make_shared<HIRConstant>();
            c->kind = Kind::BOOL;
            c->bool_val = v;
            c->type = TypeNode::make_base(BaseTypeKind::BOOL);
            return c;
        }

        static HIRConstantPtr make_char(char v) {
            auto c = std::make_shared<HIRConstant>();
            c->kind = Kind::CHAR;
            c->char_val = v;
            c->type = TypeNode::make_base(BaseTypeKind::CHAR);
            return c;
        }

        static HIRConstantPtr make_string(std::string v) {
            auto c = std::make_shared<HIRConstant>();
            c->kind = Kind::STRING;
            c->str_val = std::move(v);
            c->type = TypeNode::make_pointer(TypeNode::make_base(BaseTypeKind::CHAR));
            return c;
        }

        static HIRConstantPtr make_null(TypeNodePtr ptr_type) {
            auto c = std::make_shared<HIRConstant>();
            c->kind = Kind::NULL_PTR;
            c->type = std::move(ptr_type);
            return c;
        }

        std::string to_string() const;
    };

    // 2. Объект (Object: переменные программы или временные значения)
    class HIRObject {
    public:
        enum class Kind {
            TEMPORARY, // Автоматическое временное значение (SSA-регистр %t0, %t1...)
            VARIABLE,  // Именованная переменная программы
            PARAMETER, // Входной параметр функции
            GLOBAL     // Глобальный объект
        };

        Kind kind;
        std::string name;
        TypeNodePtr type;

        HIRObject(Kind k, std::string n, TypeNodePtr t)
            : kind(k), name(std::move(n)), type(std::move(t)) {}

        bool is_temporary() const noexcept { return kind == Kind::TEMPORARY; }
        bool is_variable() const noexcept { return kind == Kind::VARIABLE; }
        bool is_parameter() const noexcept { return kind == Kind::PARAMETER; }
        bool is_global() const noexcept { return kind == Kind::GLOBAL; }

        std::string to_string() const {
            return name + ": " + (type ? type->to_string() : "void");
        }
    };

    // 3. Коды операций HIR (Opcode)
    enum class Opcode {
        // Загрузка констант
        CONST,

        // Численная и битовая арифметика
        ADD, SUB, MUL, DIV, MOD,
        AND, OR, XOR, SHL, SHR,
        NEG, NOT,

        // Сравнения
        EQ, NE, LT, LE, GT, GE,

        // Работа с объектами, указателями и сложными типами
        ALLOCA,         // Выделение памяти под локальную переменную: dest = alloca Type
        ADDR_OF,        // Взятие адреса объекта: dest = &src
        LOAD,           // Чтение значения по указателю: dest = *ptr
        STORE,          // Запись значения по указателю: *ptr = val
        GET_ELEM_PTR,   // Получение указателя на элемент массива: dest = &arr[idx]
        GET_FIELD_PTR,  // Получение указателя на поле структуры

        // Преобразование типа
        CAST,

        // Переходы и управление потоком
        LABEL,          // Метка в коде (цель для переходов)
        JUMP,           // Безусловный переход: jump label
        BRANCH,         // Условный переход: branch cond, true_label, false_label
        CALL,           // Вызов функции по имени: dest = call fn(args...)
        CALL_PTR,       // Вызов функции по указателю: dest = call_ptr ptr(args...)
        RETURN          // Возврат из функции: return val или return
    };

    std::string opcode_to_string(Opcode op);

    // 4. Операция (Operation)
    class HIROperation {
    public:
        Opcode opcode;
        HIRObjectPtr dest = nullptr;      // Результат операции (может быть nullptr для STORE, JUMP и т.д.)
        HIRObjectPtr src1 = nullptr;      // Первый операнд
        HIRObjectPtr src2 = nullptr;      // Второй операнд
        std::vector<HIRObjectPtr> args;   // Список аргументов переменной длины (для CALL)
        HIRConstantPtr constant = nullptr;// Для операции CONST

        std::string label_target;         // Для LABEL, JUMP или ветки TRUE у BRANCH
        std::string label_false;          // Для ветки FALSE у BRANCH
        std::string func_name;            // Имя функции для прямого CALL

        int line = 1;
        int col = 1;

        explicit HIROperation(Opcode op, int l = 1, int c = 1)
            : opcode(op), line(l), col(c) {}

        // Фабричные методы для удобного конструирования
        static HIROperationPtr make_const(HIRObjectPtr dst, HIRConstantPtr val, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::CONST, l, c);
            op->dest = std::move(dst);
            op->constant = std::move(val);
            return op;
        }

        static HIROperationPtr make_binary(Opcode opc, HIRObjectPtr dst, HIRObjectPtr l_obj, HIRObjectPtr r_obj, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(opc, l, c);
            op->dest = std::move(dst);
            op->src1 = std::move(l_obj);
            op->src2 = std::move(r_obj);
            return op;
        }

        static HIROperationPtr make_unary(Opcode opc, HIRObjectPtr dst, HIRObjectPtr src, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(opc, l, c);
            op->dest = std::move(dst);
            op->src1 = std::move(src);
            return op;
        }

        static HIROperationPtr make_load(HIRObjectPtr dst, HIRObjectPtr ptr, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::LOAD, l, c);
            op->dest = std::move(dst);
            op->src1 = std::move(ptr);
            return op;
        }

        static HIROperationPtr make_store(HIRObjectPtr ptr, HIRObjectPtr val, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::STORE, l, c);
            op->src1 = std::move(ptr); // куда пишем
            op->src2 = std::move(val); // что пишем
            return op;
        }

        static HIROperationPtr make_addr_of(HIRObjectPtr dst, HIRObjectPtr src, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::ADDR_OF, l, c);
            op->dest = std::move(dst);
            op->src1 = std::move(src);
            return op;
        }

        static HIROperationPtr make_alloca(HIRObjectPtr dst, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::ALLOCA, l, c);
            op->dest = std::move(dst);
            return op;
        }

        static HIROperationPtr make_gep(HIRObjectPtr dst, HIRObjectPtr arr_ptr, HIRObjectPtr idx, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::GET_ELEM_PTR, l, c);
            op->dest = std::move(dst);
            op->src1 = std::move(arr_ptr);
            op->src2 = std::move(idx);
            return op;
        }

        static HIROperationPtr make_cast(HIRObjectPtr dst, HIRObjectPtr src, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::CAST, l, c);
            op->dest = std::move(dst);
            op->src1 = std::move(src);
            return op;
        }

        static HIROperationPtr make_label(std::string lbl, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::LABEL, l, c);
            op->label_target = std::move(lbl);
            return op;
        }

        static HIROperationPtr make_jump(std::string target, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::JUMP, l, c);
            op->label_target = std::move(target);
            return op;
        }

        static HIROperationPtr make_branch(HIRObjectPtr cond, std::string t_lbl, std::string f_lbl, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::BRANCH, l, c);
            op->src1 = std::move(cond);
            op->label_target = std::move(t_lbl);
            op->label_false  = std::move(f_lbl);
            return op;
        }

        static HIROperationPtr make_call(HIRObjectPtr dst, std::string fn, std::vector<HIRObjectPtr> call_args, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::CALL, l, c);
            op->dest = std::move(dst);
            op->func_name = std::move(fn);
            op->args = std::move(call_args);
            return op;
        }

        static HIROperationPtr make_call_ptr(HIRObjectPtr dst, HIRObjectPtr fn_ptr, std::vector<HIRObjectPtr> call_args, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::CALL_PTR, l, c);
            op->dest = std::move(dst);
            op->src1 = std::move(fn_ptr);
            op->args = std::move(call_args);
            return op;
        }

        static HIROperationPtr make_return(HIRObjectPtr val = nullptr, int l = 1, int c = 1) {
            auto op = std::make_shared<HIROperation>(Opcode::RETURN, l, c);
            op->src1 = std::move(val);
            return op;
        }

        bool is_terminator() const noexcept {
            return opcode == Opcode::JUMP || opcode == Opcode::BRANCH || opcode == Opcode::RETURN;
        }

        bool is_branch_or_jump() const noexcept {
            return opcode == Opcode::JUMP || opcode == Opcode::BRANCH;
        }

        std::string to_string() const;
    };

    // 5. Функция (Function)
    class HIRFunction {
    private:
        size_t temp_counter_ = 0;
        size_t label_counter_ = 0;

    public:
        std::string name;
        TypeNodePtr return_type;
        std::vector<HIRObjectPtr> params;
        std::vector<HIRObjectPtr> local_objects;
        std::vector<HIROperationPtr> operations;

        HIRFunction(std::string n, TypeNodePtr ret_t)
            : name(std::move(n)), return_type(std::move(ret_t)) {}

        // Создание нового автоматического временного значения (SSA-регистра)
        HIRObjectPtr create_temp(TypeNodePtr type) {
            std::string temp_name = "%t" + std::to_string(temp_counter_++);
            auto obj = std::make_shared<HIRObject>(HIRObject::Kind::TEMPORARY, temp_name, std::move(type));
            local_objects.push_back(obj);
            return obj;
        }

        // Создание уникальной метки перехода
        std::string create_label(const std::string& prefix = "L") {
            return prefix + "_" + std::to_string(label_counter_++);
        }

        // Добавление локальной переменной
        HIRObjectPtr add_local_var(const std::string& var_name, TypeNodePtr type) {
            auto obj = std::make_shared<HIRObject>(HIRObject::Kind::VARIABLE, var_name, std::move(type));
            local_objects.push_back(obj);
            return obj;
        }

        // Добавление параметра
        HIRObjectPtr add_param(const std::string& param_name, TypeNodePtr type) {
            auto obj = std::make_shared<HIRObject>(HIRObject::Kind::PARAMETER, param_name, std::move(type));
            params.push_back(obj);
            return obj;
        }

        // Добавление инструкции
        void emit(HIROperationPtr op) {
            operations.push_back(std::move(op));
        }

        std::string to_string() const;
    };

    // 6. Единица трансляции (TranslationUnit)
    class HIRTranslationUnit {
    public:
        std::string name;
        std::vector<HIRObjectPtr> global_objects;
        std::vector<HIRFunctionPtr> functions;

        explicit HIRTranslationUnit(std::string n = "main_module")
            : name(std::move(n)) {}

        void add_global(HIRObjectPtr obj) {
            global_objects.push_back(std::move(obj));
        }

        void add_function(HIRFunctionPtr fn) {
            functions.push_back(std::move(fn));
        }

        HIRFunctionPtr find_function(const std::string& fn_name) const {
            for (const auto& fn : functions) {
                if (fn && fn->name == fn_name) return fn;
            }
            return nullptr;
        }

        std::string to_string() const;
    };

} // namespace to_lang::hir

#endif // TOC_HIR_HPP

