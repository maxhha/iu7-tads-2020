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
    printf("Примеры:\n");
    printf("\n");
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

    if (divide_decimal(&a, &b, &ans) != OK)
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
