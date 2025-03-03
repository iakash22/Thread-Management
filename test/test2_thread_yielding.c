#include "uthreads.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void thread_func1() {
    printf("Thread 1 started\n");
    uthread_yield();
    printf("Thread 1 resumed\n");
    uthread_exit(get_current_thread());
}

void thread_func2() {
    printf("Thread 2 started\n");
    uthread_yield();
    printf("Thread 2 resumed\n");
    uthread_exit(get_current_thread());
}

int main() {
    initQueue(&q);
    
    userThread t1, t2;
    uthread_create(&t1, thread_func1, 1);
    uthread_create(&t2, thread_func2, 2);
    
    printf("Starting scheduler\n");
    uthread_run();

    // memory free 
    freeQueue(&q);
    freeThread(&t1);
    freeThread(&t2);
    printf("All threads completed.\n");
    return 0;
}