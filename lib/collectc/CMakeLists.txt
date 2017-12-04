file(GLOB source_files "src/*.c")
file(GLOB header_files "include/*.h")

include_directories(include)
add_library(collectc STATIC ${source_files})