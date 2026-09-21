//
// Created by dangel on 21.09.2026.
//

#ifndef TOC_AST_HPP
#define TOC_AST_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace to_lang {

    // Категории базовых типов
    enum class BaseTypeKind {
        I32,
        I64,
        BOOL,
        CHAR,
        VOID
    };

    // Узел типа (базовый, указатель ptr$, массив [N] или разделитель \ \ argv)
    class TypeNode {
    public:
        enum class Kind { BASE, POINTER, ARRAY };

        Kind kind;
        BaseTypeKind base_kind;
        std::shared_ptr<TypeNode> subtype;
        int64_t array_size = -1; // -1 для динамического размера / argv

        TypeNode(BaseTypeKind bk)
            : kind(Kind::BASE), base_kind(bk), subtype(nullptr) {}

        TypeNode(Kind k, std::shared_ptr<TypeNode> sub, int64_t sz = -1)
            : kind(k), base_kind(BaseTypeKind::VOID), subtype(std::move(sub)), array_size(sz) {}

        static std::shared_ptr<TypeNode> make_base(BaseTypeKind bk) {
            return std::make_shared<TypeNode>(bk);
        }

        static std::shared_ptr<TypeNode> make_pointer(std::shared_ptr<TypeNode> sub) {
            return std::make_shared<TypeNode>(Kind::POINTER, std::move(sub));
        }

        static std::shared_ptr<TypeNode> make_array(std::shared_ptr<TypeNode> sub, int64_t sz = -1) {
            return std::make_shared<TypeNode>(Kind::ARRAY, std::move(sub), sz);
        }

        std::string to_string() const;
    };

    using TypeNodePtr = std::shared_ptr<TypeNode>;

    // Базовый класс для всех узлов AST
    class ASTNode {
    public:
        int line = 1;
        int col = 1;

        ASTNode() = default;
        ASTNode(int l, int c) : line(l), col(c) {}
        virtual ~ASTNode() = default;

        virtual std::string get_node_type() const = 0;
        virtual void dump(std::ostream& os, int indent = 0) const = 0;
    };

    using ASTNodePtr = std::shared_ptr<ASTNode>;

    // Выражения
    class ExprNode : public ASTNode {
    public:
        TypeNodePtr inferred_type = nullptr;
        using ASTNode::ASTNode;
    };

    using ExprNodePtr = std::shared_ptr<ExprNode>;

    // Литералы
    class LiteralExpr : public ExprNode {
    public:
        enum class LitKind { INT, BOOL, CHAR, STRING };
        LitKind lit_kind;
        int64_t int_val = 0;
        bool bool_val = false;
        char char_val = '\0';
        std::string str_val;

        LiteralExpr(int64_t v, int l = 1, int c = 1)
            : ExprNode(l, c), lit_kind(LitKind::INT), int_val(v) {}
        LiteralExpr(bool v, int l = 1, int c = 1)
            : ExprNode(l, c), lit_kind(LitKind::BOOL), bool_val(v) {}
        LiteralExpr(char v, int l = 1, int c = 1)
            : ExprNode(l, c), lit_kind(LitKind::CHAR), char_val(v) {}
        LiteralExpr(std::string v, int l = 1, int c = 1)
            : ExprNode(l, c), lit_kind(LitKind::STRING), str_val(std::move(v)) {}

        std::string get_node_type() const override { return "LiteralExpr"; }
        void dump(std::ostream& os, int indent = 0) const override;
        std::string value_string() const;
    };

    // Имя переменной
    class VariableExpr : public ExprNode {
    public:
        std::string name;

        VariableExpr(std::string n, int l = 1, int c = 1)
            : ExprNode(l, c), name(std::move(n)) {}

        std::string get_node_type() const override { return "VariableExpr"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Бинарные операторы
    class BinaryExpr : public ExprNode {
    public:
        std::string op;
        ExprNodePtr lhs;
        ExprNodePtr rhs;

        BinaryExpr(std::string o, ExprNodePtr l_node, ExprNodePtr r_node, int l = 1, int c = 1)
            : ExprNode(l, c), op(std::move(o)), lhs(std::move(l_node)), rhs(std::move(r_node)) {}

        std::string get_node_type() const override { return "BinaryExpr"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Унарные операторы
    class UnaryExpr : public ExprNode {
    public:
        std::string op;
        ExprNodePtr operand;

        UnaryExpr(std::string o, ExprNodePtr op_node, int l = 1, int c = 1)
            : ExprNode(l, c), op(std::move(o)), operand(std::move(op_node)) {}

        std::string get_node_type() const override { return "UnaryExpr"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Доступ по индексу массива: arr[i]
    class IndexExpr : public ExprNode {
    public:
        ExprNodePtr target;
        ExprNodePtr index;

        IndexExpr(ExprNodePtr t, ExprNodePtr i, int l = 1, int c = 1)
            : ExprNode(l, c), target(std::move(t)), index(std::move(i)) {}

        std::string get_node_type() const override { return "IndexExpr"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Вызов функции: foo(a, b)
    class CallExpr : public ExprNode {
    public:
        std::string callee;
        std::vector<ExprNodePtr> args;

        CallExpr(std::string c_name, std::vector<ExprNodePtr> a, int l = 1, int c = 1)
            : ExprNode(l, c), callee(std::move(c_name)), args(std::move(a)) {}

        std::string get_node_type() const override { return "CallExpr"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Инструкции
    class StmtNode : public ASTNode {
    public:
        using ASTNode::ASTNode;
    };

    using StmtNodePtr = std::shared_ptr<StmtNode>;

    // Блок инструкций { ... }
    class BlockStmt : public StmtNode {
    public:
        std::vector<StmtNodePtr> statements;

        BlockStmt(std::vector<StmtNodePtr> stmts, int l = 1, int c = 1)
            : StmtNode(l, c), statements(std::move(stmts)) {}

        std::string get_node_type() const override { return "BlockStmt"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    using BlockStmtPtr = std::shared_ptr<BlockStmt>;

    // Объявление переменной: i32 i = 0;
    class VarDeclStmt : public StmtNode {
    public:
        TypeNodePtr type;
        std::string name;
        ExprNodePtr init; // может быть nullptr

        VarDeclStmt(TypeNodePtr t, std::string n, ExprNodePtr i_expr = nullptr, int l = 1, int c = 1)
            : StmtNode(l, c), type(std::move(t)), name(std::move(n)), init(std::move(i_expr)) {}

        std::string get_node_type() const override { return "VarDeclStmt"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Присваивание: i = 0; i += 1; (является выражением, как в Си)
    class AssignStmt : public ExprNode {
    public:
        std::string op; // "=", "+=", "-=", "*=", "/=" и т.д.
        ExprNodePtr target; // VariableExpr или IndexExpr
        ExprNodePtr value;

        AssignStmt(std::string o, ExprNodePtr t, ExprNodePtr v, int l = 1, int c = 1)
            : ExprNode(l, c), op(std::move(o)), target(std::move(t)), value(std::move(v)) {}

        std::string get_node_type() const override { return "AssignStmt"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Ветка elif (пара: условие и тело-блок)
    struct ElifBranch {
        ExprNodePtr condition;
        BlockStmtPtr body;
    };

    // Инструкция if - elif - else
    class IfStmt : public StmtNode {
    public:
        ExprNodePtr condition;
        BlockStmtPtr then_branch;
        std::vector<ElifBranch> elif_branches;
        BlockStmtPtr else_branch; // может быть nullptr

        IfStmt(ExprNodePtr cond, BlockStmtPtr then_b,
               std::vector<ElifBranch> elifs, BlockStmtPtr else_b = nullptr,
               int l = 1, int c = 1)
            : StmtNode(l, c), condition(std::move(cond)),
              then_branch(std::move(then_b)), elif_branches(std::move(elifs)),
              else_branch(std::move(else_b)) {}

        std::string get_node_type() const override { return "IfStmt"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Цикл while (cond) { body }
    class WhileStmt : public StmtNode {
    public:
        ExprNodePtr condition;
        BlockStmtPtr body;

        WhileStmt(ExprNodePtr cond, BlockStmtPtr b, int l = 1, int c = 1)
            : StmtNode(l, c), condition(std::move(cond)), body(std::move(b)) {}

        std::string get_node_type() const override { return "WhileStmt"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Инструкция return expr;
    class ReturnStmt : public StmtNode {
    public:
        ExprNodePtr value; // может быть nullptr для void

        ReturnStmt(ExprNodePtr val = nullptr, int l = 1, int c = 1)
            : StmtNode(l, c), value(std::move(val)) {}

        std::string get_node_type() const override { return "ReturnStmt"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Выражение как инструкция (например вызов функции: prints("hello");)
    class ExprStmt : public StmtNode {
    public:
        ExprNodePtr expr;

        ExprStmt(ExprNodePtr e, int l = 1, int c = 1)
            : StmtNode(l, c), expr(std::move(e)) {}

        std::string get_node_type() const override { return "ExprStmt"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    // Функции и программа

    // Описание параметра функции
    struct ParamDef {
        TypeNodePtr type;
        std::string name;
    };

    // Декларация функции: i32 main(i32 argc, ptr$ ptr$ char \ \ argv) { ... }
    class FunctionDecl : public ASTNode {
    public:
        TypeNodePtr return_type;
        std::string name;
        std::vector<ParamDef> params;
        BlockStmtPtr body;

        FunctionDecl(TypeNodePtr ret_t, std::string n, std::vector<ParamDef> p, BlockStmtPtr b, int l = 1, int c = 1)
            : ASTNode(l, c), return_type(std::move(ret_t)), name(std::move(n)),
              params(std::move(p)), body(std::move(b)) {}

        std::string get_node_type() const override { return "FunctionDecl"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    using FunctionDeclPtr = std::shared_ptr<FunctionDecl>;

    // Корневой узел программы (список функций)
    class ProgramNode : public ASTNode {
    public:
        std::vector<FunctionDeclPtr> functions;

        ProgramNode(std::vector<FunctionDeclPtr> funcs, int l = 1, int c = 1)
            : ASTNode(l, c), functions(std::move(funcs)) {}

        std::string get_node_type() const override { return "ProgramNode"; }
        void dump(std::ostream& os, int indent = 0) const override;
    };

    using ProgramNodePtr = std::shared_ptr<ProgramNode>;

    // Функция экспорта AST в Graphviz DOT
    std::string export_ast_to_dot(const ASTNodePtr& root);

} // namespace to_lang

#endif // TOC_AST_HPP
