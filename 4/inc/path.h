#ifndef __PATH_H__
#define __PATH_H__

#include "log.h"
#include "stack_list.h"
#include "stack_array.h"

typedef struct {
    int x;
    int y;
    int v;
} point_t;

#include "map.h"

#define NOT_VISITED (-1)

int get_path_using_stack_list(const map_t *map, stack_list_t *stack, point_t **result_path);
int get_path_using_stack_array(const map_t *map, stack_array_t *stack, point_t **result_path);

#endif // __PATH_H__
