/* vim:set ts=4 sw=4 sts=4 et: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unibinlog/buffer.h>
#include <unibinlog/lowlevel.h>

static const char* ub_i_header_marker = "UNIBIN";
static size_t ub_i_header_marker_length = 6;

ub_error_t ub_write_u8_array(FILE* f, const u8* array, size_t length) {
    if (fwrite(array, sizeof(u8), length, f) != length) {
        return UB_EWRITE;
    } else {
        return UB_SUCCESS;
    }
}

ub_error_t ub_write_header(FILE* f, u8 version, ub_chksum_type_t chksum_type) {
	u8 header[ub_i_header_marker_length+2];
    size_t pos;

	/* format marker */
	memcpy(header, ub_i_header_marker, ub_i_header_marker_length);
    pos = ub_i_header_marker_length;

	/* version number */
	header[pos] = version;

	/* checksum type */
	header[pos+1] = chksum_type;

    /* write the header */
    UB_CHECK(ub_write_u8_array(f, header, sizeof(header)));

    return UB_SUCCESS;
}

ub_error_t ub_write_block(FILE* f, ub_block_type_t block_type,
        const void* payload, size_t length, ub_chksum_type_t chksum_type) {
    ub_buffer_t buf;
    ub_buffer_location_t loc;
    size_t buf_size;
    size_t chksum_size;

    if (length > 65535)
        return UB_ETOOLONG;

    /* create the buffer where we will assemble the block */
    chksum_size = ub_chksum_size(chksum_type);
    buf_size = length + chksum_size + 3;
    UB_CHECK(ub_buffer_init(&buf, buf_size));

    /* write the header */
    UB_BUFFER(buf)[0] = block_type;
    UB_BUFFER_FROM_INDEX_AS(buf, 1, u16)[0] = htons(length);

    /* copy the payload */
    loc = ub_buffer_location(&buf, 3);
    ub_buffer_update_from_array(&loc, payload, length);

    /* write the checksum if needed */
    UB_CHECK(ub_buffer_update_checksum(&buf, chksum_type));

    /* write the entire buffer into a file */
    UB_CHECK(ub_buffer_fwrite(&buf, f));

    /* destroy the buffer */
    ub_buffer_destroy(&buf);

    return UB_SUCCESS;
}

static ub_error_t ub_i_write_block_from_buffer(FILE* f, ub_block_type_t block_type,
        ub_buffer_t* payload, ub_chksum_type_t chksum_type) {
    return ub_write_block(f, block_type, UB_BUFFER(*payload),
            ub_buffer_size(payload), chksum_type);
}

ub_error_t ub_write_comment_block(FILE* f, const char* comment,
        ub_chksum_type_t chksum_type) {
    return ub_write_block(f, UB_BLOCK_COMMENT, comment, strlen(comment),
            chksum_type);
}

ub_error_t ub_write_log_header_block(FILE* f, ub_log_column_t* columns,
        size_t num_columns, ub_chksum_type_t chksum_type) {
    ub_buffer_t buf;
    ub_buffer_location_t loc;

    if (num_columns > 255)
        return UB_ETOOLONG;

    /* create the buffer where we will assemble the block */
    UB_CHECK(ub_buffer_init(&buf, 1));

    /* write the number of columns */
    UB_BUFFER(buf)[0] = num_columns;

    /* write the columns themselves */
    loc = ub_buffer_location(&buf, 1);
    while (num_columns > 0) {
        UB_CHECK(ub_log_column_write(columns, &loc));
        columns++;
        num_columns--;
    }

    /* write the entire buffer into a file */
    UB_CHECK(ub_i_write_block_from_buffer(f, UB_BLOCK_LOG_HEADER, &buf, chksum_type));

    /* destroy the buffer */
    ub_buffer_destroy(&buf);

    return UB_SUCCESS;
}

