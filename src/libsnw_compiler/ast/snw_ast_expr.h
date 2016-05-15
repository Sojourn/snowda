#ifndef SNW_AST_EXPR_H
#define SNW_AST_EXPR_H

namespace Snowda {
    namespace Ast {
        class Expr : public Node {
        public:
            Expr(NodeContent nodeContent, NodeType type);
        };
        using ExprPtr = std::unique_ptr<Expr>;
        using ExprVec = std::vector<ExprPtr>;

        class NumberExpr : public Node {
        public:
            NumberExpr(NodeContent nodeContent, int value);

            int value() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const int value_;
        };
        using NumberExprPtr = std::unique_ptr<NumberExpr>;

        class CharacterExpr : public Expr {
        public:
            CharacterExpr(NodeContent nodeContent, char value);

            char value() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const char value_;
        };
        using CharacterExprPtr = std::unique_ptr<CharacterExpr>;

        class StringExpr : public Expr {
        public:
            StringExpr(NodeContent nodeContent, StringView value);

            const StringView &value() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const StringView value_;
        };
        using StringExprPtr = std::unique_ptr<StringExpr>;

        class IdentifierExpr : public Expr {
        public:
            IdentifierExpr(NodeContent nodeContent, StringView name);

            const StringView &name() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const StringView name_;
        };
        using IdentifierExprPtr = std::unique_ptr<IdentifierExpr>;

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
        using UnaryExprPtr = std::unique_ptr<UnaryExpr>;

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

        private:
            const Operator op_;
            const ExprPtr lhsExpr_;
            const ExprPtr rhsExpr_;
        };
        using BinaryExprPtr = std::unique_ptr<BinaryExpr>;

        class CallExpr : public Expr {
        public:
            // TODO
        };
    }
}

#endif // SNW_AST_EXPR_H
