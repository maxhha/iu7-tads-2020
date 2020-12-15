#include "graph.h"

#define G graph_matrix
#include "graph.tmpl.c"
#undef G

uint64_t ticks(void)
{
    uint64_t x;
    __asm__ volatile ("rdtsc\n\tshl $32, %%rdx\n\tor %%rdx, %%rax" : "=a" (x) : : "rdx");
    return x;
}

int stack_push_vertex(stack_t *s, int vertex, int distance)
{
    int *data = malloc(sizeof(int) * 2);

    if (data == NULL)
        return -1;

    data[0] = vertex;
    data[1] = distance;

    return stack_push(s, data);
}

int stack_pop_vertex(stack_t *s, int *vertex, int *distance)
{
    int *data = stack_pop(s);

    if (data == NULL)
        return -1;

    *vertex = data[0];
    *distance = data[1];

    free(data);
    return 0;
}

void print_result_verteces(int *distances, int n, int t)
{
    int any = 0;
    for (int i = 0; i < n && !any; i++)
        any = distances[i] > t;

    if (!any)
    {
        printf("В этом графе нет вершин, расстояние от которых до столицы больше %d\n\n", t);
        return;
    }

    printf("Вершины, расстояние от которых до столицы больше %d:\n", t);

    for (int i = 0; i < n; i++)
    {
        if (distances[i] > t)
            printf("%d ", i + 1);
    }

    printf("\n\n");
}
