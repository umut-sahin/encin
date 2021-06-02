#pragma once

#include <assert.h>
#include <encin/job.h>
#include <encin/result.h>
#include <encin/stack.h>
#include <encin/status.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <ucontext.h>

#define encin_spawn(...)                                                                           \
    encin_concat(encin_spawn_, encin_count_arguments(__VA_ARGS__))(__VA_ARGS__)

#define encin_spawn_blocking(...)                                                                  \
    encin_concat(encin_spawn_blocking_, encin_count_arguments(__VA_ARGS__))(__VA_ARGS__)

#define encin_spawn_void(...)                                                                      \
    encin_concat(encin_spawn_void_, encin_count_arguments(__VA_ARGS__))(__VA_ARGS__)

#define encin_spawn_blocking_void(...)                                                             \
    encin_concat(encin_spawn_blocking_void_, encin_count_arguments(__VA_ARGS__))(__VA_ARGS__)

#define encin_spawn_detached(...)                                                                  \
    encin_concat(encin_spawn_detached_, encin_count_arguments(__VA_ARGS__))(__VA_ARGS__)

#define encin_spawn_blocking_detached(...)                                                         \
    encin_concat(encin_spawn_blocking_detached_, encin_count_arguments(__VA_ARGS__))(__VA_ARGS__)


#define encin_concat(x, y)                                                                         \
    encin_concat_hack(x, y)

#define encin_concat_hack(x, y)                                                                    \
    x ## y

#define encin_count_arguments(...)                                                                 \
    encin_count_arguments_hack(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0, is_used_incorrectly)

#define encin_count_arguments_hack(_function, _1, _2, _3, _4, _5, _6, _stack_size, n, ...)         \
    n


#define encin_spawn_0(function, stack_size)                                                        \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn"                                                     \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(function()) result;                                                             \
        },                                                                                         \
        {                                                                                          \
                                                                                                   \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        container->result                                                                          \
            = function(),                                                                          \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_0(function, stack_size)                                               \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(function()) result;                                                             \
        },                                                                                         \
        {                                                                                          \
                                                                                                   \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        container->result                                                                          \
            = function(),                                                                          \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_void_0(function, stack_size)                                                   \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_void"                                                \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
        },                                                                                         \
        {                                                                                          \
                                                                                                   \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(),                                                                                \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_void_0(function, stack_size)                                          \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_void"                                       \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
        },                                                                                         \
        {                                                                                          \
                                                                                                   \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(),                                                                                \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_detached_0(function, stack_size)                                               \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_detached"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
        },                                                                                         \
        {                                                                                          \
                                                                                                   \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(),                                                                                \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )

#define encin_spawn_blocking_detached_0(function, stack_size)                                      \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_detached"                                   \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
        },                                                                                         \
        {                                                                                          \
                                                                                                   \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(),                                                                                \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )


#define encin_spawn_1(function, arg1, stack_size)                                                  \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn"                                                     \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(function(arg1)) result;                                                         \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_1(function, arg1, stack_size)                                         \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(function(arg1)) result;                                                         \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_void_1(function, arg1, stack_size)                                             \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_void"                                                \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_void_1(function, arg1, stack_size)                                    \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_void"                                       \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_detached_1(function, arg1, stack_size)                                         \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_detached"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )

#define encin_spawn_blocking_detached_1(function, arg1, stack_size)                                \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_detached"                                   \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )


#define encin_spawn_2(function, arg1, arg2, stack_size)                                            \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn"                                                     \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(function(arg1, arg2)) result;                                                   \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_2(function, arg1, arg2, stack_size)                                   \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(function(arg1, arg2)) result;                                                   \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_void_2(function, arg1, arg2, stack_size)                                       \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_void"                                                \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_void_2(function, arg1, arg2, stack_size)                              \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_void"                                       \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_detached_2(function, arg1, arg2, stack_size)                                   \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_detached"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )

#define encin_spawn_blocking_detached_2(function, arg1, arg2, stack_size)                          \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_detached"                                   \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )


#define encin_spawn_3(function, arg1, arg2, arg3, stack_size)                                      \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn"                                                     \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(function(arg1, arg2, arg3)) result;                                             \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_3(function, arg1, arg2, arg3, stack_size)                             \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(function(arg1, arg2, arg3)) result;                                             \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_void_3(function, arg1, arg2, arg3, stack_size)                                 \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_void"                                                \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_void_3(function, arg1, arg2, arg3, stack_size)                        \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_void"                                       \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_detached_3(function, arg1, arg2, arg3, stack_size)                             \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_detached"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )

