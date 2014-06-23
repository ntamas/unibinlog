/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_LOWLEVEL_H
#define UNIBINLOG_LOWLEVEL_H

#include <unibinlog/basic_types.h>
#include <unibinlog/chksum.h>
#include <unibinlog/error.h>

ub_error_t ub_write_header(FILE* f, u8 version, ub_chksum_type_t chksum_type);

#endif

