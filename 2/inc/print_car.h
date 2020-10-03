#ifndef __PRINT_CAR_H__
#define __PRINT_CAR_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "errors.h"
#include "colors.h"
#include "delimeters.h"
#include "car.h"

#define TABLE_COL_SIZE_INDEX 2
#define TABLE_COL_SIZE_BRAND 14
#define TABLE_COL_SIZE_COUNTRY 14
#define TABLE_COL_SIZE_PRICE 14
#define TABLE_COL_SIZE_COLOR 10
#define TABLE_COL_SIZE_STATE 9
#define TABLE_COL_SIZE_GUARANTEE 8
#define TABLE_COL_SIZE_YEAR 6
#define TABLE_COL_SIZE_MILEAGE 6
#define TABLE_COL_SIZE_REPAIRS_N 8
#define TABLE_COL_SIZE_OWNERS_N 13

void print_car_table_header(void);
void print_car_table_row(const size_t index, const car_t *car);
void print_car_key_table_header(void);
void print_car_key_table_row(const car_key_t *key);

#endif // __PRINT_CAR_H__
