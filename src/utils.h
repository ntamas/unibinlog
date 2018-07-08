/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_I_UTILS_H
#define UNIBINLOG_I_UTILS_H

#include <stdint.h>
#include "config.h"

#ifdef HAVE_UINT64
#  ifndef HAVE_HTONLL
/**
 * Converts the given 64-bit value from host byte order to network byte order.
 *
 * \param  value  the value to convert
 * \return the converted value
 */
uint64_t htonll(uint64_t value);
#  endif
#endif

#ifdef HAVE_IEEE754_FLOATS

/**
 * Converts the given IEEE-754 float from host byte order to network byte order.
 *
 * \param  value  the value to convert
 * \return the converted value
 */
float htonf(float value);

/**
 * Converts the given IEEE-754 double from host byte order to network byte order.
 *
 * \param  value  the value to convert
 * \return the converted value
 */
double htonlf(double value);

#endif

#endif

