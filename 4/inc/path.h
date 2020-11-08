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

int get_path_using_stack_list(const map_t *map, stack_list_t *stack, point_t **result_path);

#endif // __PATH_H__
