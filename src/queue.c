#include "graph.h"


void newQueue(Queue *q) {
    q->data = (int *)tcalloc(INIT_QUEUE_SIZE, sizeof(int));
    q->size = INIT_QUEUE_SIZE;
    q->first = 0;
    q->last = INIT_QUEUE_SIZE - 1;
    q->count = 0;
}

void freeQueue(Queue *q) {
    free(q->data);
}

void enqueue(Queue *q, int x) {
    // double queue size if necessary
    if (q->count >= q->size) doubleQueueSize(q);

    // add the new element to the end of the queue
    q->last = (q->last+1) % q->size;
    q->data[q->last] = x;
    q->count++;
}

void doubleQueueSize(Queue *q) {
    q->size *= 2;
    q->data = (int *)realloc(q->data, q->size);
}

int dequeue(Queue *q) {
    assert(q->count > 0);

    int x;
    x = q->data[q->first];
    q->first = (q->first+1) % q->size;
    q->count = q->count - 1;
    return(x);
}

int queueIsEmpty(Queue *q) {
    // return 1 if no elements are in the queue, else 0
    if (q->count <= 0) return 1;
    else return 0;
}

void printQueue(Queue *q) {
    int i = q->first;

    while (i != q->last) {
        printf("%c ", q->data[i]);
        i = (i+1) % q->size;
    }

    printf("%2d ",q->data[i]);
    printf("\n");
}
