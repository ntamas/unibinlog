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

	return 0;
}

START_OF_TESTS;
RUN_TEST_CASE(get_info);
NO_MORE_TEST_CASES;

