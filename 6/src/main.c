#include "main.h"

int main(int argc, char **argv)
{
    char *filename;

    #ifndef DEBUG
        if (argc != 2)
        {
            printf("Использование: ./app.exe ФАЙЛ\n");
            return EXIT_FAILURE;
        }
        filename = argv[1];
    #else
        filename = "in.txt";
    #endif

    FILE *f = fopen(filename, "r+");

    if (!f)
    {
        LOG_ERROR("не получилось открыть файл%s", "");
        return EXIT_FAILURE;
    }

    tree_t *default_tree = read_to_tree(f);

    print_tree(default_tree);

    fclose(f);

    return EXIT_SUCCESS;
}
