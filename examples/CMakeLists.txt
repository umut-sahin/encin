add_executable(handling-signals handling-signals.c)

set_target_properties(
    handling-signals

    PROPERTIES

    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

target_link_libraries(handling-signals PRIVATE encin)


add_executable(copying-files copying-files.c)

set_target_properties(
    copying-files

    PROPERTIES

    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

target_link_libraries(copying-files PRIVATE encin)


add_executable(runtime-information runtime-information.c)

set_target_properties(
    runtime-information

    PROPERTIES

    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

target_link_libraries(runtime-information PRIVATE encin)


add_executable(spawning-tasks spawning-tasks.c)

set_target_properties(
    spawning-tasks

    PROPERTIES

    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

target_link_libraries(spawning-tasks PRIVATE encin)


add_executable(web-server web-server.c)

set_target_properties(
    web-server

    PROPERTIES

    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

target_link_libraries(web-server PRIVATE encin)
