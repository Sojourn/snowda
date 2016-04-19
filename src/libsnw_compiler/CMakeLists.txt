set(SNW_LIBS
    snw_util
)

set(SNW_HDRS
    lexer.h
    parser.h
)

set(SNW_SRCS
    lexer.cpp
    parser.cpp
)

add_library(snw_compiler ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_compiler ${SNW_LIBS})
target_include_directories(snw_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
