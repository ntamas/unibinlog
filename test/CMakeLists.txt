set(TESTS buffer buffer_writer chksum log_column lowlevel types)
set(TEST_SUPPORT_SRCS fmemopen.c)

foreach(test_name ${TESTS})
    add_executable(test_${test_name} test_${test_name}.c ${TEST_SUPPORT_SRCS})
    target_include_directories(test_${test_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../src)
    target_link_libraries(test_${test_name} unibinlog)
    add_test(NAME ${test_name} COMMAND test_${test_name})
endforeach()
