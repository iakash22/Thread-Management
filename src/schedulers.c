#include "schedulers.h"
#include "queue.h"
#include "uthreads.h"
#include <signal.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// For Peermptive Round Robin Scheduling

void set_scheduling_type(SCHEDULING_TYPE type)
{
    scheduling_type = type;
}

void set_scheduling_algo(SCHEDULING algoType)
{
    scheduling_algo = algoType;
}

void timer_handler(int signNum)
{
    (void)signNum;
    printf("[TIME QUANTUM EXPIRE] : Time Quantum complete \n");
    uthread_yield();
}

void setup_timer()
{
    struct sigaction sa;
    struct itimerval timer;

    // Configure the signal handler
    sa.sa_handler = &timer_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    // Configure the timer to send SIGALRM every TIME_QUANTUM microseconds
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = TIME_QUANTUM;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = TIME_QUANTUM;

    setitimer(ITIMER_REAL, &timer, NULL);
}

int total_tickets = 0;

// Lotteray Scheduling Logic
userThread *select_next_thread_lottery()
{
    if (q.size == 0)
        return NULL;
    int winning_ticket = rand() % total_tickets;
    int ticket_sum = 0;
    userThread *selected_thread = NULL;
    queue tempQueue;
    initQueue(&tempQueue);

    // Pop each thread, check if it wins, and push back into the queue
    while (!isQueueEmpty(&q))
    {
        userThread *t = pop(&q);
        ticket_sum += t->tickets;

        if (!selected_thread && ticket_sum > winning_ticket)
        {
            selected_thread = t; // Select the winner
            selected_thread->selected_count += 1;
            printf("[LOTTERY WINNER] : TID %d \n", selected_thread->id);
        }
        else
        {
            push(&tempQueue, t);
        }
    }

    // Restore all threads back to the queue
    while (!isQueueEmpty(&tempQueue))
    {
        userThread *t = pop(&tempQueue);
        if (t->state != TERMINATED)
        {
            push(&q, t);
        }
    }

    return selected_thread;
}

void set_thread_tickets(userThread *t, int ticket)
{
    if(ticket == 0){
        ticket = rand() % 20 + 1; // Random tickets between 1 and 20
    }
    t->tickets = ticket;
    total_tickets += ticket;
}

void display_lottery_stats() {
    printf("\nLottery Scheduling Statistics:\n");
    userThread *tempQueue[MAX_THREADS]; // Temporary array to hold threads
    int index = 0, total_selections = 0;

    // Move threads to temp array while counting selections
    while (!isQueueEmpty(&statsQ)) {
        userThread *t = pop(&statsQ);
        tempQueue[index++] = t;
        total_selections += t->selected_count;
    }

    printf("\nLottery Scheduling Statistics:\n");
    printf("Thread ID | Tickets | Selection Count | Selection Percentage\n");
    printf("---------|---------|-----------------|---------------------\n");
    for (int i = 0; i < index; i++)
    {
        userThread *t = tempQueue[i];
        double percentage = (total_selections > 0) ? 
                            ((double)t->selected_count / total_selections) * 100 : 0;

        printf("%-9d | %-7d | %-15d | %-18.2f%%\n",
               t->id,
               t->tickets,
               t->selected_count,
               percentage);
    }

    freeQueue(&statsQ);
}
