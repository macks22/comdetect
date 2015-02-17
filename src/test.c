#include "graph.h"


void hash_test();


int
main (int argc, char *argv[])
{
    EdgeList elist, new_elist;
    SparseUGraph graph;
    InputArgs args;
    Vector vec;
    BFSInfo info;
    int i = 0, j = 0, size = 10;
    int nums[size];
    int *node_idmap;
    int dest, par;
    float mod = 0.0;
    Vector communities[2];
    int num_comm =2;
    float sample_percentage = 0.2f;


    ////////////////////////////////
    // TEST GRAPH READING

    hcreate(30);
    addNodeIdToMap(10, 0);
    //addNodeIdToMap(16, 1);
    //addNodeIdToMap(21, 2);
    //addNodeIdToMap(32, 3);
    //addNodeIdToMap(50, 4);
    lookupNodeId(10, &dest);
    printf("id %d mapped to %d\n", 10, dest);

    // if (argc != 2) {
    //     printf("%s: <edgelist-file>\n", argv[0]);
    //     exit(1);
    // }

    // // read graph
    // strcpy(args.infile, argv[1]);
    // printf("Params: edgelist=%s\n", args.infile);
    // readSparseUGraph(&args, &graph);
    // printSparseUGraph(&graph, graph.n);

    // ///////////////////////////////////////
    // // TEST EDGE BETWEENNESS

    // calculateEdgeBetweenness(&graph, 0.2);
    // printf("*** Calculate degree ***\n");
    // printDegree(&graph);
    // printf("*** Calculate degree ***\n");
    // printf("*** Node Sample List ***\n");
    // printArray(graph.sample, graph.n_s);
    // printf("*** Node Sample List ***\n");
    // printEdgeBetweenness(&graph);

    ///////////////////////////////////
    // TEST BFS

    // info.src = 0;      // start search from node 0
    // bfs(&graph, &info);
    // printf("nodes in non-increasing distance from src:\n");
    // printBFSStack(&info);
    // dest = 4;
    // i = info.distance[dest];
    // printf("distance from %d --> %d: %d\n", info.src, dest, i);

    // // print out predecessor info
    // printPredecessors(&info);
    // printShortestPathCounts(&info);

    // now follow the path back up
    // printShortestPath(&info, dest);

    ////////////////////////////////
    // TESTING MODULARITY

    // for(i=0; i<num_comm; i++) {
	// newVector(&communities[i]);
	// for(j=0; j<3; j++) {
	//     vectorAppend(&communities[i], j);
	// }
    // }
    // mod = modularity(&graph, communities, num_comm);
    // printf("modularity:%f\n", mod);

    ////////////////////////////////
    // TESTING VECTOR

    // newVector(&vec);
    // for (i = 0; i < vec.cap; i++) {
    //     vectorAppend(&vec, i);
    // }
    // assert(vec.cap == INIT_VECTOR_SIZE);
    // assert(vec.size == vec.cap);
    // vectorAppend(&vec, ++i);
    // assert(vec.cap == INIT_VECTOR_SIZE*2);

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

void hash_test()
{
    ENTRY e, *ep;
    int i = 10, j = 0;
    char node_id[10];

    hcreate(30);
    sprintf(node_id, "%d", i);
    e.key = node_id;
    e.data = (void *)&j;
    ep = hsearch(e, ENTER);

    ep = hsearch(e, FIND);
    if (ep == NULL) {
        printf("not found\n");
    } else {
        printf("key: %s\nvalue: %d\n", ep->key, *((int *)ep->data));
    }
}
