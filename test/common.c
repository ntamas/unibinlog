#include <stdio.h>

#define TEST_CASE(func) int test_ ## func()

#define START_OF_TESTS                 \
    int main(int argc, char* argv[]) { \
        int __test_index = 1;          \
		int __first_failed_test = 0;   \
		int __num_failed_tests = 0;    \

#define RUN_TEST_CASE(func) {  \
    int __retval;               \
    printf("%3d. " #func "... ", __test_index); fflush(stdout); \
    __retval = test_ ## func(); \
    if (__retval != 0) {   \
        printf("FAILED.\n"); \
        fprintf(stderr, "Test case '" #func "' failed with error code: %d\n", __retval); \
		if (__first_failed_test <= 0) { \
			__first_failed_test = __test_index; \
		} \
		__num_failed_tests++; \
    } else {                 \
        printf("ok.\n"); \
    }                        \
    __test_index++;          \
}

#define NO_MORE_TEST_CASES \
	printf("\nRan %d test(s), %d successful, %d failed.\n", __test_index-1, \
			__test_index-1-__num_failed_tests, __num_failed_tests); \
    return __first_failed_test;              \
}

