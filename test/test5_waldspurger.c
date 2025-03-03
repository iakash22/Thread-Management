
#include "uthreads.h"
#include "schedulers.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define NUM_THREADS 5
#define MAX_RUNS 3
int total_tickets_sum = 0;

// Global variables to track thread execution
int thread_executed[NUM_THREADS] = {0};
int thread_tickets[NUM_THREADS] = {0};

// Execution statistics
int rr_thread_runs[NUM_THREADS][MAX_RUNS] = {0};
int lottery_thread_runs[NUM_THREADS][MAX_RUNS] = {0};
int current_run = 0;

// Thread function with configurable workload and priority
void worker_thread()
{
    userThread *thread = get_current_thread();
    int thread_idx = thread->id - 1; // ID is 1-based, array is 0-based

    // Mark this thread as executed
    thread_executed[thread_idx] = 1;

    // Record this thread execution in statistics
    if (scheduling_algo == ROUND_ROBIN)
    {
        rr_thread_runs[thread_idx][current_run]++;
    }
    else
    {
        lottery_thread_runs[thread_idx][current_run]++;
    }

    printf("Thread %d executing (scheduler: %s)\n",
           thread->id, scheduling_algo == LOTTERY ? "Lottery" : "Round Robin");

    // Simulate work
    for (long j = 0; j < 5000 + (thread->id * 10000); j++)
    {
    }

    // Yield to allow scheduler to select next thread
    if (scheduling_algo == ROUND_ROBIN)
    {
        // Regular yield for round-robin
        uthread_yield();
    }
    else
    {
        // Use lottery to select next thread
        userThread *next = select_next_thread_lottery();

        if (!next)
        {
            printf("[LOTTERY] No tickets remaining or no threads ready\n");
            return;
        }

        if (next == NULL)
        {
            printf("[LOTTERY] Selected thread not found\n");
            return;
        }

        // Switch to selected thread
        userThread *current = get_current_thread();
        if (current != NULL)
        {
            printf("[LOTTERY] Switching from Thread %d to Thread %d\n",
                   current->id, next->id);
            swapcontext(&current->context, &next->context);
        }
    }
}

void thread1_func() { worker_thread(); } // High priority (10 tickets)
void thread2_func() { worker_thread(); } // Medium priority (5 tickets)
void thread3_func() { worker_thread(); } // Low priority (3 tickets)
void thread4_func() { worker_thread(); } // Medium-high priority (7 tickets)
void thread5_func() { worker_thread(); } // Very low priority (2 tickets)

void reset_system()
{
    // Reset thread tracking
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_executed[i] = 0;
    }
    initQueue(&q);
    // Reset lottery state
    total_tickets_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_tickets[i] = 0;
    }
}

void run_experiment()
{
    printf("\n==========================================\n");
    printf("RUNNING EXPERIMENT WITH %s SCHEDULER (Run %d)\n",
           scheduling_algo == LOTTERY ? "LOTTERY" : "ROUND ROBIN", current_run + 1);
    printf("==========================================\n\n");

    // Reset state
    reset_system();

    // Set preemptive mode
    set_scheduling_type(PREEMPTIVE_SCHEDULING);

    // Create thread structures
    userThread t1, t2, t3, t4, t5;

    // Create threads with same work but different priorities for lottery scheduler
    printf("Creating threads...\n");
    uthread_create(&t1, thread1_func, 1);
    uthread_create(&t2, thread2_func, 2);
    uthread_create(&t3, thread3_func, 3);
    uthread_create(&t4, thread4_func, 4);
    uthread_create(&t5, thread5_func, 5);

    // Set up lottery tickets if using lottery scheduler
    if (scheduling_algo == LOTTERY)
    {
        thread_tickets[0] = 10; // Thread 1: high priority
        thread_tickets[1] = 5;  // Thread 2: medium priority
        thread_tickets[2] = 3;  // Thread 3: low priority
        thread_tickets[3] = 7;  // Thread 4: medium-high priority
        thread_tickets[4] = 2;  // Thread 5: very low priority

        total_tickets_sum = 10 + 5 + 3 + 7 + 2;
        printf("Total lottery tickets: %d\n", total_tickets_sum);
    }

    printf("\nStarting execution...\n\n");
    uthread_run();
    // Run for a fixed number of time slices
    int time_slices = 20;
    for (int i = 0; i < time_slices; i++)
    {
        printf("\n----- Time Slice %d -----\n", i + 1);
        uthread_yield();
    }

    printf("\nExperiment with %s scheduler completed\n",
           scheduling_algo == LOTTERY ? "Lottery" : "Round Robin");

    // memory free 
    freeThread(&t1);
    freeThread(&t2);
    freeThread(&t3);
    freeThread(&t4);
    freeThread(&t5);
}

// Print execution distribution
void print_distribution()
{
    printf("\n==========================================\n");
    printf("EXECUTION DISTRIBUTION COMPARISON\n");
    printf("==========================================\n\n");

    printf("%-10s | %-30s | %-30s\n", "Thread", "Round Robin Scheduler", "Lottery Scheduler");
    printf("----------|--------------------------------|--------------------------------\n");

    // Calculate totals
    int rr_total = 0;
    int lottery_total = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (int j = 0; j < current_run; j++)
        {
            rr_total += rr_thread_runs[i][j];
            lottery_total += lottery_thread_runs[i][j];
        }
    }

    // Print each thread's percentage
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int rr_runs = 0;
        int lottery_runs = 0;

        for (int j = 0; j < current_run; j++)
        {
            rr_runs += rr_thread_runs[i][j];
            lottery_runs += lottery_thread_runs[i][j];
        }

        float rr_percent = (rr_total > 0) ? ((float)rr_runs / (rr_total + 0.00001)) * 100 : 0;
        float lottery_percent = (lottery_total > 0) ? ((float)lottery_runs / (lottery_total + 0.00001)) * 100 : 0;

        printf("Thread %-3d | %4d runs (%6.2f%%) %s        | %4d runs (%6.2f%%) %s\n",
               i + 1,
               rr_runs, rr_percent,
               (i == 0) ? "(high prio)" : (i == 4 ? "(low prio) " : "          "),
               lottery_runs, lottery_percent,
               (i == 0) ? "(high prio)" : (i == 4 ? "(low prio) " : "          "));
    }

    printf("\nNOTE: In Round Robin, all threads get equal CPU time regardless of priority.\n");
    printf("      In Lottery Scheduler, threads with more tickets (higher priority) get proportionally more CPU time.\n");
}

int main()
{
    printf("\n=== Waldspurger's Lottery Scheduler vs Round Robin Experiment ===\n");
    printf("This test compares:\n");
    printf("1. Round Robin scheduling (equal time slices)\n");
    printf("2. Lottery scheduling (priority-proportional time slices)\n");
    printf("With threads having different priorities but equal work.\n\n");

    // Seed random number generator
    srand(time(NULL));

    // Run multiple experiments to get better statistics
    for (current_run = 0; current_run < MAX_RUNS; current_run++)
    {
        // Run with round-robin scheduler
        set_scheduling_algo(ROUND_ROBIN);
        run_experiment();

        // Run with lottery scheduler
        set_scheduling_algo(LOTTERY);
        run_experiment(1);
    }

    // Print distribution comparison
    print_distribution();

    printf("\nExperiment complete! Lottery scheduling demonstrates proportional share\n");
    printf("resource allocation as described in Waldspurger's paper.\n");

    return 0;
}
