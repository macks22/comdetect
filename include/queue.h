#define INIT_QUEUE_SIZE  1000

typedef struct {

    int *data;	/* queue contents */
    int size;   /* number of elements */
    int count;  /* number of queue elements */
    int first;  /* position of first element */
    int last;   /* position of last element */

} Queue;


void newQueue(Queue *q);
void freeQueue(Queue *q);
void enqueue(Queue *q, int x);
int dequeue(Queue *q);
void doubleQueueSize(Queue *q);
int queueIsEmpty(Queue *q);
void printQueue(Queue *q);
