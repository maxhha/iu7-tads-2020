#ifndef __DELIMETERS_H__
#define __DELIMETERS_H__

#include "colors.h"

#define I GRN "|" RESET
#define _I " " I
#define I_ I " "
#define _I_ RESET " " I " "
#define _I_n _I_ "\n"

#define print_repeat(c, n) do { for(int i = 0; i < n; i++) printf(c); } while(0)

#endif // __DELIMETERS_H__
