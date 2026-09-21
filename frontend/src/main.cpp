//
// Created by dangel on 21.09.2026.
//

#include "../ast/ast.hpp"
#include "../parser/parser.hpp"
#include "../semantics/semantic_analyzer.hpp"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string source_path = "";
    std::string dot_path = "";
    std::string png_path = "";
    bool print_dot = false;
    bool dump_ast = false;
    bool run_semantics = true;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ast-dot" || arg == "--graph-dot" || arg == "--dot") {
            if (i + 1 < argc && argv[i + 1][0] != '-' && std::string(argv[i + 1]).find(".to") == std::string::npos) {
                dot_path = argv[++i];
            } else {
                print_dot = true;
            }
        } else if (arg == "--dump") {
            dump_ast = true;
        } else if (arg == "--png" && i + 1 < argc) {
            png_path = argv[++i];
        } else if (arg == "--no-sem") {
            run_semantics = false;
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        } else {
            source_path = arg;
        }
    }

#ifdef DEBUG
    std::cout << "Parsing: " << source_path << "\n";
#endif
    auto parse_res = to_lang::parse_file(source_path);

    if (!parse_res.success) {
        std::cerr << source_path << ":" << parse_res.error_line << ": syntax error: "
                  << parse_res.error_message << "\n";
        return 2;
    }

    if (run_semantics && parse_res.root) {
#ifdef DEBUG
        std::cout << "Running semantic analysis...\n";
#endif
        to_lang::SemanticAnalyzer analyzer;
        bool ok = analyzer.analyze(parse_res.root);

        if (!ok) {
            for (const auto& err : analyzer.get_errors()) {
                std::cerr << source_path << ":" << err.line << ":" << err.col << ": error: "
                          << err.message << "\n";
            }
            return 3;
        }
#ifdef DEBUG
        std::cout << "Semantic analysis passed.\n";
#endif
    }

    if (dump_ast && parse_res.root) {
        parse_res.root->dump(std::cout);
    }

    if (print_dot && parse_res.root) {
        std::cout << to_lang::export_ast_to_dot(parse_res.root);
    }

    if (!dot_path.empty() && parse_res.root) {
        std::ofstream dot_file(dot_path);
        if (dot_file.is_open()) {
            dot_file << to_lang::export_ast_to_dot(parse_res.root);
#ifdef DEBUG
            std::cout << "Saved DOT to: " << dot_path << "\n";
#endif
        } else {
            std::cerr << "Cannot open output DOT file: " << dot_path << "\n";
            return 4;
        }
    }

    if (!png_path.empty() && parse_res.root) {
        std::string tmp_dot = png_path + ".tmp.dot";
        std::ofstream out(tmp_dot);
        if (out.is_open()) {
            out << to_lang::export_ast_to_dot(parse_res.root);
            out.close();
            std::string cmd = "dot -Tpng \"" + tmp_dot + "\" -o \"" + png_path + "\"";
            int ret = system(cmd.c_str());
            remove(tmp_dot.c_str());
            if (ret != 0) {
                std::cerr << "Failed to render PNG (is Graphviz 'dot' installed?)\n";
            }
#ifdef DEBUG
            else {
                std::cout << "Rendered PNG to: " << png_path << "\n";
            }
#endif
        }
    }

    return 0;
}
