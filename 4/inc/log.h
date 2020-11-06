#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define LOG_LEVEL NO_LOG
#include "macrologger.h"

#if LOG_LEVEL == NO_LOG
#undef LOG_ERROR
#define LOG_ERROR(message, ...) \
    fprintf(stderr, "Ошибка: " message NEWLINE, ## __VA_ARGS__ )
#endif

#if LOG_LEVEL >= DEBUG_LEVEL
#define LOG_MATRIX(ptr, w, h, fmt) do { \
    for (int y = 0; y < (h); y++) \
    { \
        PRINTFUNCTION(LOG_FMT, LOG_ARGS(DEBUG_TAG)); \
        for (int x = 0; x < (w); x++) \
        { \
            PRINTFUNCTION(fmt "%c", (ptr)[x + y * (w)], x == (w) - 1 ? '\n' : ' '); \
        } \
        \
    } \
} while (0)
#else
#define LOG_MATRIX(message, ...)
#endif

#endif // __LOG_H__
