#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"
#include "smatrix.h"

#define TIME_MEASURE_REPEATS 1000
#define MICROSEC_PER_SEC 1000000LL

int action_mul_matrix(void);
int action_mul_smatrix(void);
int action_measure_matrix_mul(void);
int action_measure_smatrix_mul(void);

#endif // __ACTIONS_H__
