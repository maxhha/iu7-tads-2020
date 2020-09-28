#include "../inc/sorts.h"

#define SWAP(a,b,s) \
    do { \
        char *lo_end = (a) + (s); \
        for (char *lo = (a), *hi = (b); lo < lo_end; lo++, hi++) \
        { \
            char c = *lo; \
            *lo = *hi; \
            *hi = c; \
        } \
    } while(0)

void bsort(void *array, size_t n, size_t size, cmp_func_t cmp)
{
    if (array == NULL || n < 2 || size == 0 || cmp == NULL)
        return;
    char *base = (char *) array;
    char *j_next;
    for (char *i = base + (n - 1) * size; i >= base; i -= size)
        for (char *j = base; j < i; j += size)
            if ((*cmp) ((void *) j, (void *) (j_next = j + size)) > 0)
                SWAP(j, j_next, size);
}

void hsort(void *array, size_t n, size_t size, cmp_func_t cmp)
{
    if (array == NULL || n < 2 || size == 0 || cmp == NULL)
        return;

    char *base = (char *) array;
    size_t gap = (n * 10 / 13);
    while (gap)
    {
        if (gap > 8 && gap < 11)
            gap = 11;
        size_t swapped = 0;
        char *end = base + (n - gap) * size;
        char *j;
        for (char *i = base; i < end; i += size)
            if ((*cmp) ((void *) i, (void *) (j = i + size*gap)) > 0)
            {
                SWAP(i, j, size);
                swapped = 1;
            }
        gap = (gap * 10 / 13);
        if (gap == 0)
            gap = swapped;
    }
}
