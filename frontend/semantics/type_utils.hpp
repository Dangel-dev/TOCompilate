//
// Created by dangel on 28.09.2026.
//

#ifndef TOC_TYPE_UTILS_HPP
#define TOC_TYPE_UTILS_HPP

#include "../ast/ast.hpp"

namespace to_lang {

    // Проверка эквивалентности типов
    inline bool are_types_equal(const TypeNodePtr& t1, const TypeNodePtr& t2) {
        if (!t1 && !t2) return true;
        if (!t1 || !t2) return false;
        if (t1->kind != t2->kind) return false;

        switch (t1->kind) {
            case TypeNode::Kind::BASE:
                return t1->base_kind == t2->base_kind;
            case TypeNode::Kind::POINTER:
                return are_types_equal(t1->subtype, t2->subtype);
            case TypeNode::Kind::ARRAY:
                if (!are_types_equal(t1->subtype, t2->subtype)) return false;
                if (t1->array_size >= 0 && t2->array_size >= 0) {
                    return t1->array_size == t2->array_size;
                }
                return true;
        }
        return false;
    }

    inline bool is_integer_type(const TypeNodePtr& t) {
        return t && t->kind == TypeNode::Kind::BASE &&
               (t->base_kind == BaseTypeKind::I32 || t->base_kind == BaseTypeKind::I64);
    }

    inline bool is_boolean_type(const TypeNodePtr& t) {
        return t && t->kind == TypeNode::Kind::BASE && t->base_kind == BaseTypeKind::BOOL;
    }

    inline bool is_char_type(const TypeNodePtr& t) {
        return t && t->kind == TypeNode::Kind::BASE && t->base_kind == BaseTypeKind::CHAR;
    }

    inline bool is_pointer_type(const TypeNodePtr& t) {
        return t && t->kind == TypeNode::Kind::POINTER;
    }

    inline bool is_array_type(const TypeNodePtr& t) {
        return t && t->kind == TypeNode::Kind::ARRAY;
    }

    inline bool is_void_type(const TypeNodePtr& t) {
        return t && t->kind == TypeNode::Kind::BASE && t->base_kind == BaseTypeKind::VOID;
    }

    // Проверка допустимости неявного приведения
    inline bool can_implicitly_convert(const TypeNodePtr& from, const TypeNodePtr& to) {
        if (are_types_equal(from, to)) return true;
        if (!from || !to) return false;

        // Расширение целых: i32 -> i64, char -> i32 / i64
        if (is_integer_type(to) && is_char_type(from)) return true;
        if (to->kind == TypeNode::Kind::BASE && to->base_kind == BaseTypeKind::I64 &&
            from->kind == TypeNode::Kind::BASE && from->base_kind == BaseTypeKind::I32) {
            return true;
        }

        // Распад массива в указатель: 
        if (is_array_type(from) && is_pointer_type(to)) {
            return are_types_equal(from->subtype, to->subtype);
        }

        // Приведение любого указателя к $ void
        if (is_pointer_type(from) && is_pointer_type(to)) {
            if (to->subtype && to->subtype->kind == TypeNode::Kind::BASE &&
                to->subtype->base_kind == BaseTypeKind::VOID) {
                return true;
            }
        }

        return false;
    }

} // namespace to_lang

#endif // TOC_TYPE_UTILS_HPP

