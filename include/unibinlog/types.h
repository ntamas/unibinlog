/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_TYPES_H
#define UNIBINLOG_TYPES_H

/**
 * Enum constants for the different checksum types supported by this library.
 */
typedef enum {
	UB_CHKSUM_NONE = 0,           /**< No checksum */
	UB_CHKSUM_SUM,                /**< Sum of bytes modulo 256 */
	UB_CHKSUM_NEGATED_SUM,        /**< Sum of bytes modulo 256, bitwise negated */
	UB_CHKSUM_FLETCHER_16,        /**< 16-bit Fletcher checksum with modulo 255 */
} ub_chksum_type_t;

/**
 * Enum constants for the different block types in \c unibin files.
 */
typedef enum {
	UB_BLOCK_UNKNOWN = 0,         /**< Unknown block */
	UB_BLOCK_COMMENT,             /**< Comment block */
	UB_BLOCK_LOG_HEADER,          /**< Log header block */
	UB_BLOCK_LOG_ENTRY,           /**< Log entry block */
	UB_BLOCK_EVENT,               /**< Event block */
} ub_block_t;

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
	UB_DATATYPE_UNIX_TIMESTAMP        /**< UNIX timestamp */
} ub_datatype_t;

/**
 * Enum constants for the different transformation types in \c unibin files.
 */
typedef enum {
	UB_XFORM_IDENTITY = 0,            /**< Identity transformation */
	UB_XFORM_LINEAR = 1,              /**< Linear transformation */
	UB_XFORM_IF_THEN_ELSE = 2,        /**< If-then-else transformation */
} ub_xform_t;

#endif

