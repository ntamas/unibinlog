#include <string.h>

#include <unibinlog/lowlevel.h>
#include "fmemopen.h"
#include "common.c"

TEST_CASE(write_header) {
    char buffer[32];
    FILE* f;

    /* Version 1, 'sum' checksum */
    f = fmemopen(buffer, 32, "w+");
    ub_write_header(f, 1, UB_CHKSUM_SUM);
    fclose(f);
    if (strcmp(buffer, "UNIBIN\x01\x01"))
        return 1;

    /* Version 42, 'Fletcher 16' checksum */
    f = fmemopen(buffer, 32, "w+");
    ub_write_header(f, 42, UB_CHKSUM_FLETCHER_16);
    fclose(f);
    if (strcmp(buffer, "UNIBIN\x2A\x03"))
        return 2;

    return 0;
}

START_OF_TESTS;
RUN_TEST_CASE(write_header);
NO_MORE_TEST_CASES;
