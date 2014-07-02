#include <string.h>

#include <unibinlog/types.h>
#include "common.c"

TEST_CASE(get_info) {
	ub_typeinfo_t info;
	ub_datatype_t type;
	
	for (type = UB_DATATYPE_UNKNOWN; type < UB_MAX_DATATYPE; type++) {
		info = ub_datatype_get_info(type);

		/* info.type should match type */
		if (info.type != type) {
			fprintf(stderr, "%s: info.type = %d, expected %d\n",
					info.name, (int)info.type, (int)type);
			return 1;
		}

		/* info.name should be sensible */
		if (info.name == 0 || strlen(info.name) < 3) {
			return 2;
		}

		/* info.c_name should be sensible */
		if (info.type != UB_DATATYPE_UNKNOWN &&
				(info.c_name == 0 || strlen(info.c_name) < 3)) {
			return 3;
		}

		/* info.length should be 0 if is_variable_length */
		if (info.is_variable_length && info.length != 0) {
			return 4;
		}
	}

	/* strings and blobs are variable length */
	info = ub_datatype_get_info(UB_DATATYPE_STRING);
	if (!info.is_variable_length)
		return 5;
	info = ub_datatype_get_info(UB_DATATYPE_BLOB);
	if (!info.is_variable_length)
		return 6;
	info = ub_datatype_get_info(UB_DATATYPE_SHORT_BLOB);
	if (!info.is_variable_length)
		return 7;

	/* check lengths of common 1-byte types */
	info = ub_datatype_get_info(UB_DATATYPE_U8);
	if (info.length != sizeof(uint8_t))
		return 8;
	info = ub_datatype_get_info(UB_DATATYPE_S8);
	if (info.length != sizeof(int8_t))
		return 9;
	info = ub_datatype_get_info(UB_DATATYPE_BOOLEAN);
	if (info.length != sizeof(unsigned char))
		return 10;
	info = ub_datatype_get_info(UB_DATATYPE_CHAR);
	if (info.length != sizeof(unsigned char))
		return 11;

	/* check lengths of common 2-byte types */
	info = ub_datatype_get_info(UB_DATATYPE_U16);
	if (info.length != sizeof(uint16_t))
		return 12;
	info = ub_datatype_get_info(UB_DATATYPE_S16);
	if (info.length != sizeof(int16_t))
		return 13;

	/* check lengths of common 4-byte types */
	info = ub_datatype_get_info(UB_DATATYPE_U32);
	if (info.length != sizeof(uint32_t))
		return 14;
	info = ub_datatype_get_info(UB_DATATYPE_S32);
	if (info.length != sizeof(int32_t))
		return 15;
	info = ub_datatype_get_info(UB_DATATYPE_UNIX_TIMESTAMP);
	if (info.length != sizeof(uint32_t))
		return 16;
	info = ub_datatype_get_info(UB_DATATYPE_FLOAT);
	if (info.length != sizeof(float))
		return 17;

	/* check lengths of common 8-byte types */
	info = ub_datatype_get_info(UB_DATATYPE_DOUBLE);
	if (info.length != sizeof(double))
		return 18;

	return 0;
}

START_OF_TESTS;
RUN_TEST_CASE(get_info);
NO_MORE_TEST_CASES;

