//
// Created by dangel on 11.11.2026.
//

#include "../hir.hpp"
#include "../generator.hpp"
#include "../verifier.hpp"
#include "../cfg.hpp"
#include "../../frontend/parser/parser.hpp"
#include "../../frontend/semantics/semantic_analyzer.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace to_lang::hir;

int main(int argc, char* argv[]) {
    std::string source_path = "";
    std::string cfg_dot_path = "";
    bool print_hir = false;
    bool verify_only = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--hir") {
            print_hir = true;
        } else if (arg == "--verify") {
            verify_only = true;
        } else if (arg == "--cfg" && i + 1 < argc) {
            cfg_dot_path = argv[++i];
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        } else {
            source_path = arg;
        }
    }

    if (source_path.empty()) {
        std::cerr << "Usage: hir_cli [options] <source.to>\n"
                  << "Options:\n"
                  << "  --hir           Print linear HIR text\n"
                  << "  --cfg <file>    Export Control Flow Graph to DOT\n"
                  << "  --verify        Run HIR verification checks\n";
        return 1;
    }

    // 1. Синтаксический разбор в AST
    auto parse_res = to_lang::parse_file(source_path);
    if (!parse_res.success) {
        std::cerr << "Syntax error in " << source_path << ": " << parse_res.error_message << "\n";
        return 2;
    }

    // 2. Семантический анализ
    to_lang::SemanticAnalyzer sem;
    if (!sem.analyze(parse_res.root)) {
        std::cerr << "Semantic analysis failed for " << source_path << "\n";
        return 3;
    }

    // 3. Генерация линейного HIR из AST
    HIRGenerator generator;
    auto unit = generator.generate(parse_res.root);

    // 4. Верификация промежуточного представления
    HIRVerifier verifier;
    if (!verifier.verify(unit)) {
        std::cerr << "HIR verification failed:\n";
        for (const auto& err : verifier.get_errors()) {
            std::cerr << "  " << err.format() << "\n";
        }
        return 4;
    }

    if (verify_only) {
        std::cout << "HIR verification passed for " << source_path << "\n";
        return 0;
    }

    // 5. Вывод линейного представления
    if (print_hir || cfg_dot_path.empty()) {
        std::cout << unit->to_string() << "\n";
    }

    // 6. Построение CFG и экспорт в DOT
    if (!cfg_dot_path.empty()) {
        std::ofstream out(cfg_dot_path);
        if (!out.is_open()) {
            std::cerr << "Cannot open output file: " << cfg_dot_path << "\n";
            return 5;
        }

        for (const auto& fn : unit->functions) {
            auto cfg = ControlFlowGraph::build(fn);
            out << cfg->to_dot() << "\n";
        }
        std::cout << "Saved CFG DOT to: " << cfg_dot_path << "\n";
    }

    return 0;
}

