#ifndef __READ_HELPERS_H__
#define __READ_HELPERS_H__

#include <stdio.h>

#define wait_new_line() fwait_new_line(stdin)

char *fgetline(char *dest, int maxlen, FILE *f);
void fwait_new_line(FILE *f);

#endif // __READ_HELPERS_H__
