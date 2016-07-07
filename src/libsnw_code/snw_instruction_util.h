#ifndef SNW_INSTRUCTION_UTIL_H
#define SNW_INSTRUCTION_UTIL_H

namespace Snowda {
    StringView instructionName(InstructionKind kind);
    size_t instructionSize(InstructionKind kind);
    InstructionLayout instructionLayout(InstructionKind kind);
}

#endif // SNW_INSTRUCTION_UTIL_H
