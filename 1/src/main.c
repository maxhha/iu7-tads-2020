#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG
#define MANTISSA_LEN 5
#define MAX_EXPONENT 99999
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

    if (safe_add_int(dividend->exponent, -divider->exponent, &quotient->exponent) != OK)
        return ERR;

    quotient->exponent += dividend->point - divider->point;

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
                printf("%d", divider->digits[k]);
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
            *a -= divider->digits[j] + acc;
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

void print_hello()
{
    printf("──┼─\n");
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
