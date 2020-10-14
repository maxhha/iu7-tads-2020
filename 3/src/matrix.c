#include "../inc/matrix.h"

matrix_t *create_matrix(size_t width, size_t height)
{
    matrix_t *m = malloc(sizeof(matrix_t));
    if (m == NULL)
        return NULL;

    m->data = calloc(width * height, sizeof(*m->data));

    if (m->data == NULL)
    {
        free(m);
        return NULL;
    }

    m->width = width;
    m->height = height;

    return m;
}

int resize_matrix(matrix_t *m, size_t width, size_t height)
{
    if (m->width == width && m->height == height)
        return OK;

    typeof(m->data) tmp = realloc(m->data, width * height * sizeof(int));

    if (tmp == NULL)
        return EMEM;

    m->data = tmp;

    m->width = width;
    m->height = height;

    return OK;
}

void free_matrix(matrix_t *m)
{
    free(m->data);
    free(m);
}

void fill_random_matrix(matrix_t *m, size_t n)
{
    size_t h = m->height;
    size_t w = m->width;

    for (size_t i = 0; i < n; )
    {
        size_t x = rand() % w;
        size_t y = rand() % h;
        if (m->data[x + y * w] == 0)
        {
            m->data[x + y * w] = (rand() % 9 + 1) * (rand() % 2 * 2 - 1);
            i++;
        }
    }
}

matrix_t *scan_matrix(void)
{
    printf("Введите количество столбцов и строк через пробел:\n");
    size_t w, h;

    if (scanf("%lu %lu", &w, &h) != 2 || h == 0 || w == 0)
    {
        wait_endl();
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }

    wait_endl();

    printf("Заполнить матрицу случайно?\n");
    printf("(y - да, n - нет)\n");

    matrix_t *m;
    int c = getchar();

    wait_endl();

    if (c == 'y')
    {
        size_t n;
        double percent;
        printf("Введите процент заполненности (0-1):\n");

        if (scanf("%lf", &percent) != 1 || percent < 0 || percent > 1)
        {
            wait_endl();
            printf(RED "Неправильный ввод\n" RESET);
            return NULL;
        }

        wait_endl();

        n = (size_t) (w * h * percent);

        if (n == 0)
        {
            printf(RED "Количество элеентов не должно быть 0\n" RESET);
            return NULL;
        }

        m = create_matrix(w, h);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        fill_random_matrix(m, n);

        printf("Заполнено %lu элементов\n", n);
    }
    else if (c == 'n')
    {
        size_t n;
        printf("Введите количество элементов:\n");

        if (scanf("%lu", &n) != 1 || n == 0 || h * w < n)
        {
            wait_endl();
            printf(RED "Неправильный ввод\n" RESET);
            return NULL;
        }

        wait_endl();

        m = create_matrix(w, h);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        printf("Введите элементы матрицы:\n");
        printf("(столбец, строка и значение элемента через пробел)\n");

        for (size_t i = 0, prev_x = 0, prev_y = 0; i < n; i++)
        {
            size_t x, y;
            typeof(*m->data) el;
            if (scanf("%lu %lu %d", &x, &y, &el) != 3
            || x == 0 || y == 0 || x > w || y > h
            || y < prev_y || (y == prev_y && x <= prev_x))
            {
                free_matrix(m);
                wait_endl();
                printf(RED "Неправильный ввод элемента\n" RESET);
                return NULL;
            }
            m->data[x - 1 + (y - 1) * w] = el;
            prev_x = x;
            prev_y = y;
        }

        wait_endl();
    }
    else
    {
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }

    printf(GRN "Матрица создана:\n\n" RESET);
    print_matrix(m, false);

    return m;
}

