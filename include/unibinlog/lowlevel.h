/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_LOWLEVEL_H
#define UNIBINLOG_LOWLEVEL_H

#include <stdio.h>

#include <unibinlog/basic_types.h>
#include <unibinlog/chksum.h>
#include <unibinlog/error.h>

/**
 * Writes the header of an \c unibin log file into the given file.
 *
 * \param  f            the file to write into
 * \param  version      the version number to write into the header
 * \param  chksum_type  the checksum type that the file will use for each block
 */
ub_error_t ub_write_header(FILE* f, u8 version, ub_chksum_type_t chksum_type);

#endif

