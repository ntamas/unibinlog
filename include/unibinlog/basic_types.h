/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_BASIC_TYPES_H
#define UNIBINLOG_BASIC_TYPES_H

#include <stdint.h>

/* Typedefs for unsigned and signed integers with exact bit sizes */
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/* Typedefs for floating point numbers with exact bit sizes */
typedef float f32;
typedef double f64;
typedef long double f128;

/** Typedef for booleans */
typedef unsigned char ub_bool_t;

#endif

