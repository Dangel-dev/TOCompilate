//
// Created by dangel on 15.11.2026.
//

#include "generator.hpp"
#include <cassert>

namespace to_lang::lir {

    LIRTranslationUnitPtr LIRGenerator::generate(const hir::HIRTranslationUnitPtr& hir_unit) {
        auto lir_unit = std::make_shared<LIRTranslationUnit>(hir_unit->name);

        for (const auto& hir_fn : hir_unit->functions) {
            auto lir_fn = lower_function(hir_fn);
            lir_unit->add_function(lir_fn);
        }

        return lir_unit;
    }

    LIRFunctionPtr LIRGenerator::lower_function(const hir::HIRFunctionPtr& hir_fn) {
        LIRGenerator gen;
        gen.current_hir_fn_ = hir_fn;
        gen.lir_fn_ = std::make_shared<LIRFunction>(hir_fn->name, hir_fn->return_type);

        // 1. Поиск переменных, чей адрес брался через &x
        gen.analyze_addressed_objects();

        // 2. Разметка стекового кадра
        gen.layout_stack_frame();

        // 3. Понижение инструкций
        gen.lower_operations();

        return gen.lir_fn_;
    }

    size_t LIRGenerator::compute_type_size(const TypeNodePtr& type) {
        if (!type) return 8;

        if (type->kind == TypeNode::Kind::BASE) {
            switch (type->base_kind) {
                case BaseTypeKind::CHAR:
                case BaseTypeKind::BOOL:
                    return 1;
                case BaseTypeKind::I32:
                    return 4;
                case BaseTypeKind::I64:
                    return 8;
                case BaseTypeKind::VOID:
                    return 0;
            }
        }

        if (type->kind == TypeNode::Kind::POINTER) {
            return 8; // 64-битные указатели
        }

        if (type->kind == TypeNode::Kind::ARRAY) {
            size_t elem_sz = compute_type_size(type->subtype);
            return elem_sz * (type->array_size > 0 ? type->array_size : 1);
        }

        return 8;
    }

    void LIRGenerator::analyze_addressed_objects() {
        // Сканируем все операции в поисках ADDR_OF
        for (const auto& op : current_hir_fn_->operations) {
            if (op->opcode == hir::Opcode::ADDR_OF && op->src1) {
                addressed_objects_.insert(op->src1->name);
            }
        }
    }

    void LIRGenerator::layout_stack_frame() {
        // 1. Поиск всех ALLOCA операций в функции
        for (const auto& op : current_hir_fn_->operations) {
            if (op->opcode == hir::Opcode::ALLOCA && op->dest) {
                std::string name = op->dest->name;
                TypeNodePtr elem_type = (op->dest->type && op->dest->type->kind == TypeNode::Kind::POINTER)
                                            ? op->dest->type->subtype
                                            : op->dest->type;

                bool is_array = elem_type && elem_type->kind == TypeNode::Kind::ARRAY;
                bool is_addressed = addressed_objects_.find(name) != addressed_objects_.end();

                // Массивы и переменные со взятием адреса обязаны лежать на стеке
                if (is_array || is_addressed) {
                    size_t sz = compute_type_size(elem_type);
                    int off = lir_fn_->frame.allocate_slot(name, sz, is_addressed);
                    object_locations_[name] = Operand::make_mem(RegID::BP, off);
                } else {
                    // Скаляры живут в виртуальных регистрах
                    int vreg = lir_fn_->alloc_vreg();
                    object_locations_[name] = Operand::make_vreg(vreg);
                    reg_variables_.insert(name);
                }
            }
        }

        // 2. Параметры функции (первые 4 в r0..r3, если скаляры)
        for (size_t i = 0; i < current_hir_fn_->params.size(); ++i) {
            const auto& param = current_hir_fn_->params[i];
            if (object_locations_.find(param->name) != object_locations_.end()) continue;

            bool is_addressed = addressed_objects_.find(param->name) != addressed_objects_.end();
            if (is_addressed) {
                size_t sz = compute_type_size(param->type);
                int off = lir_fn_->frame.allocate_slot(param->name, sz, true);
                object_locations_[param->name] = Operand::make_mem(RegID::BP, off);
            } else {
                int vreg = lir_fn_->alloc_vreg();
                object_locations_[param->name] = Operand::make_vreg(vreg);
                reg_variables_.insert(param->name);
            }
        }
    }

    Operand LIRGenerator::get_or_create_operand(const hir::HIRObjectPtr& obj) {
        if (!obj) return Operand::none();

        auto it = object_locations_.find(obj->name);
        if (it != object_locations_.end()) {
            return it->second;
        }

        // Временные SSA регистры всегда в vreg
        int vreg = lir_fn_->alloc_vreg();
        Operand op = Operand::make_vreg(vreg);
        object_locations_[obj->name] = op;
        return op;
    }

