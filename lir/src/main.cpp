//
// Created by dangel on 15.11.2026.
//

#include "../lir.hpp"
#include "../generator.hpp"
#include "../../frontend/parser/parser.hpp"
#include "../../frontend/semantics/semantic_analyzer.hpp"
#include "../../hir/generator.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <source.to> [--lir]\n";
        return 1;
    }

    std::string filename = argv[1];

    try {
        // 1. Парсинг AST
        auto parse_res = to_lang::parse_file(filename);
        if (!parse_res.success || !parse_res.root) {
            std::cerr << "Ошибка парсинга файла " << filename << std::endl;
            return 1;
        }

        // 2. Семантический анализ
        to_lang::SemanticAnalyzer analyzer;
        if (!analyzer.analyze(parse_res.root)) {
            std::cerr << "Семантическая ошибка при анализе " << filename << std::endl;
            return 1;
        }

        // 3. Генерация HIR
        to_lang::hir::HIRGenerator hir_gen;
        auto hir_unit = hir_gen.generate(parse_res.root);

        // 4. Понижение в LIR и разметка стека
        auto lir_unit = to_lang::lir::LIRGenerator::generate(hir_unit);

        // Вывод LIR
        std::cout << lir_unit->to_string() << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
