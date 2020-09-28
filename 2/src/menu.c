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
}

int menu_action_list(car_t **car_table)
{
    printf("\n");
    if (vec_count(*car_table) == 0)
    {
        printf(RED "В таблице нет данных.\n" RESET);
        return OK;
    }

    print_car_table_header();
    for (int i = 0; i < vec_count(*car_table); i++)
        print_car_table_row(i, *car_table + i);

    return OK;
}

int menu_action_add(car_t **car_table)
{
    car_t car;

    printf("\n");
    if (read_car(stdin, stdout, &car) != OK)
    {
        printf(RED "Неправильный ввод.\n" RESET);
        return OK;
    }

    vec_push(*car_table, car);

    printf(GRN "\nДобавлена:\n" RESET);
    print_car_table_header();
    print_car_table_row(vec_count(*car_table) - 1, &car);

    return OK;
}

int menu_action_remove(car_t **car_table)
{
    int i;
    printf("Введите номер автомобиля:\n");
    if (scanf("%d", &i) != 1)
    {
        printf(RED "Неправильный ввод.\n" RESET);
        return OK;
    }
    wait_new_line();

    if (i < 1 || i > vec_count(*car_table))
    {
        printf(RED "Неправильный номер.\n" RESET);
        return OK;
    }
    i--;

    printf(GRN "Удалили:\n" RESET);
    print_car_table_header();
    print_car_table_row(i, *car_table + i);

    vec_remove(*car_table, i);

    return OK;
}

int menu_action_save(car_t **car_table)
{
    char s[FILE_NAME_LEN + 1];
    printf("\nВведите название файла:\n");

    if (fgetline(s, FILE_NAME_LEN + 1, stdin) == NULL)
    {
        printf(RED "Слишком длинное название.\n" RESET);
        return OK;
    }

    FILE *f = fopen(s, "wb");
    if (f == NULL)
    {
        printf(RED "Ошибка открытия файла.\n" RESET);
        return EWRITE;
    }

    fwrite(*car_table, sizeof(car_t), vec_count(*car_table), f);

    fclose(f);

    printf(GRN "\nСохранено %d строк.\n" RESET, vec_count(*car_table));

    return OK;
}

int menu_action_load(car_t **car_table)
{
    char s[FILE_NAME_LEN + 1];
    printf("\nВведите название файла:\n");

    if (fgetline(s, FILE_NAME_LEN + 1, stdin) == NULL)
    {
        printf(RED "Слишком длинное название.\n" RESET);
        return OK;
    }

    FILE *f = fopen(s, "rb");
    if (f == NULL)
    {
        printf(RED "Файл не найден.\n" RESET);
        return OK;
    }

    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);

    if (filesize < 0 || filesize % sizeof(car_t) != 0)
    {
        printf(RED "Неправильный размер файла.\n" RESET);
        return OK;
    }

    fseek(f, 0, SEEK_SET);

    filesize /= sizeof(car_t);
    long need_add = filesize - vec_count(*car_table);

    if (need_add > 0)
        vec_add(*car_table, need_add);

    fread(*car_table, sizeof(car_t), filesize, f);

    fclose(f);

    printf(GRN "\nЗагружено %ld строк.\n" RESET, filesize);

    return OK;
}


int run_menu(car_t **car_table)
{
    char action[MENU_ACTION_LEN+1] = "";
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
            rc = menu_action_list(car_table);
        else if (strcmp(action, MENU_ACTION_ADD) == 0)
            rc = menu_action_add(car_table);
        else if (strcmp(action, MENU_ACTION_REMOVE) == 0)
            rc = menu_action_remove(car_table);
        else if (strcmp(action, MENU_ACTION_SAVE) == 0)
            rc = menu_action_save(car_table);
        else if (strcmp(action, MENU_ACTION_LOAD) == 0)
            rc = menu_action_load(car_table);
        else if (strcmp(action, MENU_ACTION_QUIT) != 0)
        {
            printf(RED "Неправильный ввод.\n" RESET);
            if (feof(stdin))
                rc = EOF;
        }
    }

    return rc;
}
