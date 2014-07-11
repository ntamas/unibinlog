#include <string.h>
#include <unibinlog/buffer.h>
#include "common.c"

TEST_CASE(seek_nongrowing) {
	ub_buffer_t buffer;
	ub_buffer_writer_t writer;

	ub_buffer_init(&buffer, 32);

	ub_buffer_writer_init(&writer, &buffer, 5, /* grow = */ 0);
	if (ub_buffer_writer_tell(&writer) != 5)
		return 1;

	ub_buffer_writer_seek(&writer, 2, SEEK_SET);
	if (ub_buffer_writer_tell(&writer) != 2)
		return 2;

	ub_buffer_writer_seek(&writer, 7, SEEK_CUR);
	if (ub_buffer_writer_tell(&writer) != 9)
		return 3;

	ub_buffer_writer_seek(&writer, -2, SEEK_END);
	if (ub_buffer_writer_tell(&writer) != 30)
		return 4;

	ub_buffer_writer_seek(&writer, 0, SEEK_END);
	if (ub_buffer_writer_tell(&writer) != 32)
		return 5;
	ub_buffer_writer_seek(&writer, -32, SEEK_END);
	if (ub_buffer_writer_tell(&writer) != 0)
		return 6;

	if (ub_buffer_writer_seek(&writer, -2, SEEK_SET) != UB_EINVAL)
		return 7;
	if (ub_buffer_writer_seek(&writer, 35, SEEK_SET) != UB_EINVAL)
		return 8;

	ub_buffer_writer_seek(&writer, 8, SEEK_SET);
	ub_buffer_writer_seek(&writer, -8, SEEK_CUR);
	if (ub_buffer_writer_tell(&writer) != 0)
		return 9;

	ub_buffer_writer_seek(&writer, 8, SEEK_SET);
	if (ub_buffer_writer_seek(&writer, -10, SEEK_CUR) != UB_EINVAL)
		return 10;
	if (ub_buffer_writer_seek(&writer, 35, SEEK_CUR) != UB_EINVAL)
		return 11;
	ub_buffer_writer_seek(&writer, 24, SEEK_CUR);
	if (ub_buffer_writer_tell(&writer) != 32)
		return 12;

	if (ub_buffer_writer_seek(&writer, 1, SEEK_END) != UB_EINVAL)
		return 13;
	if (ub_buffer_writer_seek(&writer, -33, SEEK_END) != UB_EINVAL)
		return 14;

	ub_buffer_writer_destroy(&writer);
	ub_buffer_destroy(&buffer);
	return 0;
}

TEST_CASE(seek_growing) {
	ub_buffer_t buffer;
	ub_buffer_writer_t writer;

	ub_buffer_init(&buffer, 32);

	ub_buffer_writer_init(&writer, &buffer, 5, /* grow = */ 1);
	if (ub_buffer_writer_tell(&writer) != 5 || ub_buffer_size(&buffer) != 32)
		return 1;

	ub_buffer_writer_seek(&writer, 2, SEEK_SET);
	if (ub_buffer_writer_tell(&writer) != 2 || ub_buffer_size(&buffer) != 32)
		return 2;
	ub_buffer_writer_seek(&writer, 34, SEEK_SET);
	if (ub_buffer_writer_tell(&writer) != 34 || ub_buffer_size(&buffer) != 33)
		return 3;

	ub_buffer_writer_seek(&writer, 7, SEEK_CUR);
	if (ub_buffer_writer_tell(&writer) != 41 || ub_buffer_size(&buffer) != 40)
		return 4;

	ub_buffer_writer_seek(&writer, -2, SEEK_END);
	if (ub_buffer_writer_tell(&writer) != 38)
		return 5;

	ub_buffer_writer_seek(&writer, 0, SEEK_END);
	if (ub_buffer_writer_tell(&writer) != 40)
		return 5;
	ub_buffer_writer_seek(&writer, -40, SEEK_END);
	if (ub_buffer_writer_tell(&writer) != 0)
		return 6;

	if (ub_buffer_writer_seek(&writer, -2, SEEK_SET) != UB_EINVAL)
		return 7;
	ub_buffer_writer_seek(&writer, 45, SEEK_SET);
	if (ub_buffer_writer_tell(&writer) != 45 || ub_buffer_size(&buffer) != 44)
		return 8;

	ub_buffer_writer_seek(&writer, 8, SEEK_SET);
	ub_buffer_writer_seek(&writer, -8, SEEK_CUR);
	if (ub_buffer_writer_tell(&writer) != 0)
		return 9;

	ub_buffer_writer_seek(&writer, 8, SEEK_SET);
	if (ub_buffer_writer_seek(&writer, -10, SEEK_CUR) != UB_EINVAL)
		return 10;

	if (ub_buffer_writer_seek(&writer, -53, SEEK_END) != UB_EINVAL)
		return 11;

	ub_buffer_writer_destroy(&writer);
	ub_buffer_destroy(&buffer);
	return 0;
}

