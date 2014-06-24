#define TEST_CASE(func) int test_ ## func()

#define START_OF_TESTS                 \
    int main(int argc, char* argv[]) { \
        int __test_index = 1;          \

#define RUN_TEST_CASE(func) {  \
    int __retval = test_ ## func(); \
    if (__retval != 0) {   \
        fprintf(stderr, "Test case '" #func "' failed with error code: %d\n", __retval); \
        return __test_index; \
    }                      \
    __test_index++;          \
}

#define NO_MORE_TEST_CASES \
    return 0;              \
}

