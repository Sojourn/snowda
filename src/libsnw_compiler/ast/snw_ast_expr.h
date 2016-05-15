#ifndef SNW_AST_EXPR_H
#define SNW_AST_EXPR_H

namespace Snowda {
    namespace Ast {
        class Expr : public Node {
        public:
            Expr(NodeContent nodeContent, NodeType type);
        };
    }
}

#endif // SNW_AST_EXPR_H
