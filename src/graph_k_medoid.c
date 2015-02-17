#include "graph.h"

void graphKMedoid(SparseUGraph *graph, InputArgs *args)
{
    assert(graph != NULL);
    if(graph->n <= 0) return;

    int i, j;
    int k;    // num clusters
    int r;    // random number between 0 and num nodes

    k = args->num_clusters;
    srand(time(NULL));

    for(i=0; i<num_clusters; i++) {
	r = (rand() % graph->n);
	printf("rand:%d\n", r);
    }
}

