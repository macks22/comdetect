/////////////////////////////////////////////
// DYNAMIC ARRAY (with auto-resize) = Vector

#define INIT_VECTOR_SIZE    50

typedef struct {

    int *data;
    int cap;    // amount of space allocated
    int size;   // amount of space in use

} Vector;

// allocate space for new vector
void newVector(Vector *vec);

// free mem allocation for vector
void freeVector(Vector *vec);

// append item to end of vector, expanding if necessary
void vectorAppend(Vector *vec, int item);

// double capacity of vector
void doubleVectorCap(Vector *vec);

// print full vector contents
void printVector(Vector *vec);
