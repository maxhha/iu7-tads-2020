#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "colors.h"
#include "helpres.h"
#include "delimeters.h"

typedef struct {
    int *data;
    size_t width;
    size_t height;
} matrix_t;

matrix_t *matrix_create(size_t width, size_t height);
int matrix_resize(matrix_t *m, size_t width, size_t height);
void matrix_free(matrix_t *m);
matrix_t *matrix_scan(void);
int matrix_multiply(const matrix_t * restrict a, const matrix_t * restrict b, matrix_t * restrict result);
void matrix_print(matrix_t *m);

#endif // __MATRIX_H__
