/* vim:set ts=4 sw=4 sts=4 et: */

#include <stdlib.h>

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

