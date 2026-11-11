//
// Created by dangel on 11.11.2026.
//

#include "hir.hpp"
#include <iomanip>

namespace to_lang::hir {

    std::string opcode_to_string(Opcode op) {
        switch (op) {
            case Opcode::CONST:        return "const";
            case Opcode::ADD:          return "add";
            case Opcode::SUB:          return "sub";
            case Opcode::MUL:          return "mul";
            case Opcode::DIV:          return "div";
            case Opcode::MOD:          return "mod";
            case Opcode::AND:          return "and";
            case Opcode::OR:           return "or";
            case Opcode::XOR:          return "xor";
            case Opcode::SHL:          return "shl";
            case Opcode::SHR:          return "shr";
            case Opcode::NEG:          return "neg";
            case Opcode::NOT:          return "not";
            case Opcode::EQ:           return "eq";
            case Opcode::NE:           return "ne";
            case Opcode::LT:           return "lt";
            case Opcode::LE:           return "le";
            case Opcode::GT:           return "gt";
            case Opcode::GE:           return "ge";
            case Opcode::ALLOCA:       return "alloca";
            case Opcode::ADDR_OF:      return "addr_of";
            case Opcode::LOAD:         return "load";
            case Opcode::STORE:        return "store";
            case Opcode::GET_ELEM_PTR: return "get_elem_ptr";
            case Opcode::GET_FIELD_PTR:return "get_field_ptr";
            case Opcode::CAST:         return "cast";
            case Opcode::LABEL:        return "label";
            case Opcode::JUMP:         return "jump";
            case Opcode::BRANCH:       return "branch";
            case Opcode::CALL:         return "call";
            case Opcode::CALL_PTR:     return "call_ptr";
            case Opcode::RETURN:       return "return";
        }
        return "unknown";
    }

    std::string HIRConstant::to_string() const {
        switch (kind) {
            case Kind::INT:
                return std::to_string(int_val);
            case Kind::BOOL:
                return bool_val ? "true" : "false";
            case Kind::CHAR: {
                std::string s = "'";
                if (char_val == '\n') s += "\\n";
                else if (char_val == '\t') s += "\\t";
                else if (char_val == '\0') s += "\\0";
                else s += char_val;
                s += "'";
                return s;
            }
            case Kind::STRING:
                return "\"" + str_val + "\"";
            case Kind::NULL_PTR:
                return "null";
        }
        return "?";
    }

    std::string HIROperation::to_string() const {
        std::ostringstream ss;
        if (opcode == Opcode::LABEL) {
            ss << label_target << ":";
            return ss.str();
        }

        ss << "  ";
        if (dest) {
            ss << dest->name << " = ";
        }

        ss << opcode_to_string(opcode);

        switch (opcode) {
            case Opcode::CONST:
                ss << " " << (constant ? constant->to_string() : "<?>");
                break;

            case Opcode::ALLOCA:
                if (dest && dest->type) {
                    ss << " " << dest->type->to_string();
                }
                break;

            case Opcode::LOAD:
            case Opcode::ADDR_OF:
            case Opcode::NEG:
            case Opcode::NOT:
            case Opcode::CAST:
                if (src1) ss << " " << src1->name;
                break;

            case Opcode::STORE:
                if (src1 && src2) {
                    ss << " " << src1->name << ", " << src2->name;
                }
                break;

            case Opcode::ADD:
            case Opcode::SUB:
            case Opcode::MUL:
            case Opcode::DIV:
            case Opcode::MOD:
            case Opcode::AND:
            case Opcode::OR:
            case Opcode::XOR:
            case Opcode::SHL:
            case Opcode::SHR:
            case Opcode::EQ:
            case Opcode::NE:
            case Opcode::LT:
            case Opcode::LE:
            case Opcode::GT:
            case Opcode::GE:
            case Opcode::GET_ELEM_PTR:
            case Opcode::GET_FIELD_PTR:
                if (src1) ss << " " << src1->name;
                if (src2) ss << ", " << src2->name;
                break;

            case Opcode::JUMP:
                ss << " " << label_target;
                break;

            case Opcode::BRANCH:
                if (src1) ss << " " << src1->name << ", ";
                ss << label_target << ", " << label_false;
                break;

            case Opcode::CALL:
                ss << " @" << func_name << "(";
                for (size_t i = 0; i < args.size(); ++i) {
                    if (i > 0) ss << ", ";
                    ss << (args[i] ? args[i]->name : "null");
                }
                ss << ")";
                break;

            case Opcode::CALL_PTR:
                ss << " *" << (src1 ? src1->name : "null") << "(";
                for (size_t i = 0; i < args.size(); ++i) {
                    if (i > 0) ss << ", ";
                    ss << (args[i] ? args[i]->name : "null");
                }
                ss << ")";
                break;

            case Opcode::RETURN:
                if (src1) ss << " " << src1->name;
                break;

            default:
                break;
        }

        return ss.str();
    }

    std::string HIRFunction::to_string() const {
        std::ostringstream ss;
        ss << "func @" << name << "(";
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) ss << ", ";
            if (params[i]) ss << params[i]->to_string();
        }
        ss << ") -> " << (return_type ? return_type->to_string() : "void") << " {\n";

        for (const auto& obj : local_objects) {
            if (obj && obj->is_variable()) {
                ss << "  // local " << obj->to_string() << "\n";
            }
        }

        for (const auto& op : operations) {
            if (op) ss << op->to_string() << "\n";
        }
        ss << "}\n";
        return ss.str();
    }

    std::string HIRTranslationUnit::to_string() const {
        std::ostringstream ss;
        ss << "; Translation Unit: " << name << "\n\n";

        for (const auto& g : global_objects) {
            if (g) ss << "global @" << g->to_string() << "\n";
        }
        if (!global_objects.empty()) ss << "\n";

        for (const auto& fn : functions) {
            if (fn) {
                ss << fn->to_string() << "\n";
            }
        }
        return ss.str();
    }

} // namespace to_lang::hir

