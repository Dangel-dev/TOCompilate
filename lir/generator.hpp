//
// Created by dangel on 15.11.2026.
//

#ifndef TOC_LIR_GENERATOR_HPP
#define TOC_LIR_GENERATOR_HPP

#include "lir.hpp"
#include "../hir/hir.hpp"
#include <unordered_map>
#include <unordered_set>

namespace to_lang::lir {

    // Генератор LIR из HIR с разметкой стекового кадра и понижением инструкций
    class LIRGenerator {
    public:
        static LIRTranslationUnitPtr generate(const hir::HIRTranslationUnitPtr& hir_unit);
        static LIRFunctionPtr lower_function(const hir::HIRFunctionPtr& hir_fn);

    private:
        hir::HIRFunctionPtr current_hir_fn_;
        LIRFunctionPtr lir_fn_;

        // Привязка объектов HIR к операндам LIR (vreg или stack slot)
        std::unordered_map<std::string, Operand> object_locations_;

        // Множество объектов, на которые брали указатель &x
        std::unordered_set<std::string> addressed_objects_;

        // Переменные, живущие исключительно в виртуальных регистрах
        std::unordered_set<std::string> reg_variables_;

        // Анализ адресации
        void analyze_addressed_objects();

        // Разметка стека
        void layout_stack_frame();

        // Понижение инструкций
        void lower_operations();

        // Получение размера типа в байтах
        size_t compute_type_size(const TypeNodePtr& type);

        // Получение или создание LIR операнда для HIR объекта
        Operand get_or_create_operand(const hir::HIRObjectPtr& obj);
    };

} // namespace to_lang::lir

#endif // TOC_LIR_GENERATOR_HPP

