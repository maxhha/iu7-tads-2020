#ifndef __CAR_FILTER_H__
#define __CAR_FILTER_H__

#include <string.h>
#include "car.h"

void filter_cars(car_t *car_table, size_t *car_table_size, const char *brand, long int price_from, long int price_to);

#endif // __CAR_FILTER_H__
