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

void psort(void *array, size_t n, size_t size, cmp_func_t cmp)
{
    char *base = (char *) array;
    char *end = base + n * size;

    // Build heap (rearrange array)
    for (size_t i = n / 2 * size; i > 0; i -= size)
    {
        size_t j = i - size;
        char *root = base + j;
        char *largest = root; // Initialize largest as root
        char *l = base + 2 * j + size; // left = 2*i + 1
        char *r = base + 2 * j + 2 * size; // right = 2*i + 2

        for(;;)
        {
            // If left child is larger than root
            if (l < end && (*cmp) ((void *) l, (void *) (largest)) > 0)
            largest = l;

            // If right child is larger than largest so far
            if (r < end && (*cmp) ((void *) r, (void *) (largest)) > 0)
            largest = r;

            // If largest is not root
            if (largest == root)
                break;
            SWAP(root, largest, size);

            root = largest;
            l = base + 2 * (root - base) + size;
            r = base + 2 * (root - base) + 2 * size;
        }
    }

    // One by one extract an element from heap
    for (end -= size; end > base; end -= size)
    {
        // Move current root to end
        SWAP(base, end, size);

        // call max heapify on the reduced heap
        char *root = base;
        char *largest = root; // Initialize largest as root
        char *l = base + size; // left = 2*i + 1
        char *r = base + 2 * size; // right = 2*i + 2

        for(;;)
        {
            // If left child is larger than root
            if (l < end && (*cmp) ((void *) l, (void *) (largest)) > 0)
                largest = l;

            // If right child is larger than largest so far
            if (r < end && (*cmp) ((void *) r, (void *) (largest)) > 0)
                largest = r;

            // If largest is not root
            if (largest == root)
                break;
            SWAP(root, largest, size);
            root = largest;
            l = base + 2 * (root - base) + size;
            r = base + 2 * (root - base) + 2 * size;
        }
    }
}
