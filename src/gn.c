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
int labelCommunities(SparseUGraph *graph, Vector **comms)
{
    assert(graph != NULL);
    int i, idx, j, root, prev_root;
    int k, comm_id;
    int *roots, *node_ids;
    EdgeList elist;
    UnionFind *uf = uf_create(graph->n);

    for (i = 0; i < graph->n; i++) {
        for (idx = graph->index[i]; idx < graph->index[i+1]; idx++) {
            j = graph->edges[idx];
            if (j >= 0) {
                uf_union(uf, i, j);
            }
        }
    }

    // find root of each node
    roots = tcalloc(graph->n, sizeof(int));
    for (i = 0; i < graph->n; i++) {
        roots[i] = uf_root(uf, i);
    }

    // assign community ids (naive approach)
    node_ids = tcalloc(graph->n, sizeof(int));
    for (i = 0; i < graph->n; i++) {
        node_ids[i] = i;
    }

    // sort in tandem, so we have ascending roots
    assert(graph != NULL);

    // use the EdgeList to sort in tandem
    // TODO: move to a more generic sorting function
    // this allocates a bunch of extra memory and sorts the id array
    // along with the degree and node_id arrays, so it wastes quite a
    // lot of resources.
    elist.length = graph->n;
    elist.id = tcalloc(graph->n, sizeof(int));
    elist.nodes[ICOL] = roots;
    elist.nodes[JCOL] = node_ids;
    sortEdges(&elist, ICOL);
    elist.nodes[ICOL] = NULL;
    elist.nodes[JCOL] = NULL;

    // get number of communities
    memcpy(elist.id, roots, graph->n * sizeof(int));
    k = graph->n;
    removeDuplicates(elist.id, &k);
    free(elist.id);

    // now accumulate all community members
    comm_id = 0;
    j = 0;
    prev_root = -1;
    *comms = tcalloc(k, sizeof(Vector));
    for (i = 0; i < comm_id; i++) {
        newVector(&(*comms)[i]);
        root = roots[j];
        vectorAppend(&(*comms)[i], node_ids[j++]);
        while (roots[j] == root) {
            vectorAppend(&(*comms)[i], node_ids[j++]);
        }
    }
    return k;
}
