add_executable( bit_spec bit_spec.cpp )
target_link_libraries( bit_spec brandtgc c++spec ${CMAKE_THREAD_LIBS_INIT} )
add_test ( bit_spec bit_spec )

add_custom_target ( spec ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
  ${CMAKE_CTEST_COMMAND} -C $<CONFIG> -j 4 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
