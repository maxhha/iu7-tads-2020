#include "../inc/menu.h"

void print_menu(void)
{
    printf("\n");
    printf("Действия:\n");
    printf("%*s " YEL "- вывести всю таблицу\n" RESET, MENU_ACTION_LEN, MENU_ACTION_LIST);
    printf("%*s " YEL "- добавить запись\n" RESET, MENU_ACTION_LEN, MENU_ACTION_ADD);
    printf("%*s " YEL "- удалить запись\n" RESET, MENU_ACTION_LEN, MENU_ACTION_REMOVE);
    printf("%*s " YEL "- сохранить в файл\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SAVE);
    printf("%*s " YEL "- загрузить из файла\n" RESET, MENU_ACTION_LEN, MENU_ACTION_LOAD);
    printf("%*s " YEL "- выйти\n" RESET, MENU_ACTION_LEN, MENU_ACTION_QUIT);
    printf("\n");
    printf("сортировать по цене:\n");
    printf("\t%*s " YEL "- таблицу данных, пузырьком\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_TABLE_BUBBLE);
    printf("\t%*s " YEL "- таблицу данных, пирамидальная\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_TABLE_HEAPSORT);
    printf("\t%*s " YEL "- таблицу ключей, пузырьком\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_KEY_BUBBLE);
    printf("\t%*s " YEL "- таблицу ключей, пирамидальная,\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_KEY_HEAPSORT);
    printf("\t%*s " YEL "- сравнить все способы\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_ALL);
    printf("\n");
}

int menu_action_list(car_t *car_table, size_t *car_table_size)
{
    printf("\n");
    if (*car_table_size == 0)
    {
        printf(RED "В таблице нет данных.\n" RESET);
        return OK;
    }

    print_car_table_header();
    for (int i = 0; i < *car_table_size; i++)
        print_car_table_row(i, car_table + i);

    return OK;
}

int menu_action_add(car_t *car_table, size_t *car_table_size)
{
    car_t car;

    if (*car_table_size == MAX_TABLE_SIZE)
    {
        printf(RED "Таблица уже максимального размера.\n" RESET);
        return OK;
    }

    printf("\n");
    if (read_car(&car) != OK)
        return OK;

    car_table[(*car_table_size)++] = car;

    printf(GRN "\nДобавлена:\n" RESET);
    print_car_table_header();
    print_car_table_row(*car_table_size - 1, &car);

    return OK;
}

int menu_action_remove(car_t *car_table, size_t *car_table_size)
{
    int i;
    printf("Введите номер автомобиля:\n");
    if (scanf("%d", &i) != 1)
    {
        printf(RED "Неправильный ввод.\n" RESET);
        return OK;
    }
    wait_new_line();

    if (i < 1 || i > *car_table_size)
    {
        printf(RED "Неправильный номер.\n" RESET);
        return OK;
    }
    i--;

    printf(GRN "Удалили:\n" RESET);
    print_car_table_header();
    print_car_table_row(i, car_table + i);

    *car_table_size -= 1;

    car_t *last = car_table + *car_table_size;

    for (car_t *j = car_table + i; j < last; j++)
        *j = *(j + 1);

    return OK;
}

int menu_action_save(car_t *car_table, size_t *car_table_size)
{
    char s[FILE_NAME_LEN + 1];
    printf("\nВведите название файла:\n");

    if (fgetline(s, FILE_NAME_LEN + 1, stdin) == NULL)
    {
        printf(RED "Слишком длинное название.\n" RESET);
        return OK;
    }

    if (write_cars_to_csv(s, car_table, *car_table_size) != OK)
        return OK;

    printf(GRN "\nСохранено %lu строк.\n" RESET, *car_table_size);

    return OK;
}

int menu_action_load(car_t *car_table, size_t *car_table_size)
{
    char s[FILE_NAME_LEN + 1];
    printf("\nВведите название файла:\n");

    if (fgetline(s, FILE_NAME_LEN + 1, stdin) == NULL)
    {
        printf(RED "Слишком длинное название.\n" RESET);
        return OK;
    }

    if (read_cars_from_csv(s, car_table, car_table_size) != OK)
        return OK;

    printf(GRN "\nЗагружено %ld строк.\n" RESET, *car_table_size);

    return OK;
}

int compare_cars_by_price(const void *car_a, const void *car_b)
{
    return ((const car_t *) car_a)->price - ((const car_t *) car_b)->price;
}

int compare_car_keys_by_price(const void *car_a, const void *car_b)
{
    return ((const car_key_t *) car_a)->price - ((const car_key_t *) car_b)->price;
}

typedef void (*sort_func_t)(void *, size_t, size_t, cmp_func_t);

double measure_sort_table(car_t *car_table, size_t *car_table_size, sort_func_t sort, bool show)
{
    size_t cars_count = *car_table_size;
    car_t car_table2[MAX_TABLE_SIZE];

    memcpy(car_table2, car_table, cars_count*sizeof(*car_table));

    clock_t start, stop;
    start = clock();

    (*sort) (car_table2, cars_count, sizeof(*car_table2), compare_cars_by_price);

    stop = clock();

    double time = (double) (stop - start) / CLOCKS_PER_SEC * MICROSEC_PER_SEC;

    if (show)
    {
        print_car_table_header();
        for (int i = 0; i < cars_count; i++)
            print_car_table_row(i, car_table2 + i);
    }

    return time;
}

double measure_sort_key_table(car_t *car_table, car_key_t *key_table, size_t *car_table_size, sort_func_t sort, bool show)
{
    size_t cars_count = *car_table_size;
    car_key_t key_table2[MAX_TABLE_SIZE];

    memcpy(key_table2, key_table, cars_count*sizeof(*key_table));

    clock_t start, stop;
    start = clock();

    (*sort) (key_table2, cars_count, sizeof(*key_table2), compare_car_keys_by_price);

    stop = clock();

    double time = (double) (stop - start) / CLOCKS_PER_SEC * MICROSEC_PER_SEC;

    if (show)
    {
        print_car_key_table_header();
        for (int i = 0; i < cars_count; i++)
            print_car_key_table_row(key_table2 + i);

        printf("Показать отсортированную исходную таблицу?\n");
        printf("(пусто - нет, любой символ - да)\n");

        if (getchar() != '\n')
        {
            wait_new_line();
            print_car_table_header();
            for (int i = 0; i < cars_count; i++)
                print_car_table_row((key_table2 + i)->index, car_table + (key_table2 + i)->index);
        }
    }

    return time;
}

int menu_action_sort_table_bubble(car_t *car_table, size_t *car_table_size)
{
    double time = 0;

    if (*car_table_size == 0)
    {
        printf(RED "Таблица пустая.\n" RESET);
        return OK;
    }

    for (int i = 1; i <= TIME_MEASURE_REPEATS; i++)
    {
        time += measure_sort_table(car_table, car_table_size, bsort, false);
    }

    time /= TIME_MEASURE_REPEATS;

    measure_sort_table(car_table, car_table_size, bsort, true);

    printf(YEL "Сортировка таблицы пузырьком:" RESET " %12.0lfms\n", time);

    return OK;
}

int menu_action_sort_table_heapsort(car_t *car_table, size_t *car_table_size)
{
    double time;

    if (*car_table_size == 0)
    {
        printf(RED "Таблица пустая.\n" RESET);
        return OK;
    }

    for (int i = 1; i <= TIME_MEASURE_REPEATS; i++)
    {
        time += measure_sort_table(car_table, car_table_size, hsort, false);
    }

    time /= TIME_MEASURE_REPEATS;

    measure_sort_table(car_table, car_table_size, hsort, true);

    printf(YEL "Пирамидальная сортировка таблицы:" RESET " %12.0lfms\n", time);

    return OK;
}

int menu_action_sort_key_bubble(car_t *car_table, size_t *car_table_size)
{
    if (*car_table_size == 0)
    {
        printf(RED "Таблица пустая.\n" RESET);
        return OK;
    }

    car_key_t car_keys[MAX_TABLE_SIZE];

    for (size_t i = 0; i < *car_table_size; i++)
    {
        car_keys[i].index = i;
        car_keys[i].price = car_table[i].price;
    }

    double time = 0;

    for (int i = 1; i <= TIME_MEASURE_REPEATS; i++)
    {
        time += measure_sort_key_table(car_table, car_keys, car_table_size, bsort, false);
    }

    time /= TIME_MEASURE_REPEATS;

    measure_sort_key_table(car_table, car_keys, car_table_size, bsort, true);

    printf(YEL "Сортировка таблицы ключей пузырьком:" RESET " %12.0lfms\n", time);

    return OK;
}

int menu_action_sort_key_heapsort(car_t *car_table, size_t *car_table_size)
{

    if (*car_table_size == 0)
    {
        printf(RED "Таблица пустая.\n" RESET);
        return OK;
    }

    car_key_t car_keys[MAX_TABLE_SIZE];

    for (size_t i = 0; i < *car_table_size; i++)
    {
        car_keys[i].index = i;
        car_keys[i].price = car_table[i].price;
    }

    double time;

    for (int i = 1; i <= TIME_MEASURE_REPEATS; i++)
    {
        time += measure_sort_key_table(car_table, car_keys, car_table_size, hsort, false);
    }

    time /= TIME_MEASURE_REPEATS;

    measure_sort_key_table(car_table, car_keys, car_table_size, hsort, true);

    printf(YEL "Пирамидальная сортировка таблицы ключей:" RESET " %12.0lfms\n", time);

    return OK;
}

int menu_action_sort_all(car_t *car_table, size_t *car_table_size)
{
    if (*car_table_size == 0)
    {
        printf(RED "Таблица пустая.\n" RESET);
        return OK;
    }

    car_key_t car_keys[MAX_TABLE_SIZE];

    for (size_t i = 0; i < *car_table_size; i++)
    {
        car_keys[i].index = i;
        car_keys[i].price = car_table[i].price;
    }

    double timetb = 0, timeth = 0, timekb = 0, timekh = 0;

    for (int i = 1; i <= TIME_MEASURE_REPEATS; i++)
    {
        timetb += measure_sort_table(car_table, car_table_size, bsort, false);
        timeth += measure_sort_table(car_table, car_table_size, hsort, false);
        timekb += measure_sort_key_table(car_table, car_keys, car_table_size, bsort, false);
        timekh += measure_sort_key_table(car_table, car_keys, car_table_size, hsort, false);
    }

    timetb /= TIME_MEASURE_REPEATS;
    timeth /= TIME_MEASURE_REPEATS;
    timekb /= TIME_MEASURE_REPEATS;
    timekh /= TIME_MEASURE_REPEATS;

    printf(I_ "Сортировка   " _I_ "таблица данных" _I_ "таблица ключей" _I_n);
    printf(I_ YEL);
    print_repeat("-", 13);
    printf(_I_ YEL);
    print_repeat("-", 14);
    printf(_I_ YEL);
    print_repeat("-", 14);
    printf(_I_n);

    printf(I_ "пузырьком    " _I_ "%12.0lfms" _I_ "%12.0lfms" _I_n, timetb, timekb);
    printf(I_ "пирамидальная" _I_ "%12.0lfms" _I_ "%12.0lfms" _I_n, timeth, timekh);

    return OK;
}

int run_menu(car_t *car_table, size_t *car_table_size)
{
    char action[MENU_ACTION_LEN + 1] = "";
    int rc = OK;

    while (strcmp(action, MENU_ACTION_QUIT) != 0 && rc == OK)
    {
        print_menu();
        printf("Выбор:\n");

        if (fgetline(action, MENU_ACTION_LEN + 1, stdin) == NULL)
        {
            printf(RED "Неправильный ввод.\n" RESET);
            *action = '\0';
            wait_new_line();
            if (feof(stdin))
                rc = EOF;
        }
        else if (strcmp(action, MENU_ACTION_LIST) == 0)
            rc = menu_action_list(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_ADD) == 0)
            rc = menu_action_add(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_REMOVE) == 0)
            rc = menu_action_remove(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_SAVE) == 0)
            rc = menu_action_save(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_LOAD) == 0)
            rc = menu_action_load(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_TABLE_BUBBLE) == 0)
            rc = menu_action_sort_table_bubble(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_TABLE_HEAPSORT) == 0)
            rc = menu_action_sort_table_heapsort(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_KEY_BUBBLE) == 0)
            rc = menu_action_sort_key_bubble(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_KEY_HEAPSORT) == 0)
            rc = menu_action_sort_key_heapsort(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_ALL) == 0)
            rc = menu_action_sort_all(car_table, car_table_size);
        else if (strcmp(action, MENU_ACTION_QUIT) != 0)
        {
            printf("\n" RED "Неправильный ввод.\n" RESET);
            if (feof(stdin))
                rc = EOF;
        }
    }

    return rc;
}
