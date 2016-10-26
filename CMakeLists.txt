project( BrandtGC )

cmake_minimum_required( VERSION 3.1 )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -pedantic -Wall -Werror" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")

set(THREADS_PREFER_PTHREAD_FLAG ON )
find_package( Threads REQUIRED )

include_directories( include )
add_subdirectory( src )


option ( BUILD_TESTS "Build tests." ON )
if ( BUILD_TESTS )

  # C++Spec library
  add_subdirectory( cppspec )

  enable_testing ()

  # Tests
  add_subdirectory( spec )

endif ( BUILD_TESTS )
