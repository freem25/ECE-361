#include <stdio.h>
#include "../ptr.h"

static int tests_run = 0;
static int tests_failed = 0;

void check(int condition, const char *name)
{
    tests_run++;

    if (condition)
        printf("PASS: %s\n", name);
    else
    {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

int main(void)
{
    /* swap_float */
    {
        float a = 1.0f;
        float b = 2.0f;

        swap_float(&a, &b);

        check(a == 2.0f && b == 1.0f,
              "swap_float");
    }

    /* minmax multiple values */
    {
        float a[] = {3.0f, 1.0f, 7.0f, 2.0f};
        float min, max;

        int r = minmax(a, 4, &min, &max);

        check(r == 0 &&
              min == 1.0f &&
              max == 7.0f,
              "minmax normal");
    }

    /* minmax n == 0 */
    {
        float a[] = {5.0f};
        float min = 10.0f;
        float max = 20.0f;

        int r = minmax(a, 0, &min, &max);

        check(r == -1 &&
              min == 10.0f &&
              max == 20.0f,
              "minmax n==0");
    }

    /* minmax NULL pointer */
    {
        float min = 10.0f;
        float max = 20.0f;

        int r = minmax(NULL, 5, &min, &max);

        check(r == -1 &&
              min == 10.0f &&
              max == 20.0f,
              "minmax NULL");
    }

    /* find_above first of two matches */
    {
        float a[] = {2.0f, 8.0f, 9.0f};

        const float *p = find_above(a, 3, 5.0f);

        check(p == &a[1],
              "find_above first match");
    }

    /* find_above none */
    {
        float a[] = {1.0f, 2.0f, 3.0f};

        check(find_above(a, 3, 5.0f) == NULL,
              "find_above none");
    }

    /* find_above equal threshold */
    {
        float a[] = {5.0f, 4.0f};

        check(find_above(a, 2, 5.0f) == NULL,
              "find_above equal threshold");
    }

    /* reverse even */
    {
        float a[] = {1, 2, 3, 4};

        reverse(a, 4);

        check(a[0] == 4 &&
              a[1] == 3 &&
              a[2] == 2 &&
              a[3] == 1,
              "reverse even");
    }

    /* reverse odd */
    {
        float a[] = {1, 2, 3, 4, 5};

        reverse(a, 5);

        check(a[0] == 5 &&
              a[1] == 4 &&
              a[2] == 3 &&
              a[3] == 2 &&
              a[4] == 1,
              "reverse odd");
    }

    printf("\n%d tests run, %d failed\n",
           tests_run, tests_failed);

    return tests_failed ? 1 : 0;
}
