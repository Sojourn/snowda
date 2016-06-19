#ifndef SNW_CODE_GENERATOR_H
#define SNW_CODE_GENERATOR_H

namespace Snowda {

    class CodeGenerator : private Ast::NodeVisitor {
    public:
        CodeGenerator(ArenaAllocator &arena, Program &program, const Ast::RootStmt &stmt);

    private:
        virtual void visit(const Ast::NumberExpr &expr) override;
        virtual void visit(const Ast::CharacterExpr &expr) override;
        virtual void visit(const Ast::StringExpr &expr) override;
        virtual void visit(const Ast::IdentifierExpr &expr) override;
        virtual void visit(const Ast::UnaryExpr &expr) override;
        virtual void visit(const Ast::BinaryExpr &expr) override;
        virtual void visit(const Ast::CallExpr &expr) override;
        virtual void visit(const Ast::DerefExpr &stmt) override;
        virtual void visit(const Ast::RootStmt &stmt) override;
        virtual void visit(const Ast::BlockStmt &stmt) override;
        virtual void visit(const Ast::ModuleStmt &stmt) override;
        virtual void visit(const Ast::DeclStmt &stmt) override;
        virtual void visit(const Ast::FunctionArgStmt &stmt) override;
        virtual void visit(const Ast::FunctionDeclStmt &stmt) override;
        virtual void visit(const Ast::IfStmt &stmt) override;
        virtual void visit(const Ast::ForStmt &stmt) override;
        virtual void visit(const Ast::ExprStmt &stmt) override;

    private:
        ArenaAllocator &arena_;
        Program &program_;
    };

}

#endif // SNW_CODE_GENERATOR_H
