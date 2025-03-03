#include "uthreads.h"

#define TIME_QUANTUM 10000 // 10ms time slice

typedef enum
{
    NON_PREEMPTIVE_SCHEDULING,
    PREEMPTIVE_SCHEDULING,
} SCHEDULING_TYPE;

typedef enum
{
    COOPERATIVE,
    ROUND_ROBIN,
    LOTTERY,
} SCHEDULING;

extern SCHEDULING_TYPE scheduling_type;
extern SCHEDULING scheduling_algo;
extern int total_tickets;

void set_scheduling_type(SCHEDULING_TYPE type);
void set_scheduling_algo(SCHEDULING type);
void timer_handler(int signNum);
void setup_timer();

userThread *select_next_thread_lottery();
void set_thread_tickets(userThread *t,int ticket);
void display_lottery_stats();