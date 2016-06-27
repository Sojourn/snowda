#include "snw.h"

using namespace Snowda;
using namespace Snowda::Ast;

// struct Register {
//     StringView name;
// };

// struct Scope {
//     std::vector<Register> registers;
//     std::vector<Instruction> instructions;
// };

// class FunctionBuilder : private Ast::NodeVisitor {
// public:
//     FunctionBuilder(const Ast::FunctionDeclStmt &fnStmt)
//     {
//     }

// private:
//     virtual void visit(const NumberExpr &node) override final
//     {
//         if (constants_.size() > std::numeric_limits<uint16_t>::max()) {
//             abort();
//         }

//         const uint16_t constantIndex = static_cast<uint16_t>(constants_.size());
//     }

//     virtual void visit(const CharacterExpr &node) override final
//     {
//         abort();
//     }

//     virtual void visit(const StringExpr &node) override final
//     {
//         abort();
//     }

//     virtual void visit(const IdentifierExpr &node) override final
//     {
//     }

//     virtual void visit(const BinaryExpr &node) override final
//     {
//     }

//     virtual void visit(const UnaryExpr &node) override final
//     {
//     }

//     virtual void visit(const CallExpr &node) override final
//     {
//     }

//     virtual void visit(const DerefExpr &node) override final
//     {
//     }

//     virtual void visit(const RootStmt &node) override final
//     {
//         abort();
//     }

//     virtual void visit(const ModuleStmt &node) override final
//     {
//         abort();
//     }

//     virtual void visit(const BlockStmt &node) override final
//     {
//         scopes_.emplace_back();
//     }

//     virtual void visit(const DeclStmt &node) override final
//     {
//     }

//     virtual void visit(const FunctionArgStmt &node) override final
//     {
//         abort();
//     }

//     virtual void visit(const FunctionDeclStmt &node) override final
//     {
//         abort();
//     }

//     virtual void visit(const IfStmt &node) override final
//     {
//     }

//     virtual void visit(const ReturnStmt &node) override final
//     {
//     }

//     virtual void visit(const ForStmt &node) override final
//     {
//     }

//     virtual void visit(const ExprStmt &node) override final
//     {
//         node.expr()->visit(*this);
//     }

// private:
//     const Ast::FunctionDeclStmt &fnStmt_;
//     std::vector<uint16_t> constants_;
// };

int main(int argc, char **argv)
{
    MemoryManager manager;
    Program program(manager);

    VirtualMachine virtualMachine(program);
    virtualMachine.run(View<const char *>(argv, argv + argc));

#ifdef SNW_OS_WIN32
    std::system("pause");
#endif
    return 0;
}
