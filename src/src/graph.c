#include "graph.h"


int
findLargestJ(EdgeList *elist)
{   // we assume we're working with non-negative node indices.
    int i, largest=0;
    for (i=0, i<elist.length; i++) {
        if (elist->edges[i] > largest)
            largest = elist->edges[i];
    }
    return largest;
}

// Radix sort for linear time edge sorting.
void
sortEdges(EdgeList *elist)
{   /* Each edge is stored as (int i, int j). We want to sort them
     * based on the j value.
     */

    int i, loc;
    int base = 10;      /* use a base of 10 */
    int bucket[base];   /* need one bucket for each of 0-base modulus results */
    int sig_digit = 1;  /* most significant digit */
    int largest_j;
    EdgeList *semi_sorted;

    newEdgeList(semi_sorted, elist->length);
    largest_j = findLargestJ(elist);

    // loop until we reach highest significant digit
    while (largest_j / sig_digit > 0) {
        memset(bucket, 0, sizeof(bucket));

        // count number of keys/digits that will go into each bucket
        for (i=0; i<elist.length; i++)
            bucket[(elist->edges[i]->j / sig_digit) % base]++;

        // Add the count of the previous buckets.
        // This acquires the indices after the end of each bucket location in
        // the edge list -- similar to count sorting algorithm.
        for (i=0; i<base; i++)
            bucket[i] += bucket[i-1];

        // use buckets to fill a semi sorted edge list
        for (i=elist.length-1; i>=0; i--) {
            loc = --bucket[(elist->edges[i]->j / sig_digit) % 10];
            semi_sorted->edges[loc] = elist->edges[i];
        }

        for (i=0; i<elist.length; i++)
            elist->edges[i] = semi_sorted->edges[i];

        // move to next significant digit
        sig_digit *= base;
    }
    freeEdgeList(&semi_sorted);
}

void
graphToEdgeList(sparseUGraph *graph, EdgeList *elist)
{
    elist = newEdgeList(graph->m);
    for (i=0; i<graph->n; i++) {
        for (j=graph->index_ij[i]; j<graph->index_ij[i+1]; i++) {
            elist->edges[i]->i = i;
            elist->edges[j]->j = j;
        }
    }
    return elist;
}

void
populateSymmetricEdges(sparseUGraph *graph)
{   /* Since an undirected graph has symmetric edges, we normally
     * only store each once. However, storing each edge as (i, j)
     * and (j, i) allows faster lookup of neighbors. If we stored
     * each edge once, and we ask who are the neighbors of i, we must
     * look at i's edgelist, and also every other node's edgelist to
     * see if i is present in it. If we duplicate the edges, then we
     * must only check both of i's edgelists. In the first (ij), only
     * the neighbors of i that have an index greater than i will be
     * stored. In the second (ji), only those with an index less than
     * i will be stored. Together, we get all neighbors.
     */
    int i, j, edge_idx;
    EdgeList *elist;
    Edge *edge;

    // allocate space for the symmetric edges
    graph->index_ji = (int *)calloc(graph->n+1, sizeof(int));
    graph->edges_ji = (int *)calloc(graph->m, sizeof(int));

    // first make an edgelist from the ij-indexed edges
    elist = graphToEdgeList(graph, &elist);

    // next sort the edge list by the j indices
    sortEdges(&elist);

    // Now write out to the ji index and edges array. Note that we may
    // have j indices that never show up, since each edge is still only
    // listed once in this edge list. So we should loop over all
    // possible nodes, check if there exists a j index that matches, and
    // if not, we simply store the same edge index into the jth index in
    // the ji index.
    edge_idx = 0;
    edge = elist->edges[edge_idx];
    for (j=0; j<graph->n; j++) {
        graph->index_ji[j] = edge_idx;
        if (edge->j == j) { // node j has some edges
            while (edge->j == j) {
                graph->edges_ji[edge_idx++] = edge->i;
                edge = elist->edges[edge_idx];
            }
        }
    }
    // should have read all of the edges
    assert(edge_idx == elist->length-1);
}

void
readSparseUGraph(inputArgs *args, sparseUGraph *graph)
{
    FILE *fpin;
    int src_id, dest_id;
    int prev_src_id = -1;
    int id_idx=0, index_idx=0, edge_idx=0;
    SparseUGraph *graph = calloc(1, sizeof(SparseUGraph));

    // read the first line, which contains #nodes #edges
    fpin = fopen(args->infile, "r");
    fscanf(fpin, "%d %d", &graph->n, &graph->m);

    // allocate space for edge and node storage.
    graph->id = (int *)calloc(graph->n, sizeof(int));
    graph->index_ij = (int *)calloc(graph->n+1, sizeof(int));
    graph->edges_ij = (int *)calloc(graph->m, sizeof(int));

    // these remain unallocated unless/until populated by symmetric edges
    graph->edges_ji = NULL;
    graph->index_ji = NULL;

    // now read in the edgelist, one at a time
    while (!feof(fpin)) {
        fscanf(fpin, "%d %d", &src_id, &dest_id);
        if (prev_src_id != src_id) { // new source node

            // add new node id to id array
            graph->id[id_idx++] = src_id;
            prev_src_id = src_id;

            // add start index for new src node edges
            graph->index_ij[index_idx++] = edge_idx;
        } else {
            // add next edge to edge list
            graph->edges_ij[edge_idx++] = dest_id;
        }
    }

    // Write out the ID array; we won't be using it while processing.
    // It can be used later to translate the output (in node indices)
    // to the input node IDs.
    storeAndFreeIDArray(graph);
    populateSymmetricEdges(graph);
    fclose(fpin);
}

void
storeAndFreeNodeIds(sparseUGraph *graph)
{
    int i;
    FILE *fpout;
    char store_file[50] = "../output_and_stored_id_ary/storedIDAry.txt";

    fpout = fopen(store_file, "w");
    for (i=0; i < graph->n; i++) {
        fprintf(fpout, "%d\n", graph->id[i]);
    }

    fclose(fpout);
    free(graph->id);
    printf("Node id array stored in %s\n", store_file);
}
