set(SNW_LIBS
    snw_util
    snw_code
    snw_memory
)

set(SNW_HDRS
    snw_compiler.h
    snw_token.h
    snw_lexer.h
    snw_token_stream.h
    snw_parser_error.h
    grammar/snw_grammar.h
    grammar/snw_default_rules.h
    grammar/snw_error_rules.h
    grammar/snw_identifier_rules.h
    grammar/snw_number_rules.h
    grammar/snw_character_rules.h
    grammar/snw_fn_rules.h
    grammar/snw_return_rules.h
    grammar/snw_operator_rules.h
    snw_parser.h
    snw_program.h
    snw_code_generator.h
    ast/snw_ast.h
    ast/snw_ast_node.h
    ast/snw_ast_expr.h
    ast/snw_ast_stmt.h
)

set(SNW_SRCS
    snw_token.cpp
    snw_lexer.cpp
    snw_token_stream.cpp
    snw_parser_error.cpp
    grammar/snw_grammar.cpp
    grammar/snw_default_rules.cpp
    grammar/snw_error_rules.cpp
    grammar/snw_identifier_rules.cpp
    grammar/snw_number_rules.cpp
    grammar/snw_character_rules.cpp
    grammar/snw_fn_rules.cpp
    grammar/snw_return_rules.cpp
    snw_parser.cpp
    snw_program.cpp
    snw_code_generator.cpp
    ast/snw_ast_node.cpp
    ast/snw_ast_expr.cpp
    ast/snw_ast_stmt.cpp
)

add_library(snw_compiler ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_compiler ${SNW_LIBS})
target_include_directories(snw_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
