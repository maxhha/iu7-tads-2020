#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MANTISSA_LEN 30
#define EXPONENT_LEN 5
#define OK 0
#define ERR 1

struct Decimal {
    int sign;
    int point;
    int exponent;
    int digits[MANTISSA_LEN];
};

void clear_decimal(struct Decimal *val) {
    val->sign = 1;
    val->point = 0;
    val->exponent = 0;
    memset(val->digits, 0, sizeof(val->digits));
}

int scanf_decimal(struct Decimal *val) {

    char c;
    int digit_i = 0;
    bool have_point = false;
    bool have_exp = false;
    int exp_sign = 1;
    int exp_i = 0;

    clear_decimal(val);

    do {
        if (scanf("%c", &c) != 1)
            return ERR;
    } while (c == ' ' || c == '\n');

    if (c == '+')
        val->sign = 1;
    else if ( c == '-')
        val->sign = -1;
    else if ( c == '.') {
        val->point = 0;
        have_point = true;
    }
    else if ('0' <= c && c <= '9')
        val->digits[digit_i++] = c - '0';
    else
        return ERR;

    while (1) {
        if (scanf("%c", &c) != 1)
            return ERR;

        if ('0' <= c && c <= '9') {
            if (digit_i >= MANTISSA_LEN)
                return ERR;
            val->digits[digit_i++] = c - '0';
        }
        else if (c == '.') {
            if (have_point)
                return ERR;
            have_point = true;
            val->point = digit_i;
        }
        else if (c == 'e' || c == 'E') {
            have_exp = true;
            break;
        }
        else if (c == '\n' || c == ' ')
            break;
        else
            return ERR;
    }

    if (!have_point) {
        val->point = digit_i;
    }

    if (!have_exp)
        return OK;

    if (scanf("%c", &c) != 1)
        return ERR;

    if (c == '+')
        exp_sign = 1;
    else if ( c == '-')
        exp_sign = -1;
    else if ('0' <= c && c <= '9') {
        val->exponent = c - '0';
        exp_i++;
    }
    else
        return ERR;

    while (1) {
        if (scanf("%c", &c) != 1)
            return ERR;

        if ('0' <= c && c <= '9') {
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

void print_decimal(struct Decimal *val) {
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

    printf("E%+d\n", val->exponent);
}

int main() {
    struct Decimal a = {
        .sign = 1,
        .point = 0,
        .exponent = 2,
        .digits = {1},
    };

    printf("ENTER: ");
    if (scanf_decimal(&a) != OK) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }

    printf("a: ");
    print_decimal(&a);

    return EXIT_SUCCESS;
}
