#include "graph.h"


int
main (int argc, char *argv[])
{
    EdgeList elist, new_elist;
    SparseUGraph graph;
    InputArgs args;
    int i = 0, j = 0, size = 10;
    int nums[size];
    int *node_idmap;

    ////////////////////////////////
    // TEST GRAPH READING

    if (argc != 2) {
        printf("%s: <edgelist-file>\n", argv[0]);
        exit(1);
    } else {
        strcpy(args.infile, argv[1]);
        printf("Params: edgelist=%s\n", args.infile);
        readSparseUGraph(&args, &graph);
        // printSparseUGraph(&graph);
    }

    ////////////////////////////////
    // TESTING UTIL FUNCTIONS

    // fill int array with junk
    // j = 0;
    // for (i = 0; i < size; i++) {
    //     nums[i] = j;
    //     if (i % 2 == 0) j++;
    // }
    // printArray(nums, size);

    // // now remove duplicates
    // removeDuplicates(nums, &size);
    // printArray(nums, size);

    // ////////////////////////////////
    // // TESTING EDGELIST FUNCTIONS

    // // initialize new edge list
    // size = 10;
    // newEdgeList(&elist, size);

    // // populate edgelist with junk data
    // j = size;
    // for (i = 0; i < size; i++) {
    //     elist.nodes[ICOL][i] = i;
    //     elist.nodes[JCOL][i] = j--;
    // }
    // printEdgeList(&elist, size);

    // // test largest endpoint finding for both i and j columns
    // i = findLargestEndpoint(&elist, ICOL);
    // printf("largest value in i column: %d\n", i);
    // j = findLargestEndpoint(&elist, JCOL);
    // printf("largest value in j column: %d\n", j);

    // // test sorting from j direction
    // sortEdges(&elist, JCOL);
    // printf("edges sorted by j column:\n");
    // printEdgeList(&elist, size);

    // // test sorting from i direction
    // sortEdges(&elist, ICOL);
    // printf("edges sorted by i column:\n");
    // printEdgeList(&elist, size);

    // // test copy edgelist
    // copyEdgeList(&elist, &new_elist);
    // printf("copy of edge list:\n");
    // printEdgeList(&new_elist, size);

    // // test out the node id mapping
    // mapNodeIds(&elist, &node_idmap, &size);
    // printf("number of unique nodes: %d\n", size);
    // printArray(node_idmap, size);

    // // clean up and exit
    // freeEdgeList(&elist);
    return 0;
}
