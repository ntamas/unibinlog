/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_LOG_COLUMN_H
#define UNIBINLOG_LOG_COLUMN_H

#include <unibinlog/buffer.h>
#include <unibinlog/error.h>
#include <unibinlog/types.h>

/**
 * Typedef that represents a log column in \c unibin files.
 */
typedef struct {
    char* name;                 /**< Name of the column */
    ub_datatype_t type;         /**< Type of the column */
    ub_xform_type_t xform;      /**< Transformation type of the column */
    void* xform_params;         /**< Parameters of the transformation */
} ub_log_column_t;

/**
 * Initializes an \c ub_log_column_t structure.
 *
 * \param  column  the structure to initialize
 * \param  name    the name of the column; null if there is no name yet
 * \param  type    the type of the column; UB_DATATYPE_UNKNOWN if it is not
 *                 known yet
 */
ub_error_t ub_log_column_init(ub_log_column_t* column,
		const char* name, ub_datatype_t type);

/**
 * Destroys an \c ub_log_column_t structure and releases all memory that
 * it allocated.
 *
 * \param  column  the structure to destroy
 */
void ub_log_column_destroy(ub_log_column_t* column);

/**
 * Returns a pointer to the name of the column. The name should not be modified
 * directly; use \ref ub_log_column_set_name instead.
 * 
 * \param  column  the column
 * \return a pointer to the name of the column
 */
const char* ub_log_column_get_name(const ub_log_column_t* column);

/**
 * Returns the type of the column.
 * 
 * \param  column  the column
 * \return the type of the column
 */
ub_datatype_t ub_log_column_get_type(const ub_log_column_t* column);

/**
 * Sets the name of the column to a new value.
 * 
 * \param  column  the column
 * \param  name    the name of the column; null if there is no name yet
 */
ub_error_t ub_log_column_set_name(ub_log_column_t* column, const char* name);

/**
 * Sets the type of the column to a new value.
 *
 * \param  column  the column
 * \param  type    the new type; \c UB_DATATYPE_UNKNOWN if there is no type
 */
ub_error_t ub_log_column_set_type(ub_log_column_t* column, ub_datatype_t type);

/**
 * Serializes the log column description into the given buffer in \c unibin format.
 * The buffer will be resized accordingly if needed.
 * 
 * \param  column  the column
 * \param  loc     the location in the buffer to write into
 */
ub_error_t ub_log_column_write(const ub_log_column_t* column,
		ub_buffer_location_t* loc);

#endif

