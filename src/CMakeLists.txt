CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set(unibinlog_SRCS
	buffer.c
	buffer_writer.c
	chksum.c
	debug.c
	error.c
	log_column.c
	lowlevel.c
	typeinfo.c
	utils.c
)
	
add_library(unibinlog
	${unibinlog_SRCS}
)
