#include <string.h>

#include <unibinlog/log_column.h>
#include "fmemopen.h"
#include "common.c"

TEST_CASE(get_length) {
	ub_log_column_t columns[7];
	int i;

	ub_log_column_init(&columns[0], 0, UB_DATATYPE_UNKNOWN);
	ub_log_column_init(&columns[1], 0, UB_DATATYPE_U8);
	ub_log_column_init(&columns[2], 0, UB_DATATYPE_S16);
	ub_log_column_init(&columns[3], 0, UB_DATATYPE_UNIX_TIMESTAMP);
	ub_log_column_init(&columns[4], 0, UB_DATATYPE_DOUBLE);
	ub_log_column_init(&columns[5], 0, UB_DATATYPE_STRING);
	ub_log_column_init(&columns[6], 0, UB_DATATYPE_BLOB);

	if (ub_log_column_get_length(&columns[0]) != 0)
		return 1;
	if (ub_log_column_get_length(&columns[1]) != 1)
		return 2;
	if (ub_log_column_get_length(&columns[2]) != 2)
		return 3;
	if (ub_log_column_get_length(&columns[3]) != 4)
		return 4;
	if (ub_log_column_get_length(&columns[4]) != 8)
		return 5;
	if (ub_log_column_get_length(&columns[5]) != 0)
		return 6;
	if (ub_log_column_get_length(&columns[6]) != 0)
		return 7;
	if (ub_log_columns_get_total_length(columns, 5) != 15)
		return 8;
	if (ub_log_columns_get_total_length(columns, 6) != 0)
		return 9;
	if (ub_log_columns_get_total_length(columns, 7) != 0)
		return 10;

	for (i = 0; i < sizeof(columns) / sizeof(columns[0]); i++) {
		ub_log_column_destroy(&columns[i]);
	}


	return 0;
}

TEST_CASE(get_set_name) {
    const char* str = "Spanish inquisition";
	ub_log_column_t col;

	ub_log_column_init(&col, 0, UB_DATATYPE_UNKNOWN);
	if (ub_log_column_get_name(&col) != 0)
		return 1;

	ub_log_column_set_name(&col, str);
	if (strcmp(str, ub_log_column_get_name(&col)))
		return 2;
	if (ub_log_column_get_name(&col) == str)
		return 3;

	ub_log_column_set_name(&col, 0);
	if (ub_log_column_get_name(&col) != 0)
		return 4;

	ub_log_column_init(&col, str, UB_DATATYPE_UNKNOWN);
	if (strcmp(str, ub_log_column_get_name(&col)))
		return 5;
	if (ub_log_column_get_name(&col) == str)
		return 6;

	ub_log_column_destroy(&col);

    return 0;
}

TEST_CASE(get_set_type) {
    const char* str = "Spanish inquisition";
	ub_log_column_t col;

	ub_log_column_init(&col, str, UB_DATATYPE_UNKNOWN);
	if (ub_log_column_get_type(&col) != UB_DATATYPE_UNKNOWN)
		return 1;

	ub_log_column_set_type(&col, UB_DATATYPE_DOUBLE);
	if (ub_log_column_get_type(&col) != UB_DATATYPE_DOUBLE)
		return 2;

	ub_log_column_set_type(&col, 0);
	if (ub_log_column_get_type(&col) != 0)
		return 3;

	ub_log_column_init(&col, str, UB_DATATYPE_U8);
	if (ub_log_column_get_type(&col) != UB_DATATYPE_U8)
		return 4;

	ub_log_column_destroy(&col);

    return 0;
}

TEST_CASE(write) {
	ub_buffer_t buffer;
	ub_buffer_location_t loc;
	ub_log_column_t col;

	/* Create test buffer */
	ub_buffer_init(&buffer, 0);
	loc = ub_buffer_front(&buffer);

	/* Name: 'lat', type: single-precision float, no transformation */
	ub_log_column_init(&col, "lat", UB_DATATYPE_FLOAT);
	ub_log_column_write(&col, &loc);
	if (ub_buffer_size(&buffer) != 6 || loc.index != 6)
		return 1;
	if (memcmp(UB_BUFFER(buffer), "\x03lat\x08\x00", 6))
		return 2;
	ub_log_column_destroy(&col);

	/* Name: 'lon', type: single-precision float, no transformation */
	ub_log_column_init(&col, "lon", UB_DATATYPE_FLOAT);
	ub_log_column_write(&col, &loc);
	if (ub_buffer_size(&buffer) != 12 || loc.index != 12)
		return 3;
	if (memcmp(UB_BUFFER(buffer), "\x03lat\x08\x00\x03lon\x08\x00\x03", 12))
		return 4;
	ub_log_column_destroy(&col);

	/* Name: 'heading', type: uint16_t, linear transformation */
	ub_log_column_init(&col, "heading", UB_DATATYPE_U16);
	col.xform = UB_XFORM_LINEAR;
	if (ub_log_column_write(&col, &loc) != UB_EUNIMPLEMENTED)
		return 5;
	if (ub_buffer_size(&buffer) != 12 || loc.index != 12)
		return 6;
	if (memcmp(UB_BUFFER(buffer), "\x03lat\x08\x00\x03lon\x08\x00\x03", 12))
		return 7;
	ub_log_column_destroy(&col);
	
    return 0;
}

START_OF_TESTS;
RUN_TEST_CASE(get_set_name);
RUN_TEST_CASE(get_set_type);
RUN_TEST_CASE(write);
NO_MORE_TEST_CASES;
