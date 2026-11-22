
#include "bst.h"
#include <stdio.h>

// Reads integers from standard input and stores them
// in a binary search tree in their original order.
int main(void) {
    // Create an empty binary search tree.
    bst_t *tree = bst_create();

    if (tree == NULL) {
        fprintf(stderr, "Could not create tree\n");
        return 1;
    }

    int key;
    int result;
    size_t count = 0;

    // Read integers until the end of input.
    while ((result = scanf("%d", &key)) == 1) {

        // Insert each integer into the tree.
        if (bst_insert(tree, key) != 0) {
            fprintf(stderr, "Could not insert key\n");
            bst_destroy(tree);
            return 1;
        }

        // Track how many integers were successfully read.
        count++;
    }

    // Report invalid input if a non-integer is encountered.
    if (result == 0) {
        printf("not a number after %zu values\n", count);
        bst_destroy(tree);
        return 1;
    }

    // Check whether an input error occurred.
    if (ferror(stdin)) {
        fprintf(stderr, "Input error\n");
        bst_destroy(tree);
        return 1;
    }

    // Print the required message for an empty tree.
    if (bst_size(tree) == 0) {
        printf("empty tree\n");
    } else {
        // Print the number of keys and the tree height.
        printf("n %zu, height %zu\n",
               bst_size(tree), bst_height(tree));
    }

    // Release all allocated memory.
    bst_destroy(tree);

    return 0;
}
