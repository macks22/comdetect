#define _GNU_SOURCE

/********************************************************************/
/*                            INCLUDES                               */
/********************************************************************/

#include<stddef.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>
#include<stdio.h>
#include<string.h>

/********************************************************************/
/*                            STRUCTS                               */
/********************************************************************/

typedef struct{

    char infile[200];
    int num_clusters;
    char outfile[200];

} ctrlType;

typedef struct{

    int num_nodes;
    int num_edges;

    int *id;    // size = |V|
    int *index; // size = |V| + 1
    int *edges; // size = |E|

} crsMatrix;

/********************************************************************/
/*                       FUNCTION PROTOTYPES                        */
/********************************************************************/

// frees all 3 CRS arrays
void freeCRSMatrix (crsMatrix *mat);

// reads in the passed in input file and puts the data into CRS crs matrix form
void readCRSMatrix (ctrlType *ctrl, crsMatrix *crs_mat, int storeIDAry);

// function used primarily for debugging
void printCRSMatrix(crsMatrix *crs_mat, int storeIDAry);

// added functionality to decrease computational lookup times by saving out the
// id array to a file and performing translation in post processing
void storeAndFreeIDAry(ctrlType *ctrl, crsMatrix *crs_mat);
