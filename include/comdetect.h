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
#include<sys/time.h>

/********************************************************************/
/*                            STRUCTS                               */
/********************************************************************/

typedef struct{
  
  char infile[200];
  int num_clusters;
  char ofile[200];
  
}ctrlType;

typedef struct{
  
    int num_nodes;
    int num_edges;

    int *id;    // size = n
    int *index; // size = n + 1
    int *edges; // size = |e|
    
}sparseComDetectDB;

/********************************************************************/
/*                       FUNCTION PROTOTYPES                        */
/********************************************************************/

// frees all 3 CSR arrays
void freeSparseMatrix (sparseComDetectDB *cddb);

// reads in the passed in input file and puts the data into CSR sparse matrix form
void readSparseMatrixFile (ctrlType *ctrl, sparseComDetectDB *sparse_cddb, int storeIDAry);

// function used primarily for debugging
void printSparseMatrix(sparseComDetectDB *sparse_cddb, int storeIDAry);

// added functionality to decrease computational lookup times by saving out the id array to a file and performing translation in post processing
void storeAndFreeIDAry(ctrlType *ctrl, sparseComDetectDB *sparse_cddb);
