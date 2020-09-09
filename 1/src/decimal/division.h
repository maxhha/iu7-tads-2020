
#ifndef __DECIMAL_DIVISION_H__
#define __DECIMAL_DIVISION_H__

#include "main.h"

/*
  Делит числа.
  Резултат записывается в quotient.
  Возвращает OK или ERR если произошла ошибка
*/
int divide_decimal(const t_decimal *dividend, const t_decimal *divider, t_decimal *quotient);

#endif // __DECIMAL_DIVISION_H__
