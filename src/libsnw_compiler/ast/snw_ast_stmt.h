#ifndef SNW_AST_STMT_H
#define SNW_AST_STMT_H

namespace Snowda {
    namespace Ast {
        class Stmt : public Node {
        public:
            Stmt(NodeType nodeType, NodeContent nodeContent);
        };
        using StmtVec = std::vector<const Stmt *>;

        class RootStmt : public Stmt {
        public:
            RootStmt(NodeContent nodeContent, StmtVec stmts);

            const StmtVec &stmts() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const StmtVec stmts_;
        };

        class BlockStmt : public Stmt {
        public:
            BlockStmt(NodeContent nodeContent, StmtVec stmts);

            const StmtVec &stmts() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const StmtVec stmts_;
        };

        class ModuleStmt : public Stmt {
        public:
            ModuleStmt(NodeContent nodeContent, StringView name, ExprVec args, BlockStmtPtr block);

            const StringView &name() const;
            const ExprVec &args() const;
            const BlockStmtPtr &block() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const StringView name_;
            const ExprVec args_;
            const BlockStmtPtr block_;
        };

        class DeclStmt : public Stmt {
        public:
            DeclStmt(NodeContent nodeContent, StringView name, ExprPtr expr);

            const StringView &name() const;
            const ExprPtr &expr() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const StringView name_;
            const ExprPtr expr_;
        };

        class IfStmt : public Stmt {
        public:
            struct Elif {
                const Expr *cond;
                const Expr *then;
            };

            using ElifVec = std::vector<Elif>;

            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr);
            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ElifVec elifs);
            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ExprPtr elseExpr);
            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ElifVec elifs, ExprPtr elseExpr);

            const ExprPtr &condExpr() const;
            const ExprPtr &thenExpr() const;
            const ElifVec &elifExprs() const;
            const ExprPtr &elseExpr() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const ExprPtr cond_;
            const ExprPtr then_;
            const ElifVec elifs_;
            const ExprPtr else_;
        };

        class ForStmt : public Stmt {
        public:
            ForStmt(NodeContent nodeContent);

            virtual void visit(NodeVisitor &visitor) const override;
        };

        class ExprStmt : public Stmt {
        public:
            ExprStmt(NodeContent nodeContent, ExprPtr expr);

            const ExprPtr &expr() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const ExprPtr expr_;
        };
    }
}

#endif // SNW_AST_STMT_H
