add_library( brandtgc
  bit.c
  complock.c
  idbaseobject.c
  wrappedlock.c
  counter.c
  locker.c
  link.c
  object.c collector.c safelist.c xthread.c root.c worker.c)

target_link_libraries(brandtgc collectc ${CMAKE_THREAD_LIBS_INIT})
if(UNIX)
    target_link_libraries(brandtgc m)
endif(UNIX)


add_executable( run main.c )
target_link_libraries( run brandtgc )