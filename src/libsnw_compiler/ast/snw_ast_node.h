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
            virtual void visit(const BinaryExpr &expr) {}
            virtual void visit(const UnaryExpr &expr) {}
            virtual void visit(const CallExpr &expr) {}
            virtual void visit(const AnchorExpr &expr) {}
            virtual void visit(const DerefExpr &stmt) {}
            virtual void visit(const RootStmt &stmt) {}
            virtual void visit(const ModuleStmt &stmt) {}
            virtual void visit(const BlockStmt &stmt) {}
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
            BinaryExpr,
            UnaryExpr,
            CallExpr,
            AnchorExpr,
            DerefExpr,
            ExprEnd,

            StmtBegin,
            Stmt,
            BlockStmt,
            RootStmt,
            DeclStmt,
            IfStmt,
            ForStmt,
            ExprStmt,
            StmtEnd,
        };

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

        struct Type {};
        using TypePtr = std::unique_ptr<Type>;

        class Expr : public Node {
        public:
            Expr(NodeType type, NodeContent content);

            virtual const TypePtr &type() const = 0;
        };
        using ExprPtr = std::unique_ptr<Expr>;

        class NumberExpr : public Expr {
        public:
            NumberExpr(NodeContent content, int value);

            int value() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const int value_;
        };

        class Stmt : public Node {
        public:
            Stmt(NodeType type, NodeContent content);
        };

        class IfStmt : public Stmt {
        public:
            using ElifVec = std::vector<std::tuple<ExprPtr, ExprPtr>>;

            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr);
            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ElifVec elifs);
            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ExprPtr elseExpr);
            IfStmt(NodeContent nodeContent, ExprPtr condExpr, ExprPtr thenExpr, ElifVec elifs, ExprPtr elseExpr);

            bool hasElseExpr() const;
            bool hasElifExprs() const;

            const ExprPtr &condExpr() const;
            const ExprPtr &thenExpr() const;
            const ElifVec &elifExprs() const;
            const ExprPtr &elseExpr() const;

            virtual void visit(NodeVisitor &visitor) const override;

        private:
            const ExprPtr cond_;
            const ExprPtr then_;
            const ElifVec elifs_;
            const ExprPtr else_;
        };
    }
}

#endif // SNW_ASt_NODE_H
