#include "graph.h"


float modularity(SparseUGraph *graph, Vector *communities, int num_comm)
{
    int i, j;
    int m;
    int e_i = 0;
    int expected_e = 0;
    float actual_edges = 0.0f, random_edges = 0.0f;


    // iterate through each community
    m = graph->m;
    for (i = 0; i < num_comm; i++) {
        // find num edges within the community
        for (j = 0; j < communities[i].size; j++) {
            e_i += getEdgesInComm(graph, communities[i].data[j]);
            expected_e += getDegreeInNetwork(graph, communities[i].data[j]);
        }
        // add the tally on for the community
        actual_edges += ((float) e_i / (float) (m*2));
        random_edges += ((float) expected_e * (float) expected_e) / (4 * (m * m));

        // reset for the next community
        e_i = 0;
        expected_e = 0;
    }

    return (actual_edges - random_edges);
}

int getEdgesInComm(SparseUGraph *graph, int node) {
    int i, edge_counter=0;

    for (i = graph->index[node]; i < graph->index[node+1]; i++) {
        if (graph->edges[i] >= 0) edge_counter++;
    }
    return edge_counter;
}

int getDegreeInNetwork(SparseUGraph *graph, int node) {
    return graph->degree[node];
}
