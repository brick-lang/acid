add_library(acid_internal STATIC
        idlock.c locker.c link.c
        object.c collector.c safelist.c
        task.c worker.c memory.c counter.c
        $<TARGET_OBJECTS:rpmalloc>)

set_target_properties(acid_internal PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(acid_internal PROPERTIES C_VISIBILITY_PRESET hidden)

add_library(acid acid.c)
target_link_libraries(acid acid_internal collectc)

if(NOT HAVE_THREADS_H)
  target_link_libraries(acid_internal tinycthread)
endif()

set_target_properties(acid PROPERTIES PUBLIC_HEADER "../include/acid.h")

install(TARGETS acid
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include )
