add_library(acid
        acid.c
        complock.c
        idbaseobject.c
        wrappedlock.c
        locker.c
        link.c
        object.c
        collector.c
        safelist.c
        task.c
        worker.c)

target_link_libraries(acid collectc tinycthread ${CMAKE_THREAD_LIBS_INIT})
if(UNIX)
    target_link_libraries(acid m)
endif(UNIX)