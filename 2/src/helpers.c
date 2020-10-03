#include "../inc/helpers.h"

/*
    Работает как fgets, только не добавляет конец строки
*/
char *fgetline(char *dest, int maxlen, FILE *f)
{
    int c;
    char *i = dest;
    char *end = dest + maxlen - 1;

    while((c = fgetc(f)) != EOF && c != '\n' && i < end)
        *i++ = c;

    if (c != EOF && c != '\n')
    {
        wait_new_line();
        return NULL;
    }

    *i = '\0';

    return c == EOF ? NULL : dest;
}


void wait_new_line(void)
{
    int c;
    do {
        c = getchar();
    } while (c != EOF && c != '\n');
}
