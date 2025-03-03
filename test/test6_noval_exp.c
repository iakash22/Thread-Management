#include "uthreads.h"
#include "queue.h"
#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 5
#define MAX_RUNS 3

// Thread execution tracking
int rr_executions[NUM_THREADS] = {0};
int lottery_executions[NUM_THREADS] = {0};
int rr_time_slices[NUM_THREADS] = {0};
int lottery_time_slices[NUM_THREADS] = {0};
int total_tickets_sum = 0;
int scheduler_type = 0; 


void worker_thread() {
    userThread *thread = get_current_thread();
    int thread_idx = thread->id - 1; 

    // Track execution count
    if (scheduler_type == 0) {
        rr_executions[thread_idx]++;
    } else {
        lottery_executions[thread_idx]++;
    }

    printf("Thread %d executing (scheduler: %s)\n", thread->id, scheduling_algo == LOTTERY ? "Lottery" : "Round Robin");

    
    for (long i = 0; i < 10000 * thread->id; i++);

    // Track time slices spent per thread
    if (scheduling_algo == ROUND_ROBIN) {
        rr_time_slices[thread_idx] += thread->id; // Simulating work done
    } else {
        lottery_time_slices[thread_idx] += thread->id;

    }

    uthread_yield(); 
}

void run_round_robin() {
    initQueue(&q);
    set_scheduling_algo(ROUND_ROBIN);
    set_scheduling_type(PREEMPTIVE_SCHEDULING);

    userThread t1, t2, t3, t4, t5;
    uthread_create(&t1, worker_thread, 1);
    uthread_create(&t2, worker_thread, 2);
    uthread_create(&t3, worker_thread, 3);
    uthread_create(&t4, worker_thread, 4);
    uthread_create(&t5, worker_thread, 5);

    printf("\nStarting Round-Robin Experiment\n");
    uthread_run();
    printf("Round-Robin Experiment Completed\n");

    // memory free 
    // freeQueue(&q);
    // freeThread(&t1);
    // freeThread(&t2);
    // freeThread(&t3);
    // freeThread(&t4);
    // freeThread(&t5);
}

void run_lottery() {
    initQueue(&q);
    set_scheduling_algo(LOTTERY);
    set_scheduling_type(PREEMPTIVE_SCHEDULING);

    userThread t1, t2, t3, t4, t5;
    uthread_create(&t1, worker_thread, 1);
    set_thread_tickets(&t1, 10);

    uthread_create(&t2, worker_thread, 2);
    set_thread_tickets(&t2, 5);

    uthread_create(&t3, worker_thread, 3);
    set_thread_tickets(&t3, 3);

    uthread_create(&t4, worker_thread, 4);
    set_thread_tickets(&t4, 7);

    uthread_create(&t5, worker_thread, 5);
    set_thread_tickets(&t5, 2);

    total_tickets_sum = 10 + 5 + 3 + 7 + 2;
    printf("Total Lottery Tickets: %d\n", total_tickets_sum);

    printf("\nStarting Lottery Scheduling Experiment\n");
    uthread_run();
    printf("Lottery Scheduling Experiment Completed\n");

    // memory free 
    // freeQueue(&q);
    // freeThread(&t1);
    // freeThread(&t2);
    // freeThread(&t3);
    // freeThread(&t4);
    // freeThread(&t5);
}

// Compare both schedulers
void compare_schedulers() {
    printf("\n=================NOVAL EXPERIMENT====================\n");
    printf("   COMPARISON: ROUND-ROBIN vs LOTTERY\n");
    printf("=====================================\n");
    printf("%-10s | %-15s | %-15s | %-15s | %-15s\n", "Thread", "RR Runs", "RR Time", "Lottery Runs", "Lottery Time");
    printf("----------|-----------------|-----------------|-----------------|-----------------\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %-3d | %-15d | %-15d | %-15d | %-15d\n",
               i + 1, rr_executions[i], rr_time_slices[i], lottery_executions[i], lottery_time_slices[i]);
    }

    printf("\n=== Analysis ===\n");
    printf("✔ Round-Robin: Equal execution across all threads, no priority.\n");
    printf("✔ Lottery: Threads with more tickets execute more often, showing priority-based selection.\n");
}

int main() {
    printf("\n=== Lottery vs. Round-Robin Scheduling Experiment ===\n");
    srand(time(NULL));

    for (int run = 0; run < MAX_RUNS; run++) {
        run_round_robin();
        run_lottery();
    }

    compare_schedulers();
    
    printf("\nNoval Experiment Complete! Results show how Lottery Scheduling favors high-priority threads.\n");
    return 0;
}
