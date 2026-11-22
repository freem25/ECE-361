
#include "../bst.h"
#include <stdio.h>

// Prints PASS or FAIL and counts failed tests.
static int check(const char *name, bool passed) {
    printf("%s: %s\n", name, passed ? "PASS" : "FAIL");
    return passed ? 0 : 1;
}

int main(void) {
    int failures = 0;
    size_t visits = 0;
    bool found;

    // TEST 1: Search an empty tree.
    // This is the test that exposed the original crash.
    bst_t *empty = bst_create();

    if (empty == NULL) {
        puts("FAIL: Create empty tree");
        return 1;
    }

    found = bst_contains(empty, 42, &visits);

    failures += check("Empty tree search", !found);
    failures += check("Empty tree visits", visits == 0);
    failures += check("Empty tree size", bst_size(empty) == 0);
    failures += check("Empty tree height", bst_height(empty) == 0);

    bst_destroy(empty);

    // TEST 2: Insert the original seven keys.
    int keys[] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *t = bst_create();

    if (t == NULL) {
        puts("FAIL: Create original tree");
        return 1;
    }

    bool inserted = true;

    for (size_t i = 0; i < 7; i++) {
        if (bst_insert(t, keys[i]) != 0) {
            inserted = false;
        }
    }

    failures += check("Insert seven keys", inserted);
    failures += check("Original tree size is 7",
                      bst_size(t) == 7);
    failures += check("Original tree height is 3",
                      bst_height(t) == 3);

    // TEST 3: Search for key 31.
    visits = 0;
    found = bst_contains(t, 31, &visits);

    failures += check("Find key 31", found);
    failures += check("Key 31 takes 3 visits", visits == 3);

    // TEST 4: Search for missing key 15.
    visits = 0;
    found = bst_contains(t, 15, &visits);

    failures += check("Key 15 not found", !found);
    failures += check("Key 15 takes 3 visits", visits == 3);

    // TEST 5: Destroy the original full tree.
    // AddressSanitizer checks for memory errors.
    bst_destroy(t);
    failures += check("Destroy full tree", true);

    // TEST 6: Verify duplicate keys are retained.
    bst_t *dup = bst_create();

    if (dup == NULL) {
        puts("FAIL: Create duplicate tree");
        return 1;
    }

    int first = bst_insert(dup, 50);
    int second = bst_insert(dup, 50);

    failures += check("Insert duplicate keys",
                      first == 0 && second == 0);
    failures += check("Duplicate tree size is 2",
                      bst_size(dup) == 2);
    failures += check("Duplicate tree height is 2",
                      bst_height(dup) == 2);

    bst_destroy(dup);

    // TEST 7: Increasing-order insertion.
    // A sorted sequence creates a tall tree.
    bst_t *sorted = bst_create();

    if (sorted == NULL) {
        puts("FAIL: Create sorted tree");
        return 1;
    }

    bool sorted_inserted = true;

    for (int i = 1; i <= 10; i++) {
        if (bst_insert(sorted, i) != 0) {
            sorted_inserted = false;
        }
    }

    failures += check("Insert increasing keys",
                      sorted_inserted);
    failures += check("Increasing tree size is 10",
                      bst_size(sorted) == 10);
    failures += check("Increasing tree height is 10",
                      bst_height(sorted) == 10);

    bst_destroy(sorted);

    // TEST 8: Verify NULL argument handling.
    visits = 99;
    found = bst_contains(NULL, 42, &visits);

    failures += check("NULL tree search", !found);
    failures += check("NULL tree visits", visits == 0);
    failures += check("NULL tree size", bst_size(NULL) == 0);
    failures += check("NULL tree height", bst_height(NULL) == 0);
    failures += check("NULL tree insertion",
                      bst_insert(NULL, 42) == -1);

    // Destroying NULL should do nothing.
    bst_destroy(NULL);

    // Report total failures and set the exit status.
    printf("\nTests failed: %d\n", failures);

    return failures == 0 ? 0 : 1;
}
