#include "ptr.h"
#include <stddef.h>

void swap_float(float *a, float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

int minmax(const float *a, int n,
           float *min, float *max)
{
    if (a == NULL || min == NULL ||
        max == NULL || n < 1)
    {
        return -1;
    }

    const float *p = a;

    *min = *p;
    *max = *p;

    p++;

    while (p < a + n)
    {
        if (*p < *min)
            *min = *p;

        if (*p > *max)
            *max = *p;

        p++;
    }

    return 0;
}

const float *find_above(const float *a,
                        int n,
                        float threshold)
{
    if (a == NULL || n < 1)
        return NULL;

    const float *p = a;

    while (p < a + n)
    {
        if (*p > threshold)
            return p;

        p++;
    }

    return NULL;
}

void reverse(float *a, int n)
{
    if (a == NULL || n < 2)
        return;

    float *left = a;
    float *right = a + n - 1;

    while (left < right)
    {
        swap_float(left, right);
        left++;
        right--;
    }
}
