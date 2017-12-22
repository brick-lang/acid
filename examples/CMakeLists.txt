add_executable( run main.c )
target_link_libraries( run acid )

add_executable(gypsum gypsum.c stack.c)
target_link_libraries(gypsum acid)