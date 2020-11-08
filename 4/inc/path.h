#ifndef __PATH_H__
#define __PATH_H__

#include "log.h"
#include "stack_list.h"

typedef struct {
    int x;
    int y;
    int v;
} point_t;

#include "map.h"

#define NOT_VISITED (-1)

int get_path(const map_t *map, point_t **result_path);

#endif // __PATH_H__
