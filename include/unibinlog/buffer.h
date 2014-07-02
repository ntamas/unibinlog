/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_BUFFER_H
#define UNIBINLOG_BUFFER_H

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include <unibinlog/basic_types.h>
#include <unibinlog/chksum.h>
#include <unibinlog/error.h>

/**
 * Structure for a heap-allocated buffer that keeps track of its size.
 */
typedef struct {
    union {
        /** This field can be used to access the bytes of the buffer
         *  as unsigned 8-bit values. */
        uint8_t* bytes;
        /** This field can be used to access the bytes of the buffer
         *  as signed 8-bit values. */
        int8_t* signed_bytes;
    };
    /** Pointer to the end of the buffer */
    uint8_t* end;
    /** Pointer to the end of the \em allocated area of the buffer */
    uint8_t* alloc_end;
    /** Stores whether the memory area is owned by the buffer. This also
     * determines whether the size of the buffer is fixed; if the area is
     * not owned by the buffer, it cannot be grown or shrunk. */
    ub_bool_t owner;
} ub_buffer_t;

/**
 * Lightweight structure that holds a buffer and an index to a location
 * in the buffer.
 */
typedef struct {
    ub_buffer_t* buffer;
    size_t index;
} ub_buffer_location_t;

/**
 * \def UB_BUFFER(buf)
 *
 * Returns a pointer to the data area of the given buffer.
 */
#define UB_BUFFER(buf) ((buf).bytes)

/**
 * \def UB_BUFFER_AS(buf, type)
 *
 * Returns a pointer to the data area of the given buffer, cast to the specified
 * type.
 */
#define UB_BUFFER_AS(buf, type) ((type*)(buf).bytes)

/**
 * \def UB_BUFFER_AS_STRING(buf)
 *
 * Returns a pointer to the data area of the given buffer, cast to a string.
 */
#define UB_BUFFER_AS_STRING(buf) ((char*)(buf).bytes)

/**
 * \def UB_BUFFER_FROM_INDEX_AS(buf, index, type)
 *
 * Returns a pointer to the data area of the given buffer, starting at the
 * indexth byte, cast to the specified type.
 */
#define UB_BUFFER_FROM_INDEX_AS(buf, index, type) ((type*)((buf).bytes + index))

/**
 * \def UB_BUFFER_LOCATION(loc)
 *
 * Returns a pointer to the location pointed to by the given location object.
 */
#define UB_BUFFER_LOCATION(loc) ((loc).buffer->bytes + (loc).index)

/**
 * \def UB_BUFFER_LOCATION_AS(loc, type)
 *
 * Returns a pointer to the location pointed to by the given location object,
 * cast into the given type.
 */
#define UB_BUFFER_LOCATION_AS(loc, type) ((type*)((loc).buffer->bytes + (loc).index))

/**
 * Creates a buffer of the given initial size.
 * 
 * \param  buf     the buffer to initialize
 * \param  size    the size of the buffer
 * \return error code or \c UB_SUCCESS if everything was OK.
 */
ub_error_t ub_buffer_init(ub_buffer_t* buf, size_t size);

/**
 * Creates a buffer by copying another buffer.
 * 
 * \param  dest    the buffer to initialize
 * \param  src     the buffer to copy
 * \return error code or \c UB_SUCCESS if everything was OK.
 */
ub_error_t ub_buffer_copy(ub_buffer_t* dest, const ub_buffer_t* src);

/**
 * Returns a buffer backed by a memory area not owned by the buffer.
 *
 * \param  data    pointer to the start of the memory area
 * \param  size    the size of the buffer
 * \return the newly created buffer.
 */
ub_buffer_t ub_buffer_view(void* data, size_t size);

/**
 * Returns the capacity of the buffer. The capacity of the buffer is the number of
 * bytes that can be stored in the buffer without resizing it.
 */
size_t ub_buffer_capacity(const ub_buffer_t* buf);

/**
 * Fills the buffer with zeros.
 *
 * \param  buf   an initialized buffer
 */
void ub_buffer_clear(ub_buffer_t* buf);

/**
 * Fills the buffer with the given byte.
 * 
 * \param  buf   an initialized buffer
 * \param  byte  the byte
 */
