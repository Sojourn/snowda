#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    void pad(size_t count)
    {
        for (size_t i = 0; i < (count - 1); ++i) {
            std::cout << "  ";
        }
    }
}

Printer::Printer()
    : depth_(0)
{
}

void Printer::visit(const NumberExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << " value: " << expr.value() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const CharacterExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << " value: " << expr.value() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const StringExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << " value: " << expr.value() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const IdentifierExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << " name: " << expr.value() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const UnaryExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << std::endl;
    pad(); std::cout << "op: " << expr.opName() std::endl;
    pad(); std::cout << "expr: " << expr.expr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const BinaryExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << std::endl;
    pad(); std::cout << "op: " << expr.opName() std::endl;
    pad(); std::cout << "lhs: " << expr.lhsExpr()->visit(*this);
    pad(); std::cout << "rhs: " << expr.rhsExpr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const CallExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << std::endl;
    pad(); std::cout << " ident: " << expr.ident()->visit(*this);
    for (size_t i = 0; i < expr.args().size(); ++i) {
        pad(); std::cout << "arg" << i << ": "; expr.args()[i]->visit(*this);
    }
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const DerefExpr &stmt)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << std::endl;
    if (expr.lhsIdent()) {
        pad(); std::cout << " lhs: " << expr.lhsIdent()->visit(*this);
    }
    pad(); std::cout << " rhs: " << expr.rhsIdent()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const RootStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << std::endl;
    for (size_t i = 0; i < expr.stmts().size(); ++i) {
        pad(); std::cout << "stmt" << i << ": "; expr.stmts()[i]->visit(*this);
    }
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const BlockStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName(); << std::endl;
    for (size_t i = 0; i < expr.stmts().size(); ++i) {
        pad(); std::cout << "stmt" << i << ": "; expr.stmts()[i]->visit(*this);
    }
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const ModuleStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName(); << std::endl;
    pad(); std::cout << "name: " << stmt.name() << std::endl;
    pad(); std::cout << "block: " << i << ": "; stmt.block()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const DeclStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName(); << std::endl;
    pad(); std::cout << "name: " << stmt.name() << std::endl;
    pad(); std::cout << "expr: " << i << ": "; stmt.expr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const IfStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName(); << std::endl;
    pad(); std::cout << "cond: " << i << ": "; stmt.condExpr()->visit(*this);
    pad(); std::cout << "then: " << i << ": "; stmt.thenExpr()->visit(*this);
    for (size_t i = 0; i < stmt.elifExprs().size(); ++i) {
        pad(); std::cout << "elif" << i << ": "; stmt.elifExprs()[i]->visit(*this);
    }
    if (stmt.elseExpr()) {
        pad(); std::cout << "else: " << i << ": "; stmt.elseExpr()->visit(*this);
    }
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const ForStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const ExprStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName(); << std::endl;
    pad(); std::cout << "expr: " << i << ": "; stmt.expr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}
