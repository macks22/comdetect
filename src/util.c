#include "graph.h"

const int OOM_ERROR = -1;

// print error and exit
void error(int err_code)
{
    // for now, just print generic message and exit with error code
    printf("fatal error occured; exiting.");
    exit(err_code);
}

// attempt to calloc memory, error out if failure
void *
tcalloc(size_t nitems, size_t size)
{
    void *block = calloc(nitems, size);
    if (block == NULL) error(OOM_ERROR);
    return block;
}

// attempt to malloc memory, error out if failure
void *
tmalloc(size_t size)
{
    void *block = malloc(size);
    if (block == NULL) error(OOM_ERROR);
    return block;
}

// attempt to realloc memory, error out if failure
void *
trealloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (ptr == NULL) error(OOM_ERROR);
    return ptr;
}

// find the largest number in the array
int findLargest(int *array, int length)
{
    int i, largest = -1;
    for (i = 0; i < length; i++) {
        if (array[i] > largest) largest = array[i];
    }
    return largest;
}

// perform an in-place radix sort on the array; result is ascending order
void radixSort(int *array, int length)
{
    // Base 10 is used
    int i;
    int base = 10;        /* base 10 is used */
    int sig_digit = 1;    /* most significant digit */
    int bucket[base];     /* need one bucket for each of 0-base modulus results */
    int semi_sorted[length];
    int largest = findLargest(array, length);

    // Loop until we reach the largest significant digit
    while ((largest / sig_digit) > 0) {
        memset(bucket, 0, sizeof(bucket));

        // Counts the number of "keys" or digits that will go into each bucket
        for (i = 0; i < length; i++) {
            bucket[(array[i] / sig_digit) % base]++;
        }

        // Add the count of the previous buckets.
        // This acquires the indices after the end of each bucket location in
        // the edge list -- similar to count sorting algorithm.
        for (i = 1; i < base; i++) {
            bucket[i] += bucket[i-1];
        }

        // Use the bucket to fill a "semi_sorted" array
        for (i = length-1; i >= 0; i--) {
            semi_sorted[--bucket[(array[i] / sig_digit) % base]] = array[i];
        }

        for (i = 0; i < length; i++) {
            array[i] = semi_sorted[i];
        }

        // Move to next significant digit
        sig_digit *= base;
    }
}

// remove all duplicate values from the integer array
// return the size of the new array
void removeDuplicates(int *a, int *length)
{
    int i, cur = 0;
    radixSort(a, *length);
    for (i = 1; i < *length; i++) {
        if (a[cur] != a[i]) {
            a[++cur] = a[i];
        }
    }
    *length = cur + 1;  // new array size
}

// print an array of integers
void printArray(int *array, int length)
{
    int i;
    for (i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
