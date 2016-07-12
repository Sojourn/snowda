#include "snw_code.h"

using namespace Snowda;

InstructionBuffer::InstructionBuffer()
    : frozen_(false)
{
}

void InstructionBuffer::freeze() {
    frozen_ = true;
}

Result<uint16_t, StringView> InstructionBuffer::emit(InstructionKind kind) {
    assert(instructionLayout(kind) == InstructionLayout::A);
    assert(!frozen_);

    Instruction inst;
    inst.kind = kind;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    const uint16_t label = static_cast<uint16_t>(buffer_.size());
    buffer_.insert(buffer_.end(), begin, end);
    return finishEmit(label);
}

Result<uint16_t, StringView> InstructionBuffer::emit(InstructionKind kind, uint8_t dst, uint16_t src) {
    assert(instructionLayout(kind) == InstructionLayout::B);
    assert(!frozen_);

    Instruction inst;
    inst.kind = kind;
    inst.b.dst = dst;
    inst.b.src = src;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    const uint16_t label = static_cast<uint16_t>(buffer_.size());
    buffer_.insert(buffer_.end(), begin, end);
    return finishEmit(label);
}

Result<uint16_t, StringView> InstructionBuffer::emit(InstructionKind kind, uint8_t dst, uint8_t src) {
    assert(instructionLayout(kind) == InstructionLayout::C);
    assert(!frozen_);

    Instruction inst;
    inst.kind = kind;
    inst.c.dst = dst;
    inst.c.src = src;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    const uint16_t label = static_cast<uint16_t>(buffer_.size());
    buffer_.insert(buffer_.end(), begin, end);
    return finishEmit(label);
}

Result<uint16_t, StringView> InstructionBuffer::emit(InstructionKind kind, uint8_t dst, uint8_t lhs, uint8_t rhs) {
    assert(instructionLayout(kind) == InstructionLayout::D);
    assert(!frozen_);

    Instruction inst;
    inst.kind = kind;
    inst.d.dst = dst;
    inst.d.lhs = lhs;
    inst.d.rhs = rhs;

    const uint8_t *begin = reinterpret_cast<const uint8_t *>(&inst);
    const uint8_t *end = begin + instructionSize(kind);
    const uint16_t label = static_cast<uint16_t>(buffer_.size());
    buffer_.insert(buffer_.end(), begin, end);
    return finishEmit(label);
}

Instruction InstructionBuffer::operator[](uint16_t index) const {
    const uint8_t *instructionAddr = &buffer_.at(index);
    const InstructionKind kind = static_cast<InstructionKind>(*instructionAddr);

    Instruction instruction;
    memcpy(&instruction, instructionAddr, instructionSize(kind));
    return instruction;
}

Result<uint16_t, StringView> InstructionBuffer::finishEmit(uint16_t label) {
    if (buffer_.size() > std::numeric_limits<uint16_t>::max()) {
        buffer_.resize(label);
        return StringView("Instruction buffer overflow");
    }
    else {
        return label;
    }
}
