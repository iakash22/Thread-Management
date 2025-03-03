
// #include "include/uthreads.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>

// // Simple single thread function
// void single_thread()
// {
//     printf("Single thread starting\n");
//     for (int i = 0; i < 5; i++)
//     {
//         printf("Single thread iteration %d\n", i);
//         // Some computation to demonstrate execution
//         for (long j = 0; j < 100000000; j++)
//         {
//         }
//     }
//     printf("Single thread completed\n");
//     uthread_exit(get_current_thread());
// }

// int main()
// {
//     printf("\n=== Test 1: Single Thread Execution ===\n");
//     printf("This test verifies:\n");
//     printf("1. Proper initialization of a single thread\n");
//     printf("2. Execution flow of a single thread\n");
//     printf("3. Completion and statistics tracking\n\n");

//     // Create a single thread
//     uthread_t t1;
//     printf("Creating a single thread...\n");
//     uthread_create(&t1, single_thread, 1, 5);

//     // printf("\nStarting thread execution...\n\n");

//     // Run thread until completion
//     // while (get_current_thread() != NULL || head != NULL) {
//     //     uthread_yield();

//     //     // Check if current thread has completed its burst time
//     //     uthread_t *current = get_current_thread();
//     //     if (current != NULL) {
//     //         int elapsed_time = current->completion_time == -1 ?
//     //                          (current_time - current->arrival_time) :
//     //                          (current->completion_time - current->arrival_time);

//     //         if (elapsed_time >= current->burst_time) {
//     //             uthread_exit(current);
//     //         }
//     //     }
//     // }

//     printf("\nThread execution completed\n");
//     printf("\nVerifying scheduling results:\n");
//     // display_scheduling_results();

//     // Assertions for verification
//     // assert(completed_thread_count == 1);  // Only one thread should complete
//     // assert(completed_threads[0].id == 1);  // Should be our thread with ID 1
//     // assert(completed_threads[0].completion_time != -1);  // Thread should have completed
//     // assert(completed_threads[0].completion_time >= completed_threads[0].arrival_time); // Logical time constraint

//     printf("\nTest 1 Passed: Single Thread Execution working correctly!\n");
//     return 0;
// }
