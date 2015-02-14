#include "comdetect.h"
#include "queue.h"


// Get array of node ids which are neighbors of this node.
int *neighbors(crsMatrix *graph, int node) {
    assert(node >= 0);
    assert(node < graph->num_nodes);

    intArray *neighbors;
    int edge_idx=0, i=0;
    int start, size;

    start = graph->index[node];
    size = graph->index[node+1] - start;
    neighbors = newIntArray(size);

    for (edge_idx=graph->index[node]; edge_idx < graph->index[node+1]; edge_idx++) {
        neighbors->data[i++] = graph->edges[edge_idx];
    }
    return neighbors;
}

// Breadth-First Search (BFS) implementation
int bfs(crsMatrix *graph, int src, int target) {
    Queue *q;
    int i, j, node, neighbor, cur_layer;
    int visited[graph->num_nodes];
    int parent[graph->num_nodes];
    int distance[graph->num_nodes];

    // TODO: perhaps store these 3 arrays in a bfs_data struct?
    // then post-processing could be used to determine shortest paths.

    // initialize tracking data
    for (i=0; i < graph->num_nodes; i++) {
        visited[i] = 0;
        parent[i] = i;
        distance[i] = 0;
    }

    q = newQueue();
    enqueue(q, src);
    visited[src] = 1;
    distance[src] = 0;
    parent[src] = src;  // root node

    while (!queueIsEmpty(q)) {
        // check if next node is our search target
        node = dequeue(q);
        if (node == target) { // search finished
            return distance[node];
        }

        // visit all immediate neighbors
        cur_layer = distance[node] + 1;
        for (i=graph->index[node]; i < graph->index[node+1]; i++) {
            neighbor = graph->edges[i];

            if (!visited[neighbor]) {
                parent[neighbor] = node;
                distance[neighbor] = cur_layer;
                visited[neighbor] = 1;

                // add all of this neighbors' neighbors to the queue
                for (j=graph->index[neighbor]; i < graph->index[neighbor+1]; i++) {
                    enqueue(q, neighbor);
                }
            }
        }
    }

    // return -1 to indicate the search failed; target unreachable
    return -1;
}
