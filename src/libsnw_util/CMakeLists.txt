set(SNW_LIBS
)

set(SNW_HDRS
    snw_util.h
    snw_fixed_vector.h
    snw_result.h
    snw_view.h
    snw_bit_tree.h
    snw_bit_tree_inline.h
)

set(SNW_SRCS
    snw_bit_tree.cpp
)

add_library(snw_util ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_util ${SNW_LIBS})
target_include_directories(snw_util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
