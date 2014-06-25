/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_ERROR_H
#define UNIBINLOG_ERROR_H

/**
 * Error codes used throughout the library.
 */
typedef enum {
    UB_SUCCESS = 0,                    /**< No error */
    UB_ENOMEM,                         /**< Not enough memory */
    UB_EINVAL,                         /**< Invalid value */
    UB_EOPEN,                          /**< Error while opening an IO channel */
    UB_ECLOSE,                         /**< Error while closing an IO channel */
    UB_EREAD,                          /**< Error while reading from an IO channel */
    UB_EWRITE,                         /**< Error while writing to an IO channel */
    UB_EPARSE,                         /**< Error while parsing some protocol */
    UB_FAILURE,                        /**< Generic failure code */
    UB_EUNSUPPORTED,                   /**< Unsupported operation */
    UB_EUNIMPLEMENTED,                 /**< Unimplemented operation */
    UB_ETOOLONG                        /**< Payload too long */
} ub_error_t;

#ifdef UB_LOG_ERRORS
#  define UB_CHECK(func) { \
    ub_error_t __ub_retval = (func); \
    if (__ub_retval != UB_SUCCESS) { \
        fprintf(stderr, "[CHK][%s:%s:%d] retval=%d\n", __FILE__, __func__, __LINE__, \
                (int)__nc_retval);   \
        return __ub_retval;          \
    }                                \
}
#else
#  define UB_CHECK(func) { \
    ub_error_t __ub_retval = (func); \
    if (__ub_retval != UB_SUCCESS) { \
        return __ub_retval;          \
    }                                \
}
#endif

/**
 * Converts an error code to a human-readable string.
 *
 * \return  a pointer to the string containing the error message. This string
 *          should not be modified under any circumstances.
 */
const char* ub_error_to_string(int code);

#endif