#define encin_spawn_blocking_detached_3(function, arg1, arg2, arg3, stack_size)                    \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_detached"                                   \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )


#define encin_spawn_4(function, arg1, arg2, arg3, arg4, stack_size)                                \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn"                                                     \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(function(arg1, arg2, arg3, arg4)) result;                                       \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3,                                                         \
                container->evaluated_arg4                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_4(function, arg1, arg2, arg3, arg4, stack_size)                       \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(function(arg1, arg2, arg3, arg4)) result;                                       \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3,                                                         \
                container->evaluated_arg4                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_void_4(function, arg1, arg2, arg3, arg4, stack_size)                           \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_void"                                                \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_void_4(function, arg1, arg2, arg3, arg4, stack_size)                  \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_void"                                       \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_detached_4(function, arg1, arg2, arg3, arg4, stack_size)                       \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_detached"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )

#define encin_spawn_blocking_detached_4(function, arg1, arg2, arg3, arg4, stack_size)              \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_detached"                                   \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )


#define encin_spawn_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                          \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn"                                                     \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(function(arg1, arg2, arg3, arg4, arg5)) result;                                 \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3,                                                         \
                container->evaluated_arg4,                                                         \
                container->evaluated_arg5                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                 \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(function(arg1, arg2, arg3, arg4, arg5)) result;                                 \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3,                                                         \
                container->evaluated_arg4,                                                         \
                container->evaluated_arg5                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_void_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                     \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_void"                                                \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_void_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)            \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_void"                                       \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_detached_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                 \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_detached"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )

#define encin_spawn_blocking_detached_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)        \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_detached"                                   \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )


#define encin_spawn_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)                    \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn"                                                     \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(arg6) evaluated_arg6;                                                           \
            typeof(function(arg1, arg2, arg3, arg4, arg5, arg6)) result;                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
            _encin_container->evaluated_arg6 = (arg6);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3,                                                         \
                container->evaluated_arg4,                                                         \
                container->evaluated_arg5,                                                         \
                container->evaluated_arg6                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)           \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(arg6) evaluated_arg6;                                                           \
            typeof(function(arg1, arg2, arg3, arg4, arg5, arg6)) result;                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
            _encin_container->evaluated_arg6 = (arg6);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        container->result                                                                          \
            = function(                                                                            \
                container->evaluated_arg1,                                                         \
                container->evaluated_arg2,                                                         \
                container->evaluated_arg3,                                                         \
                container->evaluated_arg4,                                                         \
                container->evaluated_arg5,                                                         \
                container->evaluated_arg6                                                          \
            ),                                                                                     \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_void_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)               \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_void"                                                \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(arg6) evaluated_arg6;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
            _encin_container->evaluated_arg6 = (arg6);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5,                                                             \
            container->evaluated_arg6                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_blocking_void_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)      \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_void"                                       \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(arg6) evaluated_arg6;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
            _encin_container->evaluated_arg6 = (arg6);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5,                                                             \
            container->evaluated_arg6                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize                                                                             \
    )

#define encin_spawn_detached_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)           \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_detached"                                            \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(arg6) evaluated_arg6;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
            _encin_container->evaluated_arg6 = (arg6);                                             \
        },                                                                                         \
        false,                                                                                     \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5,                                                             \
            container->evaluated_arg6                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )

#define encin_spawn_blocking_detached_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)  \
    encin_spawner(                                                                                 \
        stack_size,                                                                                \
        "("                                                                                        \
            "encin usage error on encin_spawn_blocking_detached"                                   \
            "stack_size is not an instance of encin_stack_size"                                    \
        ")",                                                                                       \
                                                                                                   \
        {                                                                                          \
            encin_job job;                                                                         \
            typeof(arg1) evaluated_arg1;                                                           \
            typeof(arg2) evaluated_arg2;                                                           \
            typeof(arg3) evaluated_arg3;                                                           \
            typeof(arg4) evaluated_arg4;                                                           \
            typeof(arg5) evaluated_arg5;                                                           \
            typeof(arg6) evaluated_arg6;                                                           \
        },                                                                                         \
        {                                                                                          \
            _encin_container->evaluated_arg1 = (arg1);                                             \
            _encin_container->evaluated_arg2 = (arg2);                                             \
            _encin_container->evaluated_arg3 = (arg3);                                             \
            _encin_container->evaluated_arg4 = (arg4);                                             \
            _encin_container->evaluated_arg5 = (arg5);                                             \
            _encin_container->evaluated_arg6 = (arg6);                                             \
        },                                                                                         \
        true,                                                                                      \
                                                                                                   \
        function(                                                                                  \
            container->evaluated_arg1,                                                             \
            container->evaluated_arg2,                                                             \
            container->evaluated_arg3,                                                             \
            container->evaluated_arg4,                                                             \
            container->evaluated_arg5,                                                             \
            container->evaluated_arg6                                                              \
        ),                                                                                         \
                                                                                                   \
        encin_finalize_detached                                                                    \
    )


