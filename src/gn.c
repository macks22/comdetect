#include "graph.h"


// Use the Girvan Newman (2004) algorithm to divisely
// cluster the graph into k partitions.
void girvanNewman(SparseUGraph *graph, int k, float sample_rate)
{
    Vector largest;     // edges with highest betweenness
    int edges_cut=0;    // num edges cut so far
    int iteration=1;    // which iteration the algorithm is on
    int src, dest, i;

    assert(graph != NULL);
    if (graph->m <= 0) return;

    while (edges_cut < k && edges_cut < graph->m) {
        calculateEdgeBetweenness(graph, sample_rate, &largest);
        for (i = 0; i < largest.size; i++) {
            src = largest.data[i++];
            dest = largest.data[i];
            cutEdge(graph, src, dest, iteration);
        }
        edges_cut += largest.size / 2;
        iteration++;
        freeVector(&largest);
        printSparseUGraph(graph, graph->n);
    }
}

// Cut an edge from the graph by marking it with the negative
// of the iteration number in which it was cut.
void cutEdge(SparseUGraph *graph, int src, int dest, int iteration)
{
    int i;
    for (i = graph->index[src]; i < graph->index[src+1]; i++) {
        if (graph->edges[i] == dest) {
            graph->edges[i] = -iteration;
        }
    }

    for (i = graph->index[dest]; i < graph->index[dest+1]; i++) {
        if (graph->edges[i] == src) {
            graph->edges[i] = -iteration;
        }
    }
}

// Build up the communities from the divided graph
// using a union-find data structure
void gatherCommunities(SparseUGraph *graph, Vector **comms)
{

}
