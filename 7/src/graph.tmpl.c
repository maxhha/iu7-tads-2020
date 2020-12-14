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

        if (v1 < 0 || v1 >= n || v2 < 0 || v2 >= n)
        {
            LOG_ERROR("неправильная дуга %d -- %d", v1 + 1, v2 + 1);
        }

        if (TMPL(G, get)(g, v1, v2) != -1)
        {
            LOG_ERROR("повторная дуга %d -- %d", v1 + 1, v2 + 1);
            TMPL(free, G)(g);
            return NULL;
        }

        TMPL(G, set)(g, v1, v2, w);
        TMPL(G, set)(g, v2, v1, w);
    }

    return g;
}

int TMPL(save, G)(G_T *g, char *filename)
{
    FILE *f = fopen(filename, "w");

    if (f == NULL)
    {
        LOG_ERROR("не получилось открыть файл на запись%s", "");
        return EXIT_FAILURE;
    }

    fprintf(f, "graph graphname {\n");

    for (int i = 0; i < g->verteces_n; i++)
    {
        fprintf(f, "\t%d;\n", i + 1);

        for (
            int j = TMPL(G, get_next)(g, i, -1);
            j >= 0 && j < i;
            j = TMPL(G, get_next)(g, i, j))
        {
            fprintf(f, "\t%d -- %d [label=%d];\n", i + 1, j + 1, TMPL(G, get)(g, i, j));
        }
    }

    fprintf(f, "}\n");

    return fclose(f);
}

int TMPL(process_with, G)(char *filename, int capital, int param_t)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        LOG_ERROR("не получилось прочитать граф из файла%s", "");
        return EXIT_FAILURE;
    }

    G_T *g = TMPL(read, G)(f);

    fclose(f);

    if (g == NULL)
    {
        LOG_ERROR("не получилось прочитать граф из файла%s", "");
        return EXIT_FAILURE;
    }

    int rc = TMPL(save, G)(g, OUTPUT_FILE);
    TMPL(free, G)(g);

    if (rc)
    {
        LOG_ERROR("не получилось сохранить результат в файл%s", "");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
