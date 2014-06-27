/* vim:set ts=4 sw=4 sts=4 et: */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#include <unibinlog/buffer.h>
#include <unibinlog/debug.h>
#include <unibinlog/memory.h>

/**
 * Internal function that states that the given buffer owns a section of
 * memory.
 *
 * \param  buf   the buffer
 * \param  data  start of the section of memory that the buffer will own
 * \param  size  the size of the section of memory that the buffer will own
 */
static ub_error_t ub_i_buffer_own(ub_buffer_t* buf, void* data, size_t size) {
    buf->bytes = data;
    buf->end = buf->alloc_end = data + size;
    buf->owner = 1;
    return UB_SUCCESS;
}

ub_error_t ub_buffer_init(ub_buffer_t* buf, size_t size) {
    u8* bytes = ub_calloc(u8, size);
	if (bytes == 0)
		return UB_ENOMEM;

    return ub_i_buffer_own(buf, bytes, size);
}

ub_error_t ub_buffer_copy(ub_buffer_t* dest, const ub_buffer_t* src) {
    size_t n = ub_buffer_size(src);
    UB_CHECK(ub_buffer_init(dest, n));
    ub_buffer_read_into(dest, 0, UB_BUFFER(*src), n);
    return UB_SUCCESS;
}

ub_buffer_t ub_buffer_view(void* data, size_t size) {
    ub_buffer_t buf;

    buf.bytes = data;
    buf.end = buf.alloc_end = data + size;
    buf.owner = 0;

    return buf;
}

void ub_buffer_destroy(ub_buffer_t* buf) {
	if (buf->owner) {
        ub_free_unless_null(buf->bytes);
    }
}

size_t ub_buffer_capacity(const ub_buffer_t* buf) {
    assert(buf->bytes);
	return buf->alloc_end - buf->bytes;
}

void ub_buffer_clear(ub_buffer_t* buf) {
    ub_buffer_fill(buf, 0);
}

void ub_buffer_fill(ub_buffer_t* buf, u8 byte) {
    assert(buf->bytes);
    memset(buf->bytes, byte, ub_buffer_size(buf));
}

ub_error_t ub_buffer_fwrite(const ub_buffer_t* buf, FILE* f) {
    size_t bytes_to_write, bytes_written;

    assert(buf->bytes);

    bytes_to_write = ub_buffer_size(buf);
    bytes_written = fwrite(buf->bytes, 1, bytes_to_write, f);

    return bytes_written == bytes_to_write ? UB_SUCCESS : UB_EWRITE;
}

ub_error_t ub_buffer_get_checksum(const ub_buffer_t* buf, u8* result,
        ub_chksum_type_t chksum_type, size_t skip) {
    size_t size;
    
    assert(buf->bytes);
    assert(skip <= ub_buffer_size(buf));

    size = ub_buffer_size(buf) - skip;
    return ub_get_chksum_of_array(buf->bytes, size, result, chksum_type);
}

void ub_buffer_print(const ub_buffer_t* buf, FILE* file, const char* prefix) {
    ub_buffer_print_slice(buf, 0, ub_buffer_size(buf), file, prefix);
}

void ub_buffer_print_slice(const ub_buffer_t* buf, size_t start, size_t end,
        FILE* file, const char* prefix) {
    size_t i = 0;
    const unsigned char *p;
    const unsigned char *p2;
    char bytes[17];

    assert(buf->bytes);

    p = (unsigned char*)buf->bytes + start;
    p2 = (unsigned char*)buf->bytes + end;

    for (i = 0; p < p2; p++, i++) {
        if (i % 16 == 0) {
            memset(bytes, 0, 17);
            if (prefix) {
                fprintf(file, "%s", prefix);
            }
        }
        fprintf(file, "%02X ", *p);
        bytes[i % 16] = isprint(*p) ? *p : '.';
        if (i % 16 == 15) {
            fprintf(file, "| %s\n", bytes);
        }
    }
    if (i % 16 != 0) {
        for (; i % 16 != 0; i++) {
            fprintf(file, "   ");
        }
        fprintf(file, "| %s\n", bytes);
    }
}

