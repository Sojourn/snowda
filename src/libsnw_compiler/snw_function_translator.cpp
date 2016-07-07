#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

FunctionTranslator::FunctionTranslator()
    : nextRegisterId_(0)
{
}

Result<Function, StringView> FunctionTranslator::result()
{
    if (error_.empty()) {
        return function_;
    }
    else {
        return error_;
    }
}

void FunctionTranslator::emit(InstructionKind kind)
{
    assert(instructionLayout(kind) == InstructionLayout::A);

    Instruction inst;
    inst.kind = kind;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    function_.instructions.insert(function_.instructions.end(), begin, end);
}

void FunctionTranslator::emit(InstructionKind kind, uint8_t dst, uint16_t src)
{
    assert(instructionLayout(kind) == InstructionLayout::B);

    Instruction inst;
    inst.kind = kind;
    inst.b.dst = dst;
    inst.b.src = src;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    function_.instructions.insert(function_.instructions.end(), begin, end);
}

void FunctionTranslator::emit(InstructionKind kind, uint8_t dst, uint8_t src)
{
    assert(instructionLayout(kind) == InstructionLayout::C);

    Instruction inst;
    inst.kind = kind;
    inst.c.dst = dst;
    inst.c.src = src;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    function_.instructions.insert(function_.instructions.end(), begin, end);
}

void FunctionTranslator::emit(InstructionKind kind, uint8_t dst, uint8_t lhs, uint8_t rhs)
{
    assert(instructionLayout(kind) == InstructionLayout::D);

    Instruction inst;
    inst.kind = kind;
    inst.d.dst = dst;
    inst.d.lhs = lhs;
    inst.d.rhs = rhs;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    function_.instructions.insert(function_.instructions.end(), begin, end);
}
