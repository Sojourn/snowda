#ifndef SNW_AST_H
#define SNW_AST_H

namespace Snowda {
    namespace Ast {
        class Visitor;

        class Node {
        public:
            virtual ~Node() {}

            virtual void accept(Visitor &visitor) const = 0;
        };
        using NodePtr = std::unique_ptr<Node>;

        class Statement : public Node {
        };
        using StatementPtr = std::unique_ptr<Statement>;

        class CompoundStatement : public Node {
        public:
            CompoundStatement(std::vector<StatementPtr> stmts);

            const std::vector<StatementPtr> &stmts() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const std::vector<StatementPtr> stmts_;
        };
        using CompoundStatementPtr = std::unique_ptr<CompoundStatement>;

        class Expression : public Statement {
        };
        using ExpressionPtr = std::unique_ptr<Expression>;

        class SymbolExpression : public Expression {
        public:
            SymbolExpression(StringView name);

            StringView name() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const StringView name_;
        };

        class ConditionalExpression : public Expression {
        public:
            ConditionalExpression(StatementPtr cond, StatementPtr taken);
            ConditionalExpression(StatementPtr cond, StatementPtr taken, StatementPtr notTaken);

            virtual void accept(Visitor &visitor) const;

        private:
            const StatementPtr cond_;
            const StatementPtr taken_;
            const StatementPtr notTaken_;
        };
        using ConditionalExpressionPtr = std::unique_ptr<ConditionalExpression>;

        enum class UnaryOperator {
            Plus  = static_cast<size_t>(TokenType::Plus),
            Minus = static_cast<size_t>(TokenType::Minus),
            Tilde = static_cast<size_t>(TokenType::Tilde),
            Bang  = static_cast<size_t>(TokenType::Bang),
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

        enum class BinaryOperator {
            Add,
            Sub,
            Mul,
            Div,
            Mod,

            Or,
            XOr,
            And,

            LeftShift,
            RightShift,
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
        using BinaryExpressionPtr = std::unique_ptr<BinaryExpression>;

        class LiteralExpression : public Expression {
        public:
            LiteralExpression(int value);

            int value() const;

            virtual void accept(Visitor &visitor) const;

        private:
            const int value_;
        };
        using LiteralExpressionPtr = std::unique_ptr<LiteralExpression>;

    }
}

#endif // SNW_AST_H
