//
// Created by dangel on 21.09.2026.
//

#include "ast.hpp"

namespace to_lang {

    static void print_indent(std::ostream& os, int indent) {
        for (int i = 0; i < indent; ++i) {
            os << "  ";
        }
    }

    std::string TypeNode::to_string() const {
        switch (kind) {
            case Kind::BASE:
                switch (base_kind) {
                    case BaseTypeKind::I32: return "i32";
                    case BaseTypeKind::I64: return "i64";
                    case BaseTypeKind::BOOL: return "bool";
                    case BaseTypeKind::CHAR: return "char";
                    case BaseTypeKind::VOID: return "void";
                }
                break;
            case Kind::POINTER:
                return "ptr$ " + (subtype ? subtype->to_string() : "void");
            case Kind::ARRAY:
                if (array_size >= 0) {
                    return "[" + std::to_string(array_size) + "]" + (subtype ? subtype->to_string() : "void");
                } else {
                    return "\\ \\ " + (subtype ? subtype->to_string() : "void");
                }
        }
        return "unknown_type";
    }

    std::string LiteralExpr::value_string() const {
        switch (lit_kind) {
            case LitKind::INT: return std::to_string(int_val);
            case LitKind::BOOL: return bool_val ? "true" : "false";
            case LitKind::CHAR: return std::string("'") + char_val + "'";
            case LitKind::STRING: return "\"" + str_val + "\"";
        }
        return "";
    }

    void LiteralExpr::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "LiteralExpr: " << value_string() << "\n";
    }

    void VariableExpr::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "VariableExpr: " << name << "\n";
    }

    void BinaryExpr::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "BinaryExpr ('" << op << "')\n";
        if (lhs) lhs->dump(os, indent + 1);
        if (rhs) rhs->dump(os, indent + 1);
    }

    void UnaryExpr::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "UnaryExpr ('" << op << "')\n";
        if (operand) operand->dump(os, indent + 1);
    }

    void IndexExpr::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "IndexExpr\n";
        if (target) {
            print_indent(os, indent + 1);
            os << "Target:\n";
            target->dump(os, indent + 2);
        }
        if (index) {
            print_indent(os, indent + 1);
            os << "Index:\n";
            index->dump(os, indent + 2);
        }
    }

    void CallExpr::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "CallExpr: " << callee << "()\n";
        for (const auto& a : args) {
            if (a) a->dump(os, indent + 1);
        }
    }

    void BlockStmt::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "BlockStmt (" << statements.size() << " statements)\n";
        for (const auto& s : statements) {
            if (s) s->dump(os, indent + 1);
        }
    }

    void VarDeclStmt::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "VarDeclStmt: " << (type ? type->to_string() : "unknown") << " " << name;
        if (init) {
            os << " =\n";
            init->dump(os, indent + 1);
        } else {
            os << "\n";
        }
    }

    void AssignStmt::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "AssignStmt ('" << op << "')\n";
        if (target) {
            print_indent(os, indent + 1);
            os << "Target:\n";
            target->dump(os, indent + 2);
        }
        if (value) {
            print_indent(os, indent + 1);
            os << "Value:\n";
            value->dump(os, indent + 2);
        }
    }

    void IfStmt::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "IfStmt\n";
        if (condition) {
            print_indent(os, indent + 1);
            os << "Condition:\n";
            condition->dump(os, indent + 2);
        }
        if (then_branch) {
            print_indent(os, indent + 1);
            os << "Then:\n";
            then_branch->dump(os, indent + 2);
        }
        for (size_t i = 0; i < elif_branches.size(); ++i) {
            print_indent(os, indent + 1);
            os << "Elif[" << i << "]:\n";
            if (elif_branches[i].condition) {
                print_indent(os, indent + 2);
                os << "Condition:\n";
                elif_branches[i].condition->dump(os, indent + 3);
            }
            if (elif_branches[i].body) {
                print_indent(os, indent + 2);
                os << "Body:\n";
                elif_branches[i].body->dump(os, indent + 3);
            }
        }
        if (else_branch) {
            print_indent(os, indent + 1);
            os << "Else:\n";
            else_branch->dump(os, indent + 2);
        }
    }

    void WhileStmt::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "WhileStmt\n";
        if (condition) {
            print_indent(os, indent + 1);
            os << "Condition:\n";
            condition->dump(os, indent + 2);
        }
        if (body) {
            print_indent(os, indent + 1);
            os << "Body:\n";
            body->dump(os, indent + 2);
        }
    }

    void ReturnStmt::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "ReturnStmt";
        if (value) {
            os << "\n";
            value->dump(os, indent + 1);
        } else {
            os << " (void)\n";
        }
    }

    void ExprStmt::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "ExprStmt\n";
        if (expr) expr->dump(os, indent + 1);
    }

    void FunctionDecl::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "FunctionDecl: " << (return_type ? return_type->to_string() : "void")
           << " " << name << "(";
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) os << ", ";
            os << (params[i].type ? params[i].type->to_string() : "unknown") << " " << params[i].name;
        }
        os << ")\n";
        if (body) body->dump(os, indent + 1);
    }

    void ProgramNode::dump(std::ostream& os, int indent) const {
        print_indent(os, indent);
        os << "ProgramNode (" << functions.size() << " functions)\n";
        for (const auto& f : functions) {
            if (f) f->dump(os, indent + 1);
        }
    }

} // namespace to_lang

