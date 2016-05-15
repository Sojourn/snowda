#ifndef SNW_AST_NODE_H
#define SNW_AST_NODE_H

namespace Snowda {
    namespace Ast {
        class NodeVisitor;
        class Node;
        class Expr;
        class NumberExpr;
        class CharacterExpr;
        class StringExpr;
        class IdentifierExpr;
        class BinaryExpr;
        class UnaryExpr;
        class CallExpr;
        class AnchorExpr;
        class DerefExpr;
        class Stmt;
        class RootStmt;
        class ModuleStmt;
        class BlockStmt;
        class DeclStmt;
        class IfStmt;
        class ForStmt;
        class ExprStmt;

        class NodeVisitor {
        public:
            virtual ~NodeVisitor() {}
            virtual void visit(const NumberExpr &expr) {}
            virtual void visit(const CharacterExpr &expr) {}
            virtual void visit(const StringExpr &expr) {}
            virtual void visit(const IdentifierExpr &expr) {}
            virtual void visit(const UnaryExpr &expr) {}
            virtual void visit(const BinaryExpr &expr) {}
            virtual void visit(const CallExpr &expr) {}
            virtual void visit(const AnchorExpr &expr) {}
            virtual void visit(const DerefExpr &stmt) {}
            virtual void visit(const RootStmt &stmt) {}
            virtual void visit(const BlockStmt &stmt) {}
            virtual void visit(const ModuleStmt &stmt) {}
            virtual void visit(const DeclStmt &stmt) {}
            virtual void visit(const IfStmt &stmt) {}
            virtual void visit(const ForStmt &stmt) {}
            virtual void visit(const ExprStmt &stmt) {}
        };

        enum class NodeType {
            ExprBegin,
            Expr,
            NumberExpr,
            CharacterExpr,
            StringExpr,
            IdentifierExpr,
            UnaryExpr,
            BinaryExpr,
            CallExpr,
            AnchorExpr,
            DerefExpr,
            ExprEnd,

            StmtBegin,
            Stmt,
            RootStmt,
            BlockStmt,
            ModuleStmt,
            DeclStmt,
            IfStmt,
            ForStmt,
            ExprStmt,
            StmtEnd,
        };

        // FIXME: Start row/col, end row/col?
        struct NodeContent {
            StringView str;
            size_t row;
            size_t col;
        };

        class Node {
        public:
            Node(NodeType type, NodeContent content);
            virtual ~Node() {}

            NodeType nodeType() const;
            NodeContent nodeContent() const;

            bool isStmt() const;
            bool isExpr() const;

            virtual void visit(NodeVisitor &visitor) const = 0;

        private:
            const NodeType nodeType_;
            const NodeContent nodeContent_;
        };
        using NodePtr = std::unique_ptr<Node>;

    }
}

#endif // SNW_ASt_NODE_H
