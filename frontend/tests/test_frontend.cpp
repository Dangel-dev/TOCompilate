#include "../ast/ast.hpp"
#include "../parser/parser.hpp"
#include <cassert>
#include <iostream>
#include <string>

void test_valid_files() {
    std::string valid_files[] = {
        "frontend/tests/valid/example_main.to",
        "frontend/tests/valid/expressions.to",
        "frontend/tests/valid/functions.to",
        "examples/fibonacci.to",
        "examples/array_algorithms.to",
        "examples/string_utils.to"
    };

    for (const auto& file : valid_files) {
        auto res = to_lang::parse_file(file);
        if (!res.success) {
            std::cerr << "Failed to parse valid file: " << file << "\nError: " << res.error_message << " at line " << res.error_line << "\n";
        }
        assert(res.success);
        assert(res.root != nullptr);
        assert(!res.root->functions.empty());

        std::string dot = to_lang::export_ast_to_dot(res.root);
        assert(!dot.empty());
        assert(dot.find("digraph AST") != std::string::npos);
    }
}

void test_lexer_errors() {
    std::string error_files[] = {
        "frontend/tests/lexer_errors/unclosed_string.to",
        "frontend/tests/lexer_errors/invalid_char.to",
        "frontend/tests/lexer_errors/unclosed_char.to",
        "frontend/tests/lexer_errors/unexpected_symbol.to"
    };

    for (const auto& file : error_files) {
        auto res = to_lang::parse_file(file);
        assert(!res.success);
        assert(!res.error_message.empty());
    }
}

void test_parser_errors() {
    std::string error_files[] = {
        "frontend/tests/parser_errors/missing_semicolon.to",
        "frontend/tests/parser_errors/elif_without_if.to",
        "frontend/tests/parser_errors/unclosed_brace.to",
        "frontend/tests/parser_errors/missing_paren.to",
        "frontend/tests/parser_errors/missing_type.to"
    };

    for (const auto& file : error_files) {
        auto res = to_lang::parse_file(file);
        assert(!res.success);
        assert(!res.error_message.empty());
    }
}

void test_string_parsing() {
    std::string code = R"(
        i32 square(i32 x) {
            return x * x;
        }

        i32 main() {
            i32 val = square(5);
            if (val > 10) {
                prints("Big\n");
            } elif (val == 10) {
                prints("Ten\n");
            } else {
                prints("Small\n");
            }
            return 0;
        }
    )";

    auto res = to_lang::parse_string(code);
    assert(res.success);
    assert(res.root != nullptr);
    assert(res.root->functions.size() == 2);
    assert(res.root->functions[0]->name == "square");
    assert(res.root->functions[1]->name == "main");
}

int main() {
    test_valid_files();
    test_lexer_errors();
    test_parser_errors();
    test_string_parsing();

    std::cout << "All frontend tests successfully passed!" << std::endl;
    return 0;
}

