#ifndef SNW_AST_VISITOR_H
#define SNW_AST_VISITOR_H

namespace Snowda {
    namespace Ast {
        class CompoundStatement;
        class LiteralExpression;
        class UnaryExpression;
        class BinaryExpression;
        class SymbolExpression;
        class ConditionalExpression;

        class Visitor {
        public:
            virtual ~Visitor() {}

            virtual void visit(const CompoundStatement &node) const {}
            virtual void visit(const UnaryExpression &node) const {}
            virtual void visit(const BinaryExpression &node) const {}
            virtual void visit(const SymbolExpression &node) const {}
            virtual void visit(const ConditionalExpression &node) const {}
            virtual void visit(const LiteralExpression &node) const {}
        };
    }
}

#endif // SNW_VISITOR_H
