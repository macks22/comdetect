#include "graph.h"


// Perform a BFS on the sparse undirected graph and return
// the information discovered.
void bfs(SparseUGraph *graph, BFSInfo *info)
{
    assert(graph != NULL);
    if (graph->n <= 0) return;

    Queue q;
    int i, j, par, child;

    // allocate space for bfs info
    info->parent = (int *)tcalloc(graph->n, sizeof(int));
    info->distance = (int *)tcalloc(graph->n, sizeof(int));
    info->pred = (Vector *)tcalloc(graph->n, sizeof(Vector));
    for (i = 0; i < graph->n; i++) {
        newVector(&info->pred[i]);
    }

    // set all distances to -1
    for (i = 0; i < graph->n; i++) {
        info->distance[i] = -1;
    }

    // commence the search
    newQueue(&q);
    info->distance[info->src] = 0;        // root node has 0 distance to itself
    info->parent[info->src] = info->src;  // root node has no parent
    enqueue(&q, info->src);

    while (!queueIsEmpty(&q)) {
        par = dequeue(&q);
        // explore all children of this node
        for (i = graph->index[par]; i < graph->index[par+1]; i++) {
            child = graph->edges[i];
            if (!discovered(info, child)) {
                info->parent[child] = par;
                info->distance[child] = info->distance[par]+1;
                enqueue(&q, child);
            }
            if (info->distance[child] == info->distance[par]+1) {
                // par is the predecessor of child
                vectorAppend(&info->pred[child], par);
            }
        }
    }
    freeQueue(&q);
}

// free BFSInfo struct
void
freeBFSInfo(BFSInfo *info)
{
    assert(info != NULL);
    free(info->parent);
    free(info->distance);
}

// print out the path from the target node to the src node
void
printShortestPath(BFSInfo *info, int dest)
{
    int par = dest;
    printf("path from %d --> %d:\n", dest, info->src);
    while (par != info->src) {
        printf("%d ", par);
        par = info->parent[par];
    }
    printf("%d\n", info->src);
}
