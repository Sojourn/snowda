#ifndef SNW_METHOD_H
#define SNW_METHOD_H

namespace Snowda {
    struct StackEntry {
        uint32_t type;
        uint32_t offset;
    };

    struct Method {
        uint32_t typeId;
        uint32_t methodId;
        uint16_t instructionCount;
        uint16_t stackEntryCount;
        Instruction *instructions;
        StackEntry *stackEntries;
    };
}

#endif // SNW_METHOD_H
