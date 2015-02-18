#include "graph.h"

void graphKMedoid(SparseUGraph *graph, kMedoidInfo *k_med)
{
    assert(graph != NULL);
    assert(k_med->k < graph->n);
    if(graph->n <= 0) return;

    int i, j;
    BFSInfo bfs;
    DTZ dtz_idx[graph->n];
    //Vector comm[k_med->k];

    genDTZidx(graph, k_med, dtz_idx);
    genKSeeds(graph, k_med, dtz_idx);
    labelDTZidx(graph, k_med, dtz_idx);

    for(i=0; i<graph->n; i++) {
	printf("node:%d\n", dtz_idx[i].n);
	printf("zone:%d\n", dtz_idx[i].label);
    }
    
}

void genDTZidx(SparseUGraph *graph, kMedoidInfo *k_med, DTZ *dtz_idx)
{
    int i, j;

    for(i=0; i<graph->n; i++) {
	dtz_idx[i].n = i;
	dtz_idx[i].label = -1;
	dtz_idx[i].distances = (int *)tcalloc(k_med->z, sizeof(int));
    }
}

void genKSeeds(SparseUGraph *graph, kMedoidInfo *k_med, DTZ *dtz_idx)
{
    int *temp_index;
    int i, j, r;
    
    k_med->seed_nodes = (int *)tcalloc(k_med->k, sizeof(int));
    temp_index = (int *)tcalloc(graph->n, sizeof(int));
    k_med->unlabeled = (int *)tcalloc((graph->n - k_med->k), sizeof(int));
    srand(time(NULL));

    // fill temp array
    for(i=0; i<graph->n; i++) {
	temp_index[i] = i;
    }

    // get random unique node ids
    for(i=0; i<k_med->k; i++) {
	r = (rand() % (graph->n));
	if(temp_index[r] > -1) {
	    k_med->seed_nodes[i] = temp_index[r];
	    dtz_idx[r].label = temp_index[r];
	    temp_index[r] = LABELED;
	} else {
	    i--;
	}
    }

    // compress unlabeled array by removing nodes that have been labeled
    j=0;
    for(i=0; i<graph->n; i++) {
	if(temp_index[i] > -1) {
	    k_med->unlabeled[j] = temp_index[i];
	    j++;
	}
    }
    free(temp_index);
    //debug
    //printArray(k_med->seed_nodes, k_med->k);
    //printArray(k_med->unlabeled, (graph->n - k_med->k));
}

void labelDTZidx(SparseUGraph *graph, kMedoidInfo *k_med, DTZ *dtz_idx)
{
    int i, j, from_labeled, degree;

    srand(time(NULL));

    while(1) {
	// randomly select node from labeled set
	from_labeled = k_med->seed_nodes[rand() % k_med->k];

	// find degree of selected labeled node
	degree = (graph->index[from_labeled+1] - graph->index[from_labeled]);

	printArray(graph->index, graph->n+1);
	// randomly select a neighbor
	//	neighbor = graph->edges[rand() % degree
	break;
    }
}