matrix_t *scan_row_matrix(void)
{
    printf("Введите количество столбцов и строк через пробел:\n");
    size_t w, h = 1;

    if (scanf("%lu", &w) != 1 || w == 0)
    {
        wait_endl();
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }

    wait_endl();

    printf("Заполнить матрицу случайно?\n");
    printf("(y - да, n - нет)\n");

    matrix_t *m;
    int c = getchar();

    wait_endl();

    if (c == 'y')
    {
        size_t n;
        double percent;
        printf("Введите процент заполненности (0-1):\n");

        if (scanf("%lf", &percent) != 1 || percent < 0 || percent > 1)
        {
            wait_endl();
            printf(RED "Неправильный ввод\n" RESET);
            return NULL;
        }

        wait_endl();

        n = (size_t) (w * h * percent);

        if (n == 0)
        {
            printf(RED "Количество элеентов не должно быть 0\n" RESET);
            return NULL;
        }

        m = create_matrix(w, h);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        fill_random_matrix(m, n);

        printf("Заполнено %lu элементов\n", n);
    }
    else if (c == 'n')
    {
        size_t n;
        printf("Введите количество элементов:\n");

        if (scanf("%lu", &n) != 1 || n == 0 || h * w < n)
        {
            wait_endl();
            printf(RED "Неправильный ввод\n" RESET);
            return NULL;
        }

        wait_endl();

        m = create_matrix(w, h);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        printf("Введите элементы матрицы:\n");
        printf("(столбец, строка и значение элемента через пробел)\n");

        for (size_t i = 0, prev_x = 0, prev_y = 0; i < n; i++)
        {
            size_t x, y;
            typeof(*m->data) el;
            if (scanf("%lu %lu %d", &x, &y, &el) != 3
            || x == 0 || y == 0 || x > w || y > h
            || y < prev_y || (y == prev_y && x <= prev_x))
            {
                free_matrix(m);
                wait_endl();
                printf(RED "Неправильный ввод элемента\n" RESET);
                return NULL;
            }
            m->data[x - 1 + (y - 1) * w] = el;
            prev_x = x;
            prev_y = y;
        }

        wait_endl();
    }
    else
    {
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }

    printf(GRN "Матрица создана:\n\n" RESET);
    print_matrix(m, false);

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
        typeof(*m->data) sum = 0;
        for (size_t i = 0; i < n; i++)
            sum += m_row->data[i] * m->data[x + i * w];

        result->data[x] = sum;
    }

    return OK;
}

void print_matrix(const matrix_t *m, bool force_big)
{
    printf(YEL "Тип:" RESET" 2-мерная матрица\n");
    printf(YEL "Размер:" RESET " %lu байт\n", sizeof(*m) + m->width * m->height * sizeof(*m->data));
    printf(YEL "Данные:" RESET "\n");

    if (!force_big && m->width <= PRINT_MATRIX_MAX_WIDTH && m->height <= PRINT_MATRIX_MAX_HEIGHT)
    {
        for (size_t y = 0; y < m->height; y++)
        {
            printf(I_ "%*d", PRINT_VALUE_SIZE, m->data[y * m->width]);

            for (size_t x = 1; x < m->width; x++)
                printf(_I_ "%*d", PRINT_VALUE_SIZE, m->data[x + y * m->width]);

            printf(_I_n);
        }
    }
    else
    {
        #define VALUE_TITLE_STR_LEN 8
        #define VALUE_LEN MAX(VALUE_TITLE_STR_LEN, PRINT_VALUE_SIZE)

        printf(I_ "Столбец" _I_ "Строка " _I_ "%-*s" _I_n, VALUE_LEN + VALUE_TITLE_STR_LEN, "Значение");

        printf(I_ YEL "-------" RESET _I_ YEL "-------" RESET _I_ YEL);
        print_repeat("-", VALUE_LEN);
        printf(RESET _I_n);

        for (size_t y = 0; y < m->height; y++)
            for (size_t x = 0; x < m->width; x++)
                printf(I_ "%7lu" _I_ "%7lu" _I_ "%*d" _I_n, x + 1, y + 1, VALUE_LEN, m->data[x + y * m->width]);

        #undef VALUE_TITLE_STR_LEN
        #undef VALUE_LEN
    }
}
