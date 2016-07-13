#ifndef SNW_CODE_H
#define SNW_CODE_H

#include <vector>

#include <cstdint>
#include <cassert>

#include "snw_util.h"
#include "snw_memory.h"

namespace Snowda {
    using RawTypeId = uint16_t;
    using RawModuleId = uint16_t;
    using RawFunctionId = uint16_t;
    using RawMemberId = uint16_t;
    using RawConstantId = uint16_t;

	struct TypeId {
        RawTypeId value;
    };

    struct ModuleId {
        uint16_t value;
    };

	struct FunctionId {
        uint16_t value;
    };

    struct MemberId {
        uint16_t value;
    };

    struct ConstantId {
        uint16_t value;
    };

    struct InstructionIndex {
        uint16_t value;
    };

    class Type;
    class TypeSystem;
    class Function;
    union Instruction;
}


#include "snw_instruction.h"
#include "snw_instruction_util.h"
#include "snw_instruction_buffer.h"
#include "snw_type.h"
#include "snw_function.h"

#endif // SNW_CODE_H
