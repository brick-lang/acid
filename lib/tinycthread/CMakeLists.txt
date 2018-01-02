add_library(tinycthread STATIC tinycthread.c)
target_link_libraries(tinycthread ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(tinycthread PROPERTIES C_VISIBILITY_PRESET hidden)
