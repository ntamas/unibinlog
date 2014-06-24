/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_CHKSUM_H
#define UNIBINLOG_CHKSUM_H

#include <stdlib.h>

/**
 * Enum constants for the different checksum types supported by this library.
 */
typedef enum {
	UB_CHKSUM_NONE = 0,           /**< No checksum */
	UB_CHKSUM_SUM,                /**< Sum of bytes modulo 256 */
	UB_CHKSUM_NEGATED_SUM,        /**< Sum of bytes modulo 256, bitwise negated */
	UB_CHKSUM_FLETCHER_16,        /**< 16-bit Fletcher checksum with modulo 255 */
} ub_chksum_type_t;

/**
 * Returns the number of bytes required by the given checksum type.
 */
size_t ub_chksum_size(ub_chksum_type_t type);

#endif

