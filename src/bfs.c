#include "graph.h"


// Perform a BFS on the sparse undirected graph and return
// the information discovered.
void bfs(SparseUGraph *graph, BFSInfo *info)
{
    assert(graph != NULL);
    if (graph->n <= 0) return;

    Queue q;
    int i, j, par, child, curlayer;

    // allocate space for bfs info
    info->discovered = (bool *)tcalloc(graph->n, sizeof(bool));
    info->processed = (bool *)tcalloc(graph->n, sizeof(bool));
    info->parent = (int *)tcalloc(graph->n, sizeof(int));
    info->distance = (int *)tcalloc(graph->n, sizeof(int));

    // commence the search
    newQueue(&q);
    enqueue(&q, info->src);
    info->discovered[info->src] = true;   // found but not processed
    info->parent[info->src] = info->src;  // root node has no parent and 0 distance to itself

    while (!queueIsEmpty(&q)) {
        par = dequeue(&q);
        // explore all children of this node
        for (i = graph->index[par]; i < graph->index[par+1]; i++) {
            child = graph->edges[i];
            if (!info->discovered[child]) {
                info->discovered[child] = true;
                info->parent[child] = par;
                info->distance[child] = info->distance[par]+1;
                enqueue(&q, child);
            }
        }
        info->processed[par] = true;
    }
    freeQueue(&q);
}

// free BFSInfo struct
void
freeBFSInfo(BFSInfo *info)
{
    assert(info != NULL);
    free(info->discovered);
    free(info->processed);
    free(info->parent);
    free(info->distance);
}
