
#include <stdio.h>
#include "tally.h"

int main(void)
{
    tally_t *t = tally_create();

    if (t == NULL) {
        printf("FAIL: Could not create tally\n");
        return 1;
    }

    printf("Adding names:\n");

    printf("temperature: %d\n",
           tally_add(t, "temperature"));

    printf("motion: %d\n",
           tally_add(t, "motion"));

    printf("temperature: %d\n",
           tally_add(t, "temperature"));

    printf("door: %d\n",
           tally_add(t, "door"));

    printf("motion: %d\n",
           tally_add(t, "motion"));

    printf("\nChecking counts:\n");

    printf("temperature count: %d\n",
           tally_count(t, "temperature"));

    printf("motion count: %d\n",
           tally_count(t, "motion"));

    printf("door count: %d\n",
           tally_count(t, "door"));

    printf("unknown count: %d\n",
           tally_count(t, "unknown"));

    printf("\nNumber of different names: %d\n",
           tally_size(t));

    printf("\nAlphabetical tally:\n");
    tally_print(t, stdout);

    printf("\nTesting invalid name:\n");
    printf("Empty name result: %d\n",
           tally_add(t, ""));

    tally_destroy(t);

    printf("\nTally destroyed.\n");

    return 0;
}