int test_write_helper(ub_bool_t grow) {
	ub_buffer_t buffer;
	ub_buffer_writer_t writer;
	time_t time;
	struct timeval tv;

	ub_buffer_init(&buffer, grow ? 0 : 64);
	ub_buffer_writer_init(&writer, &buffer, 0, grow);
	ub_buffer_writer_seek(&writer, 5, SEEK_SET);

	ub_buffer_writer_write_u8(&writer, 42);
	if (UB_BUFFER(buffer)[5] != 42 || (grow && ub_buffer_size(&buffer) != 6))
		return 1;

	ub_buffer_writer_write_s8(&writer, 217);
	if (UB_BUFFER(buffer)[6] != 217 || (grow && ub_buffer_size(&buffer) != 7))
		return 2;

	ub_buffer_writer_write_u16(&writer, 0xbeef);
	if (UB_BUFFER(buffer)[7] != 0xbe || UB_BUFFER(buffer)[8] != 0xef ||
			(grow && ub_buffer_size(&buffer) != 9))
		return 3;

	ub_buffer_writer_write_s16(&writer, 0x4284);
	if (UB_BUFFER(buffer)[9] != 0x42 || UB_BUFFER(buffer)[10] != 0x84 ||
			(grow && ub_buffer_size(&buffer) != 11))
		return 4;

	ub_buffer_writer_write_u32(&writer, 0xdeadbeef);
	if (UB_BUFFER(buffer)[11] != 0xde || UB_BUFFER(buffer)[12] != 0xad ||
	        UB_BUFFER(buffer)[13] != 0xbe || UB_BUFFER(buffer)[14] != 0xef ||
			(grow && ub_buffer_size(&buffer) != 15))
		return 5;

	ub_buffer_writer_write_s32(&writer, 0x0badcafe);
	if (UB_BUFFER(buffer)[15] != 0x0b || UB_BUFFER(buffer)[16] != 0xad ||
	        UB_BUFFER(buffer)[17] != 0xca || UB_BUFFER(buffer)[18] != 0xfe ||
			(grow && ub_buffer_size(&buffer) != 19))
		return 6;

	ub_buffer_writer_write_string(&writer, "Bad coffee!");
	if (strcmp(UB_BUFFER_FROM_INDEX_AS_STRING(buffer, 19), "Bad coffee!") ||
			(grow && ub_buffer_size(&buffer) != 31))
		return 7;

	time = 0x123456789ABCDEF;
	ub_buffer_writer_write_timestamp(&writer, time);
	if (UB_BUFFER(buffer)[31] != 0x01 || UB_BUFFER(buffer)[32] != 0x23 ||
	        UB_BUFFER(buffer)[33] != 0x45 || UB_BUFFER(buffer)[34] != 0x67 ||
	        UB_BUFFER(buffer)[35] != 0x89 || UB_BUFFER(buffer)[36] != 0xAB ||
	        UB_BUFFER(buffer)[37] != 0xCD || UB_BUFFER(buffer)[38] != 0xEF ||
			(grow && ub_buffer_size(&buffer) != 39))
		return 8;

	tv.tv_sec  = 0x01234567;
	tv.tv_usec = 0x87654321;
	ub_buffer_writer_write_timeval(&writer, tv);
	if (UB_BUFFER(buffer)[39] != 0x01 || UB_BUFFER(buffer)[40] != 0x23 ||
	        UB_BUFFER(buffer)[41] != 0x45 || UB_BUFFER(buffer)[42] != 0x67 ||
	        UB_BUFFER(buffer)[43] != 0x87 || UB_BUFFER(buffer)[44] != 0x65 ||
	        UB_BUFFER(buffer)[45] != 0x43 || UB_BUFFER(buffer)[46] != 0x21 ||
			(grow && ub_buffer_size(&buffer) != 47))
		return 9;

	ub_buffer_writer_write_u64(&writer, 0xdeadbeef0badcafe);
	if (UB_BUFFER(buffer)[47] != 0xde || UB_BUFFER(buffer)[48] != 0xad ||
	        UB_BUFFER(buffer)[49] != 0xbe || UB_BUFFER(buffer)[50] != 0xef ||
	        UB_BUFFER(buffer)[51] != 0x0b || UB_BUFFER(buffer)[52] != 0xad ||
	        UB_BUFFER(buffer)[53] != 0xca || UB_BUFFER(buffer)[54] != 0xfe ||
			(grow && ub_buffer_size(&buffer) != 55))
		return 10;

	ub_buffer_writer_write_s64(&writer, 0x0badcafedeadbeef);
	if (UB_BUFFER(buffer)[55] != 0x0b || UB_BUFFER(buffer)[56] != 0xad ||
	        UB_BUFFER(buffer)[57] != 0xca || UB_BUFFER(buffer)[58] != 0xfe ||
	        UB_BUFFER(buffer)[59] != 0xde || UB_BUFFER(buffer)[60] != 0xad ||
	        UB_BUFFER(buffer)[61] != 0xbe || UB_BUFFER(buffer)[62] != 0xef ||
			(grow && ub_buffer_size(&buffer) != 63))
		return 11;

	ub_buffer_writer_destroy(&writer);
	ub_buffer_destroy(&buffer);

	return 0;
}

