#include "../ast/ast.hpp"
#include "../parser/parser.hpp"
#include "../semantics/semantic_analyzer.hpp"
#include <cassert>
#include <iostream>
#include <string>

void test_valid_semantics() {
    std::string valid_files[] = {
        "frontend/tests/valid/example_main.to",
        "frontend/tests/valid/expressions.to",
        "frontend/tests/valid/functions.to",
        "frontend/tests/semantics_valid/scope_shadowing.to",
        "frontend/tests/semantics_valid/types_and_calls.to",
        "examples/fibonacci.to",
        "examples/array_algorithms.to",
        "examples/string_utils.to"
    };

    for (const auto& file : valid_files) {
        auto parse_res = to_lang::parse_file(file);
        assert(parse_res.success);
        assert(parse_res.root != nullptr);

        to_lang::SemanticAnalyzer analyzer;
        bool sem_ok = analyzer.analyze(parse_res.root);
        if (!sem_ok) {
            std::cerr << "Semantic analysis failed on valid file: " << file << "\n";
            for (const auto& err : analyzer.get_errors()) {
                std::cerr << "  " << err.format() << "\n";
            }
        }
        assert(sem_ok);
        assert(!analyzer.has_errors());
    }
}

void test_scope_errors() {
    struct TestCase {
        std::string filepath;
        std::string expected_substring;
    };

    TestCase cases[] = {
        { "frontend/tests/scope_errors/undeclared_var.to", "undeclared variable" },
        { "frontend/tests/scope_errors/redefinition_var.to", "Redefinition of variable" },
        { "frontend/tests/scope_errors/out_of_scope.to", "undeclared variable" },
        { "frontend/tests/scope_errors/undeclared_function.to", "undeclared function" },
        { "frontend/tests/scope_errors/redefinition_func.to", "Redefinition of function" },
        { "frontend/tests/scope_errors/redefinition_param.to", "Parameter" }
    };

    for (const auto& tc : cases) {
        auto parse_res = to_lang::parse_file(tc.filepath);
        assert(parse_res.success);

        to_lang::SemanticAnalyzer analyzer;
        bool sem_ok = analyzer.analyze(parse_res.root);
        (void)sem_ok;
        assert(!sem_ok);
        assert(analyzer.has_errors());

        bool found_expected = false;
        for (const auto& err : analyzer.get_errors()) {
            if (err.message.find(tc.expected_substring) != std::string::npos) {
                found_expected = true;
                break;
            }
        }
        if (!found_expected) {
            std::cerr << "Failed to find '" << tc.expected_substring << "' in errors for " << tc.filepath << ":\n";
            for (const auto& err : analyzer.get_errors()) {
                std::cerr << "  " << err.format() << "\n";
            }
        }
        assert(found_expected);
    }
}

void test_type_errors() {
    struct TestCase {
        std::string filepath;
        std::string expected_substring;
    };

    TestCase cases[] = {
        { "frontend/tests/type_errors/bad_binary_op.to", "must be integer" },
        { "frontend/tests/type_errors/bad_assignment.to", "Cannot assign value" },
        { "frontend/tests/type_errors/bad_return.to", "Return type mismatch" },
        { "frontend/tests/type_errors/bad_condition.to", "must be of boolean or integer type" },
        { "frontend/tests/type_errors/bad_call_args.to", "type mismatch" },
        { "frontend/tests/type_errors/bad_index.to", "not an array or pointer" },
        { "frontend/tests/type_errors/bad_call_arity.to", "expects" },
        { "frontend/tests/type_errors/bad_ptr_arithmetic.to", "must be integer" },
        { "frontend/tests/type_errors/bad_deref.to", "Cannot dereference non-pointer" },
        { "frontend/tests/type_errors/bad_subscript_type.to", "must be integer type" },
        { "frontend/tests/type_errors/bad_void_return.to", "Void function cannot return a value" },
        { "frontend/tests/type_errors/bad_empty_return.to", "Non-void function must return a value" }
    };

    for (const auto& tc : cases) {
        auto parse_res = to_lang::parse_file(tc.filepath);
        assert(parse_res.success);

        to_lang::SemanticAnalyzer analyzer;
        bool sem_ok = analyzer.analyze(parse_res.root);
        (void)sem_ok;
        assert(!sem_ok);
        assert(analyzer.has_errors());

        bool found_expected = false;
        for (const auto& err : analyzer.get_errors()) {
            if (err.message.find(tc.expected_substring) != std::string::npos) {
                found_expected = true;
                break;
            }
        }
        if (!found_expected) {
            std::cerr << "Failed to find '" << tc.expected_substring << "' in errors for " << tc.filepath << ":\n";
            for (const auto& err : analyzer.get_errors()) {
                std::cerr << "  " << err.format() << "\n";
            }
        }
        assert(found_expected);
    }
}

int main() {
    test_valid_semantics();
    test_scope_errors();
    test_type_errors();

    std::cout << "All semantics tests successfully passed!" << std::endl;
    return 0;
}

