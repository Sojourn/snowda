#ifndef SNW_INSTRUCTION_UTIL_H
#define SNW_INSTRUCTION_UTIL_H

namespace Snowda {
    static StringView instructionName(InstructionKind kind);
    static size_t instructionSize(InstructionKind kind);
}

#endif // SNW_INSTRUCTION_UTIL_H
