#ifndef __DECIMAL_IO_H__
#define __DECIMAL_IO_H__

#include "main.h"

/*
  Считывает из консоли целое
*/
int scanf_decimal_int(t_decimal *val);
/*
  Считывает из консоли вещественное число
*/
int scanf_decimal_float(t_decimal *val);
/*
  Записывает в консоль val как целое число
*/
void print_decimal_int(const t_decimal *val);
/*
  Записывает в консоль val как вещественное число
*/
void print_decimal_float(const t_decimal *val);

#endif // __DECIMAL_IO_H__
