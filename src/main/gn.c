#include "graph.h"


int
main (int argc, char *argv[])
{
    int k, i;
    float mod;
    SparseUGraph graph;
    InputArgs args;
    Vector *comms = NULL;

    // validate input args
    if (argc < 4) {
        printf("%s: <edgelist-file> <k> <outfile>\n", argv[0]);
        exit(1);
    }

    // check for sample size input
    if (argc == 5) {
        args.sample_rate = strtod(argv[4], NULL);
    } else {
        args.sample_rate = 0.2;
    }

    // read input arguments
    strcpy(args.infile, argv[1]);
    args.num_clusters = atoi(argv[2]);
    strcpy(args.outfile, argv[3]);
    printf("Params: edgelist=%s, num_clusters=%d, outfile=%s\n",
           args.infile, args.num_clusters, args.outfile);

    // read graph and run Girvan Newman
    readSparseUGraph(&args, &graph);
    k = girvanNewman(&graph, args.num_clusters, args.sample_rate, &comms);
    mod = modularity(&graph, comms, k);
    printf("modularity: %f\n", mod);

    // output community memberships
    writeCommunities(graph.id, comms, k, args.outfile);
    printf("community assignment output to %s\n", args.outfile);
    for (i = k-1; i >= 0; i--) {
        freeVector(&comms[i]);
    }

    free(comms);
    freeSparseUGraph(&graph);
    exit(EXIT_SUCCESS);
}

// print out node community membership to outfile
void writeCommunities(int *idmap, Vector *comms, int k, char *outfile)
{
    int i, j;
    FILE *fpout;
    fpout = fopen(outfile, "w");
    if (fpout == NULL) {
        fprintf(stderr, "unable to open output file: %s", outfile);
        error(BAD_FP);
    }
    for (i = 0; i < k; i++) {
        for (j = 0; j < comms[i].size; j++) {
            fprintf(fpout, "%d %d\n", idmap[comms[i].data[j]], i);
        }
    }
}


// Use the Girvan Newman (2004) algorithm to divisely
// cluster the graph into k partitions.
int girvanNewman(SparseUGraph *graph, int k, float sample_rate, Vector **comms)
{
    Vector largest;     // edges with highest betweenness
    int edges_cut=0;    // num edges cut so far
    int iteration=1;    // which iteration the algorithm is on
    int src, dest, i;
    int num_comms=0, checkpoint=k;

    assert(graph != NULL);
    if (graph->m <= 0) return;

    while (num_comms < k && edges_cut < graph->m) {
        printf("running iteration %d; edges cut so far: %d\n",
               iteration, edges_cut);

        // Calculate degree, then sample the nodes
        calculateDegreeAndSort(graph);
        sampleNodes(graph, sample_rate);
        if (graph->n_s <= 0) {
            printf("0 nodes are sampled with a sample rate of %f\n", sample_rate);
            exit(INVALID_SAMPLE_SIZE);
        }

        // calculate edge betweenness and cut edge(s) with highest value(s)
        calculateEdgeBetweenness(graph, &largest);
        for (i = 0; i < largest.size; i++) {
            src = largest.data[i++];
            dest = largest.data[i];
            cutEdge(graph, src, dest, iteration);
        }
        edges_cut += largest.size / 2;
        iteration++;
        freeVector(&largest);
        // printSparseUGraph(graph, graph->n);

        // if we're at a checkpoint, check the number of communities
        if (edges_cut >= checkpoint) {
            num_comms = labelCommunities(graph, comms);
            // reset if not done
            if (num_comms < k && edges_cut < graph->m) {
                for (i = num_comms-1; i >= 0; i--) {
                    freeVector(&(*comms)[i]);
                }
                free(*comms);
                checkpoint += (k - num_comms);  // set next checkpoint
                printf("communities found so far: %d\n", num_comms);
            }
        }
    }
    printf("completed %d iterations; total edges cut: %d\n",
           iteration-1, edges_cut);
    printf("total communities found: %d\n", num_comms);
    return num_comms;
}

// Cut an edge from the graph by marking it with the negative
// of the iteration number in which it was cut.
void cutEdge(SparseUGraph *graph, int src, int dest, int iteration)
{
    int i;
    for (i = graph->index[src]; i < graph->index[src+1]; i++) {
        if (graph->edges[i] == dest) {
            graph->edges[i] = -iteration;
        }
    }

    for (i = graph->index[dest]; i < graph->index[dest+1]; i++) {
        if (graph->edges[i] == src) {
            graph->edges[i] = -iteration;
        }
    }
}

// Build up the communities from the divided graph
// using a union-find data structure
int labelCommunities(SparseUGraph *graph, Vector **comms)
{
    assert(graph != NULL);
    int i, idx, j, root, prev_root;
    int k, comm_id;
    int *roots, *node_ids;
    EdgeList elist;
    UnionFind *uf = uf_create(graph->n);

    // build disjoint sets
    for (i = 0; i < graph->n; i++) {
        for (idx = graph->index[i]; idx < graph->index[i+1]; idx++) {
            j = graph->edges[idx];
            if (j >= 0) {  // ignore cut edges
                uf_union(uf, i, j);
            }
        }
    }

    // find root of each node
    roots = tcalloc(graph->n, sizeof(int));
    for (i = 0; i < graph->n; i++) {
        roots[i] = uf_root(uf, i);
    }

    // assign community ids (naive approach)
    node_ids = tcalloc(graph->n, sizeof(int));
    for (i = 0; i < graph->n; i++) {
        node_ids[i] = i;
    }

    // use the EdgeList to sort in tandem
    // TODO: move to a more generic sorting function
    // this allocates a bunch of extra memory and sorts the id array
    // along with the degree and node_id arrays, so it wastes quite a
    // lot of resources.
    elist.length = graph->n;
    elist.id = tcalloc(graph->n, sizeof(int));
    elist.nodes[ICOL] = roots;
    elist.nodes[JCOL] = node_ids;
    sortEdges(&elist, ICOL);
    elist.nodes[ICOL] = NULL;
    elist.nodes[JCOL] = NULL;

    // get number of communities
    memcpy(elist.id, roots, graph->n * sizeof(int));
    k = graph->n;
    removeDuplicates(elist.id, &k);
    free(elist.id);

    // now accumulate all community members
    j = 0;
    prev_root = -1;
    *comms = tcalloc(k, sizeof(Vector));
    for (i = 0; i < k; i++) {
        newVector(&(*comms)[i]);
        root = roots[j];
        vectorAppend(&(*comms)[i], node_ids[j++]);
        while (roots[j] == root) {
            vectorAppend(&(*comms)[i], node_ids[j++]);
        }
    }
    return k;
}
