/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_I_FMEMOPEN_H
#define UNIBINLOG_I_FMEMOPEN_H

#include "config.h"

#ifdef HAVE_FMEMOPEN
#  include <stdio.h>     /* stdio.h includes fmemopen if it is available */
#else

/**
 * Opens the given memory segment as a file.
 *
 * \param  buf   pointer to the start of the memory segment
 * \param  size  the size of the memory segment
 * \param  mode  the mode to open the file with
 * \return the memory segment as a file stream
 */
FILE *fmemopen(void* buf, size_t size, const char* mode);

#endif

#endif

