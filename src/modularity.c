#include "graph.h"

float modularity(SparseUGraph *graph, Vector *communities, int num_comm)
{
    int i, j;
    int m;
    int e_i = 0;
    float actual_edges = 0.0, random_edges = 0.0;

    m = graph->m;

    for(i=0; i<num_comm; i++) {
	printf("community %d size:%d\n", i, communities[i].size);
	for(j=0; j<communities[i].size; j++) {
	    printf("community:%d node:%d\n", i, communities[i].data[j]);
	    e_i += getEdges(graph, communities[i].data[j]);
	    printf("e_i:%d\n", e_i);
	}

	actual_edges += ((float) e_i / (float) m);
	printf("actual_edges:%f\n", actual_edges);
	e_i = 0;
    }
    
    return actual_edges;
}

int getEdges(SparseUGraph *graph, int node) {
    int i, j;
    int start_idx, end_idx;
    int edge_counter = 0;
    start_idx = graph->index[node];
    end_idx = graph->index[node+1];

    printf("start:%d end:%d\n", start_idx, end_idx);

    for(i=start_idx; i<end_idx; i++) {
	if(graph->edges[i] > -1) edge_counter++;
    }

    return edge_counter;
}

    
