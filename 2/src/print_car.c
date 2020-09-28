#include "../inc/print_car.h"

/*
    Выводит строку n раз
*/
#define print_repeat(c, n) do { for(int i = 0; i < n; i++) printf(c); } while(0)

/*
    Выводит шапку таблицы машин
*/
void print_car_table_header(void)
{

    printf(GRN "|" RESET " #");
    print_repeat(" ", TABLE_COL_SIZE_INDEX - 1);

    printf(GRN " | " RESET "Марка");
    print_repeat(" ", TABLE_COL_SIZE_BRAND - 5);

    printf(GRN " | " RESET "Страна");
    print_repeat(" ", TABLE_COL_SIZE_COUNTRY - 6);

    printf(GRN " | " RESET "Цена");
    print_repeat(" ", TABLE_COL_SIZE_PRICE - 4);

    printf(GRN " | " RESET "Цвет");
    print_repeat(" ", TABLE_COL_SIZE_COLOR - 4);

    printf(GRN " | " RESET "Состояние");
    print_repeat(" ", TABLE_COL_SIZE_STATE - 9);

    printf(GRN " | " RESET "Гарантия");
    print_repeat(" ", TABLE_COL_SIZE_GUARANTEE - 8);

    printf(GRN " | " RESET "Выпуск");
    print_repeat(" ", TABLE_COL_SIZE_YEAR - 6);

    printf(GRN " | " RESET "Пробег");
    print_repeat(" ", TABLE_COL_SIZE_MILEAGE - 6);

    printf(GRN " | " RESET "Ремонтов");
    print_repeat(" ", TABLE_COL_SIZE_REPAIRS_N - 8);

    printf(GRN " | " RESET "Собственников");
    print_repeat(" ", TABLE_COL_SIZE_OWNERS_N - 13);

    printf(GRN " |\n" RESET);

    printf(GRN "| " YEL);
    print_repeat("-", TABLE_COL_SIZE_INDEX);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_BRAND);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_COUNTRY);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_PRICE);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_COLOR);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_STATE);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_GUARANTEE);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_YEAR);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_MILEAGE);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_REPAIRS_N);

    printf(GRN " | " YEL);
    print_repeat("-", TABLE_COL_SIZE_OWNERS_N);

    printf(GRN " |\n" RESET);
}

/*
    Выводит часть строки таблицы с информацией о состоянии новой car
*/
void print_car_table_row_state_new(const car_state_new_t *info)
{
    printf(GRN " | " RESET "%*d", TABLE_COL_SIZE_GUARANTEE, info->guarantee);

    printf(GRN " | " RESET "%*s", TABLE_COL_SIZE_YEAR, "-");

    printf(GRN " | " RESET "%*s", TABLE_COL_SIZE_MILEAGE, "-");

    printf(GRN " | " RESET "%*s", TABLE_COL_SIZE_REPAIRS_N, "-");

    printf(GRN " | " RESET "%*s", TABLE_COL_SIZE_OWNERS_N, "-");
}

/*
    Выводит часть строки таблицы с информацией о состоянии старой car
*/
void print_car_table_row_state_old(const car_state_old_t *info)
{
    printf(GRN " | " RESET "%*s", TABLE_COL_SIZE_GUARANTEE, "-");

    printf(GRN " | " RESET "%*d", TABLE_COL_SIZE_YEAR, info->year);

    printf(GRN " | " RESET "%*d", TABLE_COL_SIZE_MILEAGE, info->mileage);

    printf(GRN " | " RESET "%*d", TABLE_COL_SIZE_REPAIRS_N, info->repairs_n);

    printf(GRN " | " RESET "%*d", TABLE_COL_SIZE_OWNERS_N, info->owners_n);
}

/*
    Выводит строку таблицы с информацией о car
*/
void print_car_table_row(const size_t index, const car_t *car)
{
    printf(GRN "| " RESET "%*lu", TABLE_COL_SIZE_INDEX, index + 1);

    printf(GRN " | " RESET "%-*s", TABLE_COL_SIZE_BRAND, car->brand);

    printf(GRN " | " RESET "%-*s", TABLE_COL_SIZE_COUNTRY, car->country);

    printf(GRN " | " RESET "%*ld", TABLE_COL_SIZE_PRICE, car->price);

    printf(GRN " | " RESET "%-*s", TABLE_COL_SIZE_COLOR, car->color);

    printf(GRN " | " RESET "%-*s", TABLE_COL_SIZE_STATE, car->is_new ? "new" : "used");

    if (car->is_new)
    {
        print_car_table_row_state_new(&car->state.new_info);
    }
    else
    {
        print_car_table_row_state_old(&car->state.old_info);
    }

    printf(GRN " |\n" RESET);
}