int test_write_blob_helper(ub_bool_t grow) {
	ub_buffer_t buffer;
	ub_buffer_writer_t writer;
	char bytes[300];
	int i, n = sizeof(bytes);

	for (i = 0; i < n; i++) {
		bytes[i] = 'A' + (i % 10);
	}

	ub_buffer_init(&buffer, grow ? 0 : n + 100);
	ub_buffer_writer_init(&writer, &buffer, 0, grow);

	ub_buffer_writer_write_short_blob(&writer, bytes, 150);
	if (UB_BUFFER(buffer)[0] != 150)
		return 1;
	if (memcmp(UB_BUFFER(buffer)+1, bytes, 150))
		return 2;
	if (grow && ub_buffer_size(&buffer) != 151)
		return 3;

	if (ub_buffer_writer_write_short_blob(&writer, bytes, 300) != UB_ETOOLONG)
		return 4;

	ub_buffer_writer_seek(&writer, 0, SEEK_SET);

	ub_buffer_writer_write_blob(&writer, bytes, 300);
	if (UB_BUFFER(buffer)[0] != 1)
		return 5;
	if (UB_BUFFER(buffer)[1] != 44)
		return 6;
	if (memcmp(UB_BUFFER(buffer)+2, bytes, 300))
		return 7;
	if (grow && ub_buffer_size(&buffer) != 302)
		return 8;

	ub_buffer_writer_destroy(&writer);
	ub_buffer_destroy(&buffer);

	return 0;
}

TEST_CASE(write_nongrowing) {
	return test_write_helper(0);
}

TEST_CASE(write_growing) {
	return test_write_helper(1);
}

TEST_CASE(write_blob_nongrowing) {
	return test_write_blob_helper(0);
}

TEST_CASE(write_blob_growing) {
	return test_write_blob_helper(1);
}

START_OF_TESTS;
RUN_TEST_CASE(seek_nongrowing);
RUN_TEST_CASE(seek_growing);
RUN_TEST_CASE(write_nongrowing);
RUN_TEST_CASE(write_growing);
RUN_TEST_CASE(write_blob_nongrowing);
RUN_TEST_CASE(write_blob_growing);
NO_MORE_TEST_CASES;
