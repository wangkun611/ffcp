add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/huffman_table.h
    COMMAND $<TARGET_FILE:ffcp> ARGS ${CMAKE_CURRENT_SOURCE_DIR}/qpack.txt ${CMAKE_CURRENT_BINARY_DIR}/huffman_table.h
    DEPENDS  ffcp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(ffcp_test ffcp_test.c ${CMAKE_CURRENT_BINARY_DIR}/huffman_table.h)

add_test(ffcp_test ffcp_test)
