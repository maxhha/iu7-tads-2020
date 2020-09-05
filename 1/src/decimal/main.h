
#ifndef __DECIMAL_MAIN_H__
#define __DECIMAL_MAIN_H__

#include "../common.h"

struct decimal_s
{
    signed char sign;
    int point;
    int exponent;
    signed char digits[MANTISSA_LEN];
};

typedef struct decimal_s decimal_t;

void set_zero_decimal(decimal_t *val);

#endif // __DECIMAL_MAIN_H__
