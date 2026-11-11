//
// Created by dangel on 11.11.2026.
//

#ifndef TOC_HIR_VERIFIER_HPP
#define TOC_HIR_VERIFIER_HPP

#include "hir.hpp"
#include <string>
#include <vector>
#include <unordered_set>

namespace to_lang::hir {

    struct VerificationError {
        std::string message;
        std::string function_name;
        int line = 1;
        int col = 1;

        std::string format() const {
            return "[HIR Error] in @" + function_name + " (" +
                   std::to_string(line) + ":" + std::to_string(col) + "): " + message;
        }
    };

    class HIRVerifier {
    private:
        std::vector<VerificationError> errors_;
        const HIRFunction* current_func_ = nullptr;

        void error(const std::string& msg, int l = 1, int c = 1);
        void verify_operation(const HIROperationPtr& op,
                              std::unordered_set<std::string>& defined_objects,
                              const std::unordered_set<std::string>& known_labels);

        void check_use(const HIRObjectPtr& obj,
                       const std::unordered_set<std::string>& defined_objects,
                       int l, int c);

    public:
        HIRVerifier() = default;

        bool verify(const HIRTranslationUnitPtr& unit);
        bool verify_function(const HIRFunctionPtr& fn);

        const std::vector<VerificationError>& get_errors() const noexcept { return errors_; }
        bool has_errors() const noexcept { return !errors_.empty(); }
        void clear_errors() noexcept { errors_.clear(); }
    };

} // namespace to_lang::hir

#endif // TOC_HIR_VERIFIER_HPP
