#include "uthreads.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void thread_func()
{
    printf("Single thread started\n");
    uthread_exit(get_current_thread());
}

int main()
{
    initQueue(&q);

    userThread t1;
    uthread_create(&t1, thread_func, 1);

    printf("Starting scheduler\n");
    uthread_run();

    printf("Single thread execution completed.\n");
    freeQueue(&q);
    freeThread(&t1);
    return 0;
}