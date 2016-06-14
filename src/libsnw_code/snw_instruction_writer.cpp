#include "snw_code.h"

using namespace Snowda;

void InstructionWriter::write(OpCode opCode, uint8_t res, uint8_t lhs, uint8_t rhs)
{
    Instruction instruction;
    instruction.opCode = opCode;
    instruction.a.res = res;
    instruction.a.lhs = lhs;
    instruction.a.rhs = rhs;

    assert(instruction.layout() == InstructionLayout::A);
    instructions_.push_back(instruction);
}

void InstructionWriter::write(OpCode opCode, uint8_t res, uint16_t val)
{
    Instruction instruction;
    instruction.opCode = opCode;
    instruction.b.res = res;
    instruction.b.val = val;

    assert(instruction.layout() == InstructionLayout::B);
    instructions_.push_back(instruction);
}
