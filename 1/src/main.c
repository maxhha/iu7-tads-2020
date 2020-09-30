#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "decimal.h"

/*
  Выводит в консоль приветственное сообщение c описанием программы
*/
void print_hello()
{

    printf("******************************\n");
    printf("*                            *\n");
    printf("*    Деление длинных чисел   *\n");
    printf("*                            *\n");
    printf("******************************\n");
    printf("\n");
    printf("Примеры ввода целого числа:\n");
    printf("10\n");
    printf("  +2\n");
    printf("-007\n");
    printf("+999999999999999999999999999999\n");
    printf("\n");
    printf("Примеры ввода вещественного числа\n");
    printf("<все примеры ввода целого числа>\n");
    printf(".2\n");
    printf("-.5600E20\n");
    printf("  +1.e-23\n");
    printf("+.999999999999999999999999999999E+99999\n");
    printf("\n");
}

/*
  Выводит в консоль линейку в 30 символов
*/
void print_ruler()
{
    printf("1        10        20       30\n");
    printf("|--------|---------|---------|\n");
}

int main()
{
    t_decimal a;
    t_decimal b;
    t_decimal ans;

    print_hello();

    printf("Введите числитель (вещественное):\n");
    print_ruler();

    if (scanf_decimal_float(&a) != OK)
    {
        printf("Неправильный ввод\n");
        return EXIT_FAILURE;
    }

    printf("\nВведите знаменталь (целое):\n");
    print_ruler();

    if (scanf_decimal_int(&b) != OK)
    {
        printf("Неправильный ввод\n");
        return EXIT_FAILURE;
    }

    int rc = divide_decimal(&a, &b, &ans);

    if (rc == EZERODIVISION)
    {
        printf("Ошибка деления на 0\n\n");
        return EXIT_FAILURE;
    }
    if (rc == EEXPOVERFLOW)
    {
        printf("Ошибка переполнения порядка\n\n");
        return EXIT_FAILURE;
    }
    else if (rc != OK)
    {
        printf("Ошибка деления\n\n");
        return EXIT_FAILURE;
    }

    print_decimal_float(&a);
    printf(" / ");
    print_decimal_int(&b);
    printf("\nРезультат:\n");
    print_decimal_float(&ans);
    printf("\n");

    return EXIT_SUCCESS;
}
