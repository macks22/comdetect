#include "graph.h"


void
newVector(Vector *vec)
{   // allocate space for new vector
    vec->data = (int *)tcalloc(INIT_VECTOR_SIZE, sizeof(int));
    vec->cap = INIT_VECTOR_SIZE;
    vec->size = 0;
}

void
freeVector(Vector *vec)
{   // free mem allocation for vector
    assert(vec != NULL);
    free(vec->data);
    vec->size = 0;
    vec->cap = 0;
}

void
vectorAppend(Vector *vec, int item)
{   // append item to end of vector, expanding if necessary
    if (vec->size >= vec->cap) doubleVectorCap(vec);
    vec->data[vec->size++] = item;
}

void
doubleVectorCap(Vector *vec)
{   // double capacity of vector
    vec->cap *= 2;
    vec->data = (int *)trealloc(vec->data, vec->cap*sizeof(int));
}

void printVector(Vector *vec)
{   // print full vector contents
    assert(vec != NULL);
    int i;
    for (i = 0; i < vec->size; i++) {
        printf("%d ", vec->data[i]);
    }
    printf("\n");
}
