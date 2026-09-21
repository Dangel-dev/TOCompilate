//
// Created by dangel on 21.09.2026.
//

#include "parser.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>

extern int yyparse();
extern FILE* yyin;
extern int yylineno;
extern void yyrestart(FILE*);

struct yy_buffer_state;
extern yy_buffer_state* yy_scan_string(const char* str);
extern void yy_delete_buffer(yy_buffer_state* buffer);

to_lang::ProgramNodePtr g_program_root = nullptr;
std::string g_parse_error = "";
int g_error_line = 0;

namespace to_lang {

    ParseResult parse_file(const std::string& filepath) {
        ParseResult res;
        g_program_root = nullptr;
        g_parse_error = "";
        g_error_line = 0;
        yylineno = 1;

        FILE* f = fopen(filepath.c_str(), "r");
        if (!f) {
            f = fopen(("../" + filepath).c_str(), "r");
        }
        if (!f) {
            f = fopen(("../../" + filepath).c_str(), "r");
        }
        if (!f) {
            res.success = false;
            res.error_message = "Cannot open file: " + filepath;
            return res;
        }

        yyrestart(f);
        int parse_status = yyparse();
        fclose(f);

        if (parse_status == 0 && g_parse_error.empty() && g_program_root != nullptr) {
            res.success = true;
            res.root = g_program_root;
        } else {
            res.success = false;
            res.error_message = g_parse_error.empty() ? "Syntax error" : g_parse_error;
            res.error_line = g_error_line > 0 ? g_error_line : yylineno;
        }

        return res;
    }

    ParseResult parse_string(const std::string& source_code) {
        ParseResult res;
        g_program_root = nullptr;
        g_parse_error = "";
        g_error_line = 0;
        yylineno = 1;

        yy_buffer_state* buffer = yy_scan_string(source_code.c_str());
        int parse_status = yyparse();
        yy_delete_buffer(buffer);

        if (parse_status == 0 && g_parse_error.empty() && g_program_root != nullptr) {
            res.success = true;
            res.root = g_program_root;
        } else {
            res.success = false;
            res.error_message = g_parse_error.empty() ? "Syntax error" : g_parse_error;
            res.error_line = g_error_line > 0 ? g_error_line : yylineno;
        }

        return res;
    }

} // namespace to_lang

