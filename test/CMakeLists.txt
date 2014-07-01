set(TESTS chksum log_column lowlevel)
set(TEST_SUPPORT_SRCS fmemopen.c)

foreach(test_name ${TESTS})
	add_executable(test_${test_name} test_${test_name}.c ${TEST_SUPPORT_SRCS})
    target_link_libraries(test_${test_name} unibinlog)
    add_test(NAME ${test_name} COMMAND test_${test_name})
endforeach()
