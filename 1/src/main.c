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

/*
  Считывает с консоли строку до конца
*/
void scanf_line()
{
    char c;
    do
    {
        if (scanf("%c", &c) != 1)
            return;
    } while (c != '\n');
}

int main()
{
    t_decimal a;
    t_decimal b;
    t_decimal ans;

    print_hello();

    printf("[X] / [_] =\n");
    printf("Введите числитель (вещественное):\n");
    print_ruler();

    while(scanf_decimal_float(&a) != OK)
    {
        scanf_line();
        printf("Неправильный ввод\n\n");
        printf("Введите числитель (вещественное):\n");
        print_ruler();
    }

    print_decimal_float(&a);
    printf(" / [X] =\n");
    printf("Введите знаменталь (целое):\n");
    print_ruler();

    while(scanf_decimal_int(&b) != OK)
    {
        scanf_line();
        printf("Неправильный ввод\n\n");
        printf("Введите знаменатель (целое):\n");
        print_ruler();
    }

    if (divide_decimal(&a, &b, &ans) != OK)
    {
        printf("Ошибка деления\n\n");
        return EXIT_FAILURE;
    }

    printf("Результат:\n");

    print_decimal_float(&a);
    printf(" / ");
    print_decimal_int(&b);
    printf(" = ");
    print_decimal_float(&ans);
    printf("\n");

    return EXIT_SUCCESS;
}
