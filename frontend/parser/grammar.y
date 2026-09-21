%{
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "../ast/ast.hpp"

extern int yylex();
extern int yylineno;
extern char* yytext;
void yyerror(const char* s);

extern to_lang::ProgramNodePtr g_program_root;
extern std::string g_parse_error;
extern int g_error_line;
%}

%union {
    int64_t int_val;
    char char_val;
    bool bool_val;
    char* str_val;
    to_lang::TypeNode* type_val;
    to_lang::ExprNode* expr_val;
    to_lang::StmtNode* stmt_val;
    to_lang::BlockStmt* block_val;
    to_lang::FunctionDecl* func_val;
    to_lang::ParamDef* param_val;
    to_lang::VarDeclStmt* vardecl_val;
    to_lang::IfStmt* if_val;
    to_lang::WhileStmt* while_val;
    to_lang::ReturnStmt* return_val;
    std::vector<to_lang::ExprNodePtr>* expr_list;
    std::vector<to_lang::StmtNodePtr>* stmt_list;
    std::vector<to_lang::ParamDef>* param_list;
    std::vector<to_lang::ElifBranch>* elif_list;
    std::vector<to_lang::FunctionDeclPtr>* func_list;
}

%token <int_val> INT_LITERAL
%token <char_val> CHAR_LITERAL
%token <str_val> IDENTIFIER STRING_LITERAL
%token KW_I32 KW_I64 KW_BOOL KW_CHAR KW_VOID KW_PTR
%token KW_IF KW_ELIF KW_ELSE KW_WHILE KW_RETURN KW_TRUE KW_FALSE
%token OP_ADD_ASSIGN OP_SUB_ASSIGN OP_MUL_ASSIGN OP_DIV_ASSIGN OP_MOD_ASSIGN
%token OP_EQ OP_NEQ OP_LE OP_GE OP_AND OP_OR OP_SHL OP_SHR OP_BACKSLASH
%token ERROR_TOKEN

%type <func_list> function_list
%type <func_val> function_decl
%type <param_list> param_list opt_param_list
%type <param_val> param
%type <type_val> type base_type
%type <block_val> block opt_else
%type <stmt_list> statement_list
%type <stmt_val> statement
%type <vardecl_val> var_decl
%type <if_val> if_stmt
%type <elif_list> elif_list
%type <while_val> while_stmt
%type <return_val> return_stmt
%type <expr_val> expr primary_expr
%type <expr_list> arg_list opt_arg_list

/* Приоритеты операторов (снизу вверх: от меньшего к большему) */
%right '=' OP_ADD_ASSIGN OP_SUB_ASSIGN OP_MUL_ASSIGN OP_DIV_ASSIGN OP_MOD_ASSIGN
%left OP_OR
%left OP_AND
%left '|'
%left '^'
%left '&'
%left OP_EQ OP_NEQ
%left '<' OP_LE '>' OP_GE
%left OP_SHL OP_SHR
%left '+' '-'
%left '*' '/' '%'
%right UNARY
%left '[' '('

%%

program:
    function_list {
        g_program_root = std::make_shared<to_lang::ProgramNode>(*$1, yylineno, 1);
        delete $1;
    }
    ;

function_list:
    /* empty */ {
        $$ = new std::vector<to_lang::FunctionDeclPtr>();
    }
    | function_list function_decl {
        $$ = $1;
        if ($2) $$->push_back(to_lang::FunctionDeclPtr($2));
    }
    ;

function_decl:
    type IDENTIFIER '(' opt_param_list ')' block {
        $$ = new to_lang::FunctionDecl(
            to_lang::TypeNodePtr($1),
            $2,
            *$4,
            to_lang::BlockStmtPtr($6),
            yylineno, 1
        );
        free($2);
        delete $4;
    }
    ;

opt_param_list:
    /* empty */ {
        $$ = new std::vector<to_lang::ParamDef>();
    }
    | param_list {
        $$ = $1;
    }
    ;

