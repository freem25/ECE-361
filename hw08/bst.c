
#include "bst.h"
#include <stdlib.h>

// Node structure adapted from the original BinarySearchTree.c.
// Each node stores a key and pointers to its children.
struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

// The BST structure replaces the original global root.
// Each tree keeps its own root and number of nodes.
struct bst {
    struct node *root;
    size_t count;
};

// Helper function to free all nodes in a subtree.
// New function; the original program did not free nodes.
static void destroy_nodes(struct node *current) {
    if (current == NULL) {
        return;
    }

    destroy_nodes(current->leftChild);
    destroy_nodes(current->rightChild);
    free(current);
}

// Helper function to calculate tree height.
// New function; the original program did not calculate height.
static size_t height_nodes(const struct node *current) {
    if (current == NULL) {
        return 0;
    }

    size_t left = height_nodes(current->leftChild);
    size_t right = height_nodes(current->rightChild);

    // Count the current node plus the taller subtree.
    return 1 + (left > right ? left : right);
}

// Creates a new, empty binary search tree.
// New function replacing the original global initialization.
bst_t *bst_create(void) {
    bst_t *t = malloc(sizeof(*t));

    if (t == NULL) {
        return NULL;
    }

    t->root = NULL;
    t->count = 0;

    return t;
}

// Destroys the tree and frees allocated memory.
// New function; the original program had no destroy function.
void bst_destroy(bst_t *t) {
    if (t == NULL) {
        return;
    }

    destroy_nodes(t->root);
    free(t);
}

// Adapted from the original insert() function.
// Smaller keys go left; equal or larger keys go right.
int bst_insert(bst_t *t, int key) {
    if (t == NULL) {
        return -1;
    }

    // Allocate a new node for the key.
    struct node *tempNode = malloc(sizeof(*tempNode));

    if (tempNode == NULL) {
        return -1;
    }

    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    // If the tree is empty, the new node becomes the root.
    if (t->root == NULL) {
        t->root = tempNode;
    } else {
        struct node *current = t->root;
        struct node *parent = NULL;

        // Follow the original insertion loop.
        while (1) {
            parent = current;

            if (key < parent->data) {
                current = current->leftChild;

                if (current == NULL) {
                    parent->leftChild = tempNode;
                    break;
                }
            } else {
                current = current->rightChild;

                if (current == NULL) {
                    parent->rightChild = tempNode;
                    break;
                }
            }
        }
    }

    // Increase the number of stored keys.
    t->count++;
    return 0;
}

// Adapted from the original search() function.
// Counts visited nodes instead of printing them.
// The original search loop is intentionally preserved
// for the crash investigation required in Part 2.
bool bst_contains(const bst_t *t, int key, size_t *visits) {
    if (visits != NULL) {
        *visits = 0;
    }

    if (t == NULL) {
        return false;
    }

    const struct node *current = t->root;

    // Original search loop, retained for Part 2 testing.
    while (current->data != key) {
        if (visits != NULL) {
            (*visits)++;
        }

        if (current->data > key) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }

        if (current == NULL) {
            return false;
        }
    }

    // Count the node where the key was found.
    if (visits != NULL) {
        (*visits)++;
    }

    return true;
}

// Returns the number of keys in the tree.
// New function; the original program did not track size.
size_t bst_size(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }

    return t->count;
}

// Returns the height of the tree in number of nodes.
// New function; uses the recursive height helper.
size_t bst_height(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }

    return height_nodes(t->root);
}
