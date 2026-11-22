
#include <stdio.h>
#include "store.h"

/* Print the result of each individual test. */
static int check(const char *name, int passed)
{
    printf("%s: %s\n", name, passed ? "PASS" : "FAIL");
    return passed ? 0 : 1;
}

int main(void)
{
    int failures = 0;

    /* Initialize the storage module. */
    store_init();

    /* Create an empty binary search tree. */
    store_t *store = store_create();

    if (store == NULL) {
        puts("FAIL: Cannot create store");
        return 1;
    }

    failures += check("Create empty store",
                      store_count(store) == 0);

    /* Create three sample sauna records. */
    record_t a = {0};
    record_t b = {0};
    record_t c = {0};

    a.reading.humidity = 45.2f;
    b.reading.humidity = 30.5f;
    c.reading.humidity = 45.7f;

    /* Give each record a different timestamp. */
    a.reading.timestamp = 100;
    b.reading.timestamp = 200;
    c.reading.timestamp = 300;

    /* Insert all three records into the tree. */
    failures += check("Insert record A",
                      store_insert(store, &a) == 0);

    failures += check("Insert record B",
                      store_insert(store, &b) == 0);

    failures += check("Insert record C",
                      store_insert(store, &c) == 0);

    /* Verify the tree contains all three records. */
    failures += check("Store count is three",
                      store_count(store) == 3);

    /* Humidities within the same 1% step share a key. */
    failures += check("Duplicate humidity keys",
                      record_compare(&a, &c) == 0);

    /* A lower humidity key should compare smaller. */
    failures += check("Humidity comparison",
                      record_compare(&b, &a) < 0);

    /* Verify the tree can find a stored record. */
    record_t found = {0};

    failures += check("Find humidity key 30",
                      store_find(store, 30, &found) == 0);

    failures += check("Found correct record",
                      found.reading.timestamp == 200);

    /* Verify that a missing humidity key is rejected. */
    failures += check("Missing humidity key",
                      store_find(store, 99, &found) == -1);

/* The longest path contains two nodes. */
failures += check("Tree height is two",
                  store_height(store) == 2);


    /* Free the complete binary search tree. */
    store_destroy(store);

    /* NULL arguments should be handled safely. */
    failures += check("NULL store count",
                      store_count(NULL) == 0);

    failures += check("NULL store height",
                      store_height(NULL) == 0);

    store_destroy(NULL);

    /* Report the final number of failed tests. */
    printf("\nTests failed: %d\n", failures);

    return failures == 0 ? 0 : 1;
}
