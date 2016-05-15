#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Stmt::Stmt(NodeType nodeType, NodeContent nodeContent)
    : Node(nodeType, nodeContent)
{
}

RootStmt::RootStmt(NodeContent nodeContent, StmtVec stmts)
    : Stmt(NodeType::RootStmt, nodeContent)
    , stmts_(std::move(stmts))
{
}

const StmtVec &RootStmt::stmts() const
{
    return stmts_;
}

void RootStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

BlockStmt::BlockStmt(NodeContent nodeContent, StmtVec stmts)
    : Stmt(NodeType::BlockStmt, nodeContent)
    , stmts_(std::move(stmts))
{
}

const StmtVec &BlockStmt::stmts() const
{
    return stmts_;
}

void BlockStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

ModuleStmt(NodeContent nodeContent, StringView name, ExprVec args, BlockStmtPtr block)
    : Stmt(NodeType::ModuleStmt, nodeContent)
    , name_(name)
    , args_(std::move(args))
    , block_(std::move(block))
{
}

const StringView &name() const
{
    return name_;
}

const ExprVec &args() const
{
    return args_;
}

const BlockStmtPtr &ModuleStmt::block() const
{
    return block_;
}

void ModuleStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
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
