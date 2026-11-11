//
// Created by dangel on 11.11.2026.
//

#include "../hir.hpp"
#include "../verifier.hpp"
#include "../generator.hpp"
#include "../cfg.hpp"
#include "../../frontend/parser/parser.hpp"
#include <cassert>
#include <iostream>

using namespace to_lang::hir;

// Задача 4.1: Линейное IR — проверка всех категорий операций
void test_all_operation_categories() {
    auto unit = std::make_shared<HIRTranslationUnit>("full_categories_module");

    auto i32_t   = to_lang::TypeNode::make_base(to_lang::BaseTypeKind::I32);
    auto i64_t   = to_lang::TypeNode::make_base(to_lang::BaseTypeKind::I64);
    auto bool_t  = to_lang::TypeNode::make_base(to_lang::BaseTypeKind::BOOL);
    auto ptr_i32 = to_lang::TypeNode::make_pointer(i32_t);
    auto arr_t   = to_lang::TypeNode::make_array(i32_t, 10);

    auto fn = std::make_shared<HIRFunction>("test_categories", i32_t);
    unit->add_function(fn);

    // 1. Константы
    auto c_int = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_const(c_int, HIRConstant::make_int(42, i32_t)));

    auto c_bool = fn->create_temp(bool_t);
    fn->emit(HIROperation::make_const(c_bool, HIRConstant::make_bool(true)));

    // 2. Численная и битовая арифметика
    auto t_add = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::ADD, t_add, c_int, c_int));

    auto t_sub = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::SUB, t_sub, t_add, c_int));

    auto t_mul = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::MUL, t_mul, t_sub, c_int));

    auto t_div = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::DIV, t_div, t_mul, c_int));

    auto t_mod = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::MOD, t_mod, t_div, c_int));

    auto t_and = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::AND, t_and, t_mod, c_int));

    auto t_or = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::OR, t_or, t_and, c_int));

    auto t_xor = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::XOR, t_xor, t_or, c_int));

    auto t_shl = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::SHL, t_shl, t_xor, c_int));

    auto t_shr = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_binary(Opcode::SHR, t_shr, t_shl, c_int));

    auto t_neg = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_unary(Opcode::NEG, t_neg, t_shr));

    auto t_not = fn->create_temp(bool_t);
    fn->emit(HIROperation::make_unary(Opcode::NOT, t_not, c_bool));

    // 3. Сравнения
    auto t_eq = fn->create_temp(bool_t);
    fn->emit(HIROperation::make_binary(Opcode::EQ, t_eq, t_neg, c_int));

    auto t_lt = fn->create_temp(bool_t);
    fn->emit(HIROperation::make_binary(Opcode::LT, t_lt, t_neg, c_int));

    // 4. Память и указатели
    auto slot = fn->create_temp(ptr_i32);
    fn->emit(HIROperation::make_alloca(slot));

    fn->emit(HIROperation::make_store(slot, c_int));

    auto loaded = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_load(loaded, slot));

    auto arr_slot = fn->create_temp(to_lang::TypeNode::make_pointer(arr_t));
    fn->emit(HIROperation::make_alloca(arr_slot));

    auto elem_ptr = fn->create_temp(ptr_i32);
    fn->emit(HIROperation::make_gep(elem_ptr, arr_slot, c_int));

    // 5. Преобразование типа
    auto t_cast = fn->create_temp(i64_t);
    fn->emit(HIROperation::make_cast(t_cast, loaded));

    // 6. Переходы
    fn->emit(HIROperation::make_jump("L_target"));
    fn->emit(HIROperation::make_label("L_target"));

    fn->emit(HIROperation::make_branch(t_eq, "L_true", "L_false"));
    fn->emit(HIROperation::make_label("L_true"));
    fn->emit(HIROperation::make_jump("L_exit"));

    fn->emit(HIROperation::make_label("L_false"));
    fn->emit(HIROperation::make_jump("L_exit"));

    fn->emit(HIROperation::make_label("L_exit"));

    // 7. Вызов функции
    auto call_res = fn->create_temp(i32_t);
    fn->emit(HIROperation::make_call(call_res, "printi", { c_int }));

    fn->emit(HIROperation::make_return(loaded));

    // Валидация верификатором
    HIRVerifier verifier;
    bool ok = verifier.verify(unit);
    if (!ok) {
        for (const auto& e : verifier.get_errors()) {
            std::cerr << "Verification error: " << e.format() << "\n";
        }
    }
    assert(ok);
    assert(!verifier.has_errors());

    std::string text = unit->to_string();
    assert(text.find("alloca") != std::string::npos);
    assert(text.find("store") != std::string::npos);
    assert(text.find("load") != std::string::npos);
    assert(text.find("branch") != std::string::npos);
    assert(text.find("call") != std::string::npos);
}

