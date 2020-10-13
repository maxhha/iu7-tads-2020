#include "../inc/matrix.h"

matrix_t *create_matrix(size_t width, size_t height)
{
    matrix_t *m = malloc(sizeof(matrix_t));
    if (m == NULL)
        goto broken_m;

    m->data = (int *) calloc(width * height, sizeof(int));

    if (m->data == NULL)
        goto broken_data;

    m->width = width;
    m->height = height;

    return m;

    broken_data:
    free(m);

    broken_m:
    return NULL;
}

int resize_matrix(matrix_t *m, size_t width, size_t height)
{
    if (m->width == width && m->height == height)
        return OK;

    m->data = realloc(m->data, width * height * sizeof(int));

    if (m->data == NULL)
        return EMEM;

    m->width = width;
    m->height = height;

    return OK;
}

void free_matrix(matrix_t *m)
{
    free(m->data);
    free(m);
}

matrix_t *scan_matrix(void)
{
    printf("Введите количество столбцов и строк через пробел:\n");
    size_t w, h;

    if (scanf("%lu %lu", &w, &h) != 2 && h > 0 && w > 0)
    {
        wait_endl();
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }

    wait_endl();

    matrix_t *m = create_matrix(w, h);

    if (m == NULL)
    {
        printf(RED "Не хватило памяти для создания матрицы\n" RESET);
        return NULL;
    }

    printf("Введите элементы матрицы:\n");

    for (size_t y = 0; y < h; y++)
        for (size_t x = 0; x < w; x++)
            if (scanf("%d", &m->data[x + y * w]) != 1)
            {
                free_matrix(m);
                wait_endl();
                printf(RED "Неправильный ввод элемента\n" RESET);
                return NULL;
            }

    wait_endl();

    return m;
}

matrix_t *scan_row_matrix(void)
{
    printf("Введите количество столбцов:\n");
    size_t w;

    if (scanf("%lu", &w) != 1 && w > 0)
    {
        wait_endl();
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }

    wait_endl();

    matrix_t *m = create_matrix(w, 1);

    if (m == NULL)
    {
        printf(RED "Не хватило памяти для создания матрицы\n" RESET);
        return NULL;
    }

    printf("Введите элементы матрицы:\n");

    for (size_t x = 0; x < w; x++)
        if (scanf("%d", &m->data[x]) != 1)
        {
            free_matrix(m);
            wait_endl();
            printf(RED "Неправильный ввод элемента\n" RESET);
            return NULL;
        }

    wait_endl();

    return m;
}

/*
// Valid matrix multiplication
int multiply_matrix(const matrix_t * restrict a, const matrix_t * restrict b, matrix_t * restrict result)
{
    if (a->width != b->height)
        return EMATRIXMUL;

    size_t w = b->width;
    size_t h = a->height;
    size_t n = a->width;

    if (resize_matrix(result, w, h) != OK)
        return EMEM;

    for (size_t y = 0; y < h; y++)
        for (size_t x = 0; x < w; x++)
        {
            int sum = 0;
            for (size_t i = 0; i < n; i++)
                sum += a->data[i + y * n] * b->data[x + i * w];

            result->data[x + y * w] = sum;
        }

    return OK;
}
*/

int multiply_row_matrix_by_matrix(const matrix_t * restrict m_row, const matrix_t * restrict m, matrix_t * restrict result)
{
    if (m_row->width != m->height || m_row->height != 1)
        return EMATRIXMUL;

    size_t w = m->width;
    size_t n = m_row->width;

    if (resize_matrix(result, w, 1) != OK)
        return EMEM;

    for (size_t x = 0; x < w; x++)
    {
        int sum = 0;
        for (size_t i = 0; i < n; i++)
            sum += m_row->data[i] * m->data[x + i * w];

        result->data[x] = sum;
    }

    return OK;
}

void print_matrix(const matrix_t *m)
{
    printf(YEL "Тип:" RESET" 2-мерная матрица\n");
    printf(YEL "Размер:" RESET " %lux%lu\n", m->width, m->height);
    printf(YEL "Данные:" RESET "\n");
    printf(I_ "Строка " _I_ "Столбец" _I_ "Значение        " _I_n);
    printf(I_ YEL "-------" RESET _I_ YEL "-------" RESET _I_ YEL "----------------" RESET _I_n);

    for (size_t y = 0; y < m->height; y++)
        for (size_t x = 0; x < m->width; x++)
            printf(I_ "%7lu" _I_ "%7lu" _I_ "%16d" _I_n, x + 1, y + 1, m->data[x + y * m->width]);
}
