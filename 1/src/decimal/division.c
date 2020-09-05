#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "division.h"

int safe_add_int(int a, int b, int *s)
{
    *s = a + b;
    if (b > 0)
        return *s < a ? ERR : OK;
    return *s > a ? ERR : OK;
}

int divide_decimal(const decimal_t *dividend, const decimal_t *divider, decimal_t *quotient)
{
    set_zero_decimal(quotient);

    /*
     + 1 to length for leading zero if dividend is less then divider
     + 1 to length for last digit used for rounding
    */
    #define QUOTIENT_DIGITS_LEN (MANTISSA_LEN + 2)
    signed char q_digits[QUOTIENT_DIGITS_LEN] = {0};

    /*
     MANTISSA_LEN * 2 for accumulate subtitution for last digit in q_digits
     + 1 to array length for leading zero if dividend is less then divider
    */
    #define DIVIDEND_DIGITS_LEN (MANTISSA_LEN * 2 + 1)
    signed char digits[DIVIDEND_DIGITS_LEN] = {0};

    memcpy(digits, dividend->digits, sizeof(dividend->digits));

    signed char divider_digits[MANTISSA_LEN] = {0};

    // skip first 0-s in divider digits
    int start = 0;
    while (start < MANTISSA_LEN && divider->digits[start] == 0)
        start++;

    if (start == MANTISSA_LEN)
        return ERR;

    memcpy(
        divider_digits,
        divider->digits + start,
        sizeof(divider->digits) / MANTISSA_LEN * ( MANTISSA_LEN - start)
    );

    if (safe_add_int(dividend->exponent, -divider->exponent, &quotient->exponent) != OK)
        return ERR;

    quotient->exponent += dividend->point - divider->point + start;

    quotient->sign = dividend->sign * divider->sign;

    // divide numbers using long division algorithm
    int i = 0;
    while (i < QUOTIENT_DIGITS_LEN)
    {
        #ifdef DEBUG
            printf("\n");
            for (int k = 0; k < DIVIDEND_DIGITS_LEN; k++)
                printf("%d", digits[k]);
            printf("\n");

            for (int k = 0; k < i; k++)
                printf(" ");
            for (int k = 0; k < MANTISSA_LEN; k++)
                printf("%d", divider_digits[k]);
            printf("\n");
        #endif

        // compare dividend and divider
        int compare = 0;

        for (int j = 0; j < i && compare == 0; j++)
        {
            compare = digits[j] != 0;
        }

        for (int j = 0; j < MANTISSA_LEN && compare == 0; j++)
        {
            signed char a = digits[j + i];
            signed char b = divider_digits[j];
            compare = a == b ? 0 : a < b ? -1 : 1;
        }

        #ifdef DEBUG
            if (compare == 0)
                printf("dividend is equal\n");
            else if (compare > 0)
                printf("dividend is greater\n");
            else
                printf("dividend is less\n");
        #endif

        if (compare < 0)
        {
            i++; // shift divider digits
            #ifdef DEBUG
                for (int k = 0; k <= i; k++)
                    printf("%d", q_digits[k]);
                printf("\n");
            #endif

            continue;
        }

        // increase quotient digit
        q_digits[i] += 1;

        #ifdef DEBUG
            for (int k = 0; k <= i; k++)
                printf("%d", q_digits[k]);
            printf("\n");
        #endif

        if (compare == 0)
            break;

        // subtitute divider digits from dividend
        int acc = 0;

        for (int j = MANTISSA_LEN - 1; j >= 0; j--)
        {
            signed char *a = digits+j+i;
            *a -= divider_digits[j] + acc;
            acc = 0;
            if (*a < 0)
            {
                acc = 1;
                *a += 10;
            }
        }

        for (int j = i - 1; j >= 0 && acc != 0; j--)
        {
            signed char *a = digits+j;
            *a -= acc;
            acc = 0;
            if (*a < 0)
            {
                acc = 1;
                *a += 10;
            }
        }
    }

    // if dividend digits were less then divider digits in the first row
    if (q_digits[0] == 0)
    {
        // shift back digits
        for (int i = 0; i < MANTISSA_LEN + 1; i++)
            q_digits[i] = q_digits[i + 1];
    }
    else
        quotient->exponent += 1;

    #ifdef DEBUG
        printf("exponent = %d\n", quotient->exponent);
    #endif

    if (abs(quotient->exponent) > MAX_EXPONENT)
        return ERR;

    // round last digit
    if (q_digits[MANTISSA_LEN] >= 5)
    {
        int acc = 1;
        for (int j = MANTISSA_LEN - 1; j >= 0 && acc != 0; j--)
        {
            signed char *a = q_digits+j;
            *a += acc;
            acc = 0;
            if (*a >= 10)
            {
                acc = 1;
                *a -= 10;
            }
        }
    }

    #ifdef DEBUG
        printf("\nResult digits:\n");
        for (int k = 0; k < MANTISSA_LEN; k++)
            printf("%d", q_digits[k]);
        printf("\n");
    #endif

    memcpy(quotient->digits, q_digits, sizeof(quotient->digits));

    #undef QUOTIENT_DIGITS_LEN
    #undef DIVIDEND_DIGITS_LEN

    return OK;
}
