#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>

/*
    Делает fprintf если f не NULL
*/
#define xfprintf(f, ...) if (f) fprintf(f, __VA_ARGS__)

char *fgetline(char *dest, int maxlen, FILE *f);
void wait_new_line(void);

#endif // __HELPERS_H__
