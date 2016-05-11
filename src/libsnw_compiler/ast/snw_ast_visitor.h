#ifndef SNW_AST_VISITOR_H
#define SNW_AST_VISITOR_H

namespace Snowda {
    namespace Ast {
        class NumberExpression;
        class CharacterExpression;
        class StringExpression;
        class IdentifierExpression;
        class DerefExpression;
        class UnaryExpression;
        class BinaryExpression;
        class ConditionalExpression;

        class Visitor {
        public:
            virtual ~Visitor() {}

            virtual void visit(const NumberExpression &expr) {}
            virtual void visit(const CharacterExpression &expr) {}
            virtual void visit(const StringExpression &expr) {}
            virtual void visit(const IdentifierExpression &expr) {}
            virtual void visit(const CallExpression &expr) {}
            virtual void visit(const DerefExpression &expr) {}
            virtual void visit(const UnaryExpression &expr) {}
            virtual void visit(const BinaryExpression &expr) {}
            virtual void visit(const ConditionalExpression &expr) {}
        };
    }
}

#endif // SNW_VISITOR_H
