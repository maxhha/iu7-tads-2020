#ifndef __MENU_H__
#define __MENU_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helpers.h"
#include "errors.h"
#include "colors.h"
#include "car.h"
#include "read_car.h"
#include "print_car.h"
#include "vec.h"
#include "sorts.h"

#define MENU_ACTION_LIST "ls"
#define MENU_ACTION_ADD "add"
#define MENU_ACTION_REMOVE "rm"
#define MENU_ACTION_SAVE "save"
#define MENU_ACTION_LOAD "load"
#define MENU_ACTION_QUIT "q"
#define MENU_ACTION_SORT_TABLE_BUBBLE "sorttb"
#define MENU_ACTION_SORT_TABLE_HAIRBRUSH "sortth"
#define MENU_ACTION_SORT_KEY_BUBBLE "sortkb"
#define MENU_ACTION_SORT_KEY_HAIRBRUSH "sortkh"
#define MENU_ACTION_SORT_ALL "sorta"
#define MENU_ACTION_LEN 6
#define FILE_NAME_LEN 128

#define MICROSEC_PER_SEC 1000000LL


int run_menu(car_t **car_table);

#endif // __MENU_H__
