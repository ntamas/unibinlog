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
    UB_BUFFER(buf)[1] = (length >> 8) & 0xff;
    UB_BUFFER(buf)[2] = length & 0xff;

    /* copy the payload */
    ub_buffer_read_into(&buf, 3, payload, length);

    /* write the checksum if needed */
    UB_CHECK(ub_buffer_update_checksum(&buf, chksum_type));

    /* write the entire buffer into a file */
    UB_CHECK(ub_buffer_fwrite(&buf, f));

    /* destroy the buffer */
    ub_buffer_destroy(&buf);

    return UB_SUCCESS;
}

ub_error_t ub_write_comment_block(FILE* f, const char* comment,
        ub_chksum_type_t chksum_type) {
    return ub_write_block(f, UB_BLOCK_COMMENT, comment, strlen(comment),
            chksum_type);
}


