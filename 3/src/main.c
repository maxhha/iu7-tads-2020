#include "../inc/main.h"

int main(int argc, char const *argv[])
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

        print_matrix(result);
    }

    free_matrix(result);
    free_matrix(b);
    free_matrix(a);

    return rc == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
