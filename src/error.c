/* vim:set ts=4 sw=4 sts=4 et: */

#include <unibinlog/error.h>

static char* ub_i_error_messages[] = {
    "No error",                                     /* UB_SUCCESS */
    "Not enough memory",                            /* UB_ENOMEM */
    "Invalid value",                                /* UB_EINVAL */
    "Error while opening channel",                  /* UB_EOPEN */
    "Error while closing channel",                  /* UB_ECLOSE */
    "Error while reading from input channel",       /* UB_EREAD */
    "Error while writing to an output channel",     /* UB_EWRITE */
    "Parse error",                                  /* UB_EPARSE */
    "Unspecified failure",                          /* UB_FAILURE */
    "Unsupported operation",                        /* UB_EUNSUPPORTED */
    "Unimplemented operation",                      /* UB_EUNIMPLEMENTED */
    "Payload too long",                             /* UB_ETOOLONG */
};

const char* ub_error_to_string(int code) {
    if (code >= 0 &&
            code < sizeof(ub_i_error_messages) / sizeof(ub_i_error_messages[0]))
        return ub_i_error_messages[code];
    return ub_i_error_messages[UB_FAILURE];
}


