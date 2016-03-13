add_executable( tests runner.cpp )
target_link_libraries( tests brandtgc c++spec ${CMAKE_THREAD_LIBS_INIT} )

