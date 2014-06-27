#include <string.h>

#include <unibinlog/lowlevel.h>
#include "fmemopen.h"
#include "common.c"

TEST_CASE(write_u8_array) {
    char buffer[32];
    const char* str = "Spanish inquisition";
    FILE* f;

    f = fmemopen(buffer, 32, "w+");
    ub_write_u8_array(f, (const u8*)str, strlen(str));
    fclose(f);
    if (strcmp(buffer, str))
        return 1;

    return 0;
}

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

TEST_CASE(write_block) {
    char buffer[32];
    const char* payload = "Spanish Inquisition";
    FILE* f;

    /* Testing comment block without checksum */
    f = fmemopen(buffer, 32, "w+");
    ub_write_block(f, UB_BLOCK_COMMENT, payload, strlen(payload), UB_CHKSUM_NONE);
    fclose(f);
    if (buffer[0] != UB_BLOCK_COMMENT || buffer[1] != 0 || buffer[2] != 19)
        return 1;
    if (strcmp(buffer+3, payload))
        return 2;

    /* Testing comment block with checksum */
    f = fmemopen(buffer, 32, "w+");
    ub_write_block(f, UB_BLOCK_COMMENT, payload, strlen(payload), UB_CHKSUM_SUM);
    fclose(f);
    if (buffer[0] != UB_BLOCK_COMMENT || buffer[1] != 0 || buffer[2] != 19)
        return 3;
    if (memcmp(buffer+3, payload, strlen(payload)))
        return 4;
	if (strcmp(buffer+22, "\xa6"))
		return 5;

    /* Testing comment block with two-byte checksum */
    f = fmemopen(buffer, 32, "w+");
    ub_write_block(f, UB_BLOCK_COMMENT, payload, strlen(payload), UB_CHKSUM_FLETCHER_16);
    fclose(f);
    if (buffer[0] != UB_BLOCK_COMMENT || buffer[1] != 0 || buffer[2] != 19)
        return 6;
    if (memcmp(buffer+3, payload, strlen(payload)))
        return 7;
	if (strcmp(buffer+22, "\xad\x75"))
		return 8;

    /* Testing block length too large */
    f = fmemopen(buffer, 32, "w+");
    if (ub_write_block(f, UB_BLOCK_COMMENT, payload, 12345678, UB_CHKSUM_NONE) !=
            UB_ETOOLONG) {
        fclose(f);
        return 9;
    }
    fclose(f);

    return 0;
}

START_OF_TESTS;
RUN_TEST_CASE(write_u8_array);
RUN_TEST_CASE(write_header);
RUN_TEST_CASE(write_block);
NO_MORE_TEST_CASES;
