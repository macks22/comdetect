#include "graph.h"


// Perform a BFS on the sparse undirected graph and return
// the information discovered.
void bfs(SparseUGraph *graph, BFSInfo *info)
{
    assert(graph != NULL);
    if (graph->n <= 0) return;

    Queue q;
    int i, j, par, child;

    // reset the information storage
    resetBFSInfo(info);

    // set up queue for the search
    newQueue(&q);
    info->distance[info->src] = 0;        // root node has 0 distance to itself
    info->parent[info->src] = info->src;  // root node has no parent
    info->sigma[info->src] = 1;
    enqueue(&q, info->src);

    // commence searching
    while (!queueIsEmpty(&q)) {
        par = dequeue(&q);
        vectorAppend(&info->stack, par);

        // explore all children of this node
        for (i = graph->index[par]; i < graph->index[par+1]; i++) {
            child = graph->edges[i];
            if (child < 0) continue;  // account for edges that have been cut

            // process child and queue its children if not already discovered
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

void
resetBFSInfo(BFSInfo *info)
{   // Zero out all BFS info data, to prepare for new run
    // this assumes the grpah size has not changed.
    int i;
    info->stack.size = 0;
    for (i = 0; i < info->n; i++) {
        info->pred[i].size = 0;
        info->distance[i] = -1;
        info->parent[i] = 0;
        info->sigma[i] = 0;
    }
}

void
newBFSInfo(BFSInfo *info, int n)
{   // allocate new BFSInfo struct, for size `n` graph.
    int i;
    info->n = n;
    initVector(&info->stack, n);
    info->parent = tcalloc(n, sizeof(int));
    info->distance = tcalloc(n, sizeof(int));
    info->sigma = tcalloc(n, sizeof(int));
    info->pred = (Vector *)tcalloc(n, sizeof(Vector));
    for (i = 0; i < n; i++) {
        newVector(&info->pred[i]);
    }
}

// free BFSInfo struct
void
freeBFSInfo(BFSInfo *info)
{
    int i;
    assert(info != NULL);

    free(info->parent);
    info->parent = NULL;
    free(info->distance);
    info->distance = NULL;
    free(info->sigma);
    info->sigma = NULL;

    freeVector(&info->stack);
    for (i = 0; i < info->n; i++) {
        freeVector(&info->pred[i]);
    }
    free(info->pred);
    info->pred = NULL;
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

// print out stack of vertices accumulated during BFS
// should return vertices in order of non-increasing distance from src
void
printBFSStack(BFSInfo *info)
{
    int i;
    assert(info != NULL);
    for (i = info->stack.size-1; i >= 0; i--) {
        printf("%d ", info->stack.data[i]);
    }
    printf("\n");
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