// Задача 4.2: Проверки верификатора (Use-Before-Def, типы, инварианты)
void test_verifier_strict_checks() {
    auto i32_t   = to_lang::TypeNode::make_base(to_lang::BaseTypeKind::I32);
    auto bool_t  = to_lang::TypeNode::make_base(to_lang::BaseTypeKind::BOOL);
    auto void_t  = to_lang::TypeNode::make_base(to_lang::BaseTypeKind::VOID);
    auto ptr_i32 = to_lang::TypeNode::make_pointer(i32_t);

    // 1. Use-Before-Def
    {
        auto fn = std::make_shared<HIRFunction>("ubd_func", i32_t);
        auto uninit_t = std::make_shared<HIRObject>(HIRObject::Kind::TEMPORARY, "%t_ghost", i32_t);
        auto dst = fn->create_temp(i32_t);

        fn->emit(HIROperation::make_unary(Opcode::NEG, dst, uninit_t));
        fn->emit(HIROperation::make_return(dst));

        HIRVerifier v;
        assert(!v.verify_function(fn));
        bool found = false;
        for (const auto& err : v.get_errors()) {
            if (err.message.find("Use-before-def") != std::string::npos) found = true;
        }
        assert(found);
    }

    // 2. STORE в не-указатель
    {
        auto fn = std::make_shared<HIRFunction>("bad_store", void_t);
        auto t1 = fn->create_temp(i32_t);
        auto t2 = fn->create_temp(i32_t);
        fn->emit(HIROperation::make_const(t1, HIRConstant::make_int(1, i32_t)));
        fn->emit(HIROperation::make_const(t2, HIRConstant::make_int(2, i32_t)));
        fn->emit(HIROperation::make_store(t1, t2)); // t1 не указатель!
        fn->emit(HIROperation::make_return(nullptr));

        HIRVerifier v;
        assert(!v.verify_function(fn));
        bool found = false;
        for (const auto& err : v.get_errors()) {
            if (err.message.find("STORE target must be a pointer") != std::string::npos) found = true;
        }
        assert(found);
    }

    // 3. LOAD из не-указателя
    {
        auto fn = std::make_shared<HIRFunction>("bad_load", void_t);
        auto t1 = fn->create_temp(i32_t);
        auto dst = fn->create_temp(i32_t);
        fn->emit(HIROperation::make_const(t1, HIRConstant::make_int(10, i32_t)));
        fn->emit(HIROperation::make_load(dst, t1)); // t1 не указатель!
        fn->emit(HIROperation::make_return(nullptr));

        HIRVerifier v;
        assert(!v.verify_function(fn));
        bool found = false;
        for (const auto& err : v.get_errors()) {
            if (err.message.find("LOAD source must be a pointer") != std::string::npos) found = true;
        }
        assert(found);
    }

    // 4. Переход на неизвестную метку
    {
        auto fn = std::make_shared<HIRFunction>("bad_jump", void_t);
        fn->emit(HIROperation::make_jump("L_nowhere"));
        fn->emit(HIROperation::make_return(nullptr));

        HIRVerifier v;
        assert(!v.verify_function(fn));
        bool found = false;
        for (const auto& err : v.get_errors()) {
            if (err.message.find("undefined label") != std::string::npos) found = true;
        }
        assert(found);
    }

    // 5. Дублирование меток
    {
        auto fn = std::make_shared<HIRFunction>("dup_label", void_t);
        fn->emit(HIROperation::make_label("L1"));
        fn->emit(HIROperation::make_label("L1"));
        fn->emit(HIROperation::make_return(nullptr));

        HIRVerifier v;
        assert(!v.verify_function(fn));
        bool found = false;
        for (const auto& err : v.get_errors()) {
            if (err.message.find("Duplicate label") != std::string::npos) found = true;
        }
        assert(found);
    }

    // 6. Несоответствие возврата: void-функция возвращает значение
    {
        auto fn = std::make_shared<HIRFunction>("void_returns_val", void_t);
        auto t_val = fn->create_temp(i32_t);
        fn->emit(HIROperation::make_const(t_val, HIRConstant::make_int(5, i32_t)));
        fn->emit(HIROperation::make_return(t_val));

        HIRVerifier v;
        assert(!v.verify_function(fn));
        bool found = false;
        for (const auto& err : v.get_errors()) {
            if (err.message.find("Void function cannot return a value") != std::string::npos) found = true;
        }
        assert(found);
    }
}

