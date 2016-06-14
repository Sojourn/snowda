#include "snw_code.h"

using namespace Snowda;

StringView Snowda::instructionLayoutName(InstructionLayout layout)
{
    switch (layout) {
    case InstructionLayout::A:
        return StringView::chr<'A'>();
    case InstructionLayout::B:
        return StringView::chr<'B'>();
    default:
        abort();
    }
}

StringView Snowda::opCodeName(OpCode opCode)
{
    switch (opCode) {
#define X(name) case static_cast<OpCode>(Detail::OpCodeIndex::name | (static_cast<uint8_t>(InstructionLayout::A) << 7)): \
        return StringView(#name);
        OP_CODES_A
#undef X
#define X(name) case static_cast<OpCode>(Detail::OpCodeIndex::name | (static_cast<uint8_t>(InstructionLayout::B) << 7)): \
        return StringView(#name);
        OP_CODES_B
#undef X
    default:
        abort();
    }
}

InstructionLayout Instruction::layout() const
{
    return static_cast<InstructionLayout>(static_cast<uint8_t>(opCode) >> 7);
}
