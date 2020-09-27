#include "../inc/print_car.h"

/*
    Выводит символ n раз
*/
void print_repeat(const char c, const int n)
{
    for(int i = 0; i < n; i++)
        printf("%c", c);
}

/*
    Выводит шапку таблицы машин
*/
void print_car_table_header(void)
{

    printf("| #");
    print_repeat(' ', TABLE_COL_SIZE_INDEX - 1);

    printf(" | Марка");
    print_repeat(' ', TABLE_COL_SIZE_BRAND - 5);

    printf(" | Страна");
    print_repeat(' ', TABLE_COL_SIZE_COUNTRY - 6);

    printf(" | Цена");
    print_repeat(' ', TABLE_COL_SIZE_PRICE - 4);

    printf(" | Цвет");
    print_repeat(' ', TABLE_COL_SIZE_COLOR - 4);

    printf(" | Состояние");
    print_repeat(' ', TABLE_COL_SIZE_STATE - 9);

    printf(" | Гарантия");
    print_repeat(' ', TABLE_COL_SIZE_GUARANTEE - 8);

    printf(" | Выпуск");
    print_repeat(' ', TABLE_COL_SIZE_YEAR - 6);

    printf(" | Пробег");
    print_repeat(' ', TABLE_COL_SIZE_MILEAGE - 6);

    printf(" | Ремонтов");
    print_repeat(' ', TABLE_COL_SIZE_REPAIRS_N - 8);

    printf(" | Собственников");
    print_repeat(' ', TABLE_COL_SIZE_OWNERS_N - 13);

    printf(" |\n");

    printf("| ");
    print_repeat('-', TABLE_COL_SIZE_INDEX);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_BRAND);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_COUNTRY);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_PRICE);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_COLOR);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_STATE);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_GUARANTEE);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_YEAR);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_MILEAGE);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_REPAIRS_N);

    printf(" | ");
    print_repeat('-', TABLE_COL_SIZE_OWNERS_N);

    printf(" |\n");
}

/*
    Выводит часть строки таблицы с информацией о состоянии новой car
*/
void print_car_table_row_state_new(const car_state_new_t *info)
{
    printf(" | %*d", TABLE_COL_SIZE_GUARANTEE, info->guarantee);

    printf(" | %*s", TABLE_COL_SIZE_YEAR, "-");

    printf(" | %*s", TABLE_COL_SIZE_MILEAGE, "-");

    printf(" | %*s", TABLE_COL_SIZE_REPAIRS_N, "-");

    printf(" | %*s", TABLE_COL_SIZE_OWNERS_N, "-");
}

/*
    Выводит часть строки таблицы с информацией о состоянии старой car
*/
void print_car_table_row_state_old(const car_state_old_t *info)
{
    printf(" | %*s", TABLE_COL_SIZE_GUARANTEE, "-");

    printf(" | %*d", TABLE_COL_SIZE_YEAR, info->year);

    printf(" | %*d", TABLE_COL_SIZE_MILEAGE, info->mileage);

    printf(" | %*d", TABLE_COL_SIZE_REPAIRS_N, info->repairs_n);

    printf(" | %*d", TABLE_COL_SIZE_OWNERS_N, info->owners_n);
}

/*
    Выводит строку таблицы с информацией о car
*/
void print_car_table_row(const size_t index, const car_t *car)
{
    printf("| %*lu", TABLE_COL_SIZE_INDEX, index + 1);

    printf(" | %-*s", TABLE_COL_SIZE_BRAND, car->brand);

    printf(" | %-*s", TABLE_COL_SIZE_COUNTRY, car->country);

    printf(" | %*ld", TABLE_COL_SIZE_PRICE, car->price);

    printf(" | %-*s", TABLE_COL_SIZE_COLOR, car->color);

    printf(" | %-*s", TABLE_COL_SIZE_STATE, car->is_new ? "new" : "used");

    if (car->is_new)
    {
        print_car_table_row_state_new(&car->state.new_info);
    }
    else
    {
        print_car_table_row_state_old(&car->state.old_info);
    }

    printf(" |\n");
}
