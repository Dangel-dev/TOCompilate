//
// Created by dangel on 11.11.2026.
//

#include "verifier.hpp"
#include "../frontend/semantics/type_utils.hpp"
#include <unordered_set>

namespace to_lang::hir {

    void HIRVerifier::error(const std::string& msg, int l, int c) {
        VerificationError err;
        err.message = msg;
        err.function_name = current_func_ ? current_func_->name : "unknown";
        err.line = l;
        err.col = c;
        errors_.push_back(std::move(err));
    }

    void HIRVerifier::check_use(const HIRObjectPtr& obj,
                                const std::unordered_set<std::string>& defined_objects,
                                int l, int c) {
        if (!obj) return;

        // Временные значения (%t) и локальные переменные должны быть определены перед использованием
        if (obj->is_temporary() || obj->is_variable()) {
            if (defined_objects.find(obj->name) == defined_objects.end()) {
                error("Use-before-def: object '" + obj->name + "' is used before being defined", l, c);
            }
        }
    }

    bool HIRVerifier::verify(const HIRTranslationUnitPtr& unit) {
        errors_.clear();
        if (!unit) {
            error("Null translation unit");
            return false;
        }

        std::unordered_set<std::string> func_names;
        for (const auto& fn : unit->functions) {
            if (!fn) continue;
            if (func_names.find(fn->name) != func_names.end()) {
                current_func_ = fn.get();
                error("Duplicate function definition: @" + fn->name);
            }
            func_names.insert(fn->name);
            verify_function(fn);
        }

        return errors_.empty();
    }

    bool HIRVerifier::verify_function(const HIRFunctionPtr& fn) {
        if (!fn) {
            error("Null function");
            return false;
        }

        current_func_ = fn.get();

        // 1. Сбор всех объявленных меток и проверка на дубликаты
        std::unordered_set<std::string> known_labels;
        for (const auto& op : fn->operations) {
            if (op && op->opcode == Opcode::LABEL) {
                if (known_labels.find(op->label_target) != known_labels.end()) {
                    error("Duplicate label '" + op->label_target + "' in function @" + fn->name, op->line, op->col);
                }
                known_labels.insert(op->label_target);
            }
        }

        // 2. Инициализация множества определенных объектов (параметры функции доступны сразу)
        std::unordered_set<std::string> defined_objects;
        for (const auto& p : fn->params) {
            if (p) defined_objects.insert(p->name);
        }

        // 3. Построчная проверка операций: типы и use-before-def
        for (const auto& op : fn->operations) {
            if (!op) continue;
            verify_operation(op, defined_objects, known_labels);
        }

        current_func_ = nullptr;
        return errors_.empty();
    }

    void HIRVerifier::verify_operation(const HIROperationPtr& op,
                                       std::unordered_set<std::string>& defined_objects,
                                       const std::unordered_set<std::string>& known_labels) {
        int l = op->line;
        int c = op->col;

        // Проверка использования операндов (Use-Before-Def)
        if (op->opcode != Opcode::ALLOCA && op->opcode != Opcode::CONST && op->opcode != Opcode::LABEL) {
            if (op->src1) check_use(op->src1, defined_objects, l, c);
            if (op->src2) check_use(op->src2, defined_objects, l, c);
            for (const auto& arg : op->args) {
                check_use(arg, defined_objects, l, c);
            }
        }

        // Проверка типизации по кодам операций
        switch (op->opcode) {
            case Opcode::CONST: {
                if (!op->dest) {
                    error("CONST operation missing destination object", l, c);
                } else if (!op->constant) {
                    error("CONST operation missing constant payload", l, c);
                } else if (op->dest->type && op->constant->type) {
                    if (!can_implicitly_convert(op->constant->type, op->dest->type)) {
                        error("CONST type mismatch: cannot convert constant type " +
                              op->constant->type->to_string() + " to destination type " +
                              op->dest->type->to_string(), l, c);
                    }
                }
                break;
            }

            case Opcode::ALLOCA: {
                if (!op->dest) {
                    error("ALLOCA missing destination pointer", l, c);
                } else if (!is_pointer_type(op->dest->type)) {
                    error("ALLOCA destination must be a pointer type, got " +
                          (op->dest->type ? op->dest->type->to_string() : "void"), l, c);
                }
                break;
            }

            case Opcode::LOAD: {
                if (!op->dest || !op->src1) {
                    error("LOAD missing destination or source pointer", l, c);
                } else if (!is_pointer_type(op->src1->type)) {
                    error("LOAD source must be a pointer, got " +
                          (op->src1->type ? op->src1->type->to_string() : "void"), l, c);
                } else {
                    auto pointee_t = op->src1->type->subtype;
                    if (!can_implicitly_convert(pointee_t, op->dest->type)) {
                        error("LOAD type mismatch: pointee " + pointee_t->to_string() +
                              " cannot convert to destination " + op->dest->type->to_string(), l, c);
                    }
                }
                break;
            }

            case Opcode::STORE: {
                if (!op->src1 || !op->src2) {
                    error("STORE missing target pointer or source value", l, c);
                } else if (!is_pointer_type(op->src1->type)) {
                    error("STORE target must be a pointer, got " +
                          (op->src1->type ? op->src1->type->to_string() : "void"), l, c);
                } else {
                    auto pointee_t = op->src1->type->subtype;
                    if (!can_implicitly_convert(op->src2->type, pointee_t)) {
                        error("STORE type mismatch: cannot store " + op->src2->type->to_string() +
                              " into pointee type " + pointee_t->to_string(), l, c);
                    }
                }
                break;
            }

            case Opcode::ADD:
            case Opcode::SUB:
            case Opcode::MUL:
            case Opcode::DIV:
            case Opcode::MOD:
            case Opcode::AND:
            case Opcode::OR:
            case Opcode::XOR:
            case Opcode::SHL:
            case Opcode::SHR: {
                if (!op->dest || !op->src1 || !op->src2) {
                    error("Binary operation missing operands", l, c);
                    break;
                }
                auto lt = op->src1->type;
                auto rt = op->src2->type;

                // Указательная арифметика
                if (op->opcode == Opcode::ADD && (is_pointer_type(lt) || is_pointer_type(rt))) {
                    if (is_pointer_type(lt) && is_integer_type(rt)) {
                        if (!are_types_equal(op->dest->type, lt)) {
                            error("ADD ptr + int destination must match pointer type", l, c);
                        }
                    } else if (is_integer_type(lt) && is_pointer_type(rt)) {
                        if (!are_types_equal(op->dest->type, rt)) {
                            error("ADD int + ptr destination must match pointer type", l, c);
                        }
                    } else {
                        error("Invalid pointer addition: both operands cannot be pointers", l, c);
                    }
                } else if (op->opcode == Opcode::SUB && is_pointer_type(lt)) {
                    if (is_integer_type(rt)) {
                        if (!are_types_equal(op->dest->type, lt)) {
                            error("SUB ptr - int destination must match pointer type", l, c);
                        }
                    } else if (is_pointer_type(rt)) {
                        if (!is_integer_type(op->dest->type)) {
                            error("SUB ptr - ptr destination must be integer", l, c);
                        }
                    }
                } else {
                    bool allow_bool = (op->opcode == Opcode::AND || op->opcode == Opcode::OR);
                    if (!is_integer_type(lt) && !is_char_type(lt) && !(allow_bool && is_boolean_type(lt))) {
                        error("Left operand must be integer" + std::string(allow_bool ? " or bool" : "") +
                              ", got " + (lt ? lt->to_string() : "void"), l, c);
                    }
                    if (!is_integer_type(rt) && !is_char_type(rt) && !(allow_bool && is_boolean_type(rt))) {
                        error("Right operand must be integer" + std::string(allow_bool ? " or bool" : "") +
                              ", got " + (rt ? rt->to_string() : "void"), l, c);
                    }
                }
                break;
            }

            case Opcode::EQ:
            case Opcode::NE:
            case Opcode::LT:
            case Opcode::LE:
            case Opcode::GT:
            case Opcode::GE: {
                if (!op->dest || !op->src1 || !op->src2) {
                    error("Comparison missing operands", l, c);
                } else if (!is_boolean_type(op->dest->type)) {
                    error("Comparison destination must be bool", l, c);
                }
                break;
            }

            case Opcode::GET_ELEM_PTR: {
                if (!op->dest || !op->src1 || !op->src2) {
                    error("GET_ELEM_PTR missing operands", l, c);
                } else if (!is_pointer_type(op->src1->type) && !is_array_type(op->src1->type)) {
                    error("GET_ELEM_PTR base must be array or pointer, got " +
                          (op->src1->type ? op->src1->type->to_string() : "void"), l, c);
                } else if (!is_integer_type(op->src2->type)) {
                    error("GET_ELEM_PTR index must be integer, got " +
                          (op->src2->type ? op->src2->type->to_string() : "void"), l, c);
                }
                break;
            }

            case Opcode::JUMP: {
                if (op->label_target.empty()) {
                    error("JUMP missing target label", l, c);
                } else if (known_labels.find(op->label_target) == known_labels.end()) {
                    error("JUMP to undefined label '" + op->label_target + "'", l, c);
                }
                break;
            }

            case Opcode::BRANCH: {
                if (!op->src1) {
                    error("BRANCH missing condition object", l, c);
                } else if (!is_boolean_type(op->src1->type) && !is_integer_type(op->src1->type)) {
                    error("BRANCH condition must be bool or integer, got " +
                          (op->src1->type ? op->src1->type->to_string() : "void"), l, c);
                } else if (known_labels.find(op->label_target) == known_labels.end()) {
                    error("BRANCH true target to undefined label '" + op->label_target + "'", l, c);
                } else if (known_labels.find(op->label_false) == known_labels.end()) {
                    error("BRANCH false target to undefined label '" + op->label_false + "'", l, c);
                }
                break;
            }

            case Opcode::RETURN: {
                if (current_func_) {
                    bool is_void_fn = is_void_type(current_func_->return_type);
                    if (is_void_fn && op->src1) {
                        error("Void function cannot return a value", l, c);
                    } else if (!is_void_fn && !op->src1) {
                        error("Non-void function must return a value of type " +
                              current_func_->return_type->to_string(), l, c);
                    } else if (op->src1 && !can_implicitly_convert(op->src1->type, current_func_->return_type)) {
                        error("Return type mismatch: expected " + current_func_->return_type->to_string() +
                              ", got " + op->src1->type->to_string(), l, c);
                    }
                }
                break;
            }

            default:
                break;
        }

        // Фиксация определения нового объекта в destination
        if (op->dest) {
            defined_objects.insert(op->dest->name);
        }
    }

} // namespace to_lang::hir

