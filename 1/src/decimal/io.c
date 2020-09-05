#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "io.h"

int scanf_decimal_float(decimal_t *val)
{
    char c;
    int digit_i = 0;
    bool have_point = false;
    bool have_exp = false;
    int exp_sign = 1;

    set_zero_decimal(val);

    // read first non empty char
    do
    {
        if (scanf("%c", &c) != 1)
            return ERR;
    } while (c == ' ' || c == '\n');

    // process first char of mantissa
    if (c == '+')
        val->sign = 1;
    else if (c == '-')
        val->sign = -1;
    else if (c == '.')
    {
        val->point = 0;
        have_point = true;
    }
    else if ('0' <= c && c <= '9')
    {
        val->digits[digit_i++] = c - '0';
    }
    else
        return ERR;

    // process chars of mantissa
    while (true)
    {
        // read next char
        if (scanf("%c", &c) != 1)
            return ERR;

        if ('0' <= c && c <= '9')
        {
            if (digit_i >= MANTISSA_LEN)
                return ERR;
            val->digits[digit_i++] = c - '0';
        }
        else if (c == '.')
        {
            if (have_point)
                return ERR;
            have_point = true;
            val->point = digit_i;
        }
        else if (c == 'e' || c == 'E')
        {
            have_exp = true;
            break;
        }
        else if (c == '\n' || c == ' ')
            break;
        else
            return ERR;
    }

    if (!have_point)
        val->point = digit_i;

    if (!have_exp)
        return OK;

    if (scanf("%c", &c) != 1)
        return ERR;

    // process first char of exponent
    if (c == '+')
        exp_sign = 1;
    else if (c == '-')
        exp_sign = -1;
    else if ('0' <= c && c <= '9')
        val->exponent = c - '0';
    else
        return ERR;

    // process chars of exponent
    while (1)
    {
        if (scanf("%c", &c) != 1)
            return ERR;

        if ('0' <= c && c <= '9')
        {
            val->exponent = val->exponent * 10 + c - '0';
            if (val->exponent > MAX_EXPONENT)
                return ERR;
        }
        else if (c == '\n' || c == ' ')
            break;
        else
            return ERR;
    }

    val->exponent *= exp_sign;

    return OK;
}

int scanf_decimal_int(decimal_t *val)
{
    char c;
    int digit_i = 0;

    set_zero_decimal(val);

    // read first non empty char
    do
    {
        if (scanf("%c", &c) != 1)
            return ERR;
    } while (c == ' ' || c == '\n');

    // process first char of mantissa
    if (c == '+')
        val->sign = 1;
    else if (c == '-')
        val->sign = -1;
    else if ('0' <= c && c <= '9')
    {
        val->digits[digit_i++] = c - '0';
    }
    else
        return ERR;

    // process chars of mantissa
    while (true)
    {
        // read next char
        if (scanf("%c", &c) != 1)
            return ERR;

        if ('0' <= c && c <= '9')
        {
            if (digit_i >= MANTISSA_LEN)
                return ERR;
            val->digits[digit_i++] = c - '0';
        }
        else if (c == '\n' || c == ' ')
            break;
        else
            return ERR;
    }

    val->point = digit_i;

    return OK;
}

void print_decimal_float(const decimal_t *val)
{
    printf("%c", val->sign >= 0 ? '+' : '-');

    if (val->point == 0)
        printf("0"); // print leading 0 if no digits before point

    for (int i = 0; i < val->point; i++)
        printf("%d", val->digits[i]);

    printf(".");

    int end = MANTISSA_LEN - 1; // find last non zero digit after point
    while (end > val->point && val->digits[end] == 0)
        end--;

    for (int i = val->point; i <= end; i++)
        printf("%d", val->digits[i]);

    printf("E%+d", val->exponent);
}

void print_decimal_int(const decimal_t *val)
{
    printf("%c", val->sign >= 0 ? ' ' : '-');

    for (int i = 0; i < val->point; i++)
        printf("%d", val->digits[i]);
}
