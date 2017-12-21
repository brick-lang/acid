cmake_minimum_required( VERSION 3.1 )
project( Acid )

set( Acid_VERSION_MAJOR 0 )
set( Acid_VERSION_MINOR 1 )


set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -pedantic -Wall -Werror -Wno-extended-offsetof" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")

INCLUDE(CheckIncludeFiles)
CHECK_INCLUDE_FILES(threads.h HAVE_THREADS_H)

if(NOT HAVE_THREADS_H)
  set(THREADS_PREFER_PTHREAD_FLAG ON )
  find_package( Threads )
endif()

add_subdirectory( lib )
include_directories( include lib )
add_subdirectory( src )

option ( BUILD_EXAMPLES "Build examples." ON )
if ( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif ( BUILD_EXAMPLES )

option ( BUILD_TESTS "Build tests." ON )
if ( BUILD_TESTS )
  enable_testing ()

  # Tests
  add_subdirectory( spec )
endif ( BUILD_TESTS )
