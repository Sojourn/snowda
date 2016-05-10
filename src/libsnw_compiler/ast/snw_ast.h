#ifndef SNW_AST_H
#define SNW_AST_H

namespace Snowda {
    namespace Ast {
        class Visitor;

        enum class UnaryOperator {
            Plus  = '+',
            Minus = '-',
            Tilde = '~',
            Bang  = '!',
        };

        enum class BinaryOperator {
            Add = '+',
            Sub = '-',
            Mul = '*',
            Div = '/',
            Mod = '%',

            Or = '|',
            XOr = '^',
            And = '&',

            LShift = 0,  // FIXME
            RShift = 1, // FIXME
        };

        class Expression {
        public:
            virtual ~Expression() {}

            virtual void accept(Visitor &visitor) const = 0;
        };
        using ExpressionPtr = std::unique_ptr<Expression>;

        class IdentifierExpression : public Expression {
        public:
            IdentifierExpression(StringView name);

            StringView name() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const StringView name_;
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
