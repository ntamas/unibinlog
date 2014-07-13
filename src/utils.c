/* vim:set ts=4 sw=4 sts=4 et: */

#include <arpa/inet.h>
#include "utils.h"

#ifdef HAVE_UINT64
uint64_t htonll(uint64_t value) {
	static const int endianness_check = 42;

	if (*((uint8_t*)&endianness_check) != endianness_check)
		return value;

	const uint32_t high_part = htonl((uint32_t)(value >> 32));
	const uint32_t low_part = htonl((uint32_t)(value & 0xFFFFFFFFLL));
	return (((uint64_t)low_part) << 32) | high_part;
}
#endif

#ifdef HAVE_IEEE754_FLOATS
float htonf(float value) {
#  ifndef HAVE_FLOAT_BYTES_BIGENDIAN
	uint32_t swapped = htonl(*((uint32_t*)&value));
	return *((float*)&swapped);
#  else
	return value;
#  endif
}

double htonlf(double value) {
#  ifndef HAVE_FLOAT_BYTES_BIGENDIAN
	uint32_t swapped[2];
#    ifndef HAVE_FLOAT_WORDS_BIGENDIAN
	/* little endian words, little endian bytes */
	swapped[1] = htonl(*((uint32_t*)&value));
	swapped[0] = htonl(*(((uint32_t*)&value)+1));
#    else
	/* big endian words, little endian bytes */
	swapped[0] = htonl(*((uint32_t*)&value));
	swapped[1] = htonl(*(((uint32_t*)&value)+1));
#    endif
	return *((double*)swapped);
#  else
#    ifndef HAVE_FLOAT_WORDS_BIGENDIAN
	/* little endian words, big endian bytes */
	uint32_t swapped[2];
	swapped[1] = *((uint32_t*)&value);
	swapped[0] = *(((uint32_t*)&value)+1);
	return *((double*)swapped);
#    else
	/* big endian words, big endian bytes */
	return value;
#    endif
#  endif
}

#endif

