set(SNW_LIBS
)

set(SNW_HDRS
)

set(SNW_SRCS
    snw_study.cpp
)

add_executable(snw_study ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_study ${SNW_LIBS})
