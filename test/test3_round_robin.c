#include "uthreads.h"
#include "queue.h"
#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>

void thread_func()
{
    for (int i = 0; i < 3; i++)
    {
        printf("Thread %d running [iteration] %d\n", get_current_thread()->id, i + 1);
        uthread_yield();
    }
    uthread_exit(get_current_thread());
}

void thread_func2()
{
    for (int i = 0; i < 2; i++)
    {
        for (int i = 0; i < 3100; i++){}
        printf("Thread %d running [iteration] %d\n", get_current_thread()->id, i + 1);
        uthread_yield();
    }
    uthread_exit(get_current_thread());
}

int main()
{
    initQueue(&q);
    set_scheduling_type(PREEMPTIVE_SCHEDULING);
    set_scheduling_algo(ROUND_ROBIN);
    userThread t1, t2, t3;
    uthread_create(&t1, thread_func, 1);
    uthread_create(&t2, thread_func2, 2);
    uthread_create(&t3, thread_func, 3);

    printf("Starting round-robin scheduler\n");
    uthread_run();

    // memory free 
    freeQueue(&q);
    freeThread(&t1);
    freeThread(&t2);
    freeThread(&t3);
    printf("All threads completed in round-robin scheduling.\n");
    return 0;
}