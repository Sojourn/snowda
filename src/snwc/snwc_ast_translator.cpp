#include "snwc.h"

AstTranslator::AstTranslator(const char *src)
    : src_(src)
{
    code_.stack = 0;

    Parser parser(src);
    while (AstExpr *expr = parser.parseStmt()) {
        dispatch<uint8_t>(*this, expr);
    }
}

const Code &AstTranslator::code() const {
    return code_;
}

uint8_t AstTranslator::visit(UnaryOperationAstExpr *expr)
{
    // Not implemented
    abort();
}

uint8_t AstTranslator::visit(BinaryOperationAstExpr *expr)
{
    const uint8_t lhs = dispatch<uint8_t>(*this, expr->leftExpr);
    const uint8_t rhs = dispatch<uint8_t>(*this, expr->rightExpr);
    const uint8_t dst = getTempVariable();

    Instruction inst;
    switch (expr->opKind) {
    case BinaryOperationKind::Add:
        inst.kind = InstructionKind::Add;
        break;
    case BinaryOperationKind::Subtract:
        inst.kind = InstructionKind::Subtract;
        break;
    default:
        abort();
    }

    inst.data[0] = dst;
    inst.data[1] = lhs;
    inst.data[2] = rhs;
    emit(inst);

    return dst;
}

uint8_t AstTranslator::visit(AssignAstExpr *expr)
{
    const uint8_t src = dispatch<uint8_t>(*this, expr->leftExpr);
    const uint8_t dst = dispatch<uint8_t>(*this, expr->rightExpr);

    Instruction inst;
    inst.kind = InstructionKind::Move;
    inst.data[0] = dst;
    inst.data[1] = src;
    inst.data[2] = 0;
    emit(inst);

    return dst;
}

uint8_t AstTranslator::visit(NumberAstExpr *expr)
{
    const uint8_t slot = getTempVariable();

    uint16_t value = 0;
    for (size_t i = 0; i < expr->len; ++i) {
        value = value * 10 + (src_[expr->pos + i] - '0');
    }

    Instruction inst;
    inst.kind = InstructionKind::Load;
    inst.data[0] = slot;
    memcpy(&inst.data[1], &value, sizeof(value));
    emit(inst);

    return slot;
}

uint8_t AstTranslator::visit(SymbolAstExpr *expr)
{
    return getNamedVariable(std::string(src_ + expr->pos, src_ + expr->pos + expr->len));
}

uint8_t AstTranslator::getTempVariable()
{
    return code_.stack++;
}

uint8_t AstTranslator::getNamedVariable(std::string name)
{
    auto it = code_.vars.find(name);
    if (it == code_.vars.end()) {
        std::tie(it, std::ignore) = code_.vars.insert(std::make_pair(name, code_.stack++));
    }

    return it->second;
}

void AstTranslator::emit(Instruction inst)
{
    code_.insts.push_back(inst);
}
