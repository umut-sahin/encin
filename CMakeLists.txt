cmake_minimum_required(VERSION 3.17)

project(
    encin
    VERSION 0.1.0
    LANGUAGES C
)

add_library(
    encin

    src/main.c
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

if (CMAKE_PROJECT_NAME STREQUAL encin)
    add_subdirectory(examples)
endif ()
