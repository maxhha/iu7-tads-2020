#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>

/*
    Делает fprintf если f не NULL
*/
#define wait_new_line() fwait_new_line(stdin)

char *fgetline(char *dest, int maxlen, FILE *f);
void fwait_new_line(FILE *f);

#endif // __HELPERS_H__
