#ifndef SNW_AST_NODE_H
#define SNW_AST_NODE_H

namespace Snowda {
    namespace Ast {
        class NodeVisitor;
        class Node;
        class Expr;
        class Stmt;
        class FunctionArgStmt;

        using NodeVec = std::vector<const Node *>;
        using ExprVec = std::vector<const Expr *>;
        using StmtVec = std::vector<const Stmt *>;
        using FunctionArgStmtVec = std::vector<const FunctionArgStmt *>;

#define X(xType) class xType;
        SNW_AST_NODE_TYPES
#undef X

        enum class NodeType {
            ExprBegin,
#define X(xType) xType,
        SNW_AST_EXPR_TYPES
#undef X
            ExprEnd,

            StmtBegin,
#define X(xType) xType,
        SNW_AST_STMT_TYPES
#undef X
            StmtEnd,
        };

        struct NodeContent {
            size_t row;
            size_t col;
            size_t len;
        };

        class Node {
        public:
            Node(NodeType type, NodeContent content);
            virtual ~Node() {}

            NodeType nodeType() const;
            StringView nodeTypeName() const;
            NodeContent nodeContent() const;

            bool isStmt() const;
            bool isExpr() const;

        private:
            const NodeType nodeType_;
            const NodeContent nodeContent_;
        };

        template<typename Result, typename Visitor>
        Result dispatch(Visitor &visitor, const Node &node) {
            switch (node.nodeType()) {
#define X(xType) case NodeType::xType: return visitor.visit(static_cast<const xType &>(node));
                SNW_AST_NODE_TYPES
#undef X
                default:
                    abort();
            }
        }
    }
}

#endif // SNW_ASt_NODE_H
