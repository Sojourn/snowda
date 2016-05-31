set(SNW_LIBS
    snw_util
)

set(SNW_HDRS
    snw_heap.h
    snw_page.h
    snw_page_set.h
    snw_page_pool.h
)

set(SNW_SRCS
    snw_heap.cpp
    snw_page_set.cpp
    snw_page_pool.cpp
)

add_library(snw_heap ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_heap ${SNW_LIBS})
target_include_directories(snw_heap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
