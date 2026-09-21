%{
#include <string>
#include <iostream>
#include <cstdlib>
#include "../ast/ast.hpp"
#include "grammar.hpp"

// Отслеживание позиции в коде
extern int yylineno;
int g_col_no = 1;

void count_col() {
    for (int i = 0; yytext[i] != '\0'; ++i) {
        if (yytext[i] == '\n') {
            g_col_no = 1;
        } else if (yytext[i] == '\t') {
            g_col_no += 4 - (g_col_no % 4);
        } else {
            g_col_no++;
        }
    }
}

// Преобразование escape-последовательностей
std::string parse_escaped_string(const char* text) {
    std::string res;
    // Пропускаем открывающую и закрывающую кавычки
    size_t len = strlen(text);
    for (size_t i = 1; i < len - 1; ++i) {
        if (text[i] == '\\' && i + 1 < len - 1) {
            switch (text[i + 1]) {
                case 'n': res += '\n'; ++i; break;
                case 't': res += '\t'; ++i; break;
                case 'r': res += '\r'; ++i; break;
                case '\\': res += '\\'; ++i; break;
                case '"': res += '"'; ++i; break;
                case '\'': res += '\''; ++i; break;
                default: res += text[i + 1]; ++i; break;
            }
        } else {
            res += text[i];
        }
    }
    return res;
}

char parse_escaped_char(const char* text) {
    if (text[1] == '\\') {
        switch (text[2]) {
            case 'n': return '\n';
            case 't': return '\t';
            case 'r': return '\r';
            case '\\': return '\\';
            case '\'': return '\'';
            case '0': return '\0';
            default: return text[2];
        }
    }
    return text[1];
}

void yyerror(const char* s);
%}

%option noyywrap
%option yylineno
%option nounput
%option noinput

%%

[ \t\r]+                { count_col(); /* Пропуск пробелов */ }
\n                      { count_col(); /* Перевод строки */ }
"//".*                  { count_col(); /* Однострочный комментарий */ }
"/*"([^*]|\*+[^*/])*\*+"/" { count_col(); /* Многострочный комментарий */ }

"i32"                   { count_col(); return KW_I32; }
"i64"                   { count_col(); return KW_I64; }
"bool"                  { count_col(); return KW_BOOL; }
"char"                  { count_col(); return KW_CHAR; }
"void"                  { count_col(); return KW_VOID; }
"ptr$"                  { count_col(); return KW_PTR; }

"if"                    { count_col(); return KW_IF; }
"elif"                  { count_col(); return KW_ELIF; }
"else"                  { count_col(); return KW_ELSE; }
"while"                 { count_col(); return KW_WHILE; }
"return"                { count_col(); return KW_RETURN; }
"true"                  { count_col(); return KW_TRUE; }
"false"                 { count_col(); return KW_FALSE; }

"+="                    { count_col(); return OP_ADD_ASSIGN; }
"-="                    { count_col(); return OP_SUB_ASSIGN; }
"*="                    { count_col(); return OP_MUL_ASSIGN; }
"/="                    { count_col(); return OP_DIV_ASSIGN; }
"%="                    { count_col(); return OP_MOD_ASSIGN; }

"=="                    { count_col(); return OP_EQ; }
"!="                    { count_col(); return OP_NEQ; }
"<="                    { count_col(); return OP_LE; }
">="                    { count_col(); return OP_GE; }
"&&"                    { count_col(); return OP_AND; }
"||"                    { count_col(); return OP_OR; }
"<<"                    { count_col(); return OP_SHL; }
">>"                    { count_col(); return OP_SHR; }

"\\"                    { count_col(); return OP_BACKSLASH; }

"+"                     { count_col(); return '+'; }
"-"                     { count_col(); return '-'; }
"*"                     { count_col(); return '*'; }
"/"                     { count_col(); return '/'; }
"%"                     { count_col(); return '%'; }
"="                     { count_col(); return '='; }
"<"                     { count_col(); return '<'; }
">"                     { count_col(); return '>'; }
"&"                     { count_col(); return '&'; }
"|"                     { count_col(); return '|'; }
"^"                     { count_col(); return '^'; }
"~"                     { count_col(); return '~'; }
"!"                     { count_col(); return '!'; }

"{"                     { count_col(); return '{'; }
"}"                     { count_col(); return '}'; }
"("                     { count_col(); return '('; }
")"                     { count_col(); return ')'; }
"["                     { count_col(); return '['; }
"]"                     { count_col(); return ']'; }
";"                     { count_col(); return ';'; }
","                     { count_col(); return ','; }

0x[0-9a-fA-F]+          {
                            count_col();
                            yylval.int_val = std::strtoll(yytext, nullptr, 16);
                            return INT_LITERAL;
                        }

[0-9]+                  {
                            count_col();
                            yylval.int_val = std::strtoll(yytext, nullptr, 10);
                            return INT_LITERAL;
                        }

[a-zA-Z_][a-zA-Z0-9_]*  {
                            count_col();
                            yylval.str_val = strdup(yytext);
                            return IDENTIFIER;
                        }

\"([^"\\]|\\.)*\"       {
                            count_col();
                            yylval.str_val = strdup(parse_escaped_string(yytext).c_str());
                            return STRING_LITERAL;
                        }

\"([^"\n\\]|\\.)*       {
                            count_col();
                            yyerror("Unclosed string literal");
                            return ERROR_TOKEN;
                        }

'([^'\\]|\\.)'          {
                            count_col();
                            yylval.char_val = parse_escaped_char(yytext);
                            return CHAR_LITERAL;
                        }

.                       {
                            count_col();
                            std::string err = "Unexpected character: '";
                            err += yytext;
                            err += "'";
                            yyerror(err.c_str());
                            return ERROR_TOKEN;
                        }

%%
