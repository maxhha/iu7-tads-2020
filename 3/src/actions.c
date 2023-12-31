#include "../inc/actions.h"

int action_mul_matrix(void)
{
    int rc = ERR;
    printf("Введите матрицу-строку:\n");

    matrix_t *a = scan_row_matrix();
    if (a == NULL)
        return OK;

    printf("\nВведите матрицу:\n");
    matrix_t *b = scan_matrix();

    if (b == NULL)
    {
        free_matrix(a);
        return OK;
    }

    matrix_t *result = create_matrix(1, 1);

    if (result == NULL)
    {
        free_matrix(b);
        free_matrix(a);
        return OK;
    }

    rc = multiply_row_matrix_by_matrix(a, b, result);

    if (rc == EMATRIXMUL)
    {
        printf(RED "Размеры матриц не походят для умножения.\n" RESET);
    }
    else if (rc == EMEM)
    {
        printf(RED "Не получилось выделить память для ответа.\n" RESET);
    }
    else if (rc == EMATRIXZERO)
    {
        printf(RED "Результирующая матрица не содержит ненулевые элементы.\n" RESET);
    }
    else
    {
        assert(rc == OK);
        printf("\n" GRN "Результат:\n" RESET);

        print_matrix(result);
    }

    free_matrix(result);
    free_matrix(b);
    free_matrix(a);

    return rc;
}

int action_mul_smatrix(void)
{
    int rc = ERR;
    printf("Введите матрицу-строку:\n");

    smatrix_t *a = scan_row_smatrix();
    if (a == NULL)
        return OK;

    printf("\nВведите матрицу:\n");

    smatrix_t *b = scan_smatrix();

    if (b == NULL)
    {
        free_smatrix(a);
        return OK;
    }

    smatrix_t *result = create_smatrix(1, 1, 1);

    if (result == NULL)
    {
        free_smatrix(b);
        free_smatrix(a);
        return OK;
    }

    rc = multiply_row_smatrix_by_smatrix(a, b, result);

    if (rc == EMATRIXMUL)
    {
        printf(RED "Размеры матриц не походят для умножения.\n" RESET);
    }
    else if (rc == EMEM)
    {
        printf(RED "Не получилось выделить память для ответа.\n" RESET);
    }
    else if (rc == EMATRIXZERO)
    {
        printf(RED "Результирующая матрица не содержит ненулевые элементы.\n" RESET);
    }
    else
    {
        assert(rc == OK);
        printf("\n" GRN "Результат:\n" RESET);

        print_smatrix(result);
    }

    free_smatrix(result);
    free_smatrix(b);
    free_smatrix(a);

    return OK;
}

