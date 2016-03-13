project( BrandtGC )

cmake_minimum_required( VERSION 3.0 )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Werror")

find_package( Threads )

include_directories( include )
add_subdirectory( src )


option ( BUILD_TESTS "Build tests." ON )
if ( BUILD_TESTS )

  # C++Spec library
  add_subdirectory( cppspec )

  # Tests
  add_subdirectory( spec )

endif ( BUILD_TESTS )
