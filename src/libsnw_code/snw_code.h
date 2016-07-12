#ifndef SNW_CODE_H
#define SNW_CODE_H

#include <vector>

#include <cstdint>
#include <cassert>

#include "snw_util.h"
#include "snw_memory.h"

namespace Snowda {
	struct TypeId {
        uint32_t value;
    };

	struct MethodId {
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
    class Function;
    union Instruction;
}

#include "snw_instruction.h"
#include "snw_instruction_util.h"
#include "snw_instruction_buffer.h"
#include "snw_type.h"
#include "snw_function.h"

#endif // SNW_CODE_H
