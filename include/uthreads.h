#ifndef UTHREADS_H
#define UTHREADS_H

#include <ucontext.h>
#include <stdlib.h>

#define MAX_THREADS 10
#define STACK_SIZE 10240

typedef enum
{
    READY,
    RUNNING,
    TERMINATED,
} STATE;

typedef struct userThread {
    ucontext_t context; // thread context
    int id; // thread id
    char stack[STACK_SIZE]; 
    struct userThread *next;
    STATE state;
    int tickets;          // Number of lottery tickets assigned to this thread
    int selected_count;
} userThread;

extern ucontext_t main_context;
extern userThread *head;
extern int current_time;

void uthread_create(userThread *t, void (*func)(), int id);
void uthread_yield();
void uthread_exit(userThread *thread);
void uthread_run();
userThread *get_current_thread();
void setup_timer();
void freeThread(userThread *head);

#endif