#ifndef __SORTS_H__
#define __SORTS_H__

#include <stdlib.h>

typedef int (*cmp_func_t)(const void *, const void *);

void bsort(void *array, size_t n, size_t size, cmp_func_t cmp);
void hsort(void *array, size_t n, size_t size, cmp_func_t cmp);

#endif // __SORTS_H__
