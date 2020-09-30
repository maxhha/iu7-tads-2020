#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "division.h"

int get_start_zeros_number(const t_digit *arr)
{
    int n = 0;
    while (n < MANTISSA_LEN && arr[n] == 0)
        n++;
    return n;
}

int divide_decimal(const t_decimal *dividend, const t_decimal *divider, t_decimal *quotient)
{
    set_zero_decimal(quotient);

    /*
     + 1 to length for leading zero if dividend is less then divider
     + 1 to length for last digit used for rounding
    */
    #define QUOTIENT_DIGITS_LEN (MANTISSA_LEN + 2)
    /*
     MANTISSA_LEN * 2 for accumulate subtitution for last digit in q_digits
     + 1 to array length for leading zero if dividend is less then divider
    */
    #define DIVIDEND_DIGITS_LEN (MANTISSA_LEN * 2 + 1)
    t_digit q_digits[QUOTIENT_DIGITS_LEN] = {0};
    t_digit digits[DIVIDEND_DIGITS_LEN] = {0};
    t_digit divider_digits[MANTISSA_LEN] = {0};

    memcpy(digits, dividend->digits, sizeof(dividend->digits));

    // skip first 0-s in divider digits
    int start_zeros = get_start_zeros_number(divider->digits);

    if (start_zeros == MANTISSA_LEN)
        return EZERODIVISION;

    memcpy(
        divider_digits,
        divider->digits + start_zeros,
        sizeof(t_digit) * (MANTISSA_LEN - start_zeros)
    );

    quotient->exponent = dividend->exponent - divider->exponent;
    quotient->exponent += dividend->point - divider->point;
    quotient->exponent += start_zeros + 1;

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
            t_digit a = digits[j + i];
            t_digit b = divider_digits[j];
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
            t_digit *a = digits+j+i;
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
            t_digit *a = digits+j;
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
    int shift = get_start_zeros_number(q_digits);

    quotient->exponent -= shift;

    // shift back digits
    for (int i = 0; i + shift < QUOTIENT_DIGITS_LEN; i++)
        q_digits[i] = q_digits[i + shift];

    for (int i = QUOTIENT_DIGITS_LEN - shift; i <= MANTISSA_LEN; i++)
        q_digits[i] = 0;


    // round last digit
    if (q_digits[MANTISSA_LEN] >= 5)
    {
        int acc = 1;
        for (int j = MANTISSA_LEN - 1; j >= 0 && acc != 0; j--)
        {
            t_digit *a = q_digits+j;
            *a += acc;
            acc = 0;
            if (*a >= 10)
            {
                acc = 1;
                *a -= 10;
            }
        }
    }

    if (get_start_zeros_number(q_digits) == MANTISSA_LEN)
        quotient->exponent = 0;

    #ifdef DEBUG
        printf("exponent = %d\n", quotient->exponent);
    #endif

    if (abs(quotient->exponent) > MAX_EXPONENT)
        return EEXPOVERFLOW;

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
