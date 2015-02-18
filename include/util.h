///////////////////////////////////////
// UTILITY FUNCTIONS

// print error and exit
void error(int err_code);

// attempt to calloc memory, error out if failure
void *tcalloc(size_t nitems, size_t size);

// attempt to malloc memory, error out if failure
void *tmalloc(size_t size);

// attempt to realloc memory, error out if failure
void *trealloc(void *ptr, size_t size);

// find the largest number in the array
int findLargest(int *array, int length);

// perform an in-place radix sort on the array; result is ascending order
void radixSort(int *array, int length);

// remove all duplicate values from the integer array
// return the size of the new array
void removeDuplicates(int *array, int *length);

// print an array of integers
void printArray(int *array, int length);


// ERROR CODES
#define OOM_ERROR           -1
#define BAD_FP              -2
#define INVALID_SAMPLE_SIZE -3