void ub_buffer_print_view(void* data, size_t size, FILE* file, const char* prefix) {
    ub_buffer_t tmp_buf = ub_buffer_view(data, size);
    ub_buffer_print(&tmp_buf, file, prefix);
}

void ub_buffer_read_into(ub_buffer_t* dest, size_t dest_index,
        const void* src, size_t num_bytes) {
    assert(dest->bytes);
    memcpy(dest->bytes + dest_index, src, num_bytes);
}

ub_error_t ub_buffer_resize(ub_buffer_t* buf, size_t new_size) {
    u8* new_bytes;
    size_t current_size;
    size_t new_allocated_size;

    assert(buf->bytes);
    assert(buf->owner);

    current_size = ub_buffer_size(buf);
    if (new_size <= current_size) {
        /* new size is not larger so we can simply move the 'end' pointer
         * backwards */
        buf->end = buf->bytes + new_size;
        return UB_SUCCESS;
    }

    /* new size is larger so we have to double the buffer size until we exceed
     * the new size */
    new_allocated_size = current_size;
    while (new_allocated_size < new_size) {
        current_size = new_allocated_size;
        new_allocated_size <<= 1;

        /* protect against overflows */
        if (new_allocated_size < current_size) {
            new_allocated_size = new_size;
        }
    }

    /* reallocate memory */
    new_bytes = realloc(buf->bytes, new_allocated_size);
    if (new_bytes == 0) {
        /* okay, maybe new_allocated_size is too large but we could still
         * fit new_size bytes */
        new_allocated_size = new_size;
        new_bytes = realloc(buf->bytes, new_allocated_size);
        if(new_bytes == 0) {
            /* okay, we give up */
            buf->end = buf->alloc_end = 0;
            return UB_ENOMEM;
        }
    }

    UB_CHECK(ub_i_buffer_own(buf, new_bytes, new_allocated_size));
    buf->end = buf->bytes + new_size;
    return UB_SUCCESS;
}

size_t ub_buffer_size(const ub_buffer_t* buf) {
    assert(buf->bytes);
	return buf->end - buf->bytes;
}

ub_error_t ub_buffer_truncate(ub_buffer_t* buf) {
    size_t size, capacity;
    u8* new_bytes;

    assert(buf->bytes);

    size = ub_buffer_size(buf);
    capacity = ub_buffer_capacity(buf);

    if (size < capacity) {
        new_bytes = realloc(buf->bytes, size);
        if (new_bytes == 0) {
            buf->end = buf->alloc_end = 0;
            return UB_ENOMEM;
        }
    } else if (size == capacity) {
        /* nothing to do */
        return UB_SUCCESS;
    } else {
        /* should not ever get here */
        return UB_FAILURE;
    }

    UB_CHECK(ub_i_buffer_own(buf, new_bytes, size));
    return UB_SUCCESS;
}

void ub_buffer_update(ub_buffer_t* dest, const ub_buffer_t* src) {
    assert(ub_buffer_size(dest) == ub_buffer_size(src));
    ub_buffer_read_into(dest, 0, UB_BUFFER(*src), ub_buffer_size(src));
}

u8 ub_buffer_update_checksum(ub_buffer_t* buf, ub_chksum_type_t chksum_type) {
    size_t chksum_size = ub_chksum_size(chksum_type);
    size_t buf_size = ub_buffer_size(buf);

    if (buf_size < chksum_size)
        return UB_EINVAL;

    return ub_buffer_get_checksum(buf, &buf->bytes[buf_size-chksum_size],
                chksum_type, chksum_size);
}

ub_error_t ub_buffer_validate_checksum(const ub_buffer_t* buf,
        ub_chksum_type_t chksum_type) {
    size_t chksum_size = ub_chksum_size(chksum_type);
    size_t buf_size;
    u8* chksum;
    
    if (chksum_size == 0)
        return UB_SUCCESS;

    buf_size = ub_buffer_size(buf);
    if (buf_size < chksum_size)
        return UB_FAILURE;

    chksum = ub_calloc(u8, chksum_size);
    UB_CHECK(ub_buffer_get_checksum(buf, chksum, chksum_type, chksum_size));
    return memcmp(chksum, &buf->bytes[buf_size-chksum_size], chksum_size) ?
        UB_FAILURE : UB_SUCCESS;
}

