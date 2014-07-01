/* vim:set ts=4 sw=4 sts=4 et: */

#include "config.h"

#ifndef HAVE_FMEMOPEN

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *buffer;                /**< The buffer that backs the in-memory file */
    size_t size;                 /**< The size of the in-memory file */
    size_t position;             /**< The current write/read position of the file */
    unsigned short int owner;    /**< Whether the buffer must be deallocated when the file is closed */
    unsigned short int write_null_at_end;
} fmem_t;

/**
 * Read operation handler for the \c fmemopen implementation.
 *
 * \param  handler  a structure of type \c fmem_t
 * \param  buffer   the buffer to read into
 * \param  size     the number of bytes to read
 */
static int fmemopen_read_callback(void *handler, char *buffer, int size) {
    fmem_t *fmem = (fmem_t*)handler;
    size_t num_bytes_left = fmem->size - fmem->position;

    if (size > num_bytes_left) {
		size = num_bytes_left;
	}

	memcpy(buffer, fmem->buffer + fmem->position, size);
	fmem->position += size;

    return size;
}

/**
 * Write operation handler for the \c fmemopen implementation.
 *
 * \param  handler  a structure of type \c fmem_t
 * \param  buffer   the buffer to copy bytes from
 * \param  size     the number of bytes to write into our buffer
 */
static int fmemopen_write_callback(void *handler, const char *buffer, int size) {
    fmem_t *fmem = (fmem_t*)handler;
    size_t num_bytes_left = fmem->size - fmem->position;

    if (size > num_bytes_left) {
		size = num_bytes_left;
	}

	memcpy(fmem->buffer + fmem->position, buffer, size);
	fmem->position += size;

    return size;
}

/**
 * Seek operation handler for the \c fmemopen implementation.
 *
 * \param  handler  a structure of type \c fmem_t
 * \param  offset   the offset to adjust the location with
 * \param  whence   the reference point of the offset
 */
static fpos_t fmemopen_seek_callback(void *handler, fpos_t offset, int whence) {
    fpos_t new_position;
    fmem_t *fmem = handler;

    switch(whence) {
        case SEEK_SET:
			new_position = offset;
			break;

        case SEEK_CUR:
			new_position = fmem->position + offset;
			break;

        case SEEK_END:
			new_position = fmem->size + offset;
			break;

        default:
			return -1;
    }

	if (new_position < 0 || new_position > fmem->size)
		return -1;

    fmem->position = (size_t) new_position;
    return (fpos_t) new_position;
}

/**
 * Close operation handler for the \c fmemopen implementation.
 *
 * \param  handler  a structure of type \c fmem_t
 */
static int fmemopen_close_callback(void *handler) {
    fmem_t *fmem = handler;

    if (fmem->owner) {
        free(fmem->buffer);
    } else if (fmem->write_null_at_end) {
        if (fmem->position < fmem->size) {
            fmem->buffer[fmem->position++] = '\0';
        }
    }

    free(fmem);

    return 0;
}

FILE *fmemopen(void *buf, size_t size, const char *mode) {
    fmem_t* mem;
    
    unsigned short int read_allowed, write_allowed, seek_allowed;
    unsigned short int clear, seek_to_end, write_null_at_end;
    size_t i;
    size_t mode_length;

    mode_length = strnlen(mode, 3);

    if (mode_length == 0)
        return 0;

    read_allowed = write_allowed = seek_allowed = 1;
    seek_allowed = (mode[0] != 'a');
    read_allowed = (mode[0] == 'r' || mode[mode_length-1] == '+');
    write_allowed = (mode[0] == 'w' || mode[0] == 'a' || mode[mode_length-1] == '+');
    clear = (mode[0] == 'w');
    seek_to_end  = (mode[0] == 'a');
    write_null_at_end = (mode[1] != 'b');

    mem = (fmem_t*)calloc(1, sizeof(fmem_t));

    if (buf != 0) {
        mem->buffer = buf;
        if (clear) {
            memset(buf, 0, size);
        } else if (seek_to_end) {
            for (i = 0; i < size; i++) {
                if (mem->buffer[i] == '\0')
                    break;
            }
            mem->position = i;
        }
        mem->owner = 0;
    } else {
        mem->buffer = (char*)calloc(1, size);
        mem->owner = 1;
    }

    mem->size = size;
    mem->write_null_at_end = write_null_at_end;

    return funopen(mem,
            read_allowed ? fmemopen_read_callback : 0,
            write_allowed ? fmemopen_write_callback : 0,
            seek_allowed ? fmemopen_seek_callback : 0,
            fmemopen_close_callback
    );
}

#endif
