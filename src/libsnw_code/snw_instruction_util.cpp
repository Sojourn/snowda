#include "snw_code.h"

using namespace Snowda;

StringView Snowda::instructionName(InstructionKind kind)
{
    switch (kind) {
#define X(name, form) case InstructionKind::name: \
        return StringView(#name);
        SNW_INSTRUCTION_KINDS
#undef X
        default:
            abort();
    }
}

size_t Snowda::instructionSize(InstructionKind kind)
{
    switch (kind) {
#define X(name, form) case InstructionKind::name: \
        return sizeof(Instruction::form);
        SNW_INSTRUCTION_KINDS
#undef X
        default:
            abort();
    }
}

InstructionLayout Snowda::instructionLayout(InstructionKind kind)
{
    switch (kind) {
#define X(xName, xLayout) case InstructionKind::xName: \
        return InstructionLayout::xLayout;
        SNW_INSTRUCTION_KINDS
#undef X
        default:
            abort();
    }
}
