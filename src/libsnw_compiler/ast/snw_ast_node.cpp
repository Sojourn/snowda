#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

/*

Node::Node(NodeType type, NodeContent content)
    : type_(type)
    , content_(content)
{
}

NodeType Node::type() const
{
    return type_;
}

NodeContent content() const
{
    return content_;
}

bool Node::isStmt() const
{
    return (NodeType::StmtBegin < type()) &&
           (type() < NodeType::StmtEnd);
}

bool Node::isExpr() const
{
    return (NodeType::ExprBegin < type()) &&
           (type() < NodeType::ExprEnd);
}

*/