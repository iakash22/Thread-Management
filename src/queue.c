#include <queue.h>
#include <stdio.h>

void initQueue(queue *q){
    q->front = q->rear = NULL;
    q->size = 0;
}

void push(queue *q, userThread *thread) {
    // printf("Thread TID %d push in queue \n", thread->id);
    thread->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = thread;
        return;
    }
    q->rear->next = thread;
    q->rear = thread;
}

userThread *pop(queue *q) {
    if (q->front == NULL) {
        return NULL; // Queue is empty
    }
    userThread *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    // printf("Thread TID %d pop from queue\n", temp->id);
    return temp;
}

int isQueueEmpty(queue *q) {
    return q->front == NULL;
}

void printQueue(queue *q) {
    userThread *temp = q->front;
    printf("Ready Queue Thread ID: [ ");
    while (temp) {
        printf("%d, ", temp->id);            
        temp = temp->next;
    }
    printf("]\n");
}

void freeQueue(queue *q) {
    if (q == NULL) {
        return;
    }

    userThread *current = q->front;
    while (current != NULL) {
        userThread *temp = current;
        current = current->next;
        free(temp); // Free each userThread in the queue
    }

    // Reset the queue after freeing the threads (optional, depending on usage)
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}
