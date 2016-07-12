#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

FunctionTranslator::FunctionTranslator()
{
    for (size_t i = 0; i <= std::numeric_limits<uint8_t>::max(); ++i) {
        freeRegisters_.push_back(static_cast<uint8_t>(i));
    }
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

uint8_t FunctionTranslator::dispatch(const Ast::Node &node)
{
    return Ast::dispatch<uint8_t, FunctionTranslator>(*this, node);
}

uint8_t FunctionTranslator::visit(const NumberExpr &expr)
{
    if ((expr.value() < 0) || (std::numeric_limits<uint16_t>::max() < expr.value())) {
        abort();
    }
    uint16_t value = static_cast<uint16_t>(expr.value());
    uint8_t dst = allocateTempRegister();
    emit(InstructionKind::Load, dst, value);
    return dst;
}

uint8_t FunctionTranslator::visit(const CharacterExpr &expr)
{
    return 0;
}

uint8_t FunctionTranslator::visit(const StringExpr &expr)
{
    return 0;
}

uint8_t FunctionTranslator::visit(const IdentifierExpr &expr)
{
    auto it = identifierMap_.find(expr.name());
    if (it == identifierMap_.end()) {
        const uint8_t reg = allocateRegister();
        identifierMap_.insert(std::make_pair(expr.name(), reg));
        return reg;
    }
    else {
        return it->second;
    }
}

uint8_t FunctionTranslator::visit(const BinaryExpr &expr)
{
    uint8_t lhs = dispatch(*expr.lhsExpr());
    uint8_t rhs = dispatch(*expr.rhsExpr());
    uint8_t dst = allocateTempRegister();
    switch (expr.op()) {
    case BinaryExpr::Operator::Add:
        emit(InstructionKind::Add, dst, lhs, rhs);
        break;
    case BinaryExpr::Operator::Sub:
        emit(InstructionKind::Sub, dst, lhs, rhs);
        break;
    default:
        abort();
    }

    return dst;
}

uint8_t FunctionTranslator::visit(const UnaryExpr &expr)
{
    uint8_t src = dispatch(*expr.expr());
    uint8_t dst = allocateTempRegister();
    switch (expr.op()) {
    default:
        abort();
    }

    return dst;
}

uint8_t FunctionTranslator::visit(const CallExpr &expr)
{
    abort();
    return 0;
}

uint8_t FunctionTranslator::visit(const DerefExpr &expr)
{
    abort();
    return 0;
}

uint8_t FunctionTranslator::visit(const AssignExpr &expr)
{
    uint8_t dst = dispatch(*expr.lhsExpr());
    uint8_t src = dispatch(*expr.rhsExpr());
    emit(InstructionKind::Store, dst, src);
    return dst;
}

uint8_t FunctionTranslator::visit(const ExprStmt &stmt)
{
    const uint8_t reg = dispatch(*stmt.expr());
    freeTempRegisters();
    return reg;
}

uint8_t FunctionTranslator::visit(const Node &node)
{
    // FIXME: Set error and return
    abort();
    return 0;
}

void FunctionTranslator::emit(InstructionKind kind) {
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

uint8_t FunctionTranslator::allocateRegister()
{
    if (freeRegisters_.empty()) {
        // FIXME: Return an error or something
        abort();
    }
    else {
        const uint8_t reg = freeRegisters_.back();
        freeRegisters_.pop_back();
        return reg;
    }
}

uint8_t FunctionTranslator::allocateTempRegister()
{
    const uint8_t reg = allocateRegister();
    tempRegisters_.push_back(reg);
    return reg;
}

void FunctionTranslator::freeTempRegisters()
{
    for (uint8_t reg: tempRegisters_) {
        freeRegisters_.push_back(reg);
    }

    tempRegisters_.clear();
}
