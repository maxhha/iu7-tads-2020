#ifndef __CAR_H__
#define __CAR_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "errors.h"

#define CAR_BRAND_LEN 128
#define CAR_COUNTRY_LEN 128
#define CAR_COLOR_LEN 128

typedef struct {
    int guarantee;
} car_state_new_t;

typedef struct {
    int year;
    int mileage;
    int repairs_n;
    int owners_n;
} car_state_old_t;

typedef union {
    car_state_new_t new_info;
    car_state_old_t old_info;
} car_state_t;

typedef struct {
    char brand[CAR_BRAND_LEN];
    char country[CAR_COUNTRY_LEN];
    int price;
    char color[CAR_COLOR_LEN];
    bool is_new;
    car_state_t state;
} car_t;

int read_car(FILE *fin, FILE *fout, car_t *car);

#endif // __CAR_H__
