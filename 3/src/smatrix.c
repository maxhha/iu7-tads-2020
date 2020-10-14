#include "../inc/smatrix.h"

smatrix_t *create_smatrix(size_t width, size_t height, size_t n_elems)
{
    smatrix_t *m = malloc(sizeof(smatrix_t));
    if (m == NULL)
        return NULL;

    m->values = calloc(n_elems, sizeof(*m->values));
    if (m->values == NULL)
    {
        free(m);
        return NULL;
    }

    m->columns = malloc(n_elems * sizeof(*m->columns));
    if (m->columns == NULL)
    {
        free(m->values);
        free(m);
        return NULL;
    }

    m->row_begins = malloc(height * sizeof(*m->row_begins));
    if (m->values == NULL)
    {
        free(m->columns);
        free(m->values);
        free(m);
        return NULL;
    }

    m->width = width;
    m->height = height;
    m->n_elems = n_elems;

    return m;
}

int resize_smatrix(smatrix_t *m, size_t width, size_t height, size_t n_elems)
{
    if (m->n_elems != n_elems)
    {
        typeof(m->values) tmp1 = realloc(m->values, n_elems * sizeof(*m->values));

        if (tmp1 == NULL)
            return EMEM;

        m->values = tmp1;

        typeof(m->columns) tmp2 = realloc(m->columns, n_elems * sizeof(*m->columns));

        if (tmp2 == NULL)
            return EMEM;

        m->columns = tmp2;
    }

    if (m->height != height)
    {
        typeof(m->row_begins) tmp = realloc(m->row_begins, height * sizeof(*m->row_begins));

        if (tmp == NULL)
            return EMEM;

        m->row_begins = tmp;
    }

    m->width = width;

    return OK;
}

void free_smatrix(smatrix_t *m)
{
    free(m->row_begins);
    free(m->columns);
    free(m->values);
    free(m);
}

int cmp_size_t(const void *a, const void *b)
{
    size_t sa = *(const size_t *) a;
    size_t sb = *(const size_t *) b;

    return sa < sb ? -1 : sa == sb ? 0 : 1;
}

void fill_random_smatrix(smatrix_t *m)
{
    size_t h = m->height;
    size_t w = m->width;
    size_t n = m->n_elems;

    for (size_t i = 0; i < h; i++)
    {
        m->row_begins[i] = 0;
    }

    for (size_t i = 0; i < n; )
    {
        size_t c = rand() % h;
        if (m->row_begins[c] < w)
        {
            m->row_begins[c] += 1;
            m->values[i] = (rand() % 9 + 1) * (rand() % 2 * 2 - 1);
            i++;
        }
    }

    for (size_t i = 0, j = 0; i < h; i++)
    {
        for (size_t k = 0; k < m->row_begins[i];)
        {
            size_t r = rand() % w;
            bool uniq = true;
            for (size_t kk = 0; uniq && kk < m->row_begins[i]; kk++)
                uniq = m->columns[kk + j] != r;

            if (uniq)
            {
                m->columns[k + j] = r;
                k++;
            }
        }

        qsort(m->columns + j,  m->row_begins[i], sizeof(size_t), cmp_size_t);

        size_t tmp = m->row_begins[i];
        m->row_begins[i] = j;
        j += tmp;
    }
}

smatrix_t *scan_smatrix(void)
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

    smatrix_t *m;
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

        m = create_smatrix(w, h, n);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        fill_random_smatrix(m);

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

        m = create_smatrix(w, h, n);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        printf("Введите элементы матрицы:\n");
        printf("(столбец, строка и значение элемента через пробел)\n");

        size_t prev_y = 0;
        for (size_t i = 0, prev_x = 0; i < n; i++)
        {
            size_t x, y;
            typeof(*m->values) el;
            if (scanf("%lu %lu %d", &x, &y, &el) != 3
                || x == 0 || y == 0 || x > w || y > h
                || y < prev_y || (y == prev_y && x <= prev_x))
            {
                free_smatrix(m);
                wait_endl();
                printf(RED "Неправильный ввод элемента\n" RESET);
                return NULL;
            }

            m->values[i] = el;
            m->columns[i] = x - 1;

            for (size_t yy = prev_y; yy < y; yy++)
                m->row_begins[yy] = i;

            prev_x = x;
            prev_y = y;
        }

        for (size_t yy = prev_y; yy < h; yy++)
            m->row_begins[yy] = n;

        wait_endl();
    }
    else
    {
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }


    printf(GRN "Матрица создана:\n\n" RESET);
    print_smatrix(m, false);

    return m;
}

