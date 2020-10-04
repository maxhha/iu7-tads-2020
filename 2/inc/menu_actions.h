#ifndef __MENU_ACTIONS_H__
#define __MENU_ACTIONS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "read_helpers.h"
#include "errors.h"
#include "colors.h"
#include "car.h"
#include "read_car.h"
#include "print_car.h"
#include "sorts.h"
#include "delimeters.h"
#include "car_csv.h"
#include "car_filter.h"

#define FILE_NAME_LEN 128

#define MICROSEC_PER_SEC 1000000LL
#define TIME_MEASURE_REPEATS 1000

int menu_action_list(car_t *car_table, size_t *car_table_size);
int menu_action_add(car_t *car_table, size_t *car_table_size);
int menu_action_remove(car_t *car_table, size_t *car_table_size);
int menu_action_save(car_t *car_table, size_t *car_table_size);
int menu_action_load(car_t *car_table, size_t *car_table_size);
int menu_action_find(car_t *car_table, size_t *car_table_size);
int menu_action_sort_table_bubble(car_t *car_table, size_t *car_table_size);
int menu_action_sort_table_heapsort(car_t *car_table, size_t *car_table_size);
int menu_action_sort_key_bubble(car_t *car_table, size_t *car_table_size);
int menu_action_sort_key_heapsort(car_t *car_table, size_t *car_table_size);
int menu_action_sort_all(car_t *car_table, size_t *car_table_size);

#endif // __MENU_ACTIONS_H__
