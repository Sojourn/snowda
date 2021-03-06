#ifndef SNW_AST_STMT_H
#define SNW_AST_STMT_H

namespace Snowda {
    namespace Ast {
        class Stmt : public Node {
        public:
            Stmt(NodeType nodeType, NodeContent nodeContent);
        };

        class RootStmt : public Stmt {
        public:
            RootStmt(NodeContent nodeContent, StmtVec stmts);

            const StmtVec &stmts() const;

        private:
            const StmtVec stmts_;
        };

        class BlockStmt : public Stmt {
        public:
            BlockStmt(NodeContent nodeContent, StmtVec stmts);

            const StmtVec &stmts() const;

        private:
            const StmtVec stmts_;
        };

        class ModuleStmt : public Stmt {
        public:
            ModuleStmt(NodeContent nodeContent, StringView name, ExprVec args, const BlockStmt *block);

            const StringView &name() const;
            const ExprVec &args() const;
            const BlockStmt *block() const;

        private:
            const StringView name_;
            const ExprVec args_;
            const BlockStmt *block_;
        };

        class FunctionArgStmt : public Stmt {
        public:
            FunctionArgStmt(NodeContent nodeContent, StringView name, StringView type);

            const StringView &name() const;
            const StringView &type() const;

        private:
            const StringView name_;
            const StringView type_;
        };

        class FunctionDeclStmt : public Stmt {
        public:
            FunctionDeclStmt(NodeContent nodeContent, StringView name, FunctionArgStmtVec args, const BlockStmt *block);

            const StringView &name() const;
            const FunctionArgStmtVec &args() const;
            const BlockStmt *block() const;

        private:
            const StringView name_;
            const FunctionArgStmtVec args_;
            const BlockStmt *block_;
        };

        class IfStmt : public Stmt {
        public:
            struct Elif {
                const Expr *cond;
                const Stmt *then;
            };

            using ElifVec = std::vector<Elif>;

            IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt);
            IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt, ElifVec elifs);
            IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt, const Stmt *elseStmt);
            IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt, ElifVec elifs, const Stmt *elseStmt);

            const Expr *condExpr() const;
            const Stmt *thenStmt() const;
            const ElifVec &elifs() const;
            const Stmt *elseStmt() const;

        private:
            const Expr *cond_;
            const Stmt *then_;
            const ElifVec elifs_;
            const Stmt *else_;
        };

        class ReturnStmt : public Stmt {
        public:
            ReturnStmt(NodeContent nodeContent, const ExprStmt *exprStmt);

            const ExprStmt *exprStmt() const;

        private:
            const ExprStmt *exprStmt_;
        };

        class ForStmt : public Stmt {
        public:
            ForStmt(NodeContent nodeContent);
        };

        class ExprStmt : public Stmt {
        public:
            ExprStmt(NodeContent nodeContent, const Expr *expr);

            const Expr *expr() const;

        private:
            const Expr *expr_;
        };
    }
}

#endif // SNW_AST_STMT_H
