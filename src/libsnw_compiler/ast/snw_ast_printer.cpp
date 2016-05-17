#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Printer::Printer()
    : depth_(0)
{
}

void Printer::visit(const NumberExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName() << " value: " << expr.value() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const CharacterExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName() << " value: " << expr.value() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const StringExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName() << " value: " << expr.value() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const IdentifierExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName() << " name: " << expr.name() << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const UnaryExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName() << std::endl;
    pad(); std::cout << "op: " << expr.opName() << std::endl;
	pad(); std::cout << "expr: "; expr.expr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const BinaryExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName() << std::endl;
    pad(); std::cout << "op: " << expr.opName() << std::endl;
	pad(); std::cout << "lhs: "; expr.lhsExpr()->visit(*this);
	pad(); std::cout << "rhs: "; expr.rhsExpr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const CallExpr &expr)
{
    depth_ += 1;
    std::cout << '(' << expr.nodeTypeName() << std::endl;
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
    std::cout << '(' << stmt.nodeTypeName() << std::endl;
    if (stmt.lhsIdent()) {
		pad(); std::cout << " lhs: "; stmt.lhsIdent()->visit(*this);
    }
	pad(); std::cout << " rhs: "; stmt.rhsIdent()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const RootStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName() << std::endl;
    for (size_t i = 0; i < stmt.stmts().size(); ++i) {
        pad(); std::cout << "stmt" << i << ": "; stmt.stmts()[i]->visit(*this);
    }
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const BlockStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName() << std::endl;
    for (size_t i = 0; i < stmt.stmts().size(); ++i) {
        pad(); std::cout << "stmt" << i << ": "; stmt.stmts()[i]->visit(*this);
    }
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const ModuleStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName() << std::endl;
    pad(); std::cout << "name: " << stmt.name() << std::endl;
    pad(); std::cout << "block: "; stmt.block()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const DeclStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName() << std::endl;
    pad(); std::cout << "name: " << stmt.name() << std::endl;
    pad(); std::cout << "expr: "; stmt.expr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::visit(const IfStmt &stmt)
{
    depth_ += 1;
    std::cout << '(' << stmt.nodeTypeName() << std::endl;
    pad(); std::cout << "cond: "; stmt.condExpr()->visit(*this);
    pad(); std::cout << "then: "; stmt.thenExpr()->visit(*this);
    for (size_t i = 0; i < stmt.elifExprs().size(); ++i) {
        auto &elif = stmt.elifExprs()[i];
        pad(); std::cout << "elif-cond" << i << ": "; elif.cond->visit(*this);
        pad(); std::cout << "elif-then" << i << ": "; elif.then->visit(*this);
    }
    if (stmt.elseExpr()) {
        pad(); std::cout << "else: "; stmt.elseExpr()->visit(*this);
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
    std::cout << '(' << stmt.nodeTypeName() << std::endl;
    pad(); std::cout << "expr: "; stmt.expr()->visit(*this);
    pad(); std::cout << ')' << std::endl;
    depth_ -= 1;
}

void Printer::pad() const
{
    if (depth_ > 0) {
        for (size_t i = 0; i < (depth_ - 1); ++i) {
            std::cout << "  ";
        }
    }
}
