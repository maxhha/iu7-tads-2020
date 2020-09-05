#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "decimal.h"

void print_hello()
{

    printf("******************************\n");
    printf("*                            *\n");
    printf("* Long float number division *\n");
    printf("*                            *\n");
    printf("******************************\n");
    printf("\n");
    printf("Examples:\n");
    printf("\n");
    printf("1        10        20       30\n");
    printf("|--------|---------|---------|\n");
    printf("\n");
}

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
    decimal_t a;
    decimal_t b;
    decimal_t ans;

    print_hello();

    printf("[(float) X] / [(int) _] =\n");
    printf("Enter dividend (float):\n");

    while(scanf_decimal_float(&a) != OK)
    {
        scanf_line();
        printf("Invalid input\n\n");
        printf("Enter dividend (float):\n");
    }

    print_decimal_float(&a);
    printf(" / [(int) X] =\n");
    printf("Enter divider (int):\n");

    while(scanf_decimal_int(&b) != OK)
    {
        scanf_line();
        printf("Invalid input\n\n");
        printf("Enter divider (int):\n");
    }

    if (divide_decimal(&a, &b, &ans) != OK)
    {
        printf("Error on division.\n");
        return EXIT_FAILURE;
    }

    printf("Result:\n");

    print_decimal_float(&a);
    printf(" / ");
    print_decimal_int(&b);
    printf(" = ");
    print_decimal_float(&ans);
    printf("\n");

    return EXIT_SUCCESS;
}
