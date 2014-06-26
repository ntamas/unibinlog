#include <unibinlog/basic_types.h>
#include <unibinlog/chksum.h>
#include "common.c"

TEST_CASE(chksum_size) {
    if (ub_chksum_size(UB_CHKSUM_NONE) != 0)
        return 1;
    if (ub_chksum_size(UB_CHKSUM_SUM) != 1)
        return 2;
    if (ub_chksum_size(UB_CHKSUM_NEGATED_SUM) != 1)
        return 3;
    if (ub_chksum_size(UB_CHKSUM_FLETCHER_16) != 2)
        return 4;
    return 0;
}

TEST_CASE(get_chksum_of_array) {
    u8 array[4] = { 0x01, 0x02, 0xF8, 0x04 };
    u8 chksum[2];

    /* Test no checksum */
    chksum[0] = 0xde; chksum[1] = 0xad;
    ub_get_chksum_of_array(array, 4, chksum, UB_CHKSUM_NONE);
    if (chksum[0] != 0xde && chksum[1] != 0xad)
        return 1;

    /* Test simple byte sum */
    chksum[0] = 0xde; chksum[1] = 0xad;
    ub_get_chksum_of_array(array, 4, chksum, UB_CHKSUM_SUM);
    if (chksum[0] != 255 && chksum[1] != 0xad)
        return 2;

    /* Test negated byte sum */
    chksum[0] = 0xde; chksum[1] = 0xad;
    ub_get_chksum_of_array(array, 4, chksum, UB_CHKSUM_NEGATED_SUM);
    if (chksum[0] != 0 && chksum[1] != 0xad)
        return 3;

    /* Test Fletcher-16 checksum */
    chksum[0] = 0xde; chksum[1] = 0xad;
    ub_get_chksum_of_array(array, 4, chksum, UB_CHKSUM_FLETCHER_16);
    if (chksum[0] != 0 && chksum[1] != 0)
        return 4;

    /* Test unknown checksum */
    if (ub_get_chksum_of_array(array, 4, chksum, 254) != UB_EINVAL)
        return 5;

    return 0;
}

START_OF_TESTS;
RUN_TEST_CASE(chksum_size);
RUN_TEST_CASE(get_chksum_of_array);
NO_MORE_TEST_CASES;
