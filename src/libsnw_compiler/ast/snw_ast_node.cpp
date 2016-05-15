#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Node::Node(NodeType type, NodeContent content)
    : nodeType_(type)
    , nodeContent_(content)
{
}

NodeType Node::nodeType() const
{
    return nodeType_;
}

NodeContent Node::nodeContent() const
{
    return nodeContent_;
}

bool Node::isStmt() const
{
    return (NodeType::StmtBegin < nodeType()) &&
           (nodeType() < NodeType::StmtEnd);
}

bool Node::isExpr() const
{
    return (NodeType::ExprBegin < nodeType()) &&
           (nodeType() < NodeType::ExprEnd);
}

IfStmt::IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(std::move(condExpr))
    , then_(std::move(thenExpr))
{
}

IfStmt::IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ElifVec elifs)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(std::move(condExpr))
    , then_(std::move(thenExpr))
    , elifs_(std::move(elifs))
{
}

IfStmt::IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ExprPtr elseExpr)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(std::move(condExpr))
    , then_(std::move(thenExpr))
    , else_(std::move(elseExpr))
{
}

IfStmt::IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ElifVec elifs, ExprPtr elseExpr)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(std::move(condExpr))
    , then_(std::move(thenExpr))
    , elifs_(std::move(elifs))
    , else_(std::move(elseExpr))
{
}

const ExprPtr &IfStmt::condExpr() const
{
    return cond_;
}

const ExprPtr &IfStmt::thenExpr() const
{
    return then_;
}

const IfStmt::ElifVec &IfStmt::elifExprs() const
{
    return elifs_;
}

const ExprPtr &IfStmt::elseExpr() const
{
    return else_;
}

void IfStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}
