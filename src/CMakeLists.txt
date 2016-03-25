set(SNOWDA_HDRS
    snowda.h
    result.h
    fixed_vector.h
    heap.h
    view.h
    lexer.h
    parser.h
)

set(SNOWDA_SRCS
    snowda.cpp
    heap.cpp
    lexer.cpp
    parser.cpp
)

add_executable(snowda ${SNOWDA_SRCS} ${SNOWDA_HDRS})
