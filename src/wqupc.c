// Union-Find using the weighted quick-union strategy with path compression.
// Find is O(lgn): proportional to the depth of the tree.
// Union is O(lgn) for the same reason.
// Depth guaranteed to be at most lgN
// - since trees double at each union (* 2)
// - start at size 2^0 = 1
// - can only reach size N
// - solve: 2^x = N
// - solution: lgN

#include "graph.h"


UnionFind * uf_create(int size) {
    /* Create a new UnionFind struct */
    int i;
    UnionFind *uf = calloc(1, sizeof(UnionFind));
    uf->nodes = calloc(size, sizeof(int));
    uf->sizes = calloc(size, sizeof(int));
    uf->size = size;

    // initialize all node ids to their indices
    // and all sizes to 1 (size of tree with 1 node)
    for (i = 0; i < size; i++) {
        uf->nodes[i] = i;
        uf->sizes[i] = 1;
    }
    return uf;
}

void uf_destroy(UnionFind *uf) {
    /* Free up all memory for the UnionFind data structure. */
    free(uf->nodes);
    free(uf->sizes);
    free(uf);
}

int uf_root(UnionFind *uf, int id) {
    /* Return the root of node n. */
    if (id >= uf->size) {
        printf("indices out of bounds in `uf_root`\n");
        exit(1);
    }
    while (id != uf->nodes[id]) {
        uf->nodes[id] = uf->nodes[uf->nodes[id]];  // path compression
        id = uf->nodes[id];
    }
    return id;
}

void uf_union(UnionFind *uf, int n1, int n2) {
    /* Join the two nodes together.
     * This implementation is O(lgn).
     */
    int i, root1, root2;
    if (n1 >= uf->size || n2 >= uf->size) {
        return;
    }
    root1 = uf_root(uf, n1);
    root2 = uf_root(uf, n2);

    // change root of shorter tree to root of taller
    if (uf->sizes[root1] > uf->sizes[root2]) {  // 1st tree taller than 2nd
        uf->nodes[root2] = uf->nodes[root1];
        uf->sizes[root1] += uf->sizes[root2];  // added root2 tree to root1 tree
    } else {  // 2nd tree taller, or same sizes
        uf->nodes[root1] = uf->nodes[root2];
        uf->sizes[root2] += uf->sizes[root1];  // added root1 tree to root2 tree
    }
}

int uf_find(UnionFind *uf, int n1, int n2) {
    /* Return 1 if the two nodes are connected, else 0.
     * This implementation is O(lgn).
     */
    if (n1 >= uf->size || n2 >= uf->size) {
        return 0;
    }
    return uf_root(uf, n1) == uf_root(uf, n2);
}

void uf_print(UnionFind *uf) {
    /* Print out the nodes of the UnionFind data structure. */
    int i;
    for (i = 0; i < uf->size; i++) {
        printf("%d", uf->nodes[i]);
        if (i < uf->size-1) {
            printf(" ");
        }
    }
    printf("\n");
}

void test1 () {
    UnionFind *uf = uf_create(10);
    printf("start:  ");
    uf_print(uf);

    uf_union(uf, 1, 2);
    assert(uf_find(uf, 1, 2));
    printf("(1, 2): ");
    uf_print(uf);

    uf_union(uf, 3, 4);
    assert(uf_find(uf, 3, 4));
    printf("(3, 4): ");
    uf_print(uf);

    uf_union(uf, 8, 9);
    assert(uf_find(uf, 8, 9));
    printf("(8, 9): ");
    uf_print(uf);

    uf_union(uf, 1, 8);
    assert(uf_find(uf, 1, 8));
    assert(uf_find(uf, 2, 8));
    assert(uf_find(uf, 2, 9));
    assert(uf_find(uf, 9, 1));
    printf("(1, 8): ");
    uf_print(uf);

    printf("end:    ");
    uf_print(uf);
    uf_destroy(uf);
}
