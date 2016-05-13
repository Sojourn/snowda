#ifndef SNW_AST_H
#define SNW_AST_H

#define SNW_AST_UNARY_OPERATORS \
    X(Plus,  "+") \
    X(Minus, "-") \
    X(Tilde, "~") \
    X(Bang,  "!")

#define SNW_AST_BINARY_OPERATORS \
    X(Add,    "+") \
    X(Sub,    "-") \
    X(Mul,    "*") \
    X(Div,    "/") \
    X(Mod,    "%") \
    X(Or,     "|") \
    X(XOr,    "^") \
    X(And,    "&") \
    X(LShift, "<<") \
    X(RShift, ">>")

namespace Snowda {
    namespace Ast {
        class Visitor;

        enum class UnaryOperator {
#define X(xType, xName) xType,
            SNW_AST_UNARY_OPERATORS
#undef X
        };
        StringView str(UnaryOperator op);

        enum class BinaryOperator {
#define X(xType, xName) xType,
            SNW_AST_BINARY_OPERATORS
#undef X
        };
        StringView str(BinaryOperator op);

        class Expression {
        public:
            virtual ~Expression() {}

            virtual void accept(Visitor &visitor) const = 0;
        };
        using ExpressionPtr = std::unique_ptr<Expression>;

        class StatementExpression : public Expression {
        public:
            StatementExpression(ExpressionPtr expr);

            const ExpressionPtr &expr() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const ExpressionPtr expr_;
        };

        class BlockExpression : public Expression {
        public:
            BlockExpression(std::vector<ExpressionPtr> exprs);

            const std::vector<ExpressionPtr> &exprs() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const std::vector<ExpressionPtr> exprs_;
        };

        class IdentifierExpression : public Expression {
        public:
            IdentifierExpression(StringView name);

            StringView name() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const StringView name_;
        };

        class DerefExpression : public IdentifierExpression {
        public:
            DerefExpression(ExpressionPtr parent, ExpressionPtr child);

            const ExpressionPtr &parent() const;
            const ExpressionPtr &child() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const ExpressionPtr parent_;
            const ExpressionPtr child_;
        };

        class CallExpression : public Expression {
        public:
            CallExpression(ExpressionPtr ident, std::vector<ExpressionPtr> args);

            const ExpressionPtr &ident() const;
            const std::vector<ExpressionPtr> &args() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const ExpressionPtr ident_;
            const std::vector<ExpressionPtr> args_;
        };

        class ConditionalExpression : public Expression {
        public:
            ConditionalExpression(ExpressionPtr condExpr, ExpressionPtr thenExpr);
            ConditionalExpression(ExpressionPtr condExpr, ExpressionPtr thenExpr, ExpressionPtr elseExpr);

            const ExpressionPtr &condExpr() const;
            const ExpressionPtr &thenExpr() const;
            const ExpressionPtr &elseExpr() const; // FIXME: Optional

            virtual void accept(Visitor &visitor) const;

        private:
            const ExpressionPtr cond_;
            const ExpressionPtr then_;
            const ExpressionPtr else_;
        };

        class UnaryExpression : public Expression {
        public:
            UnaryExpression(UnaryOperator op, ExpressionPtr expr);

            UnaryOperator op() const;
            const ExpressionPtr &expr() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const UnaryOperator op_;
            const ExpressionPtr expr_;
        };

        class BinaryExpression : public Expression {
        public:
            BinaryExpression(BinaryOperator op, ExpressionPtr lhs, ExpressionPtr rhs);

            BinaryOperator op() const;
            const ExpressionPtr &lhs() const;
            const ExpressionPtr &rhs() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const BinaryOperator op_;
            const ExpressionPtr lhs_;
            const ExpressionPtr rhs_;
        };

        // FIXME: Need a Number class to hold the value
        class NumberExpression : public Expression {
        public:
            NumberExpression(int value);

            int value() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const int value_;
        };

        // FIXME: Need a Character (utf8) class to hold the value
        class CharacterExpression : public Expression {
        public:
            CharacterExpression(char value);

            char value() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const char value_;
        };

        class StringExpression : public Expression {
        public:
            StringExpression(StringView value);

            const StringView &value() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const StringView value_;
        };

    }
}

#endif // SNW_AST_H
