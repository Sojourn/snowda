#include "snw_code.h"

using namespace Snowda;

RegisterLifetime::RegisterLifetime(InstructionIndex birth, InstructionIndex death)
    : birth_(birth)
    , death_(death)
{
}

InstructionIndex RegisterLifetime::birth() const
{
    return birth_;
}

InstructionIndex RegisterLifetime::death() const
{
    return death_;
}

Register::Register(StringView name, TypeId typeId, RegisterLifetime lifetime)
    : name_(name)
    , typeId_(typeId)
    , lifetime_(lifetime)
{
}

StringView Register::name() const
{
    return name_;
}

TypeId Register::typeId() const
{
    return typeId_;
}

RegisterLifetime Register::lifetime() const
{
    return lifetime_;
}

Function::Function(View<Instruction> instructions, View<Register> registers)
    : instructions_(instructions)
    , registers_(registers)
{
}

View<Instruction> Function::instructions() const
{
    return instructions_;
}

View<Register> Function::registers() const
{
	return registers_;
}
