/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_TYPES_H
#define UNIBINLOG_TYPES_H

#include <stdlib.h>
#include <unibinlog/basic_types.h>

/**
 * Enum constants for the different block types in \c unibin files.
 */
typedef enum {
	UB_BLOCK_UNKNOWN = 0,         /**< Unknown block */
	UB_BLOCK_COMMENT,             /**< Comment block */
	UB_BLOCK_LOG_HEADER,          /**< Log header block */
	UB_BLOCK_LOG_ENTRY,           /**< Log entry block */
	UB_BLOCK_EVENT,               /**< Event block */
} ub_block_type_t;

/**
 * Enum constants for the different data types in \c unibin files.
 */
typedef enum {
	UB_DATATYPE_UNKNOWN = 0,          /**< Unknown data type */
	UB_DATATYPE_BOOLEAN,              /**< Boolean, i.e. unsigned short char */
	UB_DATATYPE_U8,                   /**< Unsigned 8-bit integer */
	UB_DATATYPE_S8,                   /**< Signed 8-bit integer */
	UB_DATATYPE_U16,                  /**< Unsigned 16-bit integer */
	UB_DATATYPE_S16,                  /**< Signed 16-bit integer */
	UB_DATATYPE_U32,                  /**< Unsigned 32-bit integer */
	UB_DATATYPE_S32,                  /**< Signed 32-bit integer */
	UB_DATATYPE_FLOAT,                /**< IEEE-754 single-precision float */
	UB_DATATYPE_DOUBLE,               /**< IEEE-754 double-precision float */
	UB_DATATYPE_CHAR,                 /**< Character */
	UB_DATATYPE_STRING,               /**< Null-terminated C string */
	UB_DATATYPE_SHORT_BLOB,           /**< Length (1 byte) + binary data */
	UB_DATATYPE_BLOB,                 /**< Length (2 bytes) + binary data */
	UB_DATATYPE_UNIX_TIMESTAMP,       /**< UNIX timestamp stored as milliseconds on 32 bits */
    UB_DATATYPE_TIMEVAL,              /**< UNIX timestamp stored as seconds and microseconds on 64 bits */
    UB_MAX_DATATYPE                   /**< Not a real type; useful for enumerating all type constants */
} ub_datatype_t;

/**
 * Enum constants for the different transformation types in \c unibin files.
 */
typedef enum {
	UB_XFORM_IDENTITY = 0,            /**< Identity transformation */
	UB_XFORM_LINEAR = 1,              /**< Linear transformation */
	UB_XFORM_IF_THEN_ELSE = 2,        /**< If-then-else transformation */
} ub_xform_type_t;

/**
 * Structure encoding basic information about the various \c unibin data types.
 *
 * \see ub_datatype_t
 */
typedef struct {
    ub_datatype_t type;            /**< Type code in \c unibin files */
    const char* name;              /**< Human-readable name of the type */
    const char* c_name;            /**< Name of the corresponding C type; 0 if there is no corresponding type */
    size_t length;   /**< Length of the data type in bytes; 0 means variadic or unknown type */
    ub_bool_t is_variable_length;  /**< Whether the type is variadic */
} ub_typeinfo_t;

/**
 * Returns a structure containing basic type information about a given
 * \c unibin data type.
 *
 * \param  type  the data type
 * \return a read-only structure with the requested type information
 */
ub_typeinfo_t ub_datatype_get_info(ub_datatype_t type);

#endif

