#include "../inc/actions.h"

int action_mul_matrix(void)
{
    int rc = ERR;
    printf(YEL "Введите матрицу-строку " RESET "А" YEL ":\n" RESET);

    matrix_t *a = scan_row_matrix();
    if (a == NULL)
        return EXIT_FAILURE;

    printf("\n" YEL "Введите матрицу " RESET "B" YEL ":\n" RESET);
    matrix_t *b = scan_matrix();

    if (b == NULL)
    {
        free_matrix(a);
        return EXIT_FAILURE;
    }

    matrix_t *result = create_matrix(1, 1);

    if (result == NULL)
    {
        free_matrix(b);
        free_matrix(a);
        return EXIT_FAILURE;
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
    else
    {
        assert(rc == OK);
        printf("\n" GRN "Результат:\n" RESET);

        print_matrix(result, false);
    }

    free_matrix(result);
    free_matrix(b);
    free_matrix(a);

    return rc;
}


int action_mul_smatrix(void)
{
    int rc = ERR;
    printf(YEL "Введите матрицу-строку " RESET "А" YEL ":\n" RESET);

    smatrix_t *a = scan_row_smatrix();
    if (a == NULL)
        return EXIT_FAILURE;

    printf("\n" YEL "Введите матрицу " RESET "B" YEL ":\n" RESET);
    smatrix_t *b = scan_smatrix();

    if (b == NULL)
    {
        free_smatrix(a);
        return EXIT_FAILURE;
    }

    smatrix_t *result = create_smatrix(1, 1, 1);

    if (result == NULL)
    {
        free_smatrix(b);
        free_smatrix(a);
        return EXIT_FAILURE;
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
    else
    {
        assert(rc == OK);
        printf("\n" GRN "Результат:\n" RESET);

        print_smatrix(result, false);
    }

    free_smatrix(result);
    free_smatrix(b);
    free_smatrix(a);

    return rc;
}
