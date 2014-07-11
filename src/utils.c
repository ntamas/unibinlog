/* vim:set ts=4 sw=4 sts=4 et: */

#include <arpa/inet.h>
#include "utils.h"

uint64_t htonll(uint64_t value) {
	static const int endianness_check = 42;

	if (*((uint8_t*)&endianness_check) != endianness_check)
		return value;

	const uint32_t high_part = htonl((uint32_t)(value >> 32));
	const uint32_t low_part = htonl((uint32_t)(value & 0xFFFFFFFFLL));
	return (((uint64_t)low_part) << 32) | high_part;
}

