#ifndef __CAR_H__
#define __CAR_H__

#include <stdbool.h>

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
    char brand[CAR_BRAND_LEN + 1];
    char country[CAR_COUNTRY_LEN + 1];
    long int price;
    char color[CAR_COLOR_LEN + 1];
    bool is_new;
    car_state_t state;
} car_t;

#endif // __CAR_H__