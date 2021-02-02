add_executable(runtime-information runtime-information.c)

set_target_properties(
    runtime-information

    PROPERTIES

    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

target_link_libraries(runtime-information PRIVATE encin)
