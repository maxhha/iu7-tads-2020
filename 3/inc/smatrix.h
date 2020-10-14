#ifndef __SMATRIX_H__
#define __SMATRIX_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "errors.h"
#include "colors.h"
#include "helpers.h"
#include "delimeters.h"
#include "print_constants.h"

typedef struct {
    int *values;
    size_t *columns;
    size_t *row_begins;
    size_t width;
    size_t height;
    size_t n_elems;
} smatrix_t;

smatrix_t *create_smatrix(size_t width, size_t height, size_t n_elems);
int resize_smatrix(smatrix_t *m, size_t width, size_t height, size_t n_elems);
void free_smatrix(smatrix_t *m);
smatrix_t *scan_smatrix(void);
smatrix_t *scan_row_smatrix(void);
int multiply_row_smatrix_by_smatrix(
    const smatrix_t * restrict m_row,
    const smatrix_t * restrict m,
    smatrix_t * restrict result);
void print_smatrix(const smatrix_t *m, bool force_elements);
void fill_random_smatrix(smatrix_t *m);

#endif // __SMATRIX_H__
