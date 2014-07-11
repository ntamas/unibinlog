/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_I_UTILS_H
#define UNIBINLOG_I_UTILS_H

#include <stdint.h>

/**
 * Converts the given 64-bit value from host byte order to network byte order.
 *
 * \param  value  the value to convert
 * \return the converted value
 */
uint64_t htonll(uint64_t value);

#endif