void ub_buffer_fill(ub_buffer_t* buf, uint8_t byte);

/**
 * Returns a buffer location referring to the front of the buffer.
 *
 * \param  buf  an initialized buffer
 * \return a buffer location pointing to the first byte in the buffer
 */
ub_buffer_location_t ub_buffer_front(ub_buffer_t* buf);

/**
 * Writes the contents of the buffer into a file.
 *
 * \param  buf   an initialized buffer
 * \param  f     the file to write the buffer contents into
 */
ub_error_t ub_buffer_fwrite(const ub_buffer_t* buf, FILE* f);

/**
 * Calculates the checksum of the buffer.
 * 
 * \param  buf     an initialized buffer
 * \param  result  pointer to a memory area that will hold the checksum. It
 *                 must be large enough to store the entire checksum, and it
 *                 is the responsibility of the caller to ensure this.
 * \param  chksum_type    the type of checksum to calculate.
 * \param  skip    number of bytes to skip from the end of the buffer.
 *                 This is useful when the checksum is part of the buffer
 *                 itself and we do not want the checksum byte itself to
 *                 be included in the checksum.
 * \return error code or \c UB_SUCCESS if everything was OK.
 */
ub_error_t ub_buffer_get_checksum(const ub_buffer_t* buf, uint8_t* result,
        ub_chksum_type_t chksum_type, size_t skip);

/**
 * Returns a buffer location referring to the byte with the given index in
 * the buffer. Bytes are indexed from zero.
 *
 * \param  buf    an initialized buffer
 * \param  index  the index of the byte to point to
 * \return a buffer location pointing to the given byte in the buffer
 */
ub_buffer_location_t ub_buffer_location(ub_buffer_t* buf, size_t index);

/**
 * Prints the buffer to the given file in a human-readable format, using the
 * given prefix before each line.
 *
 * \param  buf     the buffer to print
 * \param  file    the file to print the buffer to.
 * \param  prefix  the prefix to print before each line.
 */
void ub_buffer_print(const ub_buffer_t* buf, FILE* file, const char* prefix);

/**
 * Prints a slice of the buffer to the given file in a human-readable format,
 * using the given prefix before each line.
 *
 * \param  buf     the buffer to print
 * \param  start   the index of the first byte to print (zero-based)
 * \param  end     the index of the first byte NOT to print (zero-based)
 * \param  file    the file to print the buffer to.
 * \param  prefix  the prefix to print before each line.
 */
void ub_buffer_print_slice(const ub_buffer_t* buf, size_t start, size_t end,
        FILE* file, const char* prefix);

/**
 * Prints the given memory area to the given file in a human-readable format,
 * using the given prefix before each line.
 *
 * This function creates a buffer view on-the-fly using \ref ub_buffer_view
 * and then prints it using \ref ub_buffer_print.
 *
 * \param  data    pointer to the start of the memory area
 * \param  size    the size of the buffer
 * \param  file    the file to print the buffer to.
 * \param  prefix  the prefix to print before each line.
 */
void ub_buffer_print_view(void* data, size_t size, FILE* file, const char* prefix);

/**
 * Resizes the buffer. Note that this method does not actually deallocate any
 * memory that has already been assigned to the buffer in case we need it
 * later. Use \ref ub_buffer_truncate() to truncate the allocated size of the
 * buffer to the size that is actually used.
 *
 * \param  buf        the buffer to resize.
 * \param  new_size   the new size of the buffer in bytes
 */
ub_error_t ub_buffer_resize(ub_buffer_t* buf, size_t new_size);

/**
 * Resizes the buffer if it is smaller than the given size.
 *
 * \param  buf       the buffer to resize.
 * \param  min_size  the minimum size of the buffer in bytes that we need
 */
ub_error_t ub_buffer_resize_if_smaller(ub_buffer_t* buf, size_t min_size);

/**
 * Ensures that the buffer will be able to hold the given number of bytes
 * without having to reallocate its own internal memory segment. Note that
 * this function does \em not resize the buffer, it only updates its capacity
 * if the current capacity is smaller than the requested capacity.
 *
 * Note that no memory will ever be freed by this function. If you want to
 * make the buffer free all the memory that it does not use currently, call
 * \ref ub_buffer_truncate() instead.
 *
 * \param  buf       the buffer to resize.
 * \param  capacity  the new \em minimum capacity of the buffer in bytes
 */
