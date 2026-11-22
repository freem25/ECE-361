
#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

// Opaque binary search tree type.
// The internal structure is defined only in bst.c.
typedef struct bst bst_t;

// Creates a new, empty binary search tree.
// Takes no arguments. Returns NULL if allocation fails.
// No special assumptions.
bst_t *bst_create(void);

// Frees all tree nodes and the tree itself.
// Takes a tree pointer and returns nothing.
// Passing NULL is allowed.
void bst_destroy(bst_t *t);

// Inserts a key into the tree.
// Takes a tree pointer and integer key.
// Returns 0 on success or -1 on failure.
// Assumes no valid tree is modified if insertion fails.
// Duplicate keys are inserted to the right.
int bst_insert(bst_t *t, int key);

// Searches the tree for a key without printing.
// Takes a tree pointer, key, and optional visit counter.
// Returns true if found, otherwise false.
// A NULL tree returns false and records zero visits.
bool bst_contains(const bst_t *t, int key, size_t *visits);

// Returns the total number of keys in the tree.
// Takes a tree pointer; NULL returns 0.
// No other assumptions.
size_t bst_size(const bst_t *t);

// Returns the height measured in number of nodes.
// Takes a tree pointer; NULL or empty trees return 0.
// A tree containing one node has height 1.
size_t bst_height(const bst_t *t);

#endif
