#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>

#define wait_endl() do { int c; while((c = getchar()) != '\n' && c != EOF){} } while(0)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define typeof(x) __typeof__(x)

#endif // __HELPERS_H__
