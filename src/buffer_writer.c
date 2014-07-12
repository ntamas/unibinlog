/* vim:set ts=4 sw=4 sts=4 et: */

#include <assert.h>
#include <string.h>
#include <arpa/inet.h>

#include <unibinlog/buffer.h>
#include "utils.h"

ub_error_t ub_buffer_writer_init(ub_buffer_writer_t* writer, ub_buffer_t* buffer,
        size_t index, ub_bool_t grow) {
    writer->loc = ub_buffer_location(buffer, index);
    writer->grow = grow;

    if (index > ub_buffer_size(buffer) && !grow)
        return UB_EINVAL;
    else
        return UB_SUCCESS;
}

void ub_buffer_writer_destroy(ub_buffer_writer_t* writer) {
    writer->loc.buffer = 0;
    writer->loc.index = 0;
    writer->grow = 0;
}

ub_error_t ub_buffer_writer_seek(ub_buffer_writer_t* writer, long offset,
        int whence) {
    ssize_t ref;

    assert(writer->loc.buffer != 0);
    switch (whence) {
        case SEEK_SET:
            ref = 0; break;
        case SEEK_CUR:
            ref = writer->loc.index; break;
        case SEEK_END:
            ref = ub_buffer_size(writer->loc.buffer); break;
        default:
            return UB_EINVAL;
    }

    ref += offset;
    if (ref < 0)
        return UB_EINVAL;
    
    if (ref > ub_buffer_size(writer->loc.buffer)) {
        if (!writer->grow) {
            return UB_EINVAL;
        } else {
            UB_CHECK(ub_buffer_resize(writer->loc.buffer, ref-1));
        }
    }

    writer->loc.index = ref;
    return UB_SUCCESS;
}

long ub_buffer_writer_tell(const ub_buffer_writer_t* writer) {
    assert(writer->loc.buffer != 0);
    return writer->loc.index;
}

static ub_error_t ub_i_buffer_writer_write_raw_bytes(ub_buffer_writer_t* writer,
        const void* bytes, size_t num_bytes) {
    assert(writer->loc.buffer != 0);
    if (writer->grow) {
        UB_CHECK(ub_buffer_update_and_grow_from_array(&writer->loc, bytes, num_bytes));
    } else {
        ub_buffer_update_from_array(&writer->loc, bytes, num_bytes);
    }
    return UB_SUCCESS;
}

ub_error_t ub_buffer_writer_write_u8(ub_buffer_writer_t* writer, uint8_t value) {
    return ub_i_buffer_writer_write_raw_bytes(writer, &value, sizeof(value));
}

ub_error_t ub_buffer_writer_write_s8(ub_buffer_writer_t* writer, int8_t value) {
    return ub_i_buffer_writer_write_raw_bytes(writer, &value, sizeof(value));
}

ub_error_t ub_buffer_writer_write_u16(ub_buffer_writer_t* writer, uint16_t value) {
    value = htons(value);
    return ub_i_buffer_writer_write_raw_bytes(writer, &value, sizeof(value));
}

ub_error_t ub_buffer_writer_write_s16(ub_buffer_writer_t* writer, int16_t value) {
    uint16_t unsigned_value = htons(*((uint16_t*)&value));
    return ub_i_buffer_writer_write_raw_bytes(writer, &unsigned_value,
            sizeof(unsigned_value));
}

ub_error_t ub_buffer_writer_write_u32(ub_buffer_writer_t* writer, uint32_t value) {
    value = htonl(value);
    return ub_i_buffer_writer_write_raw_bytes(writer, &value, sizeof(value));
}

ub_error_t ub_buffer_writer_write_s32(ub_buffer_writer_t* writer, int32_t value) {
    uint32_t unsigned_value = htonl(*((uint32_t*)&value));
    return ub_i_buffer_writer_write_raw_bytes(writer, &unsigned_value,
            sizeof(unsigned_value));
}

#ifdef HAVE_UINT64
ub_error_t ub_buffer_writer_write_u64(ub_buffer_writer_t* writer, uint64_t value) {
    value = htonll(value);
    return ub_i_buffer_writer_write_raw_bytes(writer, &value, sizeof(value));
}
#endif

#ifdef HAVE_INT64
ub_error_t ub_buffer_writer_write_s64(ub_buffer_writer_t* writer, int64_t value) {
    uint64_t unsigned_value = htonll(*((uint64_t*)&value));
    return ub_i_buffer_writer_write_raw_bytes(writer, &unsigned_value,
            sizeof(unsigned_value));
}
#endif

ub_error_t ub_buffer_writer_write_float(ub_buffer_writer_t* writer, float value) {
    return ub_i_buffer_writer_write_raw_bytes(writer, &value, sizeof(value));
}

ub_error_t ub_buffer_writer_write_double(ub_buffer_writer_t* writer, double value) {
    return ub_i_buffer_writer_write_raw_bytes(writer, &value, sizeof(value));
}

ub_error_t ub_buffer_writer_write_string(ub_buffer_writer_t* writer, const char* str) {
    return ub_i_buffer_writer_write_raw_bytes(writer, str, strlen(str)+1);
}

ub_error_t ub_buffer_writer_write_short_blob(ub_buffer_writer_t* writer, void* bytes,
        size_t num_bytes) {
    if (num_bytes > 255)
        return UB_ETOOLONG;

    UB_CHECK(ub_buffer_writer_write_u8(writer, num_bytes));
    return ub_i_buffer_writer_write_raw_bytes(writer, bytes, num_bytes);
}

ub_error_t ub_buffer_writer_write_blob(ub_buffer_writer_t* writer, void* bytes,
        size_t num_bytes) {
    if (num_bytes > 65535)
        return UB_ETOOLONG;

    UB_CHECK(ub_buffer_writer_write_u16(writer, num_bytes));
    return ub_i_buffer_writer_write_raw_bytes(writer, bytes, num_bytes);
}

ub_error_t ub_buffer_writer_write_timestamp(ub_buffer_writer_t* writer, time_t value) {
#ifdef HAVE_UINT64
    uint64_t value_as_uint64 = (uint64_t)value;
    return ub_buffer_writer_write_u64(writer, value_as_uint64);
#else
    long long int value_as_long_long = (long long int)value;
    UB_CHECK(ub_buffer_writer_write_u32(writer, value_as_long_long >> 32));
    return ub_buffer_writer_write_u32(writer, value_as_long_long & 0xFFFFFFFFLL);
#endif
}

ub_error_t ub_buffer_writer_write_timeval(ub_buffer_writer_t* writer, struct timeval time) {
    uint32_t values[2];

    values[0] = (uint32_t)time.tv_sec;
    values[1] = (uint32_t)time.tv_usec;
    values[0] = htonl(values[0]);
    values[1] = htonl(values[1]);

    return ub_i_buffer_writer_write_raw_bytes(writer, values, sizeof(values));
}

