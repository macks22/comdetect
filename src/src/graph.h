/********************************************************************/
/*                            INCLUDES                               */
/********************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/********************************************************************/
/*                       STRUCTS AND PROTOTYPES                     */
/********************************************************************/

///////////////////////////////////////
// UTILITY FUNCTIONS

// print error and exit
void error(int err_code);

// attempt to calloc memory, error out if failure
void *tcalloc(size_t nitems, size_t size);

// find the largest number in the array
int findLargest(int *array, int length);

// perform an in-place radix sort on the array; result is ascending order
void radixSort(int *array, int length);

// remove all duplicate values from the integer array
// return the size of the new array
void removeDuplicates(int *array, int *length);

// print an array of integers
void printArray(int *array, int length);


///////////////////////////////////////
// EDGE HANDLING

// struct to hold user input arguments
typedef struct {

    char infile[200];
    int num_clusters;
    char outfile[200];

} InputArgs;

// struct to hold edgelist when converting file to CRS graph
typedef struct {

    int *nodes[2];  // start and end nodes
    int length; // number of edges

} EdgeList;

#define ICOL    0
#define JCOL    1

#define IEND (elist, idx) (elist->nodes[ICOL][idx])
#define JEND (elist, idx) (elist->nodes[JCOL][idx])


// allocate space for a new edge list of the given length
void newEdgeList(EdgeList *elist, int length);

// copy an entire edgelist into a new one
void copyEdgeList(EdgeList *cur, EdgeList *new);

// Free the memory allocated for the edgelist.
void freeEdgeList(EdgeList *elist);

// sort edges by i column or by j column
void sortEdges(EdgeList *elist, int column);

// find largest value in i or j column
int findLargestEndpoint(EdgeList *elist, int column);

// Return an array with all unique node ids sorted in ascending order.
void mapNodeIds(EdgeList *elist, int **idmap, int *num_nodes);

// Print out the edge list (for debugging purposes), up to `num_edges` edges.
void printEdgeList(EdgeList *elist, int num_edges);


///////////////////////////////////////
// GRAPH FUNCTIONS

// Sparse undirected graph.
// Utilizes a non-symmetric CSR format for compression In other words,
// maintain a duplicate copy of each edge in order to facilitate quick
// lookup of neighbors.
//
// Since an undirected graph has symmetric edges, we normally only store
// each once. However, storing each edge as (i, j) and (j, i) allows
// faster lookup of neighbors. If we stored each edge once, and we ask
// who are the neighbors of i, we must look at i's edgelist, and also
// every other node's edgelist to see if i is present in it. If we
// duplicate the edges, then we must only check i's edgelist.
typedef struct {

    int n;          // number of nodes: |V|
    int m;          // number of edges: |E|

    int *id;        // size = |V|; ids for all nodes
    int *index;     // size = |V| + 1
    int *edges;     // size = 2|E|

} SparseUGraph;


// Compress edges from edge list into a compressed row storage (CRS) format
void rowCompressEdges(EdgeList *elist, SparseUGraph *graph);

// read a sparse undirected graph from an edgelist file
void readSparseUGraph(InputArgs *args, SparseUGraph *graph);

// free the sparse undirected graph
void freeSparseUGraph(SparseUGraph *graph);

// write an edgelist for the sparse undirected graph
void writeSparseUGraph(FILE *outfile);

// return 1 if there is an edge from a to b, else 0
int hasEdge(SparseUGraph *graph, int a, int b);

// return shortest path distance from src to target node
// returns -1 if the target is unreachable from the src
int distance(SparseUGraph *graph, int src, int target);

// return the degree of the node
int degree(SparseUGraph *graph, int node);


// Holds information discovered while performing a BFS.
// This will be useful when finding centrality measures.
// It also allows one to trace back the shortest paths
// that were found, by using the distance and parent info.
typedef struct {

    bool *discovered;
    bool *processed;
    int *parent;
    int *distance;

} BFSInfo;


// Perform a BFS on the sparse undirected graph and return
// the information discovered.
void bfs(SparseUGraph *graph, BFSInfo *info);

