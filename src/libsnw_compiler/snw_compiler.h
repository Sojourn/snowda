#ifndef SNW_COMPILER_H
#define SNW_COMPILER_H

#include <map>
#include <array>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>

#include <cstdint>
#include <cstddef>
#include <cassert>

#include "snw_util.h"
#include "snw_code.h"
#include "snw_memory.h"

namespace Snowda {
    class Lexer;
    class Grammar;
    class Parser;
    class Program;
}

#include "snw_token.h"
#include "snw_lexer.h"
#include "snw_token_stream.h"

#include "ast/snw_ast.h"
#include "ast/snw_ast_node.h"
#include "ast/snw_ast_expr.h"
#include "ast/snw_ast_stmt.h"
#include "ast/snw_ast_printer.h"
#include "snw_parser_error.h"

#include "grammar/snw_grammar.h"
#include "grammar/snw_default_rules.h"
#include "grammar/snw_identifier_rules.h"
#include "grammar/snw_number_rules.h"
#include "grammar/snw_character_rules.h"
#include "grammar/snw_fn_rules.h"

#include "snw_parser.h"
#include "snw_program.h"
#include "snw_code_generator.h"

#endif // SNW_COMPILER_H
