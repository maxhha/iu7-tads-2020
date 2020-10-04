#include "../inc/car.h"

int compare_cars_by_price(const void *car_a, const void *car_b)
{
    return ((const car_t *) car_a)->price - ((const car_t *) car_b)->price;
}

int compare_car_keys_by_price(const void *car_a, const void *car_b)
{
    return ((const car_key_t *) car_a)->price - ((const car_key_t *) car_b)->price;
}
