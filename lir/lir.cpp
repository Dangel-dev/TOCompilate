//
// Created by dangel on 15.11.2026.
//

#include "lir.hpp"
#include <iomanip>
#include <sstream>

namespace to_lang::lir {

    std::string reg_to_string(RegID reg, int vreg_id) {
        switch (reg) {
            case RegID::R0:  return "r0";
            case RegID::R1:  return "r1";
            case RegID::R2:  return "r2";
            case RegID::R3:  return "r3";
            case RegID::R4:  return "r4";
            case RegID::R5:  return "r5";
            case RegID::R6:  return "r6";
            case RegID::R7:  return "r7";
            case RegID::R8:  return "r8";
            case RegID::R9:  return "r9";
            case RegID::R10: return "r10";
            case RegID::R11: return "r11";
            case RegID::R12: return "r12";
            case RegID::R13: return "r13";
            case RegID::R14: return "r14";
            case RegID::R15: return "r15";
            case RegID::SP:  return "sp";
            case RegID::BP:  return "bp";
            case RegID::VREG:
                return "v" + std::to_string(vreg_id);
        }
        return "unknown_reg";
    }

    std::string opcode_to_string(Opcode op) {
        switch (op) {
            case Opcode::MOV:    return "mov";
            case Opcode::ADD:    return "add";
            case Opcode::SUB:    return "sub";
            case Opcode::MUL:    return "mul";
            case Opcode::DIV:    return "div";
            case Opcode::MOD:    return "mod";
            case Opcode::AND:    return "and";
            case Opcode::OR:     return "or";
            case Opcode::XOR:    return "xor";
            case Opcode::SHL:    return "shl";
            case Opcode::SHR:    return "shr";
            case Opcode::NEG:    return "neg";
            case Opcode::NOT:    return "not";
            case Opcode::CMP_EQ: return "cmp_eq";
            case Opcode::CMP_NE: return "cmp_ne";
            case Opcode::CMP_LT: return "cmp_lt";
            case Opcode::CMP_LE: return "cmp_le";
            case Opcode::CMP_GT: return "cmp_gt";
            case Opcode::CMP_GE: return "cmp_ge";
            case Opcode::JMP:    return "jmp";
            case Opcode::JZ:     return "jz";
            case Opcode::JNZ:    return "jnz";
            case Opcode::CALL:   return "call";
            case Opcode::CALL_PTR: return "call_ptr";
            case Opcode::RET:    return "ret";
            case Opcode::LOAD:   return "load";
            case Opcode::STORE:  return "store";
            case Opcode::LEA:    return "lea";
            case Opcode::LABEL:  return "label";
        }
        return "unknown_op";
    }

    std::string Operand::to_string() const {
        switch (kind) {
            case Kind::NONE:
                return "";
            case Kind::REG:
                return reg_to_string(reg, vreg_id);
            case Kind::IMM:
                return std::to_string(imm_val);
            case Kind::MEM: {
                std::stringstream ss;
                ss << "[" << reg_to_string(base_reg) << (mem_offset >= 0 ? " + " : " - ")
                   << std::abs(mem_offset) << "]";
                return ss.str();
            }
        }
        return "?";
    }

    std::string Instruction::to_string() const {
        std::stringstream ss;

        if (op == Opcode::LABEL) {
            ss << label_target << ":";
            return ss.str();
        }

        ss << "  " << std::left << std::setw(8) << opcode_to_string(op);

        std::vector<std::string> parts;
        if (dst.kind != Operand::Kind::NONE) parts.push_back(dst.to_string());
        if (src1.kind != Operand::Kind::NONE) parts.push_back(src1.to_string());
        if (src2.kind != Operand::Kind::NONE) parts.push_back(src2.to_string());
        if (!label_target.empty()) parts.push_back(label_target);

        for (size_t i = 0; i < parts.size(); ++i) {
            ss << parts[i];
            if (i + 1 < parts.size()) ss << ", ";
        }

        if (!comment.empty()) {
            ss << " ; " << comment;
        }

        return ss.str();
    }

    int FrameLayout::allocate_slot(const std::string& name, size_t size, bool is_addressed) {
        // Выравнивание по 8 байт
        size_t aligned = (size + 7) & ~7;
        total_stack_size += static_cast<int>(aligned);
        int offset = -total_stack_size;
        slots.push_back({name, offset, aligned, is_addressed});
        return offset;
    }

    bool FrameLayout::has_slot(const std::string& name) const {
        for (const auto& s : slots) {
            if (s.name == name) return true;
        }
        return false;
    }

    int FrameLayout::get_offset(const std::string& name) const {
        for (const auto& s : slots) {
            if (s.name == name) return s.offset;
        }
        return 0;
    }

    const StackSlot* FrameLayout::get_slot(const std::string& name) const {
        for (const auto& s : slots) {
            if (s.name == name) return &s;
        }
        return nullptr;
    }

    std::string LIRFunction::to_string() const {
        std::stringstream ss;
        ss << "func @" << name << "() {\n";
        ss << "  ; Frame layout: " << frame.total_stack_size << " bytes\n";
        for (const auto& slot : frame.slots) {
            ss << "  ;   " << slot.name << " -> [bp" << slot.offset << "]"
               << " (size: " << slot.size << ", addressed: " << (slot.is_addressed ? "yes" : "no") << ")\n";
        }

        for (const auto& inst : instructions) {
            ss << inst.to_string() << "\n";
        }
        ss << "}\n";
        return ss.str();
    }

    std::string LIRTranslationUnit::to_string() const {
        std::stringstream ss;
        ss << "; Module: " << name << "\n\n";

        for (const auto& [lbl, text] : string_literals) {
            ss << lbl << ": .string \"" << text << "\"\n";
        }
        if (!string_literals.empty()) ss << "\n";

        for (const auto& fn : functions) {
            ss << fn->to_string() << "\n";
        }
        return ss.str();
    }

} // namespace to_lang::lir