    void LIRGenerator::lower_operations() {
        auto sp = Operand::make_reg(RegID::SP);
        auto bp = Operand::make_reg(RegID::BP);

        // 1. Пролог функции
        lir_fn_->emit(Opcode::STORE, Operand::make_mem(RegID::SP, -8), bp, Operand::none(), "", "Сохранение bp");
        lir_fn_->emit(Opcode::MOV, bp, sp, Operand::none(), "", "Установка кадра");
        if (lir_fn_->frame.total_stack_size > 0) {
            lir_fn_->emit(Opcode::SUB, sp, sp, Operand::make_imm(lir_fn_->frame.total_stack_size), "", "Выделение стека");
        }

        // Копирование параметров из входных регистров r0..r3
        for (size_t i = 0; i < current_hir_fn_->params.size() && i < 4; ++i) {
            const auto& param = current_hir_fn_->params[i];
            auto src_reg = Operand::make_reg(static_cast<RegID>(static_cast<int>(RegID::R0) + i));
            auto loc = get_or_create_operand(param);
            lir_fn_->emit(Opcode::MOV, loc, src_reg, Operand::none(), "", "Перенос аргумента");
        }

        // 2. Тело функции
        for (const auto& op : current_hir_fn_->operations) {
            switch (op->opcode) {
                case hir::Opcode::CONST: {
                    auto dst = get_or_create_operand(op->dest);
                    int64_t val = 0;
                    if (op->constant) {
                        if (op->constant->kind == hir::HIRConstant::Kind::INT) val = op->constant->int_val;
                        else if (op->constant->kind == hir::HIRConstant::Kind::BOOL) val = op->constant->bool_val ? 1 : 0;
                        else if (op->constant->kind == hir::HIRConstant::Kind::CHAR) val = op->constant->char_val;
                    }
                    lir_fn_->emit(Opcode::MOV, dst, Operand::make_imm(val));
                    break;
                }

                case hir::Opcode::ALLOCA:
                    // Память уже выделена на этапе layout_stack_frame
                    break;

                case hir::Opcode::ADDR_OF: {
                    auto dst = get_or_create_operand(op->dest);
                    auto src = get_or_create_operand(op->src1);
                    if (src.kind == Operand::Kind::MEM) {
                        lir_fn_->emit(Opcode::LEA, dst, src, Operand::none(), "", "Взятие адреса &");
                    }
                    break;
                }

                case hir::Opcode::LOAD: {
                    auto dst = get_or_create_operand(op->dest);
                    auto ptr = get_or_create_operand(op->src1);

                    if (ptr.kind == Operand::Kind::MEM) {
                        lir_fn_->emit(Opcode::LOAD, dst, ptr);
                    } else if (reg_variables_.find(op->src1->name) != reg_variables_.end()) {
                        lir_fn_->emit(Opcode::MOV, dst, ptr);
                    } else {
                        lir_fn_->emit(Opcode::LOAD, dst, ptr);
                    }
                    break;
                }

                case hir::Opcode::STORE: {
                    auto ptr = get_or_create_operand(op->src1);
                    auto val = get_or_create_operand(op->src2);

                    if (ptr.kind == Operand::Kind::MEM) {
                        lir_fn_->emit(Opcode::STORE, ptr, val);
                    } else if (reg_variables_.find(op->src1->name) != reg_variables_.end()) {
                        lir_fn_->emit(Opcode::MOV, ptr, val);
                    } else {
                        lir_fn_->emit(Opcode::STORE, ptr, val);
                    }
                    break;
                }

                case hir::Opcode::GET_ELEM_PTR: {
                    // arr[idx]: LEA / base + idx * sizeof(elem)
                    auto dst = get_or_create_operand(op->dest);
                    auto base = get_or_create_operand(op->src1);
                    auto idx = get_or_create_operand(op->src2);

                    size_t elem_sz = 8;
                    if (op->src1 && op->src1->type && op->src1->type->kind == TypeNode::Kind::ARRAY) {
                        elem_sz = compute_type_size(op->src1->type->subtype);
                    }

                    // Если база в стеке, берем lea
                    Operand base_reg = base;
                    if (base.kind == Operand::Kind::MEM) {
                        int v_base = lir_fn_->alloc_vreg();
                        base_reg = Operand::make_vreg(v_base);
                        lir_fn_->emit(Opcode::LEA, base_reg, base, Operand::none(), "", "Базовый адрес массива");
                    }

                    // Смещение = idx * elem_sz
                    int v_off = lir_fn_->alloc_vreg();
                    auto off_op = Operand::make_vreg(v_off);
                    lir_fn_->emit(Opcode::MUL, off_op, idx, Operand::make_imm(elem_sz));

                    // Итоговый адрес = base + offset
                    lir_fn_->emit(Opcode::ADD, dst, base_reg, off_op, "", "Адрес элемента массива");
                    break;
                }

                case hir::Opcode::ADD:
                case hir::Opcode::SUB:
                case hir::Opcode::MUL:
                case hir::Opcode::DIV:
                case hir::Opcode::MOD:
                case hir::Opcode::AND:
                case hir::Opcode::OR:
                case hir::Opcode::XOR:
                case hir::Opcode::SHL:
                case hir::Opcode::SHR: {
                    auto dst = get_or_create_operand(op->dest);
                    auto s1 = get_or_create_operand(op->src1);
                    auto s2 = get_or_create_operand(op->src2);

                    Opcode lir_op = Opcode::ADD;
                    if (op->opcode == hir::Opcode::SUB) lir_op = Opcode::SUB;
                    else if (op->opcode == hir::Opcode::MUL) lir_op = Opcode::MUL;
                    else if (op->opcode == hir::Opcode::DIV) lir_op = Opcode::DIV;
                    else if (op->opcode == hir::Opcode::MOD) lir_op = Opcode::MOD;
                    else if (op->opcode == hir::Opcode::AND) lir_op = Opcode::AND;
                    else if (op->opcode == hir::Opcode::OR)  lir_op = Opcode::OR;
                    else if (op->opcode == hir::Opcode::XOR) lir_op = Opcode::XOR;
                    else if (op->opcode == hir::Opcode::SHL) lir_op = Opcode::SHL;
                    else if (op->opcode == hir::Opcode::SHR) lir_op = Opcode::SHR;

                    lir_fn_->emit(lir_op, dst, s1, s2);
                    break;
                }

                case hir::Opcode::NEG:
                    lir_fn_->emit(Opcode::NEG, get_or_create_operand(op->dest), get_or_create_operand(op->src1));
                    break;

                case hir::Opcode::NOT:
                    lir_fn_->emit(Opcode::NOT, get_or_create_operand(op->dest), get_or_create_operand(op->src1));
                    break;

                case hir::Opcode::EQ:
                case hir::Opcode::NE:
                case hir::Opcode::LT:
                case hir::Opcode::LE:
                case hir::Opcode::GT:
                case hir::Opcode::GE: {
                    auto dst = get_or_create_operand(op->dest);
                    auto s1 = get_or_create_operand(op->src1);
                    auto s2 = get_or_create_operand(op->src2);

                    Opcode cmp_op = Opcode::CMP_EQ;
                    if (op->opcode == hir::Opcode::NE) cmp_op = Opcode::CMP_NE;
                    else if (op->opcode == hir::Opcode::LT) cmp_op = Opcode::CMP_LT;
                    else if (op->opcode == hir::Opcode::LE) cmp_op = Opcode::CMP_LE;
                    else if (op->opcode == hir::Opcode::GT) cmp_op = Opcode::CMP_GT;
                    else if (op->opcode == hir::Opcode::GE) cmp_op = Opcode::CMP_GE;

                    lir_fn_->emit(cmp_op, dst, s1, s2);
                    break;
                }

                case hir::Opcode::LABEL:
                    lir_fn_->emit(Opcode::LABEL, Operand::none(), Operand::none(), Operand::none(), op->label_target);
                    break;

                case hir::Opcode::JUMP:
                    lir_fn_->emit(Opcode::JMP, Operand::none(), Operand::none(), Operand::none(), op->label_target);
                    break;

                case hir::Opcode::BRANCH: {
                    auto cond = get_or_create_operand(op->src1);
                    lir_fn_->emit(Opcode::JNZ, Operand::none(), cond, Operand::none(), op->label_target);
                    lir_fn_->emit(Opcode::JMP, Operand::none(), Operand::none(), Operand::none(), op->label_false);
                    break;
                }

                case hir::Opcode::CALL: {
                    // Передача аргументов в r0..r3
                    for (size_t i = 0; i < op->args.size() && i < 4; ++i) {
                        auto arg_val = get_or_create_operand(op->args[i]);
                        auto reg = Operand::make_reg(static_cast<RegID>(static_cast<int>(RegID::R0) + i));
                        lir_fn_->emit(Opcode::MOV, reg, arg_val);
                    }

                    lir_fn_->emit(Opcode::CALL, Operand::none(), Operand::none(), Operand::none(), op->func_name);

                    // Результат в r0
                    if (op->dest) {
                        auto dst = get_or_create_operand(op->dest);
                        lir_fn_->emit(Opcode::MOV, dst, Operand::make_reg(RegID::R0));
                    }
                    break;
                }

                case hir::Opcode::RETURN: {
                    if (op->src1) {
                        auto val = get_or_create_operand(op->src1);
                        lir_fn_->emit(Opcode::MOV, Operand::make_reg(RegID::R0), val, Operand::none(), "", "Возврат в r0");
                    }

                    // Эпилог
                    lir_fn_->emit(Opcode::MOV, sp, bp, Operand::none(), "", "Восстановление sp");
                    lir_fn_->emit(Opcode::LOAD, bp, Operand::make_mem(RegID::SP, -8), Operand::none(), "", "Восстановление bp");
                    lir_fn_->emit(Opcode::RET);
                    break;
                }

                default:
                    break;
            }
        }
    }

} // namespace to_lang::lir
