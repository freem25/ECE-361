
#include "store.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

/*
 * ECE 361 - Sauna Controller
 * Binary Search Tree Storage (K4)
 *
 * Stores complete records ordered by whole-percent
 * relative humidity.
 *
 * Duplicate humidity keys are retained to the right.
 */

/* Each tree node contains one complete sauna record. */
struct node {
    record_t record;
    struct node *leftChild;
    struct node *rightChild;
};

/* Private store structure, replacing the sorted array. */
struct store {
    struct node *root;
    int count;
};

/* Convert humidity to a whole-percent integer key. */
static int humidity_key(float humidity)
{
    return (int)humidity;
}

/* Compare two records using their humidity keys. */
int record_compare(const record_t *a,
                   const record_t *b)
{
    int key_a = humidity_key(a->reading.humidity);
    int key_b = humidity_key(b->reading.humidity);

    return (key_a > key_b) - (key_a < key_b);
}

/* Initialize the module; no global storage is needed. */
void store_init(void)
{
    /* Each store now manages its own allocated tree. */
}

/* Create a new, empty binary search tree. */
store_t *store_create(void)
{
    store_t *store = malloc(sizeof(*store));

    if (store == NULL) {
        return NULL;
    }

    store->root = NULL;
    store->count = 0;

    return store;
}

/* Insert a complete record into the tree. */
int store_insert(store_t *store,
                 const record_t *record)
{
    if (store == NULL || record == NULL ||
        store->count == INT_MAX) {
        return -1;
    }

    /*
     * Find the insertion position before allocation.
     * Equal humidity keys follow the right subtree.
     */
    struct node **position = &store->root;

    while (*position != NULL) {
        int comparison =
            record_compare(record, &(*position)->record);

        if (comparison < 0) {
            position = &(*position)->leftChild;
        } else {
            position = &(*position)->rightChild;
        }
    }

    /* Allocate and initialize the new node. */
    struct node *newNode = malloc(sizeof(*newNode));

    if (newNode == NULL) {
        return -1;
    }

    newNode->record = *record;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    /* Attach the node and update the record count. */
    *position = newNode;
    store->count++;

    return 0;
}

/* Find one record matching a whole-percent humidity key. */
int store_find(const store_t *store,
               int key,
               record_t *out)
{
    if (store == NULL || out == NULL) {
        return -1;
    }

    const struct node *current = store->root;

    /* Follow the BST until a match or NULL is reached. */
    while (current != NULL) {
        int current_key =
            humidity_key(current->record.reading.humidity);

        if (key == current_key) {
            *out = current->record;
            return 0;
        }

        if (key < current_key) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    return -1;
}

/* Visit matching nodes, keeping duplicate records. */
static int find_all_nodes(const struct node *current,
                          int key,
                          store_visit_fn visit,
                          void *context)
{
    if (current == NULL) {
        return 0;
    }

    int current_key =
        humidity_key(current->record.reading.humidity);

    if (key < current_key) {
        return find_all_nodes(current->leftChild,
                              key, visit, context);
    }

    if (key > current_key) {
        return find_all_nodes(current->rightChild,
                              key, visit, context);
    }

    /* The current record matches the requested key. */
    visit(&current->record, context);

    /*
     * Equal keys are inserted to the right.
     * Continue searching for additional matches.
     */
    return 1 + find_all_nodes(current->rightChild,
                              key, visit, context);
}

/* Find and visit every record with a matching key. */
int store_find_all(const store_t *store,
                   int key,
                   store_visit_fn visit,
                   void *context)
{
    if (store == NULL || visit == NULL) {
        return -1;
    }

    return find_all_nodes(store->root, key,
                          visit, context);
}

/* Visit every record in ascending humidity-key order. */
static void inorder_nodes(const struct node *current,
                          store_visit_fn visit,
                          void *context)
{
    if (current == NULL) {
        return;
    }

    /* Visit left subtree, current node, then right. */
    inorder_nodes(current->leftChild, visit, context);
    visit(&current->record, context);
    inorder_nodes(current->rightChild, visit, context);
}

/* Walk the entire binary search tree in order. */
void store_inorder(const store_t *store,
                   store_visit_fn visit,
                   void *context)
{
    if (store == NULL || visit == NULL) {
        return;
    }

    inorder_nodes(store->root, visit, context);
}

/* Return the number of complete records stored. */
int store_count(const store_t *store)
{
    if (store == NULL) {
        return 0;
    }

    return store->count;
}

/* Calculate the height of a subtree recursively. */
static int height_nodes(const struct node *current)
{
    if (current == NULL) {
        return 0;
    }

    int left = height_nodes(current->leftChild);
    int right = height_nodes(current->rightChild);

    /* Include this node plus the taller subtree. */
    return 1 + (left > right ? left : right);
}

/* Return the height measured in number of nodes. */
int store_height(const store_t *store)
{
    if (store == NULL) {
        return 0;
    }

    return height_nodes(store->root);
}

/* Recursively free all nodes in a subtree. */
static void destroy_nodes(struct node *current)
{
    if (current == NULL) {
        return;
    }

    /* Free both children before freeing the node. */
    destroy_nodes(current->leftChild);
    destroy_nodes(current->rightChild);
    free(current);
}

/* Destroy the entire tree and release its memory. */
void store_destroy(store_t *store)
{
    if (store == NULL) {
        return;
    }

    destroy_nodes(store->root);
    free(store);
}
