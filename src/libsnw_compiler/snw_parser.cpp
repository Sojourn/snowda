#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ParserError::ParserError(Parser &parser, StringView message)
    : message(message)
    , row(parser.row())
    , col(parser.col())
{
}
