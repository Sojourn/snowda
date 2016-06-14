set(SNW_LIBS
    snw_util
    snw_code
    snw_memory
    snw_compiler
)

set(SNW_HDRS
    snw_tst.h
    snw_tst_util.h
    snw_tst_memory.h
    snw_tst_compiler.h
)

set(SNW_SRCS
    snw_tst.cpp
    snw_tst_util.cpp
    snw_tst_memory.cpp
    snw_tst_compiler.cpp
)

add_executable(snw_tst ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_tst ${SNW_LIBS})
