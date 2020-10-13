#include "../inc/matrix.h"

matrix_t *matrix_create(size_t width, size_t height)
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

int matrix_resize(matrix_t *m, size_t width, size_t height)
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

void matrix_free(matrix_t *m)
{
    free(m->data);
    free(m);
}

matrix_t *matrix_scan(void)
{
    printf("Введите размер матрицы (количество столбов и строк через пробел):\n");
    size_t w, h;

    if (scanf("%lu %lu", &w, &h) != 2 && h > 0 && w > 0)
    {
        wait_endl();
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }

    wait_endl();

    matrix_t *m = matrix_create(w, h);

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
                matrix_free(m);
                wait_endl();
                printf(RED "Неправильный ввод элемента\n" RESET);
                return NULL;
            }

    wait_endl();

    return m;
}

int matrix_multiply(const matrix_t * restrict a, const matrix_t * restrict b, matrix_t * restrict result)
{
    if (a->width != b->height)
        return EMATRIXMUL;

    size_t w = b->width;
    size_t h = a->height;
    size_t n = a->width;

    if (matrix_resize(result, w, h) != OK)
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

void matrix_print(matrix_t *m)
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
