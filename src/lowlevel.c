/* vim:set ts=4 sw=4 sts=4 et: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    u8 buf[3];

    if (length > 65535)
        return UB_ETOOLONG;

    /* write the header */
    buf[0] = block_type;
    buf[1] = (length >> 8) & 0xff;
    buf[2] = length & 0xff;
    UB_CHECK(ub_write_u8_array(f, buf, 3));

    /* write the payload */
    UB_CHECK(ub_write_u8_array(f, payload, length));

    /* write the checksum if needed */
    if (chksum_type != UB_CHKSUM_NONE)
        return UB_EUNIMPLEMENTED;

    return UB_SUCCESS;
}

