file(GLOB source_files "*.c")

add_library(collectc STATIC ${source_files})
set_target_properties(collectc PROPERTIES C_VISIBILITY_PRESET hidden)