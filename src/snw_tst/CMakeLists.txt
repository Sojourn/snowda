set(SNW_LIBS
    snw_util
    snw_compiler
)

set(SNW_HDRS
    tst.h
)

set(SNW_SRCS
    tst.cpp
)

add_executable(snw_tst ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_tst ${SNW_LIBS})
