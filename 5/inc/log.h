#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define LOG_LEVEL NO_LOG
#include "macrologger.h"
#include "colors.h"

#if LOG_LEVEL == NO_LOG
#undef LOG_ERROR
#define LOG_ERROR(message, ...) \
    fprintf(stderr, RED "Ошибка: " message RESET NEWLINE, ## __VA_ARGS__ )
#endif

#if LOG_LEVEL < INFO_LEVEL
#undef LOG_INFO
#define LOG_INFO(message, ...) \
fprintf(stderr, "Инфо: " message NEWLINE, ## __VA_ARGS__ )
#endif

#endif // __LOG_H__
