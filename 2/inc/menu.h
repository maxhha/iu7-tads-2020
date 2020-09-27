#ifndef __MENU_H__
#define __MENU_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"
#include "errors.h"
#include "car.h"
#include "read_car.h"
#include "print_car.h"
#include "vec.h"

#define MENU_ACTION_LIST "ls"
#define MENU_ACTION_ADD "add"
#define MENU_ACTION_REMOVE "rm"
#define MENU_ACTION_QUIT "q"
#define MENU_ACTION_LEN 4

int run_menu(car_t **car_table);

#endif // __MENU_H__
