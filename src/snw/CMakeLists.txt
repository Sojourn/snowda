set(SNW_LIBS
    snw_util
    snw_code
    snw_memory
    snw_compiler
)

set(SNW_HDRS
    snw.h
)

set(SNW_SRCS
    snw.cpp
)

add_executable(snw ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw ${SNW_LIBS})
