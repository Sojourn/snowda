set(SNW_LIBS
)

set(SNW_HDRS
    snw_msg.h
)

set(SNW_SRCS
    snw_msg.cpp
)

add_executable(snw_msg ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_msg ${SNW_LIBS})
