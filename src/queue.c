#include "graph.h"


void initQueue(Queue *q, int size) {
    q->data = tcalloc(size, sizeof(int));
    q->size = size;
    q->first = 0;
    q->last = size - 1;
    q->count = 0;
}

void newQueue(Queue *q) {
    initQueue(q, INIT_QUEUE_SIZE);
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
    q->data = realloc(q->data, q->size * sizeof(int));
}

int dequeue(Queue *q) {
    assert(q->count > 0);

    int item;
    item = q->data[q->first];
    q->first = (q->first+1) % q->size;
    q->count--;
    return item;
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
