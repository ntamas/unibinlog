/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_CHKSUM_H
#define UNIBINLOG_CHKSUM_H

#include <stdlib.h>

#include <unibinlog/error.h>

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
 *
 * \param  type  the checksum type
 * \return the number of bytes needed to store the checksum
 */
size_t ub_chksum_size(ub_chksum_type_t type);

/**
 * Calculates the given checksum type on the given byte array.
 *
 * \param  array   the array to calculate the checksum on
 * \param  size    the length of the buffer
 * \param  chksum  the memory segment to write the checksum to. It must be
 *                 large enough to hold the checksum.
 * \param  chksum_type  the type of the checksum to calculate
 *
 * \return \c UB_SUCCESS if the checksum was calculated successfully,
 *         \c UB_EINVAL if the checksum type is unknown
 */
ub_error_t ub_get_chksum_of_array(const void* array, size_t size,
        void* chksum, ub_chksum_type_t chksum_type);

#endif

