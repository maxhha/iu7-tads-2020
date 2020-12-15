#include "main.h"

#define TYPE_GRAPH_MATRIX "m"
#define TYPE_GRAPH_LIST "l"
#define DRAW_GRAPH "draw"
#define NODRAW_GRAPH "nodraw"

void print_usage(void)
{
    printf("Неправильное количество параметров\n");
    printf("Использование: app.exe ФАЙЛ СТОЛИЦА ПАРАМЕТР_T ТИП [РИСОВАТЬ]\n");
    printf("  ФАЙЛ - имя файла с описанием графа.\n");
    printf("  СТОЛИЦА - номер вершины, к которой нужно считать путь.\n");
    printf("  ПАРАМЕТР_T - длина пути,"
        " с которого вершина попадает в выходной список.\n");
    printf("  ТИП - тип структуры графа."
        " Принимает значения \"" TYPE_GRAPH_MATRIX "\" (матрица)"
        " или \"" TYPE_GRAPH_LIST "\" (список).\n");
    printf("  РИСОВАТЬ - рисовать ли графа."
        " Принимает значения \"" DRAW_GRAPH "\" (по умолчанию)"
        " или \"" NODRAW_GRAPH "\"\n");
    printf("\n");
}

int main(int argc, char **argv)
{
    #ifdef DEBUG

        char *filename = "in0.txt";
        int capital = 1;
        int param_t = 4;
        char *type = "m";
        int draw = 1;

    #else

        if (argc < 5 || argc > 6)
        {
            print_usage();
            return EXIT_FAILURE;
        }

        char *filename = argv[1];
        int capital;
        int param_t;
        char *type = argv[4];
        int draw = 1;

        if (sscanf(argv[2], "%d", &capital) != 1 || capital < 1)
        {
            LOG_ERROR("Неправильный номер врешины-столицы%s", "");
            return EXIT_FAILURE;
        }

        if (sscanf(argv[3], "%d", &param_t) != 1 || param_t < 0)
        {
            LOG_ERROR("Неправильный параметр%s", "");
            return EXIT_FAILURE;
        }

        if (argc > 5)
        {
            if (strcmp(argv[5], NODRAW_GRAPH) == 0)
                draw = 0;
            else if (strcmp(argv[5], DRAW_GRAPH) == 0)
                draw = 1;
            else
            {
                LOG_ERROR("Неправильное значение для рисовать%s", "");
                return EXIT_FAILURE;
            }
        }

    #endif

    capital--;

    int rc;

    if (strcmp(type, TYPE_GRAPH_MATRIX) == 0)
    {
        rc = process_with_graph_matrix(filename, capital, param_t);
    }
    else if (strcmp(type, TYPE_GRAPH_LIST) == 0)
    {
        rc = process_with_graph_list(filename, capital, param_t);
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

    if (draw == 0)
        return rc;

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
