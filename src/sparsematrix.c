#include "comdetect.h"


void freeSparseMatrix (sparseComDetectDB *cddb) {
    free (cddb->id);
    free (cddb->index);
    free (cddb->edges);
}

void readSparseMatrixFile (ctrlType *ctrl, sparseComDetectDB *sparse_cddb, int storeIDAry) {
    FILE *fpin;
    int srcNodeID, destNodeID, prevSrcNodeID = -1;
    int id_ary_counter = 0;
    int index_ary_counter = 0;
    int edge_ary_counter = 0;
    fpin = fopen (ctrl->infile, "r");
    fscanf(fpin, "%d %d", &sparse_cddb->num_nodes, &sparse_cddb->num_edges);

    printf("num_nodes=%d num_edges=%d\n", sparse_cddb->num_nodes, sparse_cddb->num_edges);

    // Allocate Space
    sparse_cddb->id = (int *)malloc(sizeof(int) * sparse_cddb->num_nodes);
    sparse_cddb->index = (int *)malloc(sizeof(int) * (sparse_cddb->num_nodes + 1));
    sparse_cddb->edges = (int *)malloc(sizeof(int) * sparse_cddb->num_edges);

    while (!feof (fpin)){
        fscanf(fpin,"%d %d", &srcNodeID, &destNodeID);
        if (prevSrcNodeID != srcNodeID) { // new source node

            //add new node id to id array
            sparse_cddb->id[id_ary_counter] = srcNodeID;
            prevSrcNodeID = srcNodeID;
            id_ary_counter++;

            // add first edge to edgelist
            sparse_cddb->index[index_ary_counter] = edge_ary_counter;
            index_ary_counter ++;
        }
        sparse_cddb->edges[edge_ary_counter] = destNodeID;
        edge_ary_counter++;
    }

    if (storeIDAry == 1) {
        storeAndFreeIDAry(ctrl, sparse_cddb);
    }
}

void storeAndFreeIDAry(ctrlType *ctrl, sparseComDetectDB *sparse_cddb) {
    int i;
    FILE *fpout;
    char store_file[50] = "../output_and_stored_id_ary/storedIDAry.txt";

    fpout = fopen(store_file, "w");
    for(i=0; i<sparse_cddb->num_nodes; i++) {
      fprintf(fpout, "%d\n", sparse_cddb->id[i]);
    }
    fclose(fpout);

    free(sparse_cddb->id);
    printf("ID array stored in %s\n", store_file);
}


// Debugging
void printSparseMatrix (sparseComDetectDB *sparse_cddb, int storeIDAry) {
    int i,j,k;

    if(storeIDAry < 1) {
        for(i=0; i<20; i++) {
            printf("%d:%d ", i, sparse_cddb->id[i]);
        }
        printf("\n\n");
    }

    for(j=0; j<20; j++) {
        printf("%d:%d ", j, sparse_cddb->index[j]);
    }
    printf("\n\n");

    for(k=0; k<20; k++) {
        printf("%d:%d ", k, sparse_cddb->edges[k]);
    }
    printf("\n\n");
}