smatrix_t *scan_row_smatrix(void)
{
    printf("Введите количество столбцов:\n");
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

    smatrix_t *m;
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

        m = create_smatrix(w, h, n);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        fill_random_smatrix(m);

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

        m = create_smatrix(w, h, n);

        if (m == NULL)
        {
            printf(RED "Не хватило памяти для создания матрицы\n" RESET);
            return NULL;
        }

        printf("Введите элементы матрицы:\n");
        printf("(столбец, строка и значение элемента через пробел)\n");

        size_t prev_y = 0;
        for (size_t i = 0, prev_x = 0; i < n; i++)
        {
            size_t x, y;
            typeof(*m->values) el;
            if (scanf("%lu %lu %d", &x, &y, &el) != 3
                || x == 0 || y == 0 || x > w || y > h
                || y < prev_y || (y == prev_y && x <= prev_x))
            {
                free_smatrix(m);
                wait_endl();
                printf(RED "Неправильный ввод элемента\n" RESET);
                return NULL;
            }

            m->values[i] = el;
            m->columns[i] = x - 1;

            for (size_t yy = prev_y; yy < y; yy++)
                m->row_begins[yy] = i;

            prev_x = x;
            prev_y = y;
        }

        for (size_t yy = prev_y; yy < h; yy++)
            m->row_begins[yy] = n;

        wait_endl();
    }
    else
    {
        printf(RED "Неправильный ввод\n" RESET);
        return NULL;
    }


    printf(GRN "Матрица создана:\n\n" RESET);
    print_smatrix(m, false);

    return m;
}

int multiply_row_smatrix_by_smatrix(const smatrix_t * restrict m_row, const smatrix_t * restrict m, smatrix_t * restrict result)
{
    if (m_row->width != m->height || m_row->height != 1)
        return EMATRIXMUL;

    size_t w = m->width;
    size_t n = m_row->width;

    typeof(*m->values) *res = calloc(w, sizeof(*m->values));

    if (res == NULL)
        return EMEM;

    for (size_t i = 0; i < m_row->n_elems; i++)
    {
        size_t y = m_row->columns[i];
        typeof(*m->values) m_row_el = m_row->values[i];

        size_t end = y + 1 < n ? m->row_begins[y + 1] : m->n_elems;

        for (size_t j = m->row_begins[y]; j < end; j++)
        {
            res[m->columns[j]] += m_row_el * m->values[j];
        }
    }

    size_t n_elems = 0;
    for (size_t i = 0; i < w; i++)
        n_elems += res[i] != 0;

    if (resize_smatrix(result, w, 1, n_elems) != OK)
    {
        free(res);
        return EMEM;
    }

    result->row_begins[0] = 0;

    for (size_t i = 0, j = 0; i < w; i++)
        if (res[i] != 0)
        {
            result->values[j] = res[i];
            result->columns[j] = i;
            j++;
        }

    free(res);
    return OK;
}

void print_small_matrix(const smatrix_t *m)
{
    for (size_t y = 0; y < m->height; y++)
    {
        size_t i = m->row_begins[y];
        size_t end;

        if (y == m->height - 1)
            end = m->n_elems;
        else
            end = m->row_begins[y + 1];

        for (size_t x = 0; x < m->width; x++)
        {
            typeof(*m->values) val;

            if (i < end && m->columns[i] == x)
                val = m->values[i++];
            else
                val = 0;

            if (x == 0)
                printf(I_ "%*d", PRINT_VALUE_SIZE, val);
            else
                printf(_I_ "%*d", PRINT_VALUE_SIZE, val);
        }

        printf(_I_n);
    }
}

void print_big_matrix(const smatrix_t *m)
{
    #define VALUE_TITLE_STR_LEN 8
    #define VALUE_LEN MAX(VALUE_TITLE_STR_LEN, PRINT_VALUE_SIZE)

    printf(I_ "Столбец" _I_ "Строка " _I_ "%-*s" _I_n, VALUE_LEN + VALUE_TITLE_STR_LEN, "Значение");

    printf(I_ YEL "-------" RESET _I_ YEL "-------" RESET _I_ YEL);
    print_repeat("-", VALUE_LEN);
    printf(RESET _I_n);

    for (size_t y = 0; y < m->height; y++)
    {
        size_t end;

        if (y == m->height - 1)
            end = m->n_elems;
        else
            end = m->row_begins[y + 1];

        for (size_t i = m->row_begins[y]; i < end; i++)
            printf(I_ "%7lu" _I_ "%7lu" _I_ "%*d" _I_n, m->columns[i] + 1, y + 1, VALUE_LEN, m->values[i]);
    }

    #undef VALUE_TITLE_STR_LEN
    #undef VALUE_LEN
}

void print_smatrix(const smatrix_t *m, bool force_big)
{
    printf("m->values = ");
    for (size_t i = 0; i < m->n_elems; i++)
        printf(" %d", m->values[i]);
    printf("\n");

    printf("m->columns = ");
    for (size_t i = 0; i < m->n_elems; i++)
        printf(" %lu", m->columns[i]);
    printf("\n");

    printf("m->row_begins = ");
    for (size_t i = 0; i < m->height; i++)
        printf(" %lu", m->row_begins[i]);
    printf("\n");

    printf(YEL "Тип:" RESET" 2-мерная разреженная матрица\n");
    printf(YEL "Размер:" RESET " %lu байт\n",
        sizeof(*m) + (sizeof(*m->values) + sizeof(*m->columns)) * m->n_elems + sizeof(*m->row_begins) * m->height);
    printf(YEL "Данные:" RESET "\n");

    if (!force_big && m->width <= PRINT_MATRIX_MAX_WIDTH && m->height <= PRINT_MATRIX_MAX_HEIGHT)
    {
        print_small_matrix(m);
    }
    else
    {
        print_big_matrix(m);
    }
}