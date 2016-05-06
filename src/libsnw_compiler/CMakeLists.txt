set(SNW_LIBS
    snw_util
)

set(SNW_HDRS
    snw_compiler.h
    snw_token.h
    snw_lexer.h
    snw_token_stream.h
    snw_parser.h
    ast/snw_ast.h
    ast/snw_ast_visitor.h
)

set(SNW_SRCS
    snw_token.cpp
    snw_lexer.cpp
    snw_token_stream.cpp
    snw_parser.cpp
    ast/snw_ast.cpp
)

add_library(snw_compiler ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_compiler ${SNW_LIBS})
target_include_directories(snw_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
