#include "../inc/path.h"

int push_point_to_stack(stack_list_t *stack, point_t p)
{
    LOG_DEBUG("push (%d, %d, %d)", p.x, p.y, p.v);

    point_t *point = malloc(sizeof(point_t));

    if (point == NULL)
    {
        LOG_ERROR("не получилось выделить память для позиции (%d, %d, %d)", p.x, p.y, p.v);

        return -1;
    }

    *point = p;

    if (stack_list_push(stack, point))
    {
        LOG_ERROR("не получилось добавить позицию (%d, %d, %d) в стэк", p.x, p.y, p.v);

        free(point);
        return -1;
    }

    return 0;
}

int push_neighboors(const map_t *map, stack_list_t *stack, point_t p)
{
    int w = map->width;
    int h = map->height;
    point_t t = p;

    t.x = p.x - 1;

    if (t.x >= 0 && !map->walls[t.x + t.y * w])
        if (push_point_to_stack(stack, t))
            return -1;

    t.x = p.x + 1;

    if (t.x < w && !map->walls[t.x + t.y * w])
        if (push_point_to_stack(stack, t))
            return -1;

    t.x = p.x;
    t.y = p.y - 1;

    if (t.y >= 0 && !map->walls[t.x + t.y * w])
        if (push_point_to_stack(stack, t))
            return -1;

    t.y = p.y + 1;

    if (t.y < h && !map->walls[t.x + t.y * w])
        if (push_point_to_stack(stack, t))
            return -1;

    return 0;
}

#define CLEAR_POINT_STACK(stack) do { \
    point_t *p; \
    while ((p = stack_list_pop(stack)) != NULL) \
        free(p); \
} while(0)

int *create_visited_map(const map_t *map)
{
    int w = map->width;
    int h = map->height;
    int *visited = malloc(w * h * sizeof(int));

    if (visited == NULL)
    {
        LOG_ERROR("не получилось выделить память для карты посещенных ячеек%s", "");
        return NULL;
    }


    for (int i = 0; i < map->width * map->height; i++)
        visited[i] = NOT_VISITED;

    stack_list_t *stack = create_stack_list();

    if (stack == NULL)
    {
        LOG_ERROR("не получилось создать стэк%s", "");
        free(visited);
        return NULL;
    }

    point_t start_point = {
        .x = map->start_x,
        .y = map->start_y,
        .v = 0,
    };

    if (push_point_to_stack(stack, start_point))
    {
        LOG_ERROR("не получилось добавить начальную позицию%s", "");

        free_stack_list(stack);
        free(visited);
        return NULL;
    }

    point_t *point_ptr;

    while ((point_ptr = stack_list_pop(stack)) != NULL)
    {
        point_t p = *point_ptr;

        free(point_ptr);

        LOG_DEBUG("pop (%d, %d, %d)", p.x, p.y, p.v);

        if (visited[p.x + p.y * w] != NOT_VISITED && visited[p.x + p.y * w] <= p.v)
            continue;

        visited[p.x + p.y * w] = p.v++;
        LOG_MATRIX(visited, w, h, "%2d");

        if (push_neighboors(map, stack, p))
        {
            LOG_ERROR("не получилось добавить всех пути из позиции%s", "");

            CLEAR_POINT_STACK(stack);
            free_stack_list(stack);
            free(visited);
            return NULL;
        }
    }

    LOG_DEBUG("visited:%s", "");
    LOG_MATRIX(visited, w, h, "%2d");

    free_stack_list(stack);

    return visited;
}

int build_path(const map_t *map, const int *visited, point_t **result_path)
{
    int w = map->width;
    int h = map->height;

    point_t p = {
        .x = map->finish_x,
        .y = map->finish_y,
        .v = visited[map->finish_x + map->finish_y * w],
    };

    if (p.v == NOT_VISITED)
    {
        LOG_ERROR("из А невозмножно попасть в B%s", "");

        return -1;
    }

    stack_list_t *stack = create_stack_list();

    if (stack == NULL)
    {
        LOG_ERROR("не получилось создать стэк%s", "");
        return -1;
    }

    int path_len = 0;

    LOG_DEBUG("start build path%s", "");

    while (p.v != 0)
    {
        LOG_DEBUG("watch %d point", path_len);

        if (push_point_to_stack(stack, p))
        {
            LOG_ERROR("не получилось добавить позицию пути%s", "");

            point_t *p;
            while ((p = stack_list_pop(stack)) != NULL)
                free(p);

            free_stack_list(stack);
            return -1;
        }

        path_len++;

        point_t min_p = p;

        int x = p.x - 1;
        int y = p.y;

        if (x >= 0 && visited[x + y * w] != NOT_VISITED && min_p.v > visited[x + y * w])
        {
            min_p.v = visited[x + y * w];
            min_p.x = x;
            min_p.y = y;
        }

        x = p.x + 1;

        if (x < w && visited[x + y * w] != NOT_VISITED && min_p.v > visited[x + y * w])
        {
            min_p.v = visited[x + y * w];
            min_p.x = x;
            min_p.y = y;
        }

        x = p.x;
        y = p.y - 1;

        if (y >= 0 && visited[x + y * w] != NOT_VISITED && min_p.v > visited[x + y * w])
        {
            min_p.v = visited[x + y * w];
            min_p.x = x;
            min_p.y = y;
        }

        y = p.y + 1;

        if (y < h && visited[x + y * w] != NOT_VISITED && min_p.v > visited[x + y * w])
        {
            min_p.v = visited[x + y * w];
            min_p.x = x;
            min_p.y = y;
        }

        LOG_DEBUG("min_p.v = %d", min_p.v);

        if (min_p.v == p.v)
        {
            LOG_ERROR("не получилось найти более близкую к старту позицию из (%d, %d, %d)", p.x, p.y, p.v);

            CLEAR_POINT_STACK(stack);
            free_stack_list(stack);
            return -1;
        }

        p = min_p;
    }

    point_t *path = malloc(path_len * sizeof(point_t));

    if (path == NULL)
    {
        LOG_ERROR("не получилось выделить память для массива точек пути%s", "");

        CLEAR_POINT_STACK(stack);
        free_stack_list(stack);
        return -1;
    }

    point_t *p_ptr;
    int i = 0;

    while ((p_ptr = stack_list_pop(stack)) != NULL)
    {
        path[i] = *p_ptr;
        free(p_ptr);
        i++;
    }

    *result_path = path;

    return path_len;
}

int get_path(const map_t *map, point_t **result)
{
    int *visited_map = create_visited_map(map);

    if (visited_map == NULL)
        return -1;

    int rc = build_path(map, visited_map, result);

    free(visited_map);
    return rc;
}
