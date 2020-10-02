#include "../inc/print_car.h"
#define I GRN "|" RESET
#define _I " " I
#define I_ I " "
#define _I_ " " I " "
#define _I_n _I_ "\n"

/*
    Выводит строку n раз
*/
#define print_repeat(c, n) do { for(int i = 0; i < n; i++) printf(c); } while(0)

/*
    Выводит шапку таблицы машин
*/
void print_car_table_header(void)
{

    printf(I_ "#");
    print_repeat(" ", TABLE_COL_SIZE_INDEX - 1);

    printf(_I_ "Марка");
    print_repeat(" ", TABLE_COL_SIZE_BRAND - 5);

    printf(_I_ "Страна");
    print_repeat(" ", TABLE_COL_SIZE_COUNTRY - 6);

    printf(_I_ "Цена");
    print_repeat(" ", TABLE_COL_SIZE_PRICE - 4);

    printf(_I_ "Цвет");
    print_repeat(" ", TABLE_COL_SIZE_COLOR - 4);

    printf(_I_ "Состояние");
    print_repeat(" ", TABLE_COL_SIZE_STATE - 9);

    printf(_I_ "Гарантия");
    print_repeat(" ", TABLE_COL_SIZE_GUARANTEE - 8);

    printf(_I_ "Выпуск");
    print_repeat(" ", TABLE_COL_SIZE_YEAR - 6);

    printf(_I_ "Пробег");
    print_repeat(" ", TABLE_COL_SIZE_MILEAGE - 6);

    printf(_I_ "Ремонтов");
    print_repeat(" ", TABLE_COL_SIZE_REPAIRS_N - 8);

    printf(_I_ "Собственников");
    print_repeat(" ", TABLE_COL_SIZE_OWNERS_N - 13);

    printf(_I_n);

    printf(I_);
    print_repeat("-", TABLE_COL_SIZE_INDEX);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_BRAND);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_COUNTRY);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_PRICE);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_COLOR);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_STATE);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_GUARANTEE);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_YEAR);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_MILEAGE);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_REPAIRS_N);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_OWNERS_N);

    printf(_I_n);
}

/*
    Выводит часть строки таблицы с информацией о состоянии новой car
*/
void print_car_table_row_state_new(const car_state_new_t *info)
{
    printf(_I_ "%*d", TABLE_COL_SIZE_GUARANTEE, info->guarantee);

    printf(_I_ "%*s", TABLE_COL_SIZE_YEAR, "-");

    printf(_I_ "%*s", TABLE_COL_SIZE_MILEAGE, "-");

    printf(_I_ "%*s", TABLE_COL_SIZE_REPAIRS_N, "-");

    printf(_I_ "%*s", TABLE_COL_SIZE_OWNERS_N, "-");
}

/*
    Выводит часть строки таблицы с информацией о состоянии старой car
*/
void print_car_table_row_state_old(const car_state_old_t *info)
{
    printf(_I_ "%*s", TABLE_COL_SIZE_GUARANTEE, "-");

    printf(_I_ "%*d", TABLE_COL_SIZE_YEAR, info->year);

    printf(_I_ "%*d", TABLE_COL_SIZE_MILEAGE, info->mileage);

    printf(_I_ "%*d", TABLE_COL_SIZE_REPAIRS_N, info->repairs_n);

    printf(_I_ "%*d", TABLE_COL_SIZE_OWNERS_N, info->owners_n);
}

/*
    Выводит строку таблицы с информацией о car
*/
void print_car_table_row(const size_t index, const car_t *car)
{
    printf(GRN "| " RESET "%*lu", TABLE_COL_SIZE_INDEX, index + 1);

    printf(_I_ "%-*s", TABLE_COL_SIZE_BRAND, car->brand);

    printf(_I_ "%-*s", TABLE_COL_SIZE_COUNTRY, car->country);

    printf(_I_ "%*ld", TABLE_COL_SIZE_PRICE, car->price);

    printf(_I_ "%-*s", TABLE_COL_SIZE_COLOR, car->color);

    printf(_I_ "%-*s", TABLE_COL_SIZE_STATE, car->is_new ? "new" : "used");

    if (car->is_new)
    {
        print_car_table_row_state_new(&car->state.new_info);
    }
    else
    {
        print_car_table_row_state_old(&car->state.old_info);
    }

    printf(_I_n);
}

/*
    Выводит шапку таблицы ключей машин
*/
void print_car_key_table_header(void)
{

    printf(I_ "#");
    print_repeat(" ", TABLE_COL_SIZE_INDEX - 1);

    printf(_I_ "Цена");
    print_repeat(" ", TABLE_COL_SIZE_PRICE - 4);

    printf(_I_n);

    printf(I_);
    print_repeat("-", TABLE_COL_SIZE_INDEX);

    printf(_I_ YEL);
    print_repeat("-", TABLE_COL_SIZE_PRICE);

    printf(_I_n);
}

/*
    Выводит строку таблицы ключей
*/
void print_car_key_table_row(const car_key_t *key)
{
    printf(GRN "| " RESET "%*lu", TABLE_COL_SIZE_INDEX, key->index + 1);

    printf(_I_ "%*ld", TABLE_COL_SIZE_PRICE, key->price);

    printf(_I_n);
}
