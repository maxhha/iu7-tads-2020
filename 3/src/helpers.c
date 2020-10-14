#include "../inc/helpers.h"

char *fgetline(char *dest, int maxlen, FILE *f)
{
    int c;
    char *i = dest;
    char *end = dest + maxlen - 1;

    while ((c = fgetc(f)) != EOF && (c != '\n' || i == dest) && i < end)
        if (c != '\n')
            *i++ = c;

    if (c != EOF && c != '\n')
    {
        wait_endl();
        return NULL;
    }

    *i = '\0';

    return c == EOF ? NULL : dest;
}
