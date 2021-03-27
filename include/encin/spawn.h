#pragma once

#include <assert.h>
#include <stdbool.h>

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
    ({ static_assert(false, "encin_spawn is not implemented yet"); })

#define encin_spawn_blocking_0(function, stack_size)                                               \
    ({ static_assert(false, "encin_spawn_blocking is not implemented yet"); })

#define encin_spawn_void_0(function, stack_size)                                                   \
    ({ static_assert(false, "encin_spawn_void is not implemented yet"); })

#define encin_spawn_blocking_void_0(function, stack_size)                                          \
    ({ static_assert(false, "encin_spawn_blocking_void is not implemented yet"); })

#define encin_spawn_detached_0(function, stack_size)                                               \
    ({ static_assert(false, "encin_spawn_detached is not implemented yet"); })

#define encin_spawn_blocking_detached_0(function, stack_size)                                      \
    ({ static_assert(false, "encin_spawn_blocking_detached is not implemented yet"); })


#define encin_spawn_1(function, arg1, stack_size)                                                  \
    ({ static_assert(false, "encin_spawn is not implemented yet"); })

#define encin_spawn_blocking_1(function, arg1, stack_size)                                         \
    ({ static_assert(false, "encin_spawn_blocking is not implemented yet"); })

#define encin_spawn_void_1(function, arg1, stack_size)                                             \
    ({ static_assert(false, "encin_spawn_void is not implemented yet"); })

#define encin_spawn_blocking_void_1(function, arg1, stack_size)                                    \
    ({ static_assert(false, "encin_spawn_blocking_void is not implemented yet"); })

#define encin_spawn_detached_1(function, arg1, stack_size)                                         \
    ({ static_assert(false, "encin_spawn_detached is not implemented yet"); })

#define encin_spawn_blocking_detached_1(function, arg1, stack_size)                                \
    ({ static_assert(false, "encin_spawn_blocking_detached is not implemented yet"); })


#define encin_spawn_2(function, arg1, arg2, stack_size)                                            \
    ({ static_assert(false, "encin_spawn is not implemented yet"); })

#define encin_spawn_blocking_2(function, arg1, arg2, stack_size)                                   \
    ({ static_assert(false, "encin_spawn_blocking is not implemented yet"); })

#define encin_spawn_void_2(function, arg1, arg2, stack_size)                                       \
    ({ static_assert(false, "encin_spawn_void is not implemented yet"); })

#define encin_spawn_blocking_void_2(function, arg1, arg2, stack_size)                              \
    ({ static_assert(false, "encin_spawn_blocking_void is not implemented yet"); })

#define encin_spawn_detached_2(function, arg1, arg2, stack_size)                                   \
    ({ static_assert(false, "encin_spawn_detached is not implemented yet"); })

#define encin_spawn_blocking_detached_2(function, arg1, arg2, stack_size)                          \
    ({ static_assert(false, "encin_spawn_blocking_detached is not implemented yet"); })


#define encin_spawn_3(function, arg1, arg2, arg3, stack_size)                                      \
    ({ static_assert(false, "encin_spawn is not implemented yet"); })

#define encin_spawn_blocking_3(function, arg1, arg2, arg3, stack_size)                             \
    ({ static_assert(false, "encin_spawn_blocking is not implemented yet"); })

#define encin_spawn_void_3(function, arg1, arg2, arg3, stack_size)                                 \
    ({ static_assert(false, "encin_spawn_void is not implemented yet"); })

#define encin_spawn_blocking_void_3(function, arg1, arg2, arg3, stack_size)                        \
    ({ static_assert(false, "encin_spawn_blocking_void is not implemented yet"); })

#define encin_spawn_detached_3(function, arg1, arg2, arg3, stack_size)                             \
    ({ static_assert(false, "encin_spawn_detached is not implemented yet"); })

#define encin_spawn_blocking_detached_3(function, arg1, arg2, arg3, stack_size)                    \
    ({ static_assert(false, "encin_spawn_blocking_detached is not implemented yet"); })


#define encin_spawn_4(function, arg1, arg2, arg3, arg4, stack_size)                                \
    ({ static_assert(false, "encin_spawn is not implemented yet"); })

#define encin_spawn_blocking_4(function, arg1, arg2, arg3, arg4, stack_size)                       \
    ({ static_assert(false, "encin_spawn_blocking is not implemented yet"); })

#define encin_spawn_void_4(function, arg1, arg2, arg3, arg4, stack_size)                           \
    ({ static_assert(false, "encin_spawn_void is not implemented yet"); })

#define encin_spawn_blocking_void_4(function, arg1, arg2, arg3, arg4, stack_size)                  \
    ({ static_assert(false, "encin_spawn_blocking_void is not implemented yet"); })

#define encin_spawn_detached_4(function, arg1, arg2, arg3, arg4, stack_size)                       \
    ({ static_assert(false, "encin_spawn_detached is not implemented yet"); })

#define encin_spawn_blocking_detached_4(function, arg1, arg2, arg3, arg4, stack_size)              \
    ({ static_assert(false, "encin_spawn_blocking_detached is not implemented yet"); })


#define encin_spawn_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                          \
    ({ static_assert(false, "encin_spawn is not implemented yet"); })

#define encin_spawn_blocking_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                 \
    ({ static_assert(false, "encin_spawn_blocking is not implemented yet"); })

#define encin_spawn_void_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                     \
    ({ static_assert(false, "encin_spawn_void is not implemented yet"); })

#define encin_spawn_blocking_void_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)            \
    ({ static_assert(false, "encin_spawn_blocking_void is not implemented yet"); })

#define encin_spawn_detached_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)                 \
    ({ static_assert(false, "encin_spawn_detached is not implemented yet"); })

#define encin_spawn_blocking_detached_5(function, arg1, arg2, arg3, arg4, arg5, stack_size)        \
    ({ static_assert(false, "encin_spawn_blocking_detached is not implemented yet"); })


#define encin_spawn_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)                    \
    ({ static_assert(false, "encin_spawn is not implemented yet"); })

#define encin_spawn_blocking_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)           \
    ({ static_assert(false, "encin_spawn_blocking is not implemented yet"); })

#define encin_spawn_void_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)               \
    ({ static_assert(false, "encin_spawn_void is not implemented yet"); })

#define encin_spawn_blocking_void_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)      \
    ({ static_assert(false, "encin_spawn_blocking_void is not implemented yet"); })

#define encin_spawn_detached_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)           \
    ({ static_assert(false, "encin_spawn_detached is not implemented yet"); })

#define encin_spawn_blocking_detached_6(function, arg1, arg2, arg3, arg4, arg5, arg6, stack_size)  \
    ({ static_assert(false, "encin_spawn_blocking_detached is not implemented yet"); })
