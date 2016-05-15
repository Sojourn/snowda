#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Node::Node(NodeType type, NodeContent content)
    : nodeType_(type)
    , nodeContent_(content)
{
}

NodeType Node::nodeType() const
{
    return nodeType_;
}

StringView Node::nodeTypeName() const
{
    switch (nodeType_) {
#define X(xType) case NodeType::xType: return StringView(#xType);
        SNW_AST_NODE_TYPES
#undef X
    default:
        abort();
    }
}

NodeContent Node::nodeContent() const
{
    return nodeContent_;
}

bool Node::isStmt() const
{
    return (NodeType::StmtBegin < nodeType()) &&
           (nodeType() < NodeType::StmtEnd);
}

bool Node::isExpr() const
{
    return (NodeType::ExprBegin < nodeType()) &&
           (nodeType() < NodeType::ExprEnd);
}
