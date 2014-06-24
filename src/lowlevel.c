/* vim:set ts=4 sw=4 sts=4 et: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unibinlog/lowlevel.h>

static const char* ub_i_header_marker = "UNIBIN";
static size_t ub_i_header_marker_length = 6;

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
    if (fwrite(header, sizeof(u8), sizeof(header), f) != sizeof(header))
        return UB_EWRITE;

    return UB_SUCCESS;
}