#define encin_spawner(                                                                             \
    _encin_stack_size,                                                                             \
    _encin_invalid_stack_size_message,                                                             \
    _encin_container_definition,                                                                   \
    _encin_initializer,                                                                            \
    _encin_is_blocking,                                                                            \
    _encin_runner,                                                                                 \
    _encin_finalizer                                                                               \
) ({                                                                                               \
    assert(                                                                                        \
        _encin_invalid_stack_size_message                                                          \
            && (_encin_stack_size >= ENCIN_STACK_16K && _encin_stack_size <= ENCIN_STACK_4G)       \
    );                                                                                             \
                                                                                                   \
    struct _encin_container_definition *_encin_container                                           \
        = malloc(sizeof(struct _encin_container_definition));                                      \
                                                                                                   \
    _encin_container == NULL                                                                       \
    ? ({                                                                                           \
        encin_status = ENCIN_TASK_CREATION_FAILURE;                                                \
        (typeof(_encin_container))NULL;                                                            \
    })                                                                                             \
    : ({                                                                                           \
        _encin_initializer                                                                         \
                                                                                                   \
        _encin_container->job.is_blocking = _encin_is_blocking;                                    \
                                                                                                   \
        _encin_container->job.encin_status_ = encin_status;                                        \
        _encin_container->job.errno_ = errno;                                                      \
                                                                                                   \
        _encin_container->job.is_scheduled = false;                                                \
        getcontext(&_encin_container->job.context);                                                \
                                                                                                   \
        _encin_container->job.context.uc_stack.ss_size                                             \
            = 1 << _encin_stack_size;                                                              \
        _encin_container->job.context.uc_stack.ss_sp                                               \
            = encin_stack_acquire(_encin_stack_size);                                              \
                                                                                                   \
        _encin_container->job.is_completed = false;                                                \
        _encin_container->job.parent = encin_active_job();                                         \
        _encin_container->job.awaiting = NULL;                                                     \
        pthread_mutex_init(&_encin_container->job.lock, NULL);                                     \
                                                                                                   \
        _encin_container->job.context.uc_stack.ss_sp == NULL                                       \
        ? ({                                                                                       \
            pthread_mutex_destroy(&_encin_container->job.lock);                                    \
            free(_encin_container);                                                                \
            encin_status = ENCIN_STACK_ACQUIRY_FAILURE;                                            \
            (typeof(_encin_container))NULL;                                                        \
        })                                                                                         \
        : ({                                                                                       \
            void ignitor(typeof(_encin_container) container) {                                     \
                _encin_runner;                                                                     \
                _encin_finalizer(_encin_stack_size);                                               \
            }                                                                                      \
            makecontext(                                                                           \
                &_encin_container->job.context,                                                    \
                (void (*)(void))ignitor,                                                           \
                1, _encin_container                                                                \
            );                                                                                     \
                                                                                                   \
            encin_schedule(&_encin_container->job) == -1                                           \
            ? ({                                                                                   \
                encin_stack_release(                                                               \
                    _encin_stack_size,                                                             \
                    _encin_container->job.context.uc_stack.ss_sp                                   \
                );                                                                                 \
                pthread_mutex_destroy(&_encin_container->job.lock);                                \
                free(_encin_container);                                                            \
                encin_status = ENCIN_JOB_SUBMISSION_FAILURE;                                       \
                (typeof(_encin_container))NULL;                                                    \
            })                                                                                     \
            : ({                                                                                   \
                _encin_container;                                                                  \
            });                                                                                    \
        });                                                                                        \
    });                                                                                            \
})
