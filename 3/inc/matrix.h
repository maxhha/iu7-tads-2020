#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "errors.h"
#include "colors.h"
#include "helpers.h"
#include "delimeters.h"
#include "print_constants.h"

typedef struct matrix_s {
    int *data;
    size_t width;
    size_t height;
} matrix_t;

matrix_t *create_matrix(size_t width, size_t height);
int resize_matrix(matrix_t *m, size_t width, size_t height);
void free_matrix(matrix_t *m);
matrix_t *scan_matrix(void);
matrix_t *scan_row_matrix(void);
int multiply_row_matrix_by_matrix(
    const matrix_t * restrict m_row,
    const matrix_t * restrict m,
    matrix_t * restrict result);
void print_matrix(const matrix_t *m, bool force_big);
void fill_random_matrix(matrix_t *m, size_t n);

#endif // __MATRIX_H__
