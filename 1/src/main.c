#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG
#define MANTISSA_LEN 5
#define EXPONENT_LEN 5
#define OK 0
#define ERR 1

struct decimal_s
{
    signed char sign;
    int point;
    int exponent;
    signed char digits[MANTISSA_LEN];
};

typedef struct decimal_s decimal_t;

void set_zero_decimal(decimal_t *val)
{
    val->sign = 1;
    val->point = 0;
    val->exponent = 0;
    memset(val->digits, 0, sizeof(val->digits));
}

int scanf_decimal(decimal_t *val)
{
    char c;
    int digit_i = 0;
    bool have_point = false;
    bool have_exp = false;
    bool is_leading_zero = false;
    int exp_sign = 1;
    int exp_i = 0;

    set_zero_decimal(val);

    do
    {
        if (scanf("%c", &c) != 1)
            return ERR;
    } while (c == ' ' || c == '\n');

    if (c == '+')
        val->sign = 1;
    else if (c == '-')
        val->sign = -1;
    else if (c == '.')
    {
        val->point = 0;
        have_point = true;
        is_leading_zero = false;
    }
    else if ('0' < c && c <= '9')
    {
        val->digits[digit_i++] = c - '0';
        is_leading_zero = false;
    }
    else if (c != '0')
        return ERR;

    while (1)
    {
        if (scanf("%c", &c) != 1)
            return ERR;

        if ('0' <= c && c <= '9')
        {
            if (c == '0' && is_leading_zero)
                continue;
            if (digit_i >= MANTISSA_LEN)
                return ERR;
            val->digits[digit_i++] = c - '0';
            is_leading_zero = false;
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
    {
        val->point = digit_i;
    }

    if (!have_exp)
        return OK;

    if (scanf("%c", &c) != 1)
        return ERR;

    if (c == '+')
        exp_sign = 1;
    else if (c == '-')
        exp_sign = -1;
    else if ('0' <= c && c <= '9')
    {
        val->exponent = c - '0';
        exp_i++;
    }
    else
        return ERR;

    while (1)
    {
        if (scanf("%c", &c) != 1)
            return ERR;

        if ('0' <= c && c <= '9')
        {
            if (exp_i >= EXPONENT_LEN)
                return ERR;
            val->exponent = val->exponent * 10 + c - '0';
            exp_i++;
        }
        else if (c == '\n' || c == ' ')
            break;
        else
            return ERR;
    }

    val->exponent *= exp_sign;

    return OK;
}

int divide_decimal(const decimal_t *dividend, const decimal_t *divider, decimal_t *quotient)
{
    set_zero_decimal(quotient);

    signed char digits[MANTISSA_LEN * 2 + 1] = {0};
    signed char q_digits[MANTISSA_LEN + 2] = {0};
    memcpy(digits, dividend->digits, sizeof(dividend->digits));

    quotient->exponent = 1 + dividend->exponent - divider->exponent + dividend->point - divider->point;
    quotient->sign = dividend->sign * divider->sign;

    int i = 0;
    while (i < MANTISSA_LEN + 2)
    {
        #ifdef DEBUG
            printf("\n");
            for (int k = 0; k < MANTISSA_LEN * 2 + 1; k++)
                printf("%d", digits[k]);
            printf("\n");

            for (int k = 0; k < i; k++)
                printf(" ");
            for (int k = 0; k < MANTISSA_LEN; k++)
                printf("%d", divider->digits[k]);
            printf("\n");
        #endif

        int compare = 0;

        for (int j = 0; j < i && compare == 0; j++)
        {
            compare = digits[j] != 0;
        }

        for (int j = 0; j < MANTISSA_LEN && compare == 0; j++)
        {
            signed char a = digits[j + i];
            signed char b = divider->digits[j];
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
            i++;
            #ifdef DEBUG
                for (int k = 0; k <= i; k++)
                    printf("%d", q_digits[k]);
                printf("\n");
            #endif

            continue;
        }

        q_digits[i] += 1;

        #ifdef DEBUG
            for (int k = 0; k <= i; k++)
                printf("%d", q_digits[k]);
            printf("\n");
        #endif

        if (compare == 0)
            break;

        int acc = 0;

        for (int j = MANTISSA_LEN - 1; j >= 0; j--)
        {
            signed char *a = digits+j+i;
            *a -= divider->digits[j] + acc;
            acc = 0;
            if (*a < 0)
            {
                acc = 1;
                *a += 10;
            }
        }

        for (int j = i - 1; j >= 0; j--)
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

    if (q_digits[0] == 0)
    {
        quotient->exponent -= 1;
        for (int i = 0; i < MANTISSA_LEN + 1; i++)
            q_digits[i] = q_digits[i + 1];
    }

    if (q_digits[MANTISSA_LEN] >= 5)
    {
        q_digits[MANTISSA_LEN - 1]++;
        int acc = 0;
        for (int j = MANTISSA_LEN - 1; j >= 0; j--)
        {
            signed char *a = digits+j;
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

    return OK;
}

void print_decimal(const decimal_t *val)
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

int main()
{
    decimal_t a = {
        .sign = 1,
        .point = 2,
        .exponent = 0,
        .digits = {2,2},
    };
    decimal_t b = {
        .sign = 1,
        .point = 1,
        .exponent = 0,
        .digits = {7},
    };
    decimal_t q;

    divide_decimal(&a, &b, &q);

    print_decimal(&a);
    printf(" / ");
    print_decimal(&b);
    printf(" = ");
    print_decimal(&q);
    printf("\n");

    //
    // printf("ENTER: ");
    // if (scanf_decimal(&a) != OK)
    // {
    //     printf("Invalid input\n");
    //     return EXIT_FAILURE;
    // }

    // printf("a: ");
    // print_decimal(&a);

    return EXIT_SUCCESS;
}
