set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(libsnw_util)
add_subdirectory(libsnw_code)
add_subdirectory(libsnw_memory)
add_subdirectory(libsnw_compiler)

add_subdirectory(snw)
add_subdirectory(snwc)
add_subdirectory(snw_msg)
add_subdirectory(snw_tst)
add_subdirectory(snw_study)
