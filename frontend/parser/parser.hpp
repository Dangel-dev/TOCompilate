//
// Created by dangel on 21.09.2026.
//

#ifndef TOC_PARSER_HPP
#define TOC_PARSER_HPP

#include "../ast/ast.hpp"
#include <string>
#include <memory>

namespace to_lang {

    struct ParseResult {
        bool success = false;
        ProgramNodePtr root = nullptr;
        std::string error_message;
        int error_line = 0;
    };

    // Парсинг файла с кодом
    ParseResult parse_file(const std::string& filepath);

    // Парсинг строки с кодом
    ParseResult parse_string(const std::string& source_code);

} // namespace to_lang

#endif // TOC_PARSER_HPP

