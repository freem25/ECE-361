
#include <stdio.h>
#include "../stats.h"

static int failures = 0;

static void check(int condition, const char *name) {
    if (condition)
        printf("PASS %s\n", name);
    else {
        printf("FAIL %s\n", name);
        failures++;
    }
}

int main(void) {
    float single[] = {42.0f};

    check(stats_min(single, 1) == 42.0f &&
          stats_max(single, 1) == 42.0f &&
          stats_mean(single, 1) == 42.0f,
          "one value");

    float first[] = {9.0f, 2.0f, 3.0f};
    float last[] = {2.0f, 3.0f, 9.0f};

    check(stats_max(first, 3) == 9.0f &&
          stats_max(last, 3) == 9.0f,
          "maximum at start and end");

    float temps[] = {31.0f, 32.0f, 30.0f, 33.0f};
    int start = -1;

    int length = stats_longest_run(temps, 4, 30.0f, &start);

    check(length == 2 && start == 0,
          "threshold equality ends run");

    static float large[10000];

    for (int i = 0; i < 10000; i++)
        large[i] = (float)i;

    check(stats_max(large, 10000) == 9999.0f,
          "recursive maximum with 10000 values");

    if (failures == 0)
        printf("All statistics tests passed\n");
    else
        printf("%d statistics tests failed\n", failures);

    return failures == 0 ? 0 : 1;
}
