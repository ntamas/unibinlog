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
    memcpy(UB_BUFFER(*dest), UB_BUFFER(*src), n);
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
	return buf->alloc_end - buf->bytes;
}

void ub_buffer_clear(ub_buffer_t* buf) {
    ub_buffer_fill(buf, 0);
}

void ub_buffer_fill(ub_buffer_t* buf, u8 byte) {
    assert(buf->bytes);
    memset(buf->bytes, byte, ub_buffer_size(buf));
}

/*
u8 ub_buffer_get_checksum(const ub_buffer_t* buf,
        ub_checksum_type_t type, size_t skip) {
    const u8 *begin = buf->bytes, *end = begin + buf->size - skip;
    assert(buf->bytes);
    u8 result = 0;

    switch (type) {
        case ub_CHKSUM_NONE:
            break;

        case ub_CHKSUM_SUM:
        case ub_CHKSUM_NEGATED_SUM:
            while (begin != end) {
                result += *begin;
                begin++;
            }
            if (type == ub_CHKSUM_NEGATED_SUM)
                result = ~result;
            break;

        default:
            ub_WARNING("buffer", "unknown checksum type: %d", type);
    }

    return result;
}
*/

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

ub_error_t ub_buffer_resize(ub_buffer_t* buf, size_t new_size) {
    u8* new_bytes;

    assert(buf->bytes);
    assert(buf->owner);

    new_bytes = realloc(buf->bytes, new_size);
    if (new_bytes == 0) {
        buf->end = buf->alloc_end = 0;
        return UB_ENOMEM;
    }

    return ub_i_buffer_own(buf, new_bytes, new_size);
}

size_t ub_buffer_size(const ub_buffer_t* buf) {
	return buf->end - buf->bytes;
}

void ub_buffer_update(ub_buffer_t* dest, const ub_buffer_t* src) {
    assert(ub_buffer_size(dest) == ub_buffer_size(src));
    memcpy(dest->bytes, src->bytes, ub_buffer_size(dest));
}

/*
u8 ub_buffer_update_checksum(ub_buffer_t* buf, ub_checksum_type_t type) {
    u8 chksum;

    assert(buf->bytes);
    assert(buf->size >= 1);

    chksum = ub_buffer_get_checksum(buf, type, 1);
    UB_BUFFER(*buf)[buf->size-1] = chksum;
    return chksum;
}
*/

/*
ub_bool_t ub_buffer_validate_checksum(const ub_buffer_t* buf,
        ub_checksum_type_t type) {
    if (type == UB_CHKSUM_NONE)
        return 0;

    return UB_BUFFER(*buf)[buf->size-1] != ub_buffer_get_checksum(buf, type, 1);
}
*/

