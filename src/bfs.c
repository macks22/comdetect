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
    newVector(&info->stack);
    info->n = graph->n;
    info->parent = (int *)tcalloc(graph->n, sizeof(int));
    info->distance = (int *)tcalloc(graph->n, sizeof(int));
    info->sigma = (int *)tcalloc(graph->n, sizeof(int));
    info->pred = (Vector *)tcalloc(graph->n, sizeof(Vector));
    for (i = 0; i < graph->n; i++) {
        newVector(&info->pred[i]);
    }

    // set all distances to -1 and all sigmas to 0
    for (i = 0; i < graph->n; i++) {
        info->distance[i] = -1;
        info->sigma[i] = 0;
    }

    // commence the search
    newQueue(&q);
    info->distance[info->src] = 0;        // root node has 0 distance to itself
    info->parent[info->src] = info->src;  // root node has no parent
    info->sigma[info->src] = 1;
    enqueue(&q, info->src);

    while (!queueIsEmpty(&q)) {
        par = dequeue(&q);
        vectorAppend(&info->stack, par);

        // explore all children of this node
        for (i = graph->index[par]; i < graph->index[par+1]; i++) {
            child = graph->edges[i];
            if (!discovered(info, child)) {
                info->parent[child] = par;
                info->distance[child] = info->distance[par]+1;
                enqueue(&q, child);
            }
            // on the shortest path?
            if (info->distance[child] == info->distance[par]+1) {
                vectorAppend(&info->pred[child], par);
                info->sigma[child] += info->sigma[par];
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

void
printPredecessors(BFSInfo *info)
{   // print out predecessor info from BFS
    int i;
    printf("predecessors:\n");
    for (i = 0; i < info->n; i++) {
        printf("%d: ", i);
        printVector(&info->pred[i]);
    }
}


// print out number of shortest paths
void
printShortestPathCounts(BFSInfo *info)
{
    int i;
    printf("node\tnum_shortest_paths\n");
    for (i = 0; i < info->n; i++) {
        printf("%d\t%d\n", i, info->sigma[i]);
    }
}
