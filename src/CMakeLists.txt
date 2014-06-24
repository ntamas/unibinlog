CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set(unibinlog_SRCS
	buffer.c
	chksum.c
	debug.c
	error.c
	fmemopen.c
	lowlevel.c
)
	
add_library(unibinlog
	${unibinlog_SRCS}
)
