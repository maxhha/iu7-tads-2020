#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>

#define wait_endl() do { int c; while((c = getchar()) != '\n' && c != EOF){} } while(0)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define typeof(x) __typeof__(x)

/*
    Работает как fgets, только не добавляет конец строки
*/
char *fgetline(char *dest, int maxlen, FILE *f);

#endif // __HELPERS_H__
