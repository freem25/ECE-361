
#include <stdio.h>
#include "store.h"

int main(void)
{
    store_init();

    store_t *store = store_create();

    if (store == NULL) {
        printf("FAIL: Cannot create store\n");
        return 1;
    }

    record_t a = {0};
    record_t b = {0};
    record_t c = {0};

    a.reading.humidity = 45.2f;
    b.reading.humidity = 30.5f;
    c.reading.humidity = 45.7f;

    int ok = 1;

    ok &= store_insert(store, &a) == 0;
    ok &= store_insert(store, &b) == 0;
    ok &= store_insert(store, &c) == 0;

    ok &= store_count(store) == 3;

    /* Both humidities have the same key. */
    ok &= record_compare(&a, &c) == 0;

    /* 30.5% comes before 45.2%. */
    ok &= record_compare(&b, &a) < 0;

    printf("%s: Insert three records\n",
           ok ? "PASS" : "FAIL");

    printf("%s: Store count is three\n",
           store_count(store) == 3 ? "PASS" : "FAIL");

    store_destroy(store);

    printf("%s: Destroy clears store\n",
           store_count(store) == 0 ? "PASS" : "FAIL");

    return ok ? 0 : 1;
}

