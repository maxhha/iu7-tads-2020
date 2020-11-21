#include "../inc/map.h"

map_t *create_map(int w, int h)
{
    map_t *m = malloc(sizeof(map_t));

    if (m == NULL)
    {
        LOG_ERROR("fail to malloc for map_t%s", "");
        return NULL;
    }

    m->walls = calloc(w * h, sizeof(*m->walls));

    if (m->walls == NULL)
    {
        LOG_ERROR("fail to malloc for walls arr%s", "");
        free(m);
        return NULL;
    }

    m->width = w;
    m->height = h;

    m->start_x = -1;
    m->start_y = -1;
    m->finish_x = -1;
    m->finish_y = -1;

    return m;
}

void free_map(map_t *m)
{
    free(m->walls);
    free(m);
}

map_t *read_map_from_file(const char *filename)
{
    FILE *f = filename == NULL ? stdin : fopen(filename, "r");

    if (f == NULL)
    {
        LOG_ERROR("не получилось открыть файл '%s' для чтения", filename);
        return NULL;
    }

    size_t buf_size = 10;
    char *buf = malloc(buf_size * sizeof(char));

    if (buf == NULL)
    {
        LOG_ERROR("fail create buf%s", "");
        goto fail_cleanup_file;
    }

    if (getline(&buf, &buf_size, f) < 2)
    {
        LOG_ERROR("нет первой строки в файле%s", "");
        goto fail_cleanup_buf;
    }

    int w, h;

    if (sscanf(buf, "%d %d", &h, &w) != 2)
    {
        LOG_ERROR("не получилось прочитать размеры карты%s", "");
        goto fail_cleanup_buf;
    }

    LOG_DEBUG("w = %d, h = %d", w, h);

    if (w <= 0 || h <= 0)
    {
        LOG_ERROR("неправильные размеры карты%s", "");
        goto fail_cleanup_buf;
    }

    map_t *map = create_map(w, h);

    if (map == NULL)
    {
        LOG_ERROR("cant create map%s", "");
        goto fail_cleanup_buf;
    }

    bool has_start = false;
    bool has_finish = false;

    for (int y = 0; y < h; y++)
    {
        LOG_DEBUG("read row %d", y);

        int readed_n = getline(&buf, &buf_size, f);

        if (readed_n < 2)
        {
            LOG_ERROR("не получилось прочитать %d строку карты", y + 1);
            goto fail_cleanup_map;
        }

        buf[--readed_n] = '\0';

        LOG_DEBUG("read %d chars", readed_n);

        if (readed_n != w)
        {
            LOG_ERROR("длинна %d строки не совпадает c шириной карты", y + 1);
            goto fail_cleanup_map;
        }

        LOG_DEBUG("readed '%s'", buf);

        for (int x = 0; x < readed_n; x++)
        {
            char c = buf[x];

            switch (c)
            {
            case '#':
                map->walls[x + y * map->width] = true;
                break;
            case ' ':
                map->walls[x + y * map->width] = false;
                break;
            case 'A':
                if (has_start)
                {
                    LOG_ERROR("несколько точек старта на карте%s", "");
                    goto fail_cleanup_map;
                }
                has_start = true;

                map->start_x = x;
                map->start_y = y;
                break;
            case 'B':
                if (has_finish)
                {
                    LOG_ERROR("несколько точек финиша на карте%s", "");
                    goto fail_cleanup_map;
                }
                has_finish = true;

                map->finish_x = x;
                map->finish_y = y;
                break;
            default:
                LOG_ERROR("неизвестный символ '%c' в карте", c);
                goto fail_cleanup_map;
            }
        }
    }

    if (!has_start)
    {
        LOG_ERROR("нет точки старта%s", "");
        goto fail_cleanup_map;
    }

    if (!has_finish)
    {
        LOG_ERROR("нет точки финиша%s", "");
        goto fail_cleanup_map;
    }

    free(buf);
    if (f != stdin)
        fclose(f);

    LOG_DEBUG("readed walls:%s", "");
    LOG_MATRIX(map->walls, w, h, "%d");
    LOG_DEBUG("start_x = %d", map->start_x);
    LOG_DEBUG("start_y = %d", map->start_y);
    LOG_DEBUG("finish_x = %d", map->finish_x);
    LOG_DEBUG("finish_y = %d", map->finish_y);

    return map;

fail_cleanup_map:
    free_map(map);

fail_cleanup_buf:
    free(buf);

fail_cleanup_file:
    if (f != stdin)
        fclose(f);

    return NULL;
}

void print_map_with_path(FILE *f, const map_t *map, const point_t *path, int path_len)
{
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {

            if (x == map->start_x && y == map->start_y)
            {
                fprintf(f, "A");
                continue;
            }

            if (x == map->finish_x && y == map->finish_y)
            {
                fprintf(f, "B");
                continue;
            }

            if (map->walls[x + y * map->width])
            {
                fprintf(f, "#");
                continue;
            }

            int v = NOT_VISITED;

            for (int i = 0; i < path_len && v == NOT_VISITED; i++)
            {
                if (path[i].x == x && path[i].y == y)
                    v = path[i].v;
            }

            if (v == NOT_VISITED)
            {
                fprintf(f, " ");
            }
            else
            {
                fprintf(f, "%d", v % 10);
            }
        }
        fprintf(f, "\n");
    }
}

int print_map_with_path_to_file(char *filename, const map_t *map, const point_t *path, int path_len)
{
    FILE *f = fopen(filename, "w");

    if (f == NULL)
    {
        LOG_ERROR("не получилось открыть файл для записи%s", "");

        return -1;
    }

    fprintf(f, "%d %d\n", map->height, map->width);

    print_map_with_path(f, map, path, path_len);

    fclose(f);

    return 0;
}
