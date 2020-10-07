#include "../inc/main.h"

void print_menu(void)
{
    printf("\n");
    printf("*********************************************\n");
    printf("*** Записи с вариантами, обработка таблиц ***\n");
    printf("*********************************************\n");
    printf("\n");
    printf("Действия:\n");
    printf("%*s " YEL "- вывести всю таблицу\n" RESET, MENU_ACTION_LEN, MENU_ACTION_LIST);
    printf("%*s " YEL "- добавить запись\n" RESET, MENU_ACTION_LEN, MENU_ACTION_ADD);
    printf("%*s " YEL "- удалить запись\n" RESET, MENU_ACTION_LEN, MENU_ACTION_REMOVE);
    printf("%*s " YEL "- сохранить в файл\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SAVE);
    printf("%*s " YEL "- загрузить из файла\n" RESET, MENU_ACTION_LEN, MENU_ACTION_LOAD);
    printf("%*s " YEL "- вывести цены не новых машин \n", MENU_ACTION_LEN, MENU_ACTION_FIND);
    print_repeat(" ", MENU_ACTION_LEN + 5);
    printf("указанной марки,\n");
    print_repeat(" ", MENU_ACTION_LEN + 5);
    printf("с одним предыдущим собственником,\n");
    print_repeat(" ", MENU_ACTION_LEN + 5);
    printf("отсутствием ремонта и\n");
    print_repeat(" ", MENU_ACTION_LEN + 5);
    printf("в указанном диапазоне цен.\n" RESET);
    printf("%*s " YEL "- выйти\n" RESET, MENU_ACTION_LEN, MENU_ACTION_QUIT);
    printf("\n");
    printf("сортировать по цене:\n");
    printf("\t%*s " YEL "- таблицу данных, пузырьком\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_TABLE_BUBBLE);
    printf("\t%*s " YEL "- таблицу данных, пирамидальная\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_TABLE_HEAPSORT);
    printf("\t%*s " YEL "- таблицу ключей, пузырьком\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_KEY_BUBBLE);
    printf("\t%*s " YEL "- таблицу ключей, пирамидальная\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_KEY_HEAPSORT);
    printf("\t%*s " YEL "- сравнить все способы\n" RESET, MENU_ACTION_LEN, MENU_ACTION_SORT_ALL);
    printf("\n");
}

int main(int argc, char const **argv)
{
    car_t car_table[MAX_TABLE_SIZE];
    size_t car_table_size = 0;

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
            rc = menu_action_list(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_ADD) == 0)
            rc = menu_action_add(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_REMOVE) == 0)
            rc = menu_action_remove(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_SAVE) == 0)
            rc = menu_action_save(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_LOAD) == 0)
            rc = menu_action_load(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_FIND) == 0)
            rc = menu_action_find(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_TABLE_BUBBLE) == 0)
            rc = menu_action_sort_table_bubble(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_TABLE_HEAPSORT) == 0)
            rc = menu_action_sort_table_heapsort(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_KEY_BUBBLE) == 0)
            rc = menu_action_sort_key_bubble(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_KEY_HEAPSORT) == 0)
            rc = menu_action_sort_key_heapsort(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_SORT_ALL) == 0)
            rc = menu_action_sort_all(car_table, &car_table_size);
        else if (strcmp(action, MENU_ACTION_QUIT) != 0)
        {
            printf("\n" RED "Неправильный ввод.\n" RESET);
            if (feof(stdin))
                rc = EOF;
        }
    }

    return rc == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