param_list:
    param {
        $$ = new std::vector<to_lang::ParamDef>();
        $$->push_back(*$1);
        delete $1;
    }
    | param_list ',' param {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
    ;

param:
    type IDENTIFIER {
        $$ = new to_lang::ParamDef{ to_lang::TypeNodePtr($1), $2 };
        free($2);
    }
    | type OP_BACKSLASH OP_BACKSLASH IDENTIFIER {
        $$ = new to_lang::ParamDef{ to_lang::TypeNodePtr($1), $4 };
        free($4);
    }
    ;

type:
    base_type {
        $$ = $1;
    }
    | KW_PTR type {
        $$ = new to_lang::TypeNode(to_lang::TypeNode::Kind::POINTER, to_lang::TypeNodePtr($2));
    }
    | '[' INT_LITERAL ']' type {
        $$ = new to_lang::TypeNode(to_lang::TypeNode::Kind::ARRAY, to_lang::TypeNodePtr($4), $2);
    }
    ;

base_type:
    KW_I32  { $$ = new to_lang::TypeNode(to_lang::BaseTypeKind::I32); }
    | KW_I64  { $$ = new to_lang::TypeNode(to_lang::BaseTypeKind::I64); }
    | KW_BOOL { $$ = new to_lang::TypeNode(to_lang::BaseTypeKind::BOOL); }
    | KW_CHAR { $$ = new to_lang::TypeNode(to_lang::BaseTypeKind::CHAR); }
    | KW_VOID { $$ = new to_lang::TypeNode(to_lang::BaseTypeKind::VOID); }
    ;

block:
    '{' statement_list '}' {
        $$ = new to_lang::BlockStmt(*$2, yylineno, 1);
        delete $2;
    }
    ;

statement_list:
    /* empty */ {
        $$ = new std::vector<to_lang::StmtNodePtr>();
    }
    | statement_list statement {
        $$ = $1;
        if ($2) $$->push_back(to_lang::StmtNodePtr($2));
    }
    ;

statement:
    var_decl ';'       { $$ = $1; }
    | if_stmt           { $$ = $1; }
    | while_stmt        { $$ = $1; }
    | return_stmt ';'   { $$ = $1; }
    | expr ';'          { $$ = new to_lang::ExprStmt(to_lang::ExprNodePtr($1), yylineno, 1); }
    | block             { $$ = $1; }
    | ';'               { $$ = nullptr; }
    ;

var_decl:
    type IDENTIFIER {
        $$ = new to_lang::VarDeclStmt(to_lang::TypeNodePtr($1), $2, nullptr, yylineno, 1);
        free($2);
    }
    | type IDENTIFIER '=' expr {
        $$ = new to_lang::VarDeclStmt(to_lang::TypeNodePtr($1), $2, to_lang::ExprNodePtr($4), yylineno, 1);
        free($2);
    }
    ;

if_stmt:
    KW_IF '(' expr ')' block elif_list opt_else {
        $$ = new to_lang::IfStmt(
            to_lang::ExprNodePtr($3),
            to_lang::BlockStmtPtr($5),
            *$6,
            to_lang::BlockStmtPtr($7),
            yylineno, 1
        );
        delete $6;
    }
    ;

elif_list:
    /* empty */ {
        $$ = new std::vector<to_lang::ElifBranch>();
    }
    | elif_list KW_ELIF '(' expr ')' block {
        $$ = $1;
        $$->push_back(to_lang::ElifBranch{
            to_lang::ExprNodePtr($4),
            to_lang::BlockStmtPtr($6)
        });
    }
    ;

opt_else:
    /* empty */ {
        $$ = nullptr;
    }
    | KW_ELSE block {
        $$ = $2;
    }
    ;

while_stmt:
    KW_WHILE '(' expr ')' block {
        $$ = new to_lang::WhileStmt(to_lang::ExprNodePtr($3), to_lang::BlockStmtPtr($5), yylineno, 1);
    }
    ;

return_stmt:
    KW_RETURN {
        $$ = new to_lang::ReturnStmt(nullptr, yylineno, 1);
    }
    | KW_RETURN expr {
        $$ = new to_lang::ReturnStmt(to_lang::ExprNodePtr($2), yylineno, 1);
    }
    ;

expr:
    expr '=' expr   { $$ = new to_lang::AssignStmt("=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_ADD_ASSIGN expr { $$ = new to_lang::AssignStmt("+=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_SUB_ASSIGN expr { $$ = new to_lang::AssignStmt("-=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_MUL_ASSIGN expr { $$ = new to_lang::AssignStmt("*=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_DIV_ASSIGN expr { $$ = new to_lang::AssignStmt("/=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_MOD_ASSIGN expr { $$ = new to_lang::AssignStmt("%=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '+' expr   { $$ = new to_lang::BinaryExpr("+", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '-' expr { $$ = new to_lang::BinaryExpr("-", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '*' expr { $$ = new to_lang::BinaryExpr("*", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '/' expr { $$ = new to_lang::BinaryExpr("/", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '%' expr { $$ = new to_lang::BinaryExpr("%", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_EQ expr { $$ = new to_lang::BinaryExpr("==", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_NEQ expr { $$ = new to_lang::BinaryExpr("!=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '<' expr { $$ = new to_lang::BinaryExpr("<", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_LE expr { $$ = new to_lang::BinaryExpr("<=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '>' expr { $$ = new to_lang::BinaryExpr(">", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_GE expr { $$ = new to_lang::BinaryExpr(">=", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_AND expr { $$ = new to_lang::BinaryExpr("&&", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_OR expr { $$ = new to_lang::BinaryExpr("||", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '&' expr { $$ = new to_lang::BinaryExpr("&", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '|' expr { $$ = new to_lang::BinaryExpr("|", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr '^' expr { $$ = new to_lang::BinaryExpr("^", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_SHL expr { $$ = new to_lang::BinaryExpr("<<", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | expr OP_SHR expr { $$ = new to_lang::BinaryExpr(">>", to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1); }
    | '-' expr %prec UNARY { $$ = new to_lang::UnaryExpr("-", to_lang::ExprNodePtr($2), yylineno, 1); }
    | '!' expr %prec UNARY { $$ = new to_lang::UnaryExpr("!", to_lang::ExprNodePtr($2), yylineno, 1); }
    | '~' expr %prec UNARY { $$ = new to_lang::UnaryExpr("~", to_lang::ExprNodePtr($2), yylineno, 1); }
    | '*' expr %prec UNARY { $$ = new to_lang::UnaryExpr("*", to_lang::ExprNodePtr($2), yylineno, 1); }
    | expr '[' expr ']' {
        $$ = new to_lang::IndexExpr(to_lang::ExprNodePtr($1), to_lang::ExprNodePtr($3), yylineno, 1);
    }
    | IDENTIFIER '(' opt_arg_list ')' {
        $$ = new to_lang::CallExpr($1, *$3, yylineno, 1);
        free($1);
        delete $3;
    }
    | primary_expr {
        $$ = $1;
    }
    ;

opt_arg_list:
    /* empty */ {
        $$ = new std::vector<to_lang::ExprNodePtr>();
    }
    | arg_list {
        $$ = $1;
    }
    ;

arg_list:
    expr {
        $$ = new std::vector<to_lang::ExprNodePtr>();
        $$->push_back(to_lang::ExprNodePtr($1));
    }
    | arg_list ',' expr {
        $$ = $1;
        $$->push_back(to_lang::ExprNodePtr($3));
    }
    ;

primary_expr:
    IDENTIFIER {
        $$ = new to_lang::VariableExpr($1, yylineno, 1);
        free($1);
    }
    | INT_LITERAL {
        $$ = new to_lang::LiteralExpr($1, yylineno, 1);
    }
    | CHAR_LITERAL {
        $$ = new to_lang::LiteralExpr($1, yylineno, 1);
    }
    | STRING_LITERAL {
        $$ = new to_lang::LiteralExpr(std::string($1), yylineno, 1);
        free($1);
    }
    | KW_TRUE {
        $$ = new to_lang::LiteralExpr(true, yylineno, 1);
    }
    | KW_FALSE {
        $$ = new to_lang::LiteralExpr(false, yylineno, 1);
    }
    | '(' expr ')' {
        $$ = $2;
    }
    ;

%%

void yyerror(const char* s) {
    g_parse_error = s;
    g_error_line = yylineno;
}
