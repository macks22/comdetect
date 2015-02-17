#include "graph.h"


void
initVector(Vector *vec, int size)
{   // allocate space for new vector, with non-default initial size
    vec->data = tcalloc(size, sizeof(int));
    vec->cap = size;
    vec->size = 0;
}

void
newVector(Vector *vec)
{   // allocate space for new vector
    initVector(vec, INIT_VECTOR_SIZE);
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

int
vectorPop(Vector *vec)
{   // pop an item from the vector; don't call on empty vectors
    int item;
    //if (vec->size <= 0) error(EMPTY_VECTOR);
    assert(vec->size > 0);
    item = vec->data[--vec->size];
    return item;
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
