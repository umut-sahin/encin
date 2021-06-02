#include <encin.h>
#include <pthread.h>
#include <stdio.h>

float f(float x, float y) {
    return x + y;
}

void g() {
    pthread_t tid = pthread_self();
    printf("g is running on thread %lu\n", tid);
}

int encin(void) {
    encin_task t1 = encin_spawn(f, 2, 3, ENCIN_STACK_16K);
    float result = encin_await(t1);

    printf("f(2, 3) = %f\n\n", result);

    encin_task t2 = encin_spawn_blocking_void(g, ENCIN_STACK_1M);
    encin_task t3 = encin_spawn_blocking_void(g, ENCIN_STACK_1M);
    encin_task t4 = encin_spawn_blocking_void(g, ENCIN_STACK_1M);

    encin_await_void(t2);
    encin_await_void(t3);
    encin_await_void(t4);

    return 0;
}
