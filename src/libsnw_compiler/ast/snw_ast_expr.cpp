#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Expr::Expr(NodeContent nodeContent, NodeType type)
    : Node(nodeContent, type)
{
}

NumberExpr::NumberExpr(NodeContent nodeContent, int value)
    : Expr(NodeType::NumberExpr, nodeContent)
    , value_(value)
{
}

int NumberExpr::value() const
{
    return value_;
}

void NumberExpr::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

CharacterExpr::CharacterExpr(NodeContent nodeContent, char value)
    : Expr(NodeType::CharacterExpr, nodeContent)
    , value_(value)
{
}

char CharacterExpr::value() const
{
    return value_;
}

void CharacterExpr::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

StringExpr::StringExpr(NodeContent nodeContent, StringView value)
    : Expr(NodeType::StringExpr, nodeContent)
    , value_(value)
{
}

const StringView &StringExpr::value() const
{
    return value_;
}

void StringExpr::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

IdentifierExpr::IdentifierExpr(NodeContent nodeContent, StringView name)
    : Expr(NodeType::IdentifierExpr, nodeContent)
    , name_(name)
{
}

const StringView &IdentifierExpr::name() const
{
    return name_;
}

void IdentifierExpr::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

UnaryExpr::UnaryExpr(NodeContent nodeContent, Operator op, ExprPtr expr)
    : Expr(NodeType::UnaryExpr, nodeContent)
    , op_(op)
    , expr_(expr)
{
}

UnaryExpr::Operator UnaryExpr::op() const
{
    return op_;
}

StringView UnaryExpr::opName() const
{
    switch (op) {
#define X(xType, xName) case Operator::xType: return StringView(xName);
    SNW_AST_UNARY_OPERATORS
#undef X
    default:
        abort();
    }
}

const ExprPtr &UnaryExpr::expr() const
{
    return expr_;
}

void UnaryExpr::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

BinaryExpr::BinaryExpr(NodeContent nodeContent, Operator op, ExprPtr lhsExpr, ExprPtr rhsExpr)
    : Expr(NodeType::BinaryExpr, nodeContent)
    , op_(op)
    , lhsExpr_(std::move(lhsExpr))
    , rhsExpr_(std::move(rhsExpr))
{
}

BinaryExpr::Operator BinaryExpr::op() const
{
    return op_;
}

StringView BinaryExpr::opName() const
{
    switch (op) {
#define X(xType, xName) case Operator::xType: return StringView(xName);
    SNW_AST_BINARY_OPERATORS
#undef X
    default:
        abort();
    }
}

const ExprPtr &BinaryExpr::lhsExpr() const
{
    return lhsExpr_;
}

const ExprPtr &BinaryExpr::rhsExpr() const
{
    return rhsExpr_;
}