int action_measure_matrix_mul(void)
{
    size_t w_row, h, w, n_row, n;
    double percent_row, percent;

    printf("Введите количество столбцов в матрице-строке:\n");
    if (scanf("%lu", &w_row) != 1 || w_row == 0)
    {
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    printf("Введите процент заполненности (0-100):\n");
    if (scanf("%lf", &percent_row) != 1 || percent_row < 0 || percent_row > 100)
    {
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    n_row = (size_t) (percent_row * w_row / 100);

    if (n_row == 0)
    {
        printf(RED "Количество элементов при этом проценте равно 0\n" RESET);
        return OK;
    }

    matrix_t *a = create_matrix(w_row, 1);

    if (a == NULL)
    {
        printf(RED "Не хватило памяти\n" RESET);
        return OK;
    }

    printf("Введите количество столбцов и строк в матрице через пробел:\n");
    if (scanf("%lu %lu", &w, &h) != 2 || w == 0 || h == 0)
    {
        free_matrix(a);
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    printf("Введите процент заполненности (0-100):\n");
    if (scanf("%lf", &percent) != 1 || percent < 0 || percent > 100)
    {
        free_matrix(a);
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    n = (size_t) (percent * w * h / 100);

    if (n == 0)
    {
        free_matrix(a);
        printf(RED "Количество элементов при этом проценте равно 0\n" RESET);
        return OK;
    }

    matrix_t *b = create_matrix(w, h);

    if (b == NULL)
    {
        free_matrix(a);
        printf(RED "Не хватило памяти\n" RESET);
        return OK;
    }

    double time = 0;

    printf(YEL "Меряем...\n" RESET);

    for (int measure_i = 0; measure_i < TIME_MEASURE_REPEATS; measure_i++)
    {
        printf("\r%d/%d", measure_i + 1, TIME_MEASURE_REPEATS);

        clock_t start, end;
        matrix_t *result = create_matrix(1, 1);

        if (result == NULL)
        {
            free_matrix(b);
            free_matrix(a);
            printf(RED "\nНе хватило памяти\n" RESET);
            return OK;
        }

        fill_random_matrix(a, n_row);
        fill_random_matrix(b, n);

        int rc = multiply_row_matrix_by_matrix(a, b, result);

        if (rc == EMATRIXMUL)
        {
            free_matrix(result);
            free_matrix(b);
            free_matrix(a);
            printf(RED "\nНеправильный размер матриц\n" RESET);
            return OK;
        }
        else if (rc == EMEM)
        {
            free_matrix(result);
            free_matrix(b);
            free_matrix(a);
            printf(RED "\nНе хватило памяти для ответа\n" RESET);
            return OK;
        }
        assert(rc == OK || rc == EMATRIXZERO);

        start = clock();

        multiply_row_matrix_by_matrix(a, b, result);

        end = clock();

        time += (double) (end - start) / CLOCKS_PER_SEC * MICROSEC_PER_SEC;

        free_matrix(result);
    }

    time /= TIME_MEASURE_REPEATS;

    printf(YEL "\rВремя:" RESET " %.0lf мкс\n", time);

    return OK;
}

int action_measure_smatrix_mul(void)
{
    size_t w_row, h, w, n_row, n;
    double percent_row, percent;

    printf("Введите количество столбцов в матрице-строке:\n");
    if (scanf("%lu", &w_row) != 1 || w_row == 0)
    {
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    printf("Введите процент заполненности (0-100):\n");
    if (scanf("%lf", &percent_row) != 1 || percent_row < 0 || percent_row > 100)
    {
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    n_row = (size_t) (percent_row * w_row / 100);

    if (n_row == 0)
    {
        printf(RED "Количество элементов при этом проценте равно 0\n" RESET);
        return OK;
    }

    smatrix_t *a = create_smatrix(w_row, 1, n_row);

    if (a == NULL)
    {
        printf(RED "Не хватило памяти\n" RESET);
        return OK;
    }

    printf("Введите количество столбцов и строк в матрице через пробел:\n");
    if (scanf("%lu %lu", &w, &h) != 2 || w == 0 || h == 0)
    {
        free_smatrix(a);
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    printf("Введите процент заполненности (0-100):\n");
    if (scanf("%lf", &percent) != 1 || percent < 0 || percent > 100)
    {
        free_smatrix(a);
        printf(RED "Неправильный ввод\n" RESET);
        return OK;
    }

    n = (size_t) (percent * w * h / 100);

    if (n == 0)
    {
        free_smatrix(a);
        printf(RED "Количество элементов при этом проценте равно 0\n" RESET);
        return OK;
    }

    smatrix_t *b = create_smatrix(w, h, n);

    if (b == NULL)
    {
        free_smatrix(a);
        printf(RED "Не хватило памяти\n" RESET);
        return OK;
    }

    double time = 0;

    printf(YEL "Меряем...\n" RESET);

    for (int measure_i = 0; measure_i < TIME_MEASURE_REPEATS; measure_i++)
    {
        printf("\r%d/%d", measure_i + 1, TIME_MEASURE_REPEATS);

        clock_t start, end;
        smatrix_t *result = create_smatrix(1, 1, 1);

        if (result == NULL)
        {
            free_smatrix(b);
            free_smatrix(a);
            printf(RED "\nНе хватило памяти\n" RESET);
            return OK;
        }

        // printf("fill random\n");

        fill_random_smatrix(a);
        fill_random_smatrix(b);

        // printf("first run\n");

        int rc = multiply_row_smatrix_by_smatrix(a, b, result);

        // print_smatrix(a);
        // print_smatrix(b);
        // print_smatrix(result);

        if (rc == EMATRIXMUL)
        {
            free_smatrix(result);
            free_smatrix(b);
            free_smatrix(a);
            printf(RED "\nНеправильный размер матриц\n" RESET);
            return OK;
        }
        else if (rc == EMEM)
        {
            free_smatrix(result);
            free_smatrix(b);
            free_smatrix(a);
            printf(RED "\nНе хватило памяти для ответа\n" RESET);
            return OK;
        }
        assert(rc == OK || rc == EMATRIXZERO);

        // printf("measure run\n");

        start = clock();

        multiply_row_smatrix_by_smatrix(a, b, result);

        end = clock();

        time += (double) (end - start) / CLOCKS_PER_SEC * MICROSEC_PER_SEC;

        free_smatrix(result);
    }

    time /= TIME_MEASURE_REPEATS;

    printf(YEL "\rВремя:" RESET " %.0lf мкс\n", time);

    return OK;
}
