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

    printf("[X] / [_] =\n");
    printf("Enter dividend:\n");

    while(scanf_decimal(&a) != OK)
    {
        scanf_line();
        printf("Invalid input\n\n");
        printf("Enter dividend:\n");
    }

    print_decimal(&a);
    printf(" / [X] =\n");
    printf("Enter divider:\n");

    while(scanf_decimal(&b) != OK)
    {
        scanf_line();
        printf("Invalid input\n\n");
        printf("Enter divider:\n");
    }

    if (divide_decimal(&a, &b, &ans) != OK)
    {
        printf("Error on division.\n");
        return EXIT_FAILURE;
    }

    printf("Result:\n");

    print_decimal(&a);
    printf(" / ");
    print_decimal(&b);
    printf(" = ");
    print_decimal(&ans);
    printf("\n");

    return EXIT_SUCCESS;
}
