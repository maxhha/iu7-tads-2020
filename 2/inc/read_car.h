#ifndef __READ_CAR_H__
#define __READ_CAR_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "errors.h"
#include "helpers.h"
#include "car.h"

int read_car(FILE *fin, FILE *fout, car_t *car);

#endif // __READ_CAR_H__
