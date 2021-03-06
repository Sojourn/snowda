#ifndef SNW_AST_EXPR_H
#define SNW_AST_EXPR_H

namespace Snowda {
    namespace Ast {
        class Expr : public Node {
        public:
            Expr(NodeType type, NodeContent nodeContent);
        };

        class NumberExpr : public Expr {
        public:
            NumberExpr(NodeContent nodeContent, int value);

            int value() const;

        private:
            const int value_;
        };

        class CharacterExpr : public Expr {
        public:
            CharacterExpr(NodeContent nodeContent, char value);

            char value() const;

        private:
            const char value_;
        };

        class StringExpr : public Expr {
        public:
            StringExpr(NodeContent nodeContent, StringView value);

            const StringView &value() const;

        private:
            const StringView value_;
        };

        class IdentifierExpr : public Expr {
        public:
            IdentifierExpr(NodeContent nodeContent, StringView name);

            const StringView &name() const;

        private:
            const StringView name_;
        };

        class UnaryExpr : public Expr {
        public:
            enum class Operator {
#define X(xType, xName) xType,
                SNW_AST_UNARY_OPERATORS
#undef X
            };

            UnaryExpr(NodeContent nodeContent, Operator op, const Expr *expr);

            Operator op() const;
            StringView opName() const;
            const Expr *expr() const;

        private:
            const Operator op_;
            const Expr *expr_;
        };

        class BinaryExpr : public Expr {
        public:
            enum class Operator {
#define X(xType, xName) xType,
                SNW_AST_BINARY_OPERATORS
#undef X
            };

            BinaryExpr(NodeContent nodeContent, Operator op, const Expr *lhsExpr, const Expr *rhsExpr);

            Operator op() const;
            StringView opName() const;
            const Expr *lhsExpr() const;
            const Expr *rhsExpr() const;

        private:
            const Operator op_;
            const Expr *lhsExpr_;
            const Expr *rhsExpr_;
        };

        class CallExpr : public Expr {
        public:
            CallExpr(NodeContent nodeContent, const IdentifierExpr *ident, ExprVec args);

            const IdentifierExpr *ident() const;
            const ExprVec &args() const;

        private:
            const IdentifierExpr *ident_;
            const ExprVec args_;
        };

        class DerefExpr : public Expr {
        public:
            DerefExpr(NodeContent nodeContent, const IdentifierExpr *rhsIdent);
            DerefExpr(NodeContent nodeContent, const IdentifierExpr *lhsIdent, const IdentifierExpr *rhsIdent);

            const IdentifierExpr *rhsIdent() const;
            const IdentifierExpr *lhsIdent() const;

        private:
            const IdentifierExpr *lhsIdent_;
            const IdentifierExpr *rhsIdent_;
        };

        // FIXME: This should be a statement. Need to extend parser to support infix statements
        class AssignExpr : public Expr {
        public:
            AssignExpr(NodeContent nodeContent, const Expr *lhsExpr, const Expr *rhsExpr);

            const Expr *lhsExpr() const;
            const Expr *rhsExpr() const;

        private:
            const Expr *lhsExpr_;
            const Expr *rhsExpr_;
        };
    }
}

#endif // SNW_AST_EXPR_H
