
#include <stdio.h>
#include "store.h"

/* Print PASS or FAIL and count failures. */
static int check(const char *name, int passed)
{
    printf("%s: %s\n", name, passed ? "PASS" : "FAIL");
    return passed ? 0 : 1;
}

/* Track records encountered during an in-order walk. */
struct walk_info {
    int count;
    int sorted;
    int has_previous;
    record_t previous;
};

/* Verify records are visited in ascending humidity order. */
static void check_order(const record_t *record, void *context)
{
    struct walk_info *info = context;

    if (info->has_previous &&
        record_compare(&info->previous, record) > 0) {
        info->sorted = 0;
    }

    info->previous = *record;
    info->has_previous = 1;
    info->count++;
}

/* Collect timestamps from records with matching keys. */
struct match_info {
    int count;
    time_t timestamps[4];
};

static void collect_match(const record_t *record, void *context)
{
    struct match_info *info = context;

    if (info->count < 4) {
        info->timestamps[info->count] =
            record->reading.timestamp;
    }

    info->count++;
}

int main(void)
{
    int failures = 0;

    /* TEST 1: Create and destroy an empty tree. */
    store_t *empty = store_create();

    if (empty == NULL) {
        puts("FAIL: Create empty tree");
        return 1;
    }

    failures += check("Empty tree count",
                      store_count(empty) == 0);
    failures += check("Empty tree height",
                      store_height(empty) == 0);

    store_destroy(empty);

    /* TEST 2: Compare quantized humidity values. */
    record_t a = {0};
    record_t b = {0};
    record_t c = {0};

    a.reading.humidity = 45.2f;
    b.reading.humidity = 45.7f;
    c.reading.humidity = 46.0f;

    failures += check("Same humidity key",
                      record_compare(&a, &b) == 0);
    failures += check("Neighboring humidity keys",
                      record_compare(&b, &c) < 0);
    failures += check("Exact humidity boundary",
                      record_compare(&c, &b) > 0);

    /* TEST 3: Insert and find duplicate humidity keys. */
    store_t *tree = store_create();

    if (tree == NULL) {
        puts("FAIL: Create BST");
        return 1;
    }

    a.reading.timestamp = 101;
    b.reading.timestamp = 103;

    record_t d = {0};
    d.reading.humidity = 45.9f;
    d.reading.timestamp = 105;

    record_t low = {0};
    low.reading.humidity = 30.0f;

    int inserted =
        store_insert(tree, &a) == 0 &&
        store_insert(tree, &low) == 0 &&
        store_insert(tree, &b) == 0 &&
        store_insert(tree, &d) == 0;

    failures += check("Insert four records", inserted);
    failures += check("Duplicate records retained",
                      store_count(tree) == 4);

    struct match_info matches = {0};

    int found_count = store_find_all(
        tree, 45, collect_match, &matches);

    failures += check("Find all duplicate keys",
                      found_count == 3 && matches.count == 3);

    failures += check("Duplicate timestamps preserved",
                      matches.count == 3 &&
                      matches.timestamps[0] == 101 &&
                      matches.timestamps[1] == 103 &&
                      matches.timestamps[2] == 105);

    /* TEST 4: Check missing keys. */
    record_t result = {0};

    failures += check("Find missing key",
                      store_find(tree, 99, &result) == -1);

    /* TEST 5: Verify in-order traversal. */
    struct walk_info info = {0};
    info.sorted = 1;

    store_inorder(tree, check_order, &info);

    failures += check("In-order visits all records",
                      info.count == 4);
    failures += check("In-order keys are sorted",
                      info.sorted);

    store_destroy(tree);

    /* TEST 6: Check count and height after 100 inserts. */
    store_t *large = store_create();

    if (large == NULL) {
        puts("FAIL: Create large tree");
        return 1;
    }

    int all_inserted = 1;

    for (int i = 0; i < 100; i++) {
        record_t record = {0};
        record.reading.humidity = (float)i;
        record.reading.timestamp = i;

        if (store_insert(large, &record) != 0) {
            all_inserted = 0;
            break;
        }

        if (i == 0) {
            failures += check("One-node count",
                              store_count(large) == 1);
            failures += check("One-node height",
                              store_height(large) == 1);
        }

        if (i == 1) {
            failures += check("Two-node count",
                              store_count(large) == 2);
            failures += check("Two-node height",
                              store_height(large) == 2);
        }
    }

    failures += check("Insert 100 records", all_inserted);
    failures += check("100-node count",
                      store_count(large) == 100);
    failures += check("100-node height",
                      store_height(large) == 100);

    /* Free all allocated tree nodes. */
    store_destroy(large);
    store_destroy(NULL);

    printf("\nTests failed: %d\n", failures);

    return failures == 0 ? 0 : 1;
}
