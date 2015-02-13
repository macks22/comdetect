#include "comdetect.h"


int main (int argc, char *argv[]){
    ctrlType ctrl;
    crsMatrix crs_mat;

    // Setting this parameter greater than 1 will save the ID array out to file
    // storedIDAry.txt and free the id array in order to save look up time -
    // NOTE: translation will need to be done later if this is used
    int storeIDAry = 1;

    if (argc != 4) {
        printf("Usage: infile num_clusters outfile\n");
        exit(1);
    } else {
        strcpy(ctrl.infile, argv[1]);
        ctrl.num_clusters = atoi(argv[2]);
        strcpy(ctrl.outfile, argv[3]);

        printf("Parameters: infile=%s num_clusters=%d outfile=%s\n",
               ctrl.infile, ctrl.num_clusters, ctrl.outfile);

        readCRSMatrix(&ctrl, &crs_mat, storeIDAry);
        printCRSMatrix(&crs_mat, storeIDAry);
    }
}
