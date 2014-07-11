/* vim:set ts=4 sw=4 sts=4 et: */

#include <assert.h>
#include <unibinlog/types.h>

static ub_typeinfo_t ub_i_datatype_info[] = {
	/* UB_DATATYPE_UNKNOWN */
	{
		/* type = */   UB_DATATYPE_UNKNOWN,
		/* name = */   "unknown",
		/* c_name = */ 0,
		/* length = */ 0,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_BOOLEAN */
	{
		/* type = */   UB_DATATYPE_BOOLEAN,
		/* name = */   "Boolean",
		/* c_name = */ "unsigned char",
		/* length = */ 1,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_U8 */
	{
		/* type = */   UB_DATATYPE_U8,
		/* name = */   "8-bit unsigned integer",
		/* c_name = */ "uint8_t",
		/* length = */ 1,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_S8 */
	{
		/* type = */   UB_DATATYPE_S8,
		/* name = */   "8-bit signed integer",
		/* c_name = */ "int8_t",
		/* length = */ 1,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_U16 */
	{
		/* type = */   UB_DATATYPE_U16,
		/* name = */   "16-bit unsigned integer",
		/* c_name = */ "uint16_t",
		/* length = */ 2,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_S16 */
	{
		/* type = */   UB_DATATYPE_S16,
		/* name = */   "16-bit signed integer",
		/* c_name = */ "int16_t",
		/* length = */ 2,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_U32 */
	{
		/* type = */   UB_DATATYPE_U32,
		/* name = */   "32-bit unsigned integer",
		/* c_name = */ "uint32_t",
		/* length = */ 4,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_S32 */
	{
		/* type = */   UB_DATATYPE_S32,
		/* name = */   "32-bit signed integer",
		/* c_name = */ "int32_t",
		/* length = */ 4,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_FLOAT */
	{
		/* type = */   UB_DATATYPE_FLOAT,
		/* name = */   "IEEE-754 single-precision float",
		/* c_name = */ "float",
		/* length = */ 4,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_DOUBLE */
	{
		/* type = */   UB_DATATYPE_DOUBLE,
		/* name = */   "IEEE-754 double-precision float",
		/* c_name = */ "double",
		/* length = */ 8,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_CHAR */
	{
		/* type = */   UB_DATATYPE_CHAR,
		/* name = */   "Character",
		/* c_name = */ "char",
		/* length = */ 1,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_STRING */
	{
		/* type = */   UB_DATATYPE_STRING,
		/* name = */   "Null-terminated string",
		/* c_name = */ "char*",
		/* length = */ 0,
		/* is_variable_length */ 1
	},
	/* UB_DATATYPE_SHORT_BLOB */
	{
		/* type = */   UB_DATATYPE_SHORT_BLOB,
		/* name = */   "Binary data with 1-byte length prefix",
		/* c_name = */ "uint8_t[]",
		/* length = */ 0,
		/* is_variable_length */ 1
	},
	/* UB_DATATYPE_BLOB */
	{
		/* type = */   UB_DATATYPE_BLOB,
		/* name = */   "Binary data with 2-byte length prefix",
		/* c_name = */ "uint8_t[]",
		/* length = */ 0,
		/* is_variable_length */ 1
	},
	/* UB_DATATYPE_UNIX_TIMESTAMP */
	{
		/* type = */   UB_DATATYPE_UNIX_TIMESTAMP,
		/* name = */   "UNIX timestamp (msecs since UNIX epoch)",
		/* c_name = */ "uint64_t",
		/* length = */ 8,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_TIMEVAL */
	{
		/* type = */   UB_DATATYPE_TIMEVAL,
		/* name = */   "UNIX timestamp (secs+usecs since UNIX epoch)",
		/* c_name = */ "uint32_t[2]",
		/* length = */ 8,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_U64 */
	{
		/* type = */   UB_DATATYPE_U64,
		/* name = */   "64-bit unsigned integer",
		/* c_name = */ "uint64_t",
		/* length = */ 8,
		/* is_variable_length */ 0
	},
	/* UB_DATATYPE_S64 */
	{
		/* type = */   UB_DATATYPE_S64,
		/* name = */   "64-bit signed integer",
		/* c_name = */ "int64_t",
		/* length = */ 8,
		/* is_variable_length */ 0
	}
};

ub_typeinfo_t ub_datatype_get_info(ub_datatype_t type) {
	return ub_i_datatype_info[type];
}

