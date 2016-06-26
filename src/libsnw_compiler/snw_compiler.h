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

#include "snw_parser_error.h"
#include "ast/snw_ast.h"
#include "grammar/snw_grammar.h"
#include "snw_parser.h"
#include "snw_program.h"

#endif // SNW_COMPILER_H
