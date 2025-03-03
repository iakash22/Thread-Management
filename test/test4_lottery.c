#include "uthreads.h"
#include "queue.h"
#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>

void thread_func()
{
    userThread *t = get_current_thread();
    for (int i = 0; i < 3; i++)
    {
        printf("Thread %d running iteration %d\n", t->id, i + 1);
        uthread_yield();
    }
    uthread_exit(t);
}

int main()
{
    initQueue(&q);
    set_scheduling_type(PREEMPTIVE_SCHEDULING);
    set_scheduling_algo(LOTTERY);

    userThread t1, t2, t3;
    uthread_create(&t1, thread_func, 1);
    set_thread_tickets(&t1, 0);

    uthread_create(&t2, thread_func, 2);
    set_thread_tickets(&t2, 0);

    uthread_create(&t3, thread_func, 3);
    set_thread_tickets(&t3, 0);

    printf("Starting lottery scheduler\n");
    uthread_run();
    display_lottery_stats();

    // memory free 
    freeQueue(&q);
    freeThread(&t1);
    freeThread(&t2);
    freeThread(&t3);
    printf("All threads completed in lottery scheduling.\n");
    return 0;
}
