//
// Created by dangel on 15.11.2026.
//

#include "../lir.hpp"
#include "../generator.hpp"
#include "../../frontend/parser/parser.hpp"
#include "../../frontend/semantics/semantic_analyzer.hpp"
#include "../../hir/generator.hpp"
#include <cassert>
#include <iostream>

using namespace to_lang;
using namespace to_lang::lir;

// 1. Тест вычисления смещений и выравнивания стекового кадра
void test_frame_layout_calculation() {
    FrameLayout frame;

    // x: i32 (4 байта) -> 8 выровнено
    int off_x = frame.allocate_slot("x", 4, true);
    assert(off_x == -8);
    assert(frame.total_stack_size == 8);

    // arr: 10 * 4 = 40 байт -> 40 выровнено
    int off_arr = frame.allocate_slot("arr", 40, false);
    assert(off_arr == -48);
    assert(frame.total_stack_size == 48);

    assert(frame.get_offset("x") == -8);
    assert(frame.get_offset("arr") == -48);
    assert(frame.has_slot("x"));
    assert(!frame.has_slot("unknown"));

    std::cout << "[PASS] test_frame_layout_calculation" << std::endl;
}

// 2. Тест: скалярная переменная без взятия адреса остается в регистрах
void test_scalar_variable_in_vreg() {
    auto unit = std::make_shared<hir::HIRTranslationUnit>("test_module");
    auto i32_t = TypeNode::make_base(BaseTypeKind::I32);
    auto fn = std::make_shared<hir::HIRFunction>("test_scalar", i32_t);
    unit->add_function(fn);

    auto x = std::make_shared<hir::HIRObject>(hir::HIRObject::Kind::VARIABLE, "x", i32_t);
    fn->local_objects.push_back(x);
    fn->emit(hir::HIROperation::make_alloca(x));

    auto c10 = fn->create_temp(i32_t);
    fn->emit(hir::HIROperation::make_const(c10, hir::HIRConstant::make_int(10)));
    fn->emit(hir::HIROperation::make_store(x, c10));

    auto lir_unit = LIRGenerator::generate(unit);
    assert(lir_unit->functions.size() == 1);
    auto& l_fn = lir_unit->functions[0];

    // Так как адрес x не брался, стек пуст
    assert(l_fn->frame.total_stack_size == 0);
    assert(!l_fn->frame.has_slot("x"));

    std::cout << "[PASS] test_scalar_variable_in_vreg" << std::endl;
}

// 3. Тест: переменная со взятием адреса (&x) обязана лечь в стек
void test_address_taken_variable_on_stack() {
    auto unit = std::make_shared<hir::HIRTranslationUnit>("test_module");
    auto i32_t = TypeNode::make_base(BaseTypeKind::I32);
    auto ptr_t = TypeNode::make_pointer(i32_t);
    auto fn = std::make_shared<hir::HIRFunction>("test_addr", i32_t);
    unit->add_function(fn);

    auto x = std::make_shared<hir::HIRObject>(hir::HIRObject::Kind::VARIABLE, "x", i32_t);
    fn->local_objects.push_back(x);
    fn->emit(hir::HIROperation::make_alloca(x));

    auto p = std::make_shared<hir::HIRObject>(hir::HIRObject::Kind::VARIABLE, "p", ptr_t);
    fn->local_objects.push_back(p);
    fn->emit(hir::HIROperation::make_alloca(p));

    // &x
    fn->emit(hir::HIROperation::make_addr_of(p, x));

    auto lir_unit = LIRGenerator::generate(unit);
    auto& l_fn = lir_unit->functions[0];

    // x обязан получить слот стека
    assert(l_fn->frame.has_slot("x"));
    assert(l_fn->frame.get_offset("x") == -8);
    assert(l_fn->frame.total_stack_size >= 8);

    // Должна быть сгенерирована инструкция LEA для взятия адреса
    bool has_lea = false;
    for (const auto& inst : l_fn->instructions) {
        if (inst.op == Opcode::LEA) {
            has_lea = true;
            assert(inst.src1.kind == Operand::Kind::MEM);
            assert(inst.src1.base_reg == RegID::BP);
            assert(inst.src1.mem_offset == -8);
        }
    }
    assert(has_lea);

    std::cout << "[PASS] test_address_taken_variable_on_stack" << std::endl;
}

// 4. Тест: массив всегда аллоцируется на стеке
void test_array_on_stack() {
    auto unit = std::make_shared<hir::HIRTranslationUnit>("test_module");
    auto i32_t = TypeNode::make_base(BaseTypeKind::I32);
    auto arr_t = TypeNode::make_array(i32_t, 10); // 10 * 4 = 40 байт
    auto fn = std::make_shared<hir::HIRFunction>("test_arr_fn", i32_t);
    unit->add_function(fn);

    auto arr = std::make_shared<hir::HIRObject>(hir::HIRObject::Kind::VARIABLE, "buffer", arr_t);
    fn->local_objects.push_back(arr);
    fn->emit(hir::HIROperation::make_alloca(arr));

    auto lir_unit = LIRGenerator::generate(unit);
    auto& l_fn = lir_unit->functions[0];

    assert(l_fn->frame.has_slot("buffer"));
    assert(l_fn->frame.total_stack_size >= 40);

    std::cout << "[PASS] test_array_on_stack" << std::endl;
}

