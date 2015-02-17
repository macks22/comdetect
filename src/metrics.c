#include "graph.h"


void
calculateDegreeAndSort(SparseUGraph *graph)
{
    int index_idx = 0, degree_idx = 0;
    int prev_value = 0, cur_value = 0;

    assert(graph != NULL);
    assert(graph->index != NULL);
    if (graph->degree != NULL) return;

    prev_value = graph->index[index_idx];
    graph->degree = (int *)tcalloc(graph->n, sizeof(int));

    for (index_idx = 1; index_idx < graph->n+1; index_idx++) {
        cur_value = graph->index[index_idx];
        graph->degree[degree_idx] = (cur_value - prev_value);
        prev_value = cur_value;
        degree_idx++;
    }
    sortDegree(graph);
}

void sortDegree(SparseUGraph *graph)
{
    int i, j;
    EdgeList elist;
    assert(graph != NULL);

    // use the EdgeList to sort the nodes by degree
    newEdgeList(&elist, graph->n);
    elist.nodes[ICOL] = graph->degree;
    elist.nodes[JCOL] = graph->node_id;
    sortEdges(&elist, ICOL);
}

void
sampleNodes(SparseUGraph *graph, float sample_rate)
{
    int i, j=0;
    assert(graph != NULL);
    if (graph->sample != NULL) free(graph->sample);

    graph->n_s = (int) ((graph->n * sample_rate) + .5);
    graph->sample = (int *)tcalloc(graph->n_s, sizeof(int));

    for (i = graph->n-1; i > (graph->n - graph->n_s)-1; i--) {
        graph->sample[j++] = graph->node_id[i];
    }
}

void
printDegree(SparseUGraph *graph)
{   // print out node degrees
    assert(graph != NULL);
    assert(graph->degree != NULL);
    int i;

    printf("node:\tdegree\n");
    for (i = 0; i < graph->n; i++) {
        printf("%d:\t%d\n", graph->node_id[i], graph->degree[i]);

    }
}

void
calculateEdgeBetweenness(SparseUGraph *graph, float sample_rate)
{   // calculate edge betweenness using sampling
    // note that multiple calls calculate multiple times
    assert(graph != NULL);
    BFSInfo info;
    int i, j, pred, node, edge_id;
    float flow[graph->m];
    float coeff, c;

    // check for empty graph
    if (graph->n == 0 || graph->m == 0) {
        return;  // TODO: handle this better
    }

    // Calculate degree if not already done, then sample the nodes
    calculateDegreeAndSort(graph);
    sampleNodes(graph, sample_rate);

    // set up edge betweenness storage
    graph->edge_bet = (float *)tcalloc(graph->m, sizeof(float));

    // begin calculations
    for (i = 0; i < graph->n_s; i++) {
        info.src = graph->sample[i];  // perform bfs from src node
        bfs(graph, &info);

        // now work back up from each other node to calculate betweenness
        memset(flow, 0.0, sizeof(flow));
        while (info.stack.size > 0) {
            node = vectorPop(&info.stack);
            coeff = (1.0 + flow[node]) / info.sigma[node];

            // for all predecessors
            for (j = 0; j < info.pred[node].size; j++) {
                pred = info.pred[node].data[j];    // predecessor node id
                c = info.sigma[pred] * coeff;
                flow[pred] += c;
                edge_id = findEdgeId(graph, node, pred);
                graph->edge_bet[edge_id] += c;
            }
        }
    }
}

// print out edge betweenness per edge
void
printEdgeBetweenness(SparseUGraph *graph)
{
    assert(graph != NULL);
    assert(graph->edge_bet != NULL);
    int i, j, edge_id;

    for (i = 0; i < graph->n; i++) {
        for (j = graph->index[i]; j < graph->index[i+1]; j++) {
            edge_id = graph->edge_id[j];
            printf("%d: (%d, %d): %f\n",
                   edge_id, i, graph->edges[j], graph->edge_bet[edge_id]);
        }
    }
}

