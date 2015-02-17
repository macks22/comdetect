///////////////////////////////////////
// EDGE HANDLING

// struct to hold edgelist when converting file to CRS graph
typedef struct {

    int *nodes[2];  // start and end nodes
    int length;     // number of edges
    int *id;        // edge ids

} EdgeList;

#define ICOL    0
#define JCOL    1

#define IEND (elist, idx) (elist->nodes[ICOL][idx])
#define JEND (elist, idx) (elist->nodes[JCOL][idx])


// allocate space for a new edge list of the given length
void newEdgeList(EdgeList *elist, int length);

// reorder id array s.t. the value at each index is the index (0->m-1)
void resetEdgeIds(EdgeList *elist);

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
