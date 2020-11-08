#ifndef __MAP_H__
#define __MAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "log.h"

typedef struct {
    bool *walls;
    int width;
    int height;
    int start_x;
    int start_y;
    int finish_x;
    int finish_y;
} map_t;

#include "path.h"

map_t *create_map(int width, int height);
void free_map(map_t *m);

map_t *read_map_from_file(const char *filename);

void print_map_with_path(FILE *f, const map_t *map, const point_t *path, int path_len);
int print_map_with_path_to_file(char *filename, const map_t *map, const point_t *path, int path_len);

#endif // __MAP_H__
