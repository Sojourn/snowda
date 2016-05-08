#ifndef SNW_AST_VISITOR_H
#define SNW_AST_VISITOR_H

namespace Snowda {
    namespace Ast {
        class LiteralExpression;
        class UnaryExpression;
        class BinaryExpression;
        class IdentifierExpression;
        class ConditionalExpression;

        class Visitor {
        public:
            virtual ~Visitor() {}

            virtual void visit(const LiteralExpression &node) {}
            virtual void visit(const UnaryExpression &node) {}
            virtual void visit(const BinaryExpression &node) {}
            virtual void visit(const IdentifierExpression &node) {}
            virtual void visit(const ConditionalExpression &node) {}
        };
    }
}

#endif // SNW_VISITOR_H
