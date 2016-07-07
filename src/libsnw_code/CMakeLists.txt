set(SNW_LIBS
    snw_util
)

set(SNW_HDRS
    snw_code.h
    snw_type.h
    snw_instruction.h
    snw_instruction_util.h
    snw_function.h
)

set(SNW_SRCS
    snw_function.cpp
    snw_type.cpp
    snw_instruction_util.cpp
)

add_library(snw_code ${SNW_HDRS} ${SNW_SRCS})
target_link_libraries(snw_code ${SNW_LIBS})
target_include_directories(snw_code PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
