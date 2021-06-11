cmake_minimum_required(VERSION 3.17)

project(
    encin
    VERSION 0.1.0
    LANGUAGES C
)

add_library(
    encin

    src/io.c
    src/main.c
    src/pool.c
    src/queue.c
    src/reactor.c
    src/result.c
    src/signal.c
    src/stack.c
    src/status.c
)

set_target_properties(
    encin

    PROPERTIES

    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

target_compile_options(
    encin

    PRIVATE

    -Wall
    -Wextra
    -Wpedantic
)

target_include_directories(encin PUBLIC include)

target_link_libraries(encin PUBLIC pthread)

target_link_libraries(encin PUBLIC uring)

if (CMAKE_PROJECT_NAME STREQUAL encin)
    add_subdirectory(examples)
endif ()