// Задача 4.3: Генератор из AST (сквозные тесты на реальных программах)
void test_ast_to_hir_generation() {
    std::string test_files[] = {
        "examples/fibonacci.to",
        "examples/array_algorithms.to",
        "examples/string_utils.to",
        "frontend/tests/valid/example_main.to",
        "frontend/tests/valid/expressions.to",
        "frontend/tests/valid/functions.to"
    };

    for (const auto& filepath : test_files) {
        auto parse_res = to_lang::parse_file(filepath);
        assert(parse_res.success);
        assert(parse_res.root != nullptr);

        HIRGenerator generator;
        auto unit = generator.generate(parse_res.root);
        assert(unit != nullptr);
        assert(!unit->functions.empty());

        // Верификация сгенерированного промежуточного кода
        HIRVerifier verifier;
        bool ok = verifier.verify(unit);
        if (!ok) {
            std::cerr << "Verification failed for " << filepath << ":\n";
            for (const auto& err : verifier.get_errors()) {
                std::cerr << "  " << err.format() << "\n";
            }
        }
        assert(ok);
        assert(!verifier.has_errors());

        // Проверка непустой сериализации
        std::string ir_text = unit->to_string();
        assert(!ir_text.empty());
    }
}

// Задача 4.4: Граф потока управления (CFG) и его инварианты
void test_cfg_construction_and_invariants() {
    std::string code = R"(
        i32 compute(i32 x) {
            i32 total = 0;
            i32 i = 0;
            while (i < x) {
                if (i % 2 == 0) {
                    total += i;
                } else {
                    total -= 1;
                }
                i += 1;
            }
            return total;
        }
    )";

    auto parse_res = to_lang::parse_string(code);
    assert(parse_res.success);

    HIRGenerator gen;
    auto unit = gen.generate(parse_res.root);
    assert(unit->functions.size() == 1);

    auto fn = unit->functions[0];
    auto cfg = ControlFlowGraph::build(fn);
    assert(cfg != nullptr);

    const auto& graph = cfg->get_graph();

    // Проверка инвариантов CFG
    assert(graph.nodes_count() >= 5);
    assert(graph.edges_count() >= 5);

    // 1. Каждый базовый блок начинается с метки
    for (auto it = graph.nodes_begin(); it != graph.nodes_end(); ++it) {
        const auto& bb = it->get();
        assert(!bb.label.empty());
        assert(!bb.instructions.empty());
        assert(bb.instructions.front()->opcode == Opcode::LABEL);

        // 2. Каждый базовый блок заканчивается терминатором
        auto last_op = bb.instructions.back();
        assert(last_op->is_terminator());

        // 3. Исходящие ребра соответствуют типу терминатора
        if (last_op->opcode == Opcode::BRANCH) {
            assert(it->get_out_edges().size() == 2);
        } else if (last_op->opcode == Opcode::JUMP) {
            assert(it->get_out_edges().size() == 1);
        } else if (last_op->opcode == Opcode::RETURN) {
            assert(it->get_out_edges().empty());
            assert(bb.is_exit);
        }
    }

    // 4. Входной узел
    assert(cfg->get_entry()->get().is_entry);
    assert(cfg->get_entry()->get_in_edges().empty());

    // 5. Сериализация в Graphviz DOT
    std::string dot = cfg->to_dot();
    assert(!dot.empty());
    assert(dot.find("digraph CFG_compute") != std::string::npos);
    assert(dot.find("true") != std::string::npos);
    assert(dot.find("false") != std::string::npos);
}

int main() {
    test_all_operation_categories();
    test_verifier_strict_checks();
    test_ast_to_hir_generation();
    test_cfg_construction_and_invariants();

    std::cout << "All HIR tests successfully passed!" << std::endl;
    return 0;
}
