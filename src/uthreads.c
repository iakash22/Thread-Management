#include "uthreads.h"
#include "queue.h"
#include <stdlib.h>
#include "schedulers.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

queue q;
queue statsQ;
ucontext_t main_context;
userThread *current_thread = NULL;
SCHEDULING_TYPE scheduling_type = NON_PREEMPTIVE_SCHEDULING;
SCHEDULING scheduling_algo = COOPERATIVE;

userThread *get_current_thread()
{
    return current_thread;
}

void uthread_create(userThread *t, void (*func)(), int id)
{
    if (q.size >= MAX_THREADS)
    {
        fprintf(stderr, "Error: Maximum thread limit reached\n");
        return;
    }

    // Initialize thread context
    printf("[CREATED] : tID %d created.\n", id);
    getcontext(&t->context);
    t->context.uc_stack.ss_sp = t->stack;
    t->context.uc_stack.ss_size = STACK_SIZE;
    t->context.uc_link = &main_context;

    // Initialize thread properties
    t->id = id;
    t->next = NULL;
    t->state = READY;
    t->tickets = 0;
    t->selected_count = 0;

    push(&q, t);
    makecontext(&t->context, func, 0);

    q.size++;
    // thread_count++;
}

void uthread_run()
{
    if (scheduling_type == PREEMPTIVE_SCHEDULING)
    {
        setup_timer();
    }

    if (isQueueEmpty(&q))
    {
        printf("[NO THREADS IN QUEUE] : No threads to run.\n");
        printf("[COMPLETED] : All threads completed.\n");
        return;
    }

    while (!isQueueEmpty(&q))
    {
        current_thread = scheduling_algo == LOTTERY ? select_next_thread_lottery() : pop(&q);
        if (current_thread == NULL)
        {
            return;
        }
        current_thread->state = RUNNING;
        printf("[RUNNING] : Running thread TID %d\n", current_thread->id);
        swapcontext(&main_context, &current_thread->context);
    }
}

void uthread_yield()
{
    if (current_thread == NULL)
    {
        return;
    }

    printf("[READY] : Thread TID %d yielding\n", current_thread->id);
    current_thread->state = READY;
    if(current_thread->state != TERMINATED){
        push(&q, current_thread);
    }
    userThread *next_thread = scheduling_algo == LOTTERY ? select_next_thread_lottery() : pop(&q);;

    if (next_thread)
    {
        userThread *prev_thread = current_thread;
        current_thread = next_thread;
        current_thread->state = RUNNING;
        swapcontext(&prev_thread->context, &current_thread->context);
    }
}

void uthread_exit(userThread *thread)
{
    if (thread == NULL)
    {
        return;
    }

    printf("[TERMINATED] : Thread TID %d Completed\n", thread->id);
    thread->state = TERMINATED;
    current_thread = NULL;
    printQueue(&q);
    push(&statsQ, thread);
    uthread_run();
}

void freeThread(userThread *head) {
    userThread *current = head;
    while (current != NULL) {
        userThread *temp = current;
        current = current->next;
        free(temp);
    }
}