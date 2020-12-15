#include "graph.h"
#include "template.h"

#ifndef G
#define G graph_matrix
#endif

#define G_T TMPL(G, t)

G_T *TMPL(read, G)(FILE *f)
{
    int n;
    if (fscanf(f, "%d", &n) != 1 || n <= 0)
    {
        LOG_ERROR("не правильное количество вершин графа%s", "");
        return NULL;
    }

    G_T *g = TMPL(create, G)(n);

    while (1)
    {
        int v1, v2, w, rc = fscanf(f, "%d %d %d", &v1, &v2, &w);

        if (rc > 0 && rc != 3)
        {
            LOG_ERROR("не правильное количество чисел в файле%s", "");
            TMPL(free, G)(g);
            return NULL;
        }

        if (rc <= 0)
        {
            break;
        }

        v1--; v2--;

        if (v1 < 0 || v1 >= n || v2 < 0 || v2 >= n || w < 0)
        {
            LOG_ERROR("неправильная дуга %d %d %d", v1 + 1, v2 + 1, w);
        }

        if (TMPL(G, get)(g, v1, v2) != -1)
        {
            LOG_ERROR("повторная дуга %d %d", v1 + 1, v2 + 1);
            TMPL(free, G)(g);
            return NULL;
        }

        TMPL(G, set)(g, v1, v2, w);
        TMPL(G, set)(g, v2, v1, w);
    }

    return g;
}

int TMPL(save, G)(char *filename, G_T *g, int *distances, int capital, int color_from_dist)
{
    FILE *f = fopen(filename, "w");

    if (f == NULL)
    {
        LOG_ERROR("не получилось открыть файл на запись%s", "");
        return EXIT_FAILURE;
    }

    fprintf(f, "graph {\n");

    for (int i = 0; i < g->verteces_n; i++)
    {
        fprintf(f, "\t%d [label = \"%d (%d)\",", i + 1, i + 1, distances[i]);

        if (i == capital)
            fprintf(f, CAPITAL_VERTEX_DOT_ATTRS);

        if (distances[i] == -1)
            fprintf(f, UNVISITED_VERTEX_DOT_ATTRS);
        else if (distances[i] > color_from_dist)
            fprintf(f, SELECTED_VERTEX_DOT_ATTRS);

        fprintf(f, "]\n");
    }

    for (int i = 0; i < g->verteces_n; i++)
    {
        for (
            int j = TMPL(G, get_next)(g, i, -1);
            j >= 0 && j < i;
            j = TMPL(G, get_next)(g, i, j))
        {
            fprintf(f, "\t%d -- %d [label = %d];\n", i + 1, j + 1, TMPL(G, get)(g, i, j));
        }
    }

    fprintf(f, "}\n");

    return fclose(f);
}

int *TMPL(create_distance_map_for, G)(G_T *g, int capital)
{
    int *map = malloc(sizeof(int) * g->verteces_n);

    if (map == NULL)
    {
        LOG_ERROR("не получилось создать массив расстояний%s", "");
        return NULL;
    }

    for (int i = 0; i < g->verteces_n; i++)
        map[i] = -1;

    stack_t *stack = create_stack(g->verteces_n * g->verteces_n);

    if (stack == NULL)
    {
        LOG_ERROR("не получилось создать стек%s", "");
        free(map);
        return NULL;
    }

    if (stack_push_vertex(stack, capital, 0))
    {
        LOG_ERROR("не получилось добавить столицу%s", "");

        free_stack(stack);
        free(map);
        return NULL;
    }

    int v, d;

    while (!stack_pop_vertex(stack, &v, &d))
    {
        if (map[v] != -1 && d > map[v])
            continue;

        map[v] = d;

        for (
            int i = TMPL(G, get_next)(g, v, -1);
            i != -1;
            i = TMPL(G, get_next)(g, v, i))
        {
            if (stack_push_vertex(stack, i, d + TMPL(G, get)(g, v, i)))
            {
                LOG_ERROR("не получилось добавить вершину в стек%s", "");

                do
                {
                    int i, j;

                    if (stack_pop_vertex(stack, &i, &j))
                        break;

                } while (1);

                free_stack(stack);
                free(map);
                return NULL;
            }
        }
    }

    free_stack(stack);
    return map;
}

int TMPL(process_with, G)(char *filename, int capital, int param_t)
{
    uint64_t start, end;
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        LOG_ERROR("не получилось прочитать граф из файла%s", "");
        return EXIT_FAILURE;
    }

    start = ticks();
    G_T *g = TMPL(read, G)(f);
    end = ticks();

    printf("Время чтения графа из файла: %lu тактов\n", end - start);

    fclose(f);

    if (g == NULL)
    {
        LOG_ERROR("не получилось прочитать граф из файла%s", "");
        return EXIT_FAILURE;
    }

    if (capital >= g->verteces_n)
    {
        LOG_ERROR("номер столицы больше числа вершин в графе%s", "");

        TMPL(free, G)(g);
        return EXIT_FAILURE;
    }

    start = ticks();
    int *distances = TMPL(create_distance_map_for, G)(g, capital);
    end = ticks();

    if (distances == NULL)
    {
        LOG_ERROR("не получилось рассчитать расстояния до столицы%s", "");

        TMPL(free, G)(g);
        return EXIT_FAILURE;
    }

    printf("Время вычисления расстояний: %lu тактов\n", end - start);

    print_result_verteces(distances, g->verteces_n, param_t);

    start = ticks();
    int rc = TMPL(save, G)(OUTPUT_FILE, g, distances, capital, param_t);
    end = ticks();

    free(distances);
    TMPL(free, G)(g);

    if (rc)
    {
        LOG_ERROR("не получилось сохранить результат в файл%s", "");
        return EXIT_FAILURE;
    }

    printf("Время сохранения результата в файл: %lu тактов\n", end - start);

    return EXIT_SUCCESS;
}
