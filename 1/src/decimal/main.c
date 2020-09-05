#include <string.h>
#include "main.h"

void set_zero_decimal(decimal_t *val)
{
    val->sign = 1;
    val->point = 0;
    val->exponent = 0;
    memset(val->digits, 0, sizeof(val->digits));
}
