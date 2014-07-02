/* vim:set ts=4 sw=4 sts=4 et: */

#include <unibinlog/log_column.h>
#include <unibinlog/memory.h>
#include <unibinlog/types.h>

ub_error_t ub_log_column_init(ub_log_column_t* column,
		const char* name, ub_datatype_t type) {
	column->name = 0;
	column->type = UB_DATATYPE_UNKNOWN;
	column->xform = UB_XFORM_IDENTITY;
	column->xform_params = 0;

	UB_CHECK(ub_log_column_set_name(column, name));
	UB_CHECK(ub_log_column_set_type(column, type));
	
	return UB_SUCCESS;
}

void ub_log_column_destroy(ub_log_column_t* column) {
	ub_free_unless_null(column->name);
	column->type = UB_DATATYPE_UNKNOWN;
	column->xform = UB_XFORM_IDENTITY;
	ub_free_unless_null(column->xform_params);
}

const char* ub_log_column_get_name(const ub_log_column_t* column) {
	return column->name;
}

size_t ub_log_column_get_length(const ub_log_column_t* column) {
    return ub_log_columns_get_total_length(column, 1);
}

size_t ub_log_columns_get_total_length(const ub_log_column_t* columns,
        size_t num_columns) {
    size_t result = 0;
    ub_typeinfo_t info;

    while (num_columns > 0) {
        info = ub_datatype_get_info(ub_log_column_get_type(columns));
        if (info.is_variable_length) {
            return 0;
        } else {
            result += info.length;
        }
        columns++; num_columns--;
    }

    return result;
}

ub_datatype_t ub_log_column_get_type(const ub_log_column_t* column) {
	return column->type;
}

ub_error_t ub_log_column_set_name(ub_log_column_t* column, const char* name) {
	ub_free_unless_null(column->name);
	if (name != 0) {
		column->name = strdup(name);
		if (column->name == 0) {
			return UB_ENOMEM;
		}
	}
	return UB_SUCCESS;
}

ub_error_t ub_log_column_set_type(ub_log_column_t* column,
		ub_datatype_t type) {
	column->type = type;
	return UB_SUCCESS;
}

ub_error_t ub_log_column_write(const ub_log_column_t* column,
		ub_buffer_location_t* loc) {
	size_t name_length = column->name ? strlen(column->name) : 0;
	size_t bytes_needed = name_length + 3;

	/* Safety check: only the identity transformation is implemented now */
	if (column->xform != UB_XFORM_IDENTITY)
		return UB_EUNIMPLEMENTED;

	/* Ensures that we have space in the buffer to write what we want */
	UB_CHECK(ub_buffer_resize_if_smaller(loc->buffer, loc->index + bytes_needed));

	/* Write the length of the name and the name as well */
	*UB_BUFFER_LOCATION(*loc) = name_length;
	loc->index++;
	if (name_length > 0) {
		ub_buffer_update_from_array(loc, column->name, name_length);
	}

	/* Write the data type */
	*UB_BUFFER_LOCATION(*loc) = column->type;
	loc->index++;

	/* Write the transformation */
	*UB_BUFFER_LOCATION(*loc) = column->xform;
	loc->index++;

	return UB_SUCCESS;
}
