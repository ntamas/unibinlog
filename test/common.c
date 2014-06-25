#define TEST_CASE(func) int test_ ## func()

#define START_OF_TESTS                 \
    int main(int argc, char* argv[]) { \
        int __test_index = 1;          \

#define RUN_TEST_CASE(func) {  \
    int __retval;               \
    printf("%3d. " #func "... ", __test_index); fflush(stdout); \
    __retval = test_ ## func(); \
    if (__retval != 0) {   \
        printf("FAILED.\n"); \
        fprintf(stderr, "Test case '" #func "' failed with error code: %d\n", __retval); \
        return __test_index; \
    } else {                 \
        printf("ok.\n"); \
    }                        \
    __test_index++;          \
}

#define NO_MORE_TEST_CASES \
    return 0;              \
}

