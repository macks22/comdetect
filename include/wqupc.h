///////////////////
// wqupc.h

typedef struct {
    int *nodes;
    int *sizes;
    int size;
} UnionFind;

/* Create a new UnionFind struct */
UnionFind * uf_create(int size);

/* Free up all memory for the UnionFind data structure. */
void uf_destroy(UnionFind *uf);

/* Return the root of node n. */
int uf_root(UnionFind *uf, int id);

/* Join the two nodes together. This implementation is O(lgn). */
void uf_union(UnionFind *uf, int n1, int n2);

/* Return 1 if the two nodes are connected, else 0. O(lgn). */
int uf_find(UnionFind *uf, int n1, int n2);

/* Print out the nodes of the UnionFind data structure. */
void uf_print(UnionFind *uf);
