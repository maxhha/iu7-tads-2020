#include "../inc/main.h"

int main(int argc, char const *argv[])
{
    int rc = ERR;
    printf(YEL "Введите матрицу " RESET "А" YEL ":\n" RESET);

    matrix_t *a = matrix_scan();
    if (a == NULL)
        goto break_a;

    printf("\n" YEL "Введите матрицу " RESET "B" YEL ":\n" RESET);
    matrix_t *b = matrix_scan();

    if (b == NULL)
        goto break_b;

    matrix_t *result = matrix_create(1, 1);

    if (result == NULL)
        goto break_result;

    rc = matrix_multiply(a, b, result);

    if (rc == EMATRIXMUL)
    {
        printf(RED "Размеры матриц не походят для умножения.\n" RESET);
        goto break_mul;
    }
    if (rc == EMEM)
    {
        printf(RED "Не получилось выделить память для ответа.\n" RESET);
        goto break_mul;
    }

    assert(rc == OK);

    printf("\n" GRN "Результат:\n" RESET);

    matrix_print(result);

    break_mul:
    matrix_free(result);

    break_result:
    matrix_free(b);

    break_b:
    matrix_free(a);

    break_a:
    return rc == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