ub_error_t ub_buffer_reserve(ub_buffer_t* buf, size_t capacity);

/**
 * Returns the size of the buffer. The size of the buffer is the number of
 * bytes actually stored in the buffer.
 */
size_t ub_buffer_size(const ub_buffer_t* buf);

/**
 * Truncates the buffer so it does not allocate any extra memory for possible
 * future growth.
 *
 * \param  buf   the buffer to truncate
 * \return \c UB_SUCCESS or error code.
 */
ub_error_t ub_buffer_truncate(ub_buffer_t* buf);

/**
 * Copies the contents of a buffer to another one.
 *
 * The two buffers must have equal size. This is not checked explicitly.
 *
 * \param  dest  the destination buffer
 * \param  src   the source buffer
 */
void ub_buffer_update(ub_buffer_t* dest, const ub_buffer_t* src);

/**
 * Updates the contents of a buffer from an array, starting from a given buffer
 * location.
 *
 * The destination buffer must contain enough space. This is not checked explicitly.
 *
 * \param  dest       the location in the destination buffer where the source
 *                    array will be copied to. It will be advanced to the
 *                    first location in the buffer \em after the written copy
 *                    of the array upon exit
 * \param  src        the source array
 * \param  num_bytes  the number of bytes to copy
 */
void ub_buffer_update_from_array(ub_buffer_location_t* dest, const void* src,
        size_t num_bytes);

/**
 * Updates the contents of a buffer from an array, starting from a given buffer
 * location, growing the buffer if needed.
 *
 * \param  dest       the location in the destination buffer where the source
 *                    array will be copied to. It will be advanced to the
 *                    first location in the buffer \em after the written copy
 *                    of the array upon exit
 * \param  src        the source array
 * \param  num_bytes  the number of bytes to copy
 */
ub_error_t ub_buffer_update_and_grow_from_array(ub_buffer_location_t* dest,
        const void* src, size_t num_bytes);

/**
 * Updates the contents of a buffer from an array.
 *
 * The destination buffer must contain enough space. This is not checked explicitly.
 *
 * \param  dest       the destination buffer
 * \param  src        the source array
 * \param  num_bytes  the number of bytes to copy
 */
void ub_buffer_update_from_array_front(ub_buffer_t* dest, const void* src,
        size_t num_bytes);

/**
 * Updates the contents of a buffer from an array, growing the buffer if
 * needed.
 *
 * \param  dest       the destination buffer
 * \param  src        the source array
 * \param  num_bytes  the number of bytes to copy
 */
ub_error_t ub_buffer_update_and_grow_from_array_front(ub_buffer_t* dest,
        const void* src, size_t num_bytes);

/**
 * Updates the checksum of the buffer, assuming that it is stored in the last
 * N bytes where N depends on the type of the checksum.
 *
 * \param  buf          an initialized buffer
 * \param  chksum_type  the type of checksum to calculate.
 *
 * \return the checksum of the buffer, according to \p chksum_type. The checksum
 *   does not include the last N bytes which is assumed to be the checksum
 *   itself, but includes everything else.
 */
ub_error_t ub_buffer_update_checksum(ub_buffer_t* buf, ub_chksum_type_t chksum_type);

/**
 * Checks the checksum of the buffer.
 *
 * It is assumed that the last N bytes of the buffer is the checksum itself, and
 * that it must be left out of the calculation.
 *
 * \param  buf          an initialized buffer
 * \param  chksum_type  the type of checksum to calculate. No checking is performed
 *                      when this is equal to \c UB_CHKSUM_NONE.
 * \return  \c UB_SUCCESS if the checksum is OK, \c UB_FAILURE otherwise.
 */
ub_error_t ub_buffer_validate_checksum(const ub_buffer_t* buf,
        ub_chksum_type_t chksum_type);

/**
 * Destroys a buffer and frees its associated memory.
 */
void ub_buffer_destroy(ub_buffer_t* buf);

/***************************************************************************/

#endif

