//
// Created by dangel on 15.11.2026.
//

#ifndef TOC_LIR_HPP
#define TOC_LIR_HPP

#include "../frontend/ast/ast.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace to_lang::lir {

    // Регистры процессора / виртуальной машины
    enum class RegID {
        R0 = 0, R1, R2, R3, R4, R5, R6, R7,
        R8, R9, R10, R11, R12, R13, R14, R15,
        SP, BP,
        VREG // виртуальный регистр
    };

    std::string reg_to_string(RegID reg, int vreg_id = -1);

    // Машинные коды операций LIR
    enum class Opcode {
        MOV,
        ADD, SUB, MUL, DIV, MOD,
        AND, OR, XOR, SHL, SHR,
        NEG, NOT,
        CMP_EQ, CMP_NE, CMP_LT, CMP_LE, CMP_GT, CMP_GE,
        JMP, JZ, JNZ,
        CALL, CALL_PTR, RET,
        LOAD, STORE,
        LEA,
        LABEL
    };

    std::string opcode_to_string(Opcode op);

    // Операнд машинной инструкции
    struct Operand {
        enum class Kind { NONE, REG, IMM, MEM } kind{Kind::NONE};

        RegID reg{RegID::R0};
        int vreg_id{-1};
        int64_t imm_val{0};
        int mem_offset{0};
        RegID base_reg{RegID::BP};

        static Operand none() {
            return Operand{};
        }

        static Operand make_reg(RegID r) {
            Operand op;
            op.kind = Kind::REG;
            op.reg = r;
            return op;
        }

        static Operand make_vreg(int id) {
            Operand op;
            op.kind = Kind::REG;
            op.reg = RegID::VREG;
            op.vreg_id = id;
            return op;
        }

        static Operand make_imm(int64_t val) {
            Operand op;
            op.kind = Kind::IMM;
            op.imm_val = val;
            return op;
        }

        static Operand make_mem(RegID base, int offset) {
            Operand op;
            op.kind = Kind::MEM;
            op.base_reg = base;
            op.mem_offset = offset;
            return op;
        }

        std::string to_string() const;
    };

    // Низкоуровневая инструкция LIR
    struct Instruction {
        Opcode op;
        Operand dst;
        Operand src1;
        Operand src2;
        std::string label_target;
        std::string comment;

        std::string to_string() const;
    };

    // Слот в стековом кадре [bp - offset]
    struct StackSlot {
        std::string name;
        int offset{0};      // Смещение относительно bp (отрицательное)
        size_t size{8};     // Размер слота в байтах
        bool is_addressed{false}; // Брался ли оператор &
    };

    // Разметка стекового кадра функции
    class FrameLayout {
    public:
        int total_stack_size{0};
        std::vector<StackSlot> slots;

        // Выделение слота на стеке с выравниванием по 8 байт
        int allocate_slot(const std::string& name, size_t size, bool is_addressed = false);

        bool has_slot(const std::string& name) const;
        int get_offset(const std::string& name) const;
        const StackSlot* get_slot(const std::string& name) const;
    };

    // Функция в LIR
    class LIRFunction {
    public:
        std::string name;
        TypeNodePtr return_type;
        FrameLayout frame;
        std::vector<Instruction> instructions;
        int next_vreg_id{0};

        explicit LIRFunction(std::string n, TypeNodePtr ret_t = nullptr)
            : name(std::move(n)), return_type(std::move(ret_t)) {}

        // Выделение нового виртуального регистра
        int alloc_vreg() {
            return next_vreg_id++;
        }

        // Добавление инструкции
        void emit(Opcode op, Operand dst = Operand::none(), Operand src1 = Operand::none(),
                  Operand src2 = Operand::none(), const std::string& label = "", const std::string& comment = "") {
            instructions.push_back({op, dst, src1, src2, label, comment});
        }

        std::string to_string() const;
    };

    using LIRFunctionPtr = std::shared_ptr<LIRFunction>;

    // Модуль трансляции LIR
    class LIRTranslationUnit {
    public:
        std::string name;
        std::vector<LIRFunctionPtr> functions;
        std::vector<std::pair<std::string, std::string>> string_literals;

        explicit LIRTranslationUnit(std::string n) : name(std::move(n)) {}

        void add_function(LIRFunctionPtr fn) {
            functions.push_back(std::move(fn));
        }

        std::string to_string() const;
    };

    using LIRTranslationUnitPtr = std::shared_ptr<LIRTranslationUnit>;

} // namespace to_lang::lir

#endif // TOC_LIR_HPP

