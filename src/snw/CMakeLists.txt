set(SNW_LIBS
    snw_util
    snw_code
    snw_memory
    snw_compiler
)

set(SNW_HDRS
    snw.h
    snw_virtual_machine.h
)

set(SNW_SRCS
    snw.cpp
    snw_virtual_machine.cpp
)

add_executable(snw ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw ${SNW_LIBS})
