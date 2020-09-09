#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "io.h"

int scanf_decimal_float(t_decimal *val)
{
    char c;
    int digit_n = 0;
    t_digit *digit_p = val->digits;
    bool have_point = false;
    bool have_exp = false;
    bool start_zeros = true;
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
        start_zeros = false;
    }
    else if (c == '0')
    {
        digit_n++;
    }
    else if ('0' < c && c <= '9')
    {
        digit_n++;
        start_zeros = false;
        *digit_p++ = c - '0';
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
            if (digit_n >= MANTISSA_LEN)
                return ERR;
            digit_n++;
            if (start_zeros && c == '0')
                continue;
            start_zeros = false;
            *digit_p++ = c - '0';
        }
        else if (c == '.')
        {
            if (have_point)
                return ERR;
            have_point = true;
            start_zeros = false;
            val->point = (int) (digit_p - val->digits) / sizeof(t_digit);
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

    if (have_point && digit_n == 0)
        return ERR;

    if (!have_point)
        val->point = (int) (digit_p - val->digits) / sizeof(t_digit);

    if (!have_exp)
    {
        // read line to end
        while (c != '\n')
            if (scanf("%c", &c) != 1 || (c != ' ' && c != '\n'))
                return ERR;
        return OK;
    }

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

    // read line to end
    while (c != '\n')
        if (scanf("%c", &c) != 1 || (c != ' ' && c != '\n'))
            return ERR;

    return OK;
}

int scanf_decimal_int(t_decimal *val)
{
    char c;
    int digit_n = 0;
    t_digit *digit_p = val->digits;
    bool start_zeros = true;

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
    else if (c == '0')
    {
        digit_n++;
    }
    else if ('0' < c && c <= '9')
    {
        digit_n++;
        start_zeros = false;
        *digit_p++ = c - '0';
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
            if (digit_n >= MANTISSA_LEN)
                return ERR;
            digit_n++;
            if (start_zeros && c == '0')
                continue;
            start_zeros = false;
            *digit_p++ = c - '0';
        }
        else if (c == '\n' || c == ' ')
            break;
        else
            return ERR;
    }

    // read line to end
    while (c != '\n')
        if (scanf("%c", &c) != 1 || (c != ' ' && c != '\n'))
            return ERR;

    val->point = (int) (digit_p - val->digits) / sizeof(t_digit);

    return OK;
}

void print_decimal_float(const t_decimal *val)
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

void print_decimal_int(const t_decimal *val)
{
    if (val->sign < 0)
        printf("-");

    for (int i = 0; i < val->point; i++)
        printf("%d", val->digits[i]);
}
