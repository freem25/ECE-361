
#include "bst.h"
#include <stdlib.h>

// Node structure adapted from the original BinarySearchTree.c.
// Each node stores a key and pointers to its children.
struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

// Replaces the original global root variable.
// Each BST has its own root and node count.
struct bst {
    struct node *root;
    size_t count;
};

// New helper function to free every node in a subtree.
// The original program did not free allocated nodes.
static void destroy_nodes(struct node *current) {
    if (current == NULL) {
        return;
    }

    // Free both child subtrees before the current node.
    destroy_nodes(current->leftChild);
    destroy_nodes(current->rightChild);
    free(current);
}

// New helper function to calculate tree height.
// Height is measured by the number of nodes.
static size_t height_nodes(const struct node *current) {
    if (current == NULL) {
        return 0;
    }

    size_t left = height_nodes(current->leftChild);
    size_t right = height_nodes(current->rightChild);

    // Count this node plus the taller subtree.
    return 1 + (left > right ? left : right);
}

// Creates a new, empty binary search tree.
// New function replacing the original global initialization.
bst_t *bst_create(void) {
    bst_t *t = malloc(sizeof(*t));

    // Return NULL if memory allocation fails.
    if (t == NULL) {
        return NULL;
    }

    t->root = NULL;
    t->count = 0;

    return t;
}

// Frees the entire tree and its allocated memory.
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

    // Allocate memory for a new node.
    struct node *tempNode = malloc(sizeof(*tempNode));

    if (tempNode == NULL) {
        return -1;
    }

    // Initialize the new node.
    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    // The first inserted node becomes the root.
    if (t->root == NULL) {
        t->root = tempNode;
    } else {
        struct node *current = t->root;
        struct node *parent = NULL;

        // Follow the original insertion loop.
        while (1) {
            parent = current;

            // Smaller keys are inserted to the left.
            if (key < parent->data) {
                current = current->leftChild;

                if (current == NULL) {
                    parent->leftChild = tempNode;
                    break;
                }
            } else {
                // Equal or larger keys are inserted to the right.
                current = current->rightChild;

                if (current == NULL) {
                    parent->rightChild = tempNode;
                    break;
                }
            }
        }
    }

    // Update the total number of stored keys.
    t->count++;
    return 0;
}

// Adapted from the original search() function.
// Searches without printing and counts visited nodes.
// Fixed: checks for NULL before accessing node data.
bool bst_contains(const bst_t *t, int key, size_t *visits) {
    // Reset the optional visit counter.
    if (visits != NULL) {
        *visits = 0;
    }

    // A NULL tree cannot contain any keys.
    if (t == NULL) {
        return false;
    }

    const struct node *current = t->root;

    // Continue searching while a valid node exists.
    while (current != NULL) {
        // Count each node compared during the search.
        if (visits != NULL) {
            (*visits)++;
        }

        // The requested key was found.
        if (current->data == key) {
            return true;
        }

        // Follow the appropriate child pointer.
        if (key < current->data) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    // Reaching NULL means the key was not found.
    return false;
}

// Returns the total number of keys in the tree.
// New function; the original program did not track size.
size_t bst_size(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }

    return t->count;
}

// Returns the height measured in number of nodes.
// New function; uses the recursive height helper.
size_t bst_height(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }

    return height_nodes(t->root);
}
