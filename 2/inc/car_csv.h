#ifndef __CAR_CSV_H__
#define __CAR_CSV_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "colors.h"
#include "helpers.h"
#include "car.h"

int write_cars_to_csv(char *filename, car_t *car_table, size_t car_table_size);
int read_cars_from_csv(char *filename, car_t *car_table, size_t *car_table_size);

#endif // __CAR_CSV_H__
