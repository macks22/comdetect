#include "graph.h"


// Compress edges from edge list into a compressed row storage (CRS) format
void
rowCompressEdges(EdgeList *elist_i, SparseUGraph *graph)
{
    EdgeList elist_j;
    int edge_idx=0, id_idx=0, index_idx=0;
    int cur_id, prev_id = -1;
    int i_end, j_end, i, j, i_idx=0, j_idx=0;

    // Allocate space for edge and node storage.
    // Note that the `mapNodeIds` function allocates space for the id array.
    graph->index = (int *)tcalloc(graph->n+1, sizeof(int));
    graph->edges = (int *)tcalloc(graph->m*2, sizeof(int));
    graph->edge_id = (int *)tcalloc(graph->m*2, sizeof(int));

    // Now copy the edgelist and sort the copy on the j column
    copyEdgeList(elist_i, &elist_j);
    sortEdges(&elist_j, JCOL);

    // debug
    // printf("j direction edgelist:\n");
    // printEdgeList(&elist_j, graph->m);

    // For each node in the id array, move through the 2 edge
    // lists to build up the index and edges arrays.
    cur_id = graph->id[id_idx];
    while (id_idx <= graph->n+1) {
        if (prev_id != cur_id) {
            // move up in index array
            graph->index[index_idx++] = edge_idx;
            prev_id = cur_id;
        }

        i = elist_i->nodes[ICOL][i_idx];
        j = elist_j.nodes[JCOL][j_idx];
        if (i == cur_id) {
            i_end = elist_i->nodes[JCOL][i_idx];
            if (j == cur_id) {
                // add whichever is smaller
                j_end = elist_j.nodes[ICOL][j_idx];
                if (i_end < j_end) {  // i endpoint is smaller
                    graph->edges[edge_idx] = i_end;
                    graph->edge_id[edge_idx++] = elist_i->id[i_idx++];
                } else {  // j endpoint is smaller
                    graph->edges[edge_idx] = j_end;
                    graph->edge_id[edge_idx++] = elist_j.id[j_idx++];
                }
            } else {
                // add i value
                graph->edges[edge_idx] = i_end;
                graph->edge_id[edge_idx++] = elist_i->id[i_idx++];
            }
        } else if (j == cur_id) {  // add j value
            graph->edges[edge_idx] = elist_j.nodes[ICOL][j_idx];
            graph->edge_id[edge_idx++] = elist_j.id[j_idx++];
        } else {  // done with this node
            cur_id = graph->id[id_idx++];
        }
    }

    // debug
    assert(id_idx-1 == graph->n+1);
}

void
readSparseUGraph(InputArgs *args, SparseUGraph *graph)
{
    FILE *fpin;
    EdgeList elist;
    int num_ids, edge_idx=0;

    // read the first line, which contains #nodes #edges
    fpin = fopen(args->infile, "r");
    fscanf(fpin, "%d %d", &graph->n, &graph->m);
    printf("reading: %d nodes, %d edges\n", graph->n, graph->m);

    // allocate a new edgelist to work with while converting to CRS
    newEdgeList(&elist, graph->m);

    // now read in the edgelist, one at a time
    while (!feof(fpin)) {
        fscanf(fpin, "%d %d",
               &elist.nodes[ICOL][edge_idx],
               &elist.nodes[JCOL][edge_idx]);
        edge_idx++;
    }
    printf("# edges read: %d\n", --edge_idx);
    printEdgeList(&elist, edge_idx);
    assert(graph->m == edge_idx);

    // get listing of all unique node ids
    mapNodeIds(&elist, &graph->id, &num_ids);
    assert(graph->n == num_ids);

    // compress edgelist rows to construct index and edge list
    rowCompressEdges(&elist, graph);
    graph->node_id = (int *)tcalloc(graph->n, sizeof(int));
    graph->degree = NULL;

    // Write out the ID array; we won't be using it while processing.
    // It can be used later to translate the output (in node indices)
    // to the input node IDs.
    // storeAndFreeNodeIds(graph);
    freeEdgeList(&elist);
    fclose(fpin);
}

void
storeAndFreeNodeIds(SparseUGraph *graph)
{
    int i;
    FILE *fpout;
    char store_file[50] = "../output/node_ids.txt";

    fpout = fopen(store_file, "w");
    for (i=0; i < graph->n; i++) {
        fprintf(fpout, "%d\n", graph->id[i]);
    }

    fclose(fpout);
    free(graph->id);
    printf("Node id array stored in %s\n", store_file);
}

// print the graph, up to `num_nodes`
void
printSparseUGraph(SparseUGraph *graph, int num_nodes)
{
    assert(graph != NULL);
    int i, j;

    if (graph->n <= 0) return;
    num_nodes = (num_nodes > graph->n) ? graph->n : num_nodes;

    printf("node id mapping:\n");
    printArray(graph->id, graph->n);
    printf("graph index:\n");
    printArray(graph->index, graph->n+1);
    printf("graph edgelist:\n");

    for (i = 0; i < num_nodes; i++) {
        for (j = graph->index[i]; j < graph->index[i+1]; j++) {
            printf("%d: (%d, %d)\n", graph->edge_id[j], i, graph->edges[j]);
        }
    }
}

// convert a graph to an edgelist format; this loses the id list,
// so be sure to save it first if you want to use it later
void
graphToEdgeList(SparseUGraph *graph, EdgeList *elist)
{
    int i, j, edge_idx=0;

    // allocate space for edge list
    newEdgeList(elist, graph->m);

    // now build up edges
    for (i = 0; i < graph->n; i++) {
        for (j = graph->index[i]; j < graph->index[i+1]; j++) {
            elist->nodes[ICOL][edge_idx] = i;
            elist->nodes[JCOL][edge_idx] = graph->edges[j];
            elist->id[edge_idx] = graph->edge_id[j];
            edge_idx++;
        }
    }
}

void
calculateDegree(SparseUGraph *graph)
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
}

void sortDegree(SparseUGraph *graph)
{
    int i, j;
    EdgeList elist;

    newEdgeList(&elist, graph->n);
    for (i = 0; i < graph->n; i++) {
        graph->node_id[i] = i;
    }

    // use the EdgeList to sort the nodes by degree
    elist.nodes[ICOL] = graph->degree;
    elist.nodes[JCOL] = graph->node_id;
    sortEdges(&elist, ICOL);
}

// print out node degrees
void
printDegree(SparseUGraph *graph)
{
    assert(graph != NULL);
    assert(graph->degree != NULL);
    int i;

    printf("node:\tdegree\n");
    for (i = 0; i < graph->n; i++) {
        printf("%d:\t%d\n", graph->node_id[i], graph->degree[i]);
    }
}
