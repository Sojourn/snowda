#ifndef SNW_AST_EXPR_H
#define SNW_AST_EXPR_H

namespace Snowda {
    namespace Ast {
        class Expr : public Node {
        public:
            Expr(NodeType type, NodeContent nodeContent);
        };
        using ExprVec = std::vector<const ExprPtr *>;

        class NumberExpr : public Expr {
        public:
            NumberExpr(NodeContent nodeContent, int value);

            int value() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const int value_;
        };

        class CharacterExpr : public Expr {
        public:
            CharacterExpr(NodeContent nodeContent, char value);

            char value() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const char value_;
        };

        class StringExpr : public Expr {
        public:
            StringExpr(NodeContent nodeContent, StringView value);

            const StringView &value() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const StringView value_;
        };

        class IdentifierExpr : public Expr {
        public:
            IdentifierExpr(NodeContent nodeContent, StringView name);

            const StringView &name() const;

            virtual void visit(NodeVisitor &visitor) const override;

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

            UnaryExpr(NodeContent nodeContent, Operator op, ExprPtr expr);

            Operator op() const;
            StringView opName() const;
            const ExprPtr &expr() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const Operator op_;
            const ExprPtr expr_;
        };

        class BinaryExpr : public Expr {
        public:
            enum class Operator {
#define X(xType, xName) xType,
                SNW_AST_BINARY_OPERATORS
#undef X
            };

            BinaryExpr(NodeContent nodeContent, Operator op, ExprPtr lhsExpr, ExprPtr rhsExpr);

            Operator op() const;
            StringView opName() const;
            const ExprPtr &lhsExpr() const;
            const ExprPtr &rhsExpr() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const Operator op_;
            const ExprPtr lhsExpr_;
            const ExprPtr rhsExpr_;
        };

        class CallExpr : public Expr {
        public:
            CallExpr(NodeContent nodeContent, IdentifierExprPtr ident, ExprVec args);

            const IdentifierExprPtr &ident() const;
            const ExprVec &args() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const IdentifierExprPtr ident_;
            const ExprVec args_;
        };

        class DerefExpr : public Expr {
        public:
            DerefExpr(NodeContent nodeContent, IdentifierExprPtr rhsIdent);
            DerefExpr(NodeContent nodeContent, IdentifierExprPtr lhsIdent, IdentifierExprPtr rhsIdent);

            const IdentifierExprPtr &rhsIdent() const;
            const IdentifierExprPtr &lhsIdent() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const IdentifierExprPtr lhsIdent_;
            const IdentifierExprPtr rhsIdent_;
        };
    }
}

#endif // SNW_AST_EXPR_H
