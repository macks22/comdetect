#include "comdetect.h"


void freeCRSMatrix(crsMatrix *mat) {
    free(mat->id);
    free(mat->index);
    free(mat->edges);
}

void readCRSMatrix(ctrlType *ctrl, crsMatrix *crs_mat, int storeIDAry) {
    FILE *fpin;
    int srcNodeID, destNodeID, prevSrcNodeID = -1;
    int id_ary_counter = 0;
    int index_ary_counter = 0;
    int edge_ary_counter = 0;
    fpin = fopen(ctrl->infile, "r");
    fscanf(fpin, "%d %d", &crs_mat->num_nodes, &crs_mat->num_edges);

    printf("num_nodes=%d num_edges=%d\n", crs_mat->num_nodes, crs_mat->num_edges);

    // Allocate Space
    crs_mat->id = (int *)malloc(sizeof(int) * crs_mat->num_nodes);
    crs_mat->index = (int *)malloc(sizeof(int) * (crs_mat->num_nodes + 1));
    crs_mat->edges = (int *)malloc(sizeof(int) * crs_mat->num_edges);

    while (!feof (fpin)) {
        fscanf(fpin, "%d %d", &srcNodeID, &destNodeID);
        if (prevSrcNodeID != srcNodeID) { // new source node

            //add new node id to id array
            crs_mat->id[id_ary_counter] = srcNodeID;
            prevSrcNodeID = srcNodeID;
            id_ary_counter++;

            // add first edge to edgelist
            crs_mat->index[index_ary_counter] = edge_ary_counter;
            index_ary_counter ++;
        }
        crs_mat->edges[edge_ary_counter] = destNodeID;
        edge_ary_counter++;
    }

    if (storeIDAry == 1) {
        storeAndFreeIDAry(ctrl, crs_mat);
    }
}

void storeAndFreeIDAry(ctrlType *ctrl, crsMatrix *crs_mat) {
    int i;
    FILE *fpout;
    char store_file[50] = "../output_and_stored_id_ary/storedIDAry.txt";

    fpout = fopen(store_file, "w");
    for(i=0; i < crs_mat->num_nodes; i++) {
        fprintf(fpout, "%d\n", crs_mat->id[i]);
    }
    fclose(fpout);

    free(crs_mat->id);
    printf("ID array stored in %s\n", store_file);
}


// Debugging
void printCRSMatrix(crsMatrix *crs_mat, int storeIDAry) {
    int i;

    if (storeIDAry < 1) {
        for(i=0; i<20; i++) {
            printf("%d:%d ", i, crs_mat->id[i]);
        }
        printf("\n\n");
    }

    for (i=0; i<20; i++) {
        printf("%d:%d ", i, crs_mat->index[i]);
    }
    printf("\n\n");

    for (i=0; i<20; i++) {
        printf("%d:%d ", i, crs_mat->edges[i]);
    }
    printf("\n\n");
}
