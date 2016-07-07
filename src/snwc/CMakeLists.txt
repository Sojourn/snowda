set(SNW_LIBS
)

set(SNW_HDRS
    snwc.h
    snwc_lexer.h
    snwc_ast.h
    snwc_grammar.h
    snwc_parser.h
    snwc_ast_dispatcher.h
    snwc_ast_translator.h
)

set(SNW_SRCS
    snwc.cpp
    snwc_lexer.cpp
    snwc_grammar.cpp
    snwc_parser.cpp
    snwc_ast_translator.cpp
)

add_executable(snwc ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snwc ${SNW_LIBS})
