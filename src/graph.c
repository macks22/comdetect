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

    // Now copy the edgelist and sort the copy on the j column
    copyEdgeList(elist_i, &elist_j);
    sortEdges(&elist_j, JCOL);

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
                if (i_end < j_end) {
                    graph->edges[edge_idx++] = i_end;
                    i_idx++;
                } else {
                    graph->edges[edge_idx++] = j_end;
                    j_idx++;
                }
            } else {
                // add i value
                graph->edges[edge_idx++] = i_end;
                i_idx++;
            }
        } else if (j == cur_id) {  // add j value
            graph->edges[edge_idx++] = elist_j.nodes[ICOL][j_idx++];
        } else {  // done with this node
            cur_id = graph->id[id_idx++];
        }
    }

    // debug
    assert(id_idx-1 == graph->n+1);
    printf("graph index:\n");
    printArray(graph->index, graph->n+1);
    printf("graph edgelist:\n");
    printArray(graph->edges, graph->m*2);
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
    printArray(graph->id, num_ids);
    assert(graph->n == num_ids);

    // compress edgelist rows to construct index and edge list
    rowCompressEdges(&elist, graph);

    // Write out the ID array; we won't be using it while processing.
    // It can be used later to translate the output (in node indices)
    // to the input node IDs.
    // storeAndFreeNodeIds(graph);
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
