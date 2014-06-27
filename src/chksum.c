/* vim:set ts=4 sw=4 sts=4 et: */

#include <stdlib.h>

#include <unibinlog/basic_types.h>
#include <unibinlog/chksum.h>

static size_t ub_i_chksum_sizes[] = {
	0,             /* UB_CHKSUM_NONE */
	1,             /* UB_CHKSUM_SUM */
	1,             /* UB_CHKSUM_NEGATED_SUM */
	2              /* UB_CHKSUM_FLETCHER_16 */
};

size_t ub_chksum_size(ub_chksum_type_t type) {
	return ub_i_chksum_sizes[type];
}

ub_error_t ub_get_chksum_of_array(const void* array_, size_t size,
        void* chksum_, ub_chksum_type_t chksum_type) {
    u8* array = (u8*)array_;
    u8* chksum = (u8*)chksum_;
    size_t i;

    switch (chksum_type) {
        case UB_CHKSUM_NONE:
            /* Nothing to do */
            return UB_SUCCESS;

        case UB_CHKSUM_SUM:
        case UB_CHKSUM_NEGATED_SUM:
            *chksum = 0;
            for (i = 0; i < size; i++) {
                *chksum += array[i];
            }
            if (chksum_type == UB_CHKSUM_NEGATED_SUM) {
                *chksum = ~(*chksum);
            }
            break;

        case UB_CHKSUM_FLETCHER_16:
            chksum[0] = chksum[1] = 0;
            for (i = 0; i < size; i++) {
                chksum[0] = (chksum[0] + array[i]) % 255;
                chksum[1] = (chksum[1] + chksum[0]) % 255;
            }
            break;

        default:
            return UB_EINVAL;
    }

    return UB_SUCCESS;
}

