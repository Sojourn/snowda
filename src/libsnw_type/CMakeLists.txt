set(SNW_LIBS
    snw_util
)

set(SNW_HDRS
    snw_type.h
)

set(SNW_SRCS
)

add_library(snw_type ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_type ${SNW_LIBS})
target_include_directories(snw_type PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
