#ifndef SNW_INSTRUCTION_H
#define SNW_INSTRUCTION_H

#define OP_CODES_A \
    X(Nop) \
    X(End) \
    X(SafePoint) \
    X(Add) \
    X(Sub) \
    X(Mul) \
    X(Div) \
    X(Load) \
    X(Store)

#define OP_CODES_B \
    X(Call) \
    X(Ret) \
    X(LoadI)

#define OP_CODES \
    OP_CODES_A \
    OP_CODES_B

namespace Snowda {
    namespace Detail {
        struct OpCodeIndex {
            enum {
#define X(name) name,
                OP_CODES
#undef X
            };
        };
    }

    enum InstructionLayout {
        A = 0,
        B = 1,
    };
    StringView instructionLayoutName(InstructionLayout layout);

    enum class OpCode : uint8_t {
#define X(name) name = Detail::OpCodeIndex::name | (static_cast<uint8_t>(InstructionLayout::A) << 7),
        OP_CODES_A
#undef X
#define X(name) name = Detail::OpCodeIndex::name | (static_cast<uint8_t>(InstructionLayout::B) << 7),
        OP_CODES_B
#undef X
    };

    StringView opCodeName(OpCode opCode);

    struct Instruction {
        OpCode opCode;

        union {
            struct {
                uint8_t res;
                uint8_t lhs;
                uint8_t rhs;
            } a;

            #pragma pack(1)
            struct {
                uint8_t res;
                uint16_t val;
            } b;
            #pragma pack()
        };

        InstructionLayout layout() const;
    };
    static_assert(sizeof(Instruction) == sizeof(uint32_t), "Bad instruction layout");
}

#endif // SNW_INSTRUCTION_H
