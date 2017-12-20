add_library(acid
        acid.c complock.c locker.c link.c
        object.c collector.c safelist.c
        task.c worker.c
        $<TARGET_OBJECTS:collectc> $<TARGET_OBJECTS:tinycthread>)

target_link_libraries(acid ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(acid PROPERTIES PUBLIC_HEADER "../include/acid.h")

install(TARGETS acid
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include )
