#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

CodeGenerator::CodeGenerator(ArenaAllocator &arena, Program &program, const RootStmt &stmt)
    : arena_(arena)
    , program_(program)
{
	stmt.visit(*this);
}

void CodeGenerator::visit(const NumberExpr &expr)
{
}

void CodeGenerator::visit(const CharacterExpr &expr)
{
}

void CodeGenerator::visit(const StringExpr &expr)
{
}

void CodeGenerator::visit(const IdentifierExpr &expr)
{
}

void CodeGenerator::visit(const UnaryExpr &expr)
{
}

void CodeGenerator::visit(const BinaryExpr &expr)
{
}

void CodeGenerator::visit(const CallExpr &expr)
{
}

void CodeGenerator::visit(const DerefExpr &stmt)
{
}

void CodeGenerator::visit(const RootStmt &stmt)
{
}

void CodeGenerator::visit(const BlockStmt &stmt)
{
}

void CodeGenerator::visit(const ModuleStmt &stmt)
{
}

void CodeGenerator::visit(const DeclStmt &stmt)
{
}

void CodeGenerator::visit(const Ast::FunctionArgStmt &stmt)
{
}

void CodeGenerator::visit(const FunctionDeclStmt &stmt)
{
}

void CodeGenerator::visit(const IfStmt &stmt)
{
}

void CodeGenerator::visit(const ForStmt &stmt)
{
}

void CodeGenerator::visit(const ExprStmt &stmt)
{
}
