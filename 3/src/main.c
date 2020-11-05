#include "../inc/main.h"

void print_menu(void)
{
    printf("\n");
    printf("*********************************************\n");
    printf("***     Обработка разреженных матриц      ***\n");
    printf("*********************************************\n");
    printf("\n");
    printf("Действия умножения строку на матрицу\n");
    printf("%*s " YEL "- используя полные матрицы\n" RESET, MENU_ACTION_LEN, MENU_ACTION_MUL_M);
    printf("%*s " YEL "- используя разреженные матрицы\n" RESET, MENU_ACTION_LEN, MENU_ACTION_MUL_S);
    printf("\n");
    printf("Действия имерения времени умножения строку на матрицу\n");
    printf("%*s " YEL "- используя полные матрицы\n" RESET, MENU_ACTION_LEN, MENU_ACTION_MEASURE_M);
    printf("%*s " YEL "- используя разреженные матрицы\n" RESET, MENU_ACTION_LEN, MENU_ACTION_MEASURE_S);
    printf("\n");
    printf("%*s " YEL "- выйти\n" RESET, MENU_ACTION_LEN, MENU_ACTION_QUIT);
    printf("\n");
}

int main(int argc, char const **argv)
{
    setbuf(stdout, NULL);
    srand(time(NULL));

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
            wait_endl();
            if (feof(stdin))
                rc = EOF;
        }
        else if (strcmp(action, MENU_ACTION_MUL_M) == 0)
            rc = action_mul_matrix();
        else if (strcmp(action, MENU_ACTION_MUL_S) == 0)
            rc = action_mul_smatrix();
        else if (strcmp(action, MENU_ACTION_MEASURE_M) == 0)
            rc = action_measure_matrix_mul();
        else if (strcmp(action, MENU_ACTION_MEASURE_S) == 0)
            rc = action_measure_smatrix_mul();
        else if (strcmp(action, MENU_ACTION_QUIT) != 0)
        {
            printf("\n" RED "Неправильный ввод.\n" RESET);
            if (feof(stdin))
                rc = EOF;
        }
    }

    return rc == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
