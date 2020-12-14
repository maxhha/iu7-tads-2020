#include "main.h"

uint64_t ticks(void)
{
    uint64_t x;
    __asm__ volatile ("rdtsc\n\tshl $32, %%rdx\n\tor %%rdx, %%rax" : "=a" (x) : : "rdx");
    return x;
}

#define TYPE_GRAPH_MATRIX "m"
#define TYPE_GRAPH_LIST "l"

void print_usage(void)
{
    printf("Неправильное количество параметров\n");
    printf("Использование: app.exe ФАЙЛ СТОЛИЦА ПАРАМЕТР_T ТИП\n");
    printf("  ФАЙЛ - имя файла с описанием графа.\n");
    printf("  СТОЛИЦА - номер вершины, к которой нужно считать путь.\n");
    printf("  ПАРАМЕТР_T - длина пути,"
        " с которого вершина попадает в выходной список.\n");
    printf("  ТИП - тип структуры графа."
        " Принимает занчения \"" TYPE_GRAPH_MATRIX "\" (матрица)"
        "или \"" TYPE_GRAPH_LIST "\" (список).\n");
}

int main(int argc, char **argv)
{
    #ifdef DEBUG

    char *filename = "in.txt";
    int capital = 1;
    int param_t = 5;
    char *type = "m";

    #else

    if (argc != 5)
    {
        print_usage();
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    int capital;
    int param_t;
    char *type = argv[4];

    if (sscanf(argv[2], "%d", &capital) != 1 || capital <= 1)
    {
        LOG_ERROR("Неправильный номер врешины-столицы%s", "");
        return EXIT_FAILURE;
    }

    if (sscanf(argv[3], "%d", &param_t) != 1 || param_t < 0)
    {
        LOG_ERROR("Неправильный параметр%s", "");
        return EXIT_FAILURE;
    }

    #endif

    int rc;

    if (strcmp(type, TYPE_GRAPH_MATRIX) == 0)
    {
        rc = process_with_graph_matrix(filename, capital, param_t);
    }
    else if (strcmp(type, TYPE_GRAPH_LIST) == 0)
    {
        rc = EXIT_FAILURE;
    }
    else
    {
        LOG_ERROR("Неправильный тип структуры%s", "");
        rc = EXIT_FAILURE;
    }

    if (rc != EXIT_SUCCESS)
    {
        return rc;
    }

    printf("Граф сохранен в " OUTPUT_FILE "\n");

    rc = system("dot -Tpng " OUTPUT_FILE " -o " OUTPUT_IMG_FILE);

    if (rc != EXIT_SUCCESS)
    {
        LOG_ERROR("не получилось создать изображение графа%s", "");
        return EXIT_FAILURE;
    }

    printf("Изображение графа сохранено в " OUTPUT_IMG_FILE "\n");

    rc = system("xdg-open " OUTPUT_IMG_FILE);

    if (rc != EXIT_SUCCESS)
    {
        LOG_ERROR("не получилось вызвать отображение картинки%s", "");
        return EXIT_FAILURE;
    }

    return rc;
}
