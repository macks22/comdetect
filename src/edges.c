#include "graph.h"


// allocate space for a new edge list of the given length
void
newEdgeList(EdgeList *elist, int length)
{
    int i;
    elist->length = length;
    elist->nodes[0] = tcalloc(length, sizeof(int));
    elist->nodes[1] = tcalloc(length, sizeof(int));
    elist->id = tcalloc(length, sizeof(int));
    resetEdgeIds(elist);
}

void
resetEdgeIds(EdgeList *elist)
{   // reorder id array s.t. the value at each index is the index (0->m-1)
    int i;
    for (i = 0; i < elist->length; i++) {
        elist->id[i] = i;
    }
}

// copy an entire edgelist into a new one
void
copyEdgeList(EdgeList *cur, EdgeList *new)
{
    assert(cur != NULL);
    int i, j;

    // first allocate space for the copy
    newEdgeList(new, cur->length);
    assert(new->length == cur->length);

    // then move over i and j columns
    for (j = 0; j < 2; j++) {
        for (i = 0; i < cur->length; i++) {
            new->nodes[j][i] = cur->nodes[j][i];
        }
    }

    // copy id array
    for (i = 0; i < cur->length; i++) {
        new->id[i] = cur->id[i];
    }
}

// Free the memory allocated for the edgelist.
void
freeEdgeList(EdgeList *elist)
{
    assert(elist != NULL);
    free(elist->nodes[0]);
    free(elist->nodes[1]);
    free(elist->id);
}

// find largest value in i or j column
int findLargestEndpoint(EdgeList *elist, int col)
{
    assert(elist != NULL);
    return findLargest(elist->nodes[col], elist->length);
}

// sort edges by i column (0) or by j column (1)
// use radix sort for linear time sorting
void
sortEdges(EdgeList *elist, int col)
{
    assert(elist != NULL);

    int i, loc, largest;
    int base = 10;      /* use a base of 10 */
    int bucket[base];   /* need one bucket for each of 0-base modulus results */
    int sig_digit = 1;  /* most significant digit */
    EdgeList semi_sorted;

    newEdgeList(&semi_sorted, elist->length);
    largest = findLargestEndpoint(elist, col);

    // loop until we reach highest significant digit
    while (largest / sig_digit > 0) {
        memset(bucket, 0, sizeof(bucket));

        // count number of keys/digits that will go into each bucket
        for (i = 0; i < elist->length; i++)
            bucket[(elist->nodes[col][i] / sig_digit) % base]++;

        // Add the count of the previous buckets.
        // This acquires the indices after the end of each bucket location in
        // the edge list -- similar to count sorting algorithm.
        for (i = 1; i < base; i++) {
            bucket[i] += bucket[i-1];
        }

        // use the buckets to fill a semi sorted edge list
        for (i = elist->length-1; i >= 0; i--) {
            loc = --bucket[(elist->nodes[col][i] / sig_digit) % 10];
            semi_sorted.nodes[col][loc] = elist->nodes[col][i];
            semi_sorted.nodes[1-col][loc] = elist->nodes[1-col][i];
            semi_sorted.id[loc] = elist->id[i];
        }

        for (i = 0; i < elist->length; i++) {
            elist->nodes[col][i] = semi_sorted.nodes[col][i];
            elist->nodes[1-col][i] = semi_sorted.nodes[1-col][i];
            elist->id[i] = semi_sorted.id[i];
        }

        // move to next significant digit
        sig_digit *= base;
    }
    freeEdgeList(&semi_sorted);
}

// Return an array with all unique node ids sorted in ascending order.
// Also set the number of unique nodes after filtering.
void
mapNodeIds(EdgeList *elist, int **idmap, int *num_nodes)
{
    assert(elist != NULL);
    int i, j;
    int size = elist->length * 2;
    int *nodes = tcalloc(size, sizeof(int));

    // first add node ids from the i column
    for (i = 0; i < elist->length; i++) {
        nodes[i] = elist->nodes[ICOL][i];
    }

    // next from the j column
    for (j = 0; j < elist->length; j++) {
        nodes[i++] = elist->nodes[JCOL][j];
    }

    // now remove duplicates (which sorts) and set number of nodes
    removeDuplicates(nodes, &size);
    *num_nodes = size;

    // finally, set the result
    *idmap = tcalloc(size, sizeof(int));
    for (i = 0; i < size; i++) {
        (*idmap)[i] = nodes[i];
    }
    free(nodes);
}

// Print out the edge list (for debugging purposes), up to `num_edges` edges.
void
printEdgeList(EdgeList *elist, int num_edges)
{
    assert(elist != NULL);
    int i;

    num_edges = (num_edges >= elist->length) ? elist->length-1 : num_edges;
    for (i = 0; i <= num_edges; i++) {
        printf("%d: (%d, %d)\n",
               elist->id[i], elist->nodes[0][i], elist->nodes[1][i]);
    }
}
