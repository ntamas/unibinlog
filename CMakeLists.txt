cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)

#####################################################################
# General project properties
#####################################################################

project(unibinlog C CXX)
enable_testing()

#####################################################################
# Build options
#####################################################################

option(BUILD_SHARED_LIBS "Build package with shared libraries." OFF)

#####################################################################
# Version information
#####################################################################

#####################################################################
# Tweaking CMake
#####################################################################

#####################################################################
# Find dependencies
#####################################################################

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/etc/cmake;${CMAKE_MODULE_PATH})
# Nothing to find here yet

#####################################################################
# Platform checks
#####################################################################

INCLUDE(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(fmemopen stdio.h HAVE_FMEMOPEN)
CHECK_SYMBOL_EXISTS(funopen stdio.h HAVE_FUNOPEN)

#####################################################################
# Compiler flags for different build configurations
#####################################################################

set(CMAKE_C_FLAGS   "${CMAKE_ARCH_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_ARCH_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_ARCH_FLAGS} -O0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_ARCH_FLAGS} -O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_ARCH_FLAGS} -O3 -DNDEBUG -Wall -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_ARCH_FLAGS} -O3 -DNDEBUG -Wall -s")
set(CMAKE_C_FLAGS_PROFILING "${CMAKE_ARCH_FLAGS} -pg")
set(CMAKE_CXX_FLAGS_PROFILING "${CMAKE_ARCH_FLAGS} -pg")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_BINARY_DIR}/include
					${CMAKE_CURRENT_SOURCE_DIR}/src
					${CMAKE_CURRENT_BINARY_DIR}/src)

#####################################################################
# Process subdirectories
#####################################################################

add_subdirectory(src)

if(HAVE_FMEMOPEN OR HAVE_FUNOPEN)
	add_subdirectory(test)
else(HAVE_FMEMOPEN OR HAVE_FUNOPEN)
	message("Neither fmemopen() nor funopen() is supported on this platform; "
		"tests will not be compiled")
endif(HAVE_FMEMOPEN OR HAVE_FUNOPEN)
