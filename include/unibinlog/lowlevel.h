/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_LOWLEVEL_H
#define UNIBINLOG_LOWLEVEL_H

#include <stdio.h>

#include <unibinlog/basic_types.h>
#include <unibinlog/buffer.h>
#include <unibinlog/chksum.h>
#include <unibinlog/error.h>
#include <unibinlog/log_column.h>
#include <unibinlog/types.h>

/**
 * Writes a byte array into the given file.
 *
 * \param  f       the file to write into
 * \param  array   the array to write
 * \param  length  the length of the array in bytes
 * \return \c UB_EWRITE if there was an error while writing, \c UB_SUCCESS
 *         otherwise
 */
ub_error_t ub_write_byte_array(FILE* f, const void* array, size_t length);

/**
 * Writes the header of an \c unibin log file into the given file.
 *
 * \param  f            the file to write into
 * \param  version      the version number to write into the header
 * \param  chksum_type  the checksum type that the file will use for each block
 */
ub_error_t ub_write_header(FILE* f, uint8_t version, ub_chksum_type_t chksum_type);

/**
 * Writes a \c unibin block with the given payload into the given file.
 *
 * \param  f            the file to write into
 * \param  block_type   the type of the block to write
 * \param  payload      the payload of the block
 * \param  length       the size of the payload
 * \param  chksum_type  the checksum type at the end of the block (if any).
 *                      This must match the checksum type specified in the
 *                      header of the \c unibin file
 */
ub_error_t ub_write_block(FILE* f, ub_block_type_t block_type,
        const void* payload, size_t length, ub_chksum_type_t chksum_type);

/**
 * Writes a \c unibin block with the given payload into the given file,
 * assuming that the payload comes from a \c ub_buffer_t, which knows its
 * own size.
 *
 * \param  f            the file to write into
 * \param  block_type   the type of the block to write
 * \param  payload      the payload of the block
 * \param  chksum_type  the checksum type at the end of the block (if any).
 *                      This must match the checksum type specified in the
 *                      header of the \c unibin file
 */
ub_error_t ub_write_block_from_buffer(FILE* f, ub_block_type_t block_type,
        ub_buffer_t* payload, ub_chksum_type_t chksum_type);

/**
 * Writes a comment block containing the given string into the given file.
 *
 * \param  f            the file to write into
 * \param  comment      the comment to write
 * \param  chksum_type  the checksum type at the end of the block (if any).
 *                      This must match the checksum type specified in the
 *                      header of the \c unibin file
 */
ub_error_t ub_write_comment_block(FILE* f, const char* comment,
        ub_chksum_type_t chksum_type);

/**
 * Writes a log header block containing the given columns into the given
 * file.
 *
 * \param  f            the file to write into
 * \param  columns      pointer to an array containing the column headers
 *                      to write
 * \param  num_columns  the number of column headers to write
 * \param  chksum_type  the checksum type at the end of the block (if any).
 *                      This must match the checksum type specified in the
 *                      header of the \c unibin file
 */
ub_error_t ub_write_log_header_block(FILE* f, ub_log_column_t* columns,
        size_t num_columns, ub_chksum_type_t chksum_type);

#endif

