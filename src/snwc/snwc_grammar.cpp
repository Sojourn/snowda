#include "snwc.h"

Grammar::Grammar() {
    for (GrammarRule &rule: rules_) {
        rule.bindingPower = BindingPower::None;
        rule.nud = &defaultNud;
        rule.led = &defaultLed;
    }

    {
        GrammarRule &rule = (*this)[TokenKind::Plus];
        rule.bindingPower = BindingPower::Sum;
        rule.nud = &unaryOperationNud<UnaryOperationKind::Pos>;
        rule.led = &binaryOperationLed<BinaryOperationKind::Add>;
    }
    {
        GrammarRule &rule = (*this)[TokenKind::Minus];
        rule.bindingPower = BindingPower::Sum;
        rule.nud = &unaryOperationNud<UnaryOperationKind::Neg>;
        rule.led = &binaryOperationLed<BinaryOperationKind::Subtract>;
    }
    {
        GrammarRule &rule = (*this)[TokenKind::Assign];
        rule.bindingPower = BindingPower::Assignment;
        rule.led = &assignLed;
    }
    {
        GrammarRule &rule = (*this)[TokenKind::Number];
        rule.nud = &numberNud;
    }
    {
        GrammarRule &rule = (*this)[TokenKind::Symbol];
        rule.nud = &symbolNud;
    }
}

GrammarRule &Grammar::operator[](TokenKind kind) {
    return rules_.at(static_cast<size_t>(kind));
}

const GrammarRule &Grammar::operator[](TokenKind kind) const {
    return rules_.at(static_cast<size_t>(kind));
}

AstExpr *Grammar::defaultNud(Parser &parser, Token token) {
    return nullptr;
}

AstExpr *Grammar::defaultLed(Parser &parser, AstExpr *left, Token token) {
    return nullptr;
}

template<UnaryOperationKind operationKind>
AstExpr *Grammar::unaryOperationNud(Parser &parser, Token token) {
    AstExpr *expr = parser.parseExpr(BindingPower::Unary);
    if (!expr) {
        return nullptr;
    }

    UnaryOperationAstExpr *result = new UnaryOperationAstExpr;
    result->exprKind = AstExprKind::UnaryOperation;
    result->opKind = operationKind;
    result->expr = expr;
    return result;
}

template<BinaryOperationKind operationKind>
AstExpr *Grammar::binaryOperationLed(Parser &parser, AstExpr *left, Token token) {
    AstExpr *right = parser.parseExpr(parser.grammar()[token.kind].bindingPower);
    if (!right) {
        return nullptr;
    }

    BinaryOperationAstExpr *result = new BinaryOperationAstExpr;
    result->exprKind = AstExprKind::BinaryOperation;
    result->opKind = operationKind;
    result->leftExpr = left;
    result->rightExpr = right;
    return result;
}

AstExpr *Grammar::assignLed(Parser &parser, AstExpr *left, Token token) {
    AstExpr *right = parser.parseExpr(BindingPower::Assignment);
    if (!right) {
        return nullptr;
    }

    AssignAstExpr *result = new AssignAstExpr;
    result->exprKind = AstExprKind::Assign;
    result->leftExpr = left;
    result->rightExpr = right;
    return result;
}

AstExpr *Grammar::numberNud(Parser &parser, Token token)
{
    NumberAstExpr *result = new NumberAstExpr;
    result->exprKind = AstExprKind::Number;
    result->pos = token.pos;
    result->len = token.len;
    return result;
}

AstExpr *Grammar::symbolNud(Parser &parser, Token token)
{
    SymbolAstExpr *result = new SymbolAstExpr;
    result->exprKind = AstExprKind::Symbol;
    result->pos = token.pos;
    result->len = token.len;
    return result;
}
