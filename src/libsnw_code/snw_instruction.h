#ifndef SNW_INSTRUCTION_H
#define SNW_INSTRUCTION_H

#define SNW_INSTRUCTION_KINDS \
    X(Add,  D) \
    X(Sub,  D) \
    X(Mul,  D) \
    X(Div,  D) \
    X(Load, B) \
    X(Move, C) \
    X(Call, A) \
    X(Ret,  A)

namespace Snowda {

    enum class InstructionKind : uint8_t {
    #define X(name, form) name,
        SNW_INSTRUCTION_KINDS
    #undef X
    };

    union Instruction {
        InstructionKind kind;

        struct A {
            InstructionKind kind;
        } a;

        struct B {
            InstructionKind kind;
            uint8_t dst;
            uint16_t src;
        } b;

        struct C {
            InstructionKind kind;
            uint8_t dst;
            uint8_t src;
        } c;

        struct D {
            InstructionKind kind;
            uint8_t dst;
            uint8_t lhs;
            uint8_t rhs;
        } d;
    };

    size_t instructionSize(InstructionKind kind);
    StringView instructionName(InstructionKind kind);

}

#endif // SNW_INSTRUCTION_H
