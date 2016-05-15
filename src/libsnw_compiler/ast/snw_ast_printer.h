#ifndef SNW_AST_PRINTER_H
#define SNW_AST_PRINTER_H

namespace Snowda {
    namespace Ast {
        class Printer : public NodeVisitor {
        public:
            Printer();

            virtual void visit(const NumberExpr &expr) override;
            virtual void visit(const CharacterExpr &expr) override;
            virtual void visit(const StringExpr &expr) override;
            virtual void visit(const IdentifierExpr &expr) override;
            virtual void visit(const UnaryExpr &expr) override;
            virtual void visit(const BinaryExpr &expr) override;
            virtual void visit(const CallExpr &expr) override;
            virtual void visit(const DerefExpr &stmt) override;
            virtual void visit(const RootStmt &stmt) override;
            virtual void visit(const BlockStmt &stmt) override;
            virtual void visit(const ModuleStmt &stmt) override;
            virtual void visit(const DeclStmt &stmt) override;
            virtual void visit(const IfStmt &stmt) override;
            virtual void visit(const ForStmt &stmt) override;
            virtual void visit(const ExprStmt &stmt) override;

        private:
            size_t depth_;
        };
    }
}

#endif // SNW_AST_PRINTER_H
