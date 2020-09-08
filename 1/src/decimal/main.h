
#ifndef __DECIMAL_MAIN_H__
#define __DECIMAL_MAIN_H__

#include "../common.h"

typedef signed char t_digit;

typedef struct
{
    signed char sign;
    int point;
    int exponent;
    t_digit digits[MANTISSA_LEN];
} t_decimal;

void set_zero_decimal(t_decimal *val);

#endif // __DECIMAL_MAIN_H__