// 5. Тест понижения сложных операций: arr[i] -> LEA + MUL + ADD + STORE
void test_complex_array_indexing_lowering() {
    auto unit = std::make_shared<hir::HIRTranslationUnit>("test_module");
    auto i32_t = TypeNode::make_base(BaseTypeKind::I32);
    auto arr_t = TypeNode::make_array(i32_t, 5);
    auto fn = std::make_shared<hir::HIRFunction>("test_indexing", i32_t);
    unit->add_function(fn);

    auto arr = std::make_shared<hir::HIRObject>(hir::HIRObject::Kind::VARIABLE, "arr", arr_t);
    fn->local_objects.push_back(arr);
    fn->emit(hir::HIROperation::make_alloca(arr));

    auto idx = fn->create_temp(i32_t);
    fn->emit(hir::HIROperation::make_const(idx, hir::HIRConstant::make_int(2)));

    auto elem_ptr = fn->create_temp(TypeNode::make_pointer(i32_t));
    fn->emit(hir::HIROperation::make_gep(elem_ptr, arr, idx));

    auto val = fn->create_temp(i32_t);
    fn->emit(hir::HIROperation::make_const(val, hir::HIRConstant::make_int(42)));
    fn->emit(hir::HIROperation::make_store(elem_ptr, val));

    auto lir_unit = LIRGenerator::generate(unit);
    auto& l_fn = lir_unit->functions[0];

    bool found_lea = false;
    bool found_mul = false;
    bool found_add = false;
    bool found_store = false;

    for (const auto& inst : l_fn->instructions) {
        if (inst.op == Opcode::LEA) found_lea = true;
        if (inst.op == Opcode::MUL) found_mul = true;
        if (inst.op == Opcode::ADD) found_add = true;
        if (inst.op == Opcode::STORE) found_store = true;
    }

    assert(found_lea);
    assert(found_mul);
    assert(found_add);
    assert(found_store);

    std::cout << "[PASS] test_complex_array_indexing_lowering" << std::endl;
}

// 6. Тест пролога и эпилога: сохранение и восстановление bp, sp
void test_prologue_epilogue_generation() {
    auto unit = std::make_shared<hir::HIRTranslationUnit>("test_module");
    auto i32_t = TypeNode::make_base(BaseTypeKind::I32);
    auto arr_t = TypeNode::make_array(i32_t, 8);
    auto fn = std::make_shared<hir::HIRFunction>("test_frame", i32_t);
    unit->add_function(fn);

    auto buf = std::make_shared<hir::HIRObject>(hir::HIRObject::Kind::VARIABLE, "buf", arr_t);
    fn->local_objects.push_back(buf);
    fn->emit(hir::HIROperation::make_alloca(buf));

    auto ret_val = fn->create_temp(i32_t);
    fn->emit(hir::HIROperation::make_const(ret_val, hir::HIRConstant::make_int(0)));
    fn->emit(hir::HIROperation::make_return(ret_val));

    auto lir_unit = LIRGenerator::generate(unit);
    auto& l_fn = lir_unit->functions[0];

    // Первая инструкция пролога: store [sp - 8], bp
    assert(l_fn->instructions[0].op == Opcode::STORE);
    assert(l_fn->instructions[0].dst.kind == Operand::Kind::MEM);
    assert(l_fn->instructions[0].dst.base_reg == RegID::SP);
    assert(l_fn->instructions[0].dst.mem_offset == -8);
    assert(l_fn->instructions[0].src1.reg == RegID::BP);

    // Вторая: mov bp, sp
    assert(l_fn->instructions[1].op == Opcode::MOV);
    assert(l_fn->instructions[1].dst.reg == RegID::BP);
    assert(l_fn->instructions[1].src1.reg == RegID::SP);

    // Третья: sub sp, sp, frame_size
    assert(l_fn->instructions[2].op == Opcode::SUB);
    assert(l_fn->instructions[2].dst.reg == RegID::SP);

    // Эпилог: ret в конце
    assert(l_fn->instructions.back().op == Opcode::RET);

    std::cout << "[PASS] test_prologue_epilogue_generation" << std::endl;
}

// 7. Сквозной тест на реальной программе (fibonacci.to)
void test_end_to_end_fibonacci_lir() {
    auto parse_res = parse_file("examples/fibonacci.to");
    assert(parse_res.success && parse_res.root != nullptr);

    SemanticAnalyzer analyzer;
    bool sem_ok = analyzer.analyze(parse_res.root);
    assert(sem_ok);

    hir::HIRGenerator hir_gen;
    auto hir_unit = hir_gen.generate(parse_res.root);
    assert(hir_unit != nullptr);

    auto lir_unit = LIRGenerator::generate(hir_unit);
    assert(lir_unit != nullptr);
    assert(!lir_unit->functions.empty());

    // Проверяем наличие функции fibonacci
    bool found_fib = false;
    for (const auto& fn : lir_unit->functions) {
        if (fn->name == "fibonacci") {
            found_fib = true;
            assert(!fn->instructions.empty());
            assert(fn->instructions[0].op == Opcode::STORE); // пролог
            assert(fn->instructions.back().op == Opcode::RET); // эпилог
        }
    }
    assert(found_fib);

    std::cout << "[PASS] test_end_to_end_fibonacci_lir" << std::endl;
}

int main() {
    std::cout << "=== Running Lab 05 LIR & Stack Frame Tests ===" << std::endl;
    test_frame_layout_calculation();
    test_scalar_variable_in_vreg();
    test_address_taken_variable_on_stack();
    test_array_on_stack();
    test_complex_array_indexing_lowering();
    test_prologue_epilogue_generation();
    test_end_to_end_fibonacci_lir();
    std::cout << "All Lab 05 LIR tests passed!" << std::endl;
    return 0;
}
