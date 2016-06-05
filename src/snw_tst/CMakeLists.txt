set(SNW_LIBS
    snw_util
    snw_memory
    snw_compiler
)

set(SNW_HDRS
    snw_tst.h
)

set(SNW_SRCS
    snw_tst.cpp
)

add_executable(snw_tst ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_tst ${SNW_LIBS})
