add_library(brandtgc
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

target_link_libraries(brandtgc collectc tinycthread ${CMAKE_THREAD_LIBS_INIT})
if(UNIX)
    target_link_libraries(brandtgc m)
endif(UNIX)


add_executable( run main.c )
target_link_libraries( run brandtgc )
