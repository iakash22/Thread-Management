#include "uthreads.h"
#include <stdlib.h>

typedef struct
{
    userThread *front;
    userThread *rear;
    int size;
} queue;

extern queue q;
extern queue statsQ;
void initQueue(queue *q);
void push(queue *q, userThread *t);
userThread *pop(queue *q);
int isQueueEmpty(queue *q);
void printQueue(queue *q);
void freeQueue(queue *q);