cmake_minimum_required(VERSION 3.5)
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
INCLUDE(CheckTypeSize)
INCLUDE(CheckFloatingPointFormat)

CHECK_SYMBOL_EXISTS(fmemopen stdio.h HAVE_FMEMOPEN)
CHECK_SYMBOL_EXISTS(funopen stdio.h HAVE_FUNOPEN)
CHECK_SYMBOL_EXISTS(htonll arpa/inet.h HAVE_HTONLL)

set(CMAKE_EXTRA_INCLUDE_FILES stdint.h)
CHECK_TYPE_SIZE("int64_t" INT64)
CHECK_TYPE_SIZE("uint64_t" UINT64)
set(CMAKE_EXTRA_INCLUDE_FILES)

UB_CHECK_FLOATING_POINT_FORMAT(HAVE_IEEE754_FLOATS
    HAVE_FLOAT_BYTES_BIGENDIAN HAVE_FLOAT_WORDS_BIGENDIAN)

#####################################################################
# Compiler flags for different build configurations
#####################################################################

set(CMAKE_C_FLAGS   "${CMAKE_ARCH_FLAGS} -Wall -Wno-strict-aliasing")
set(CMAKE_CXX_FLAGS "${CMAKE_ARCH_FLAGS} -Wall -Wno-strict-aliasing")
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_ARCH_FLAGS} -O0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_ARCH_FLAGS} -O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_ARCH_FLAGS} -O3 -DNDEBUG -Wall -Wno-strict-aliasing")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_ARCH_FLAGS} -O3 -DNDEBUG -Wall -Wno-strict-aliasing")
set(CMAKE_C_FLAGS_PROFILING "${CMAKE_ARCH_FLAGS} -pg")
set(CMAKE_CXX_FLAGS_PROFILING "${CMAKE_ARCH_FLAGS} -pg")

#####################################################################
# Process subdirectories
#####################################################################

add_subdirectory(src)

#####################################################################
# Set up testing
#####################################################################

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if(HAVE_FMEMOPEN OR HAVE_FUNOPEN)
        add_subdirectory(test)
    else(HAVE_FMEMOPEN OR HAVE_FUNOPEN)
        message(
            "Neither fmemopen() nor funopen() is supported on this platform; "
            "tests will not be compiled"
        )
    endif(HAVE_FMEMOPEN OR HAVE_FUNOPEN)
endif()

