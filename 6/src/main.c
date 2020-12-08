#include "main.h"

int is_prime(unsigned int num)
{
    if (num <= 1)
        return 0;

    if (num % 2 == 0 && num > 2)
        return 0;

    for (int i = 3; i < floor(sqrt(num)); i += 2)
        if (num % i == 0)
            return 0;

    return 1;
}

int next_prime(int n)
{
    unsigned int prime = n + 1;
    while (!is_prime(prime))
        prime++;

    return prime;
}

int read_tree_from_file(char *filename, tree_t **default_tree)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        LOG_ERROR("не получилось открыть файл%s", "");
        return EXIT_FAILURE;
    }

    *default_tree = read_to_tree(f);
    fclose(f);

    if (*default_tree == NULL)
    {
        LOG_ERROR("дерево пустое%s", "");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int count_numbers_in_file(FILE *f)
{
    int x, c = 0;
    while (fscanf(f, "%d", &x) == 1)
        c++;

    return c;
}

int read_hashtable_from_file(char *filename, hashtable_t **table)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        LOG_ERROR("не получилось открыть файл%s", "");
        return EXIT_FAILURE;
    }

    int size = count_numbers_in_file(f);
    size = next_prime(size * 13 / 10 + 1);

    rewind(f);
    *table = read_to_hashtable(f, 1 ? complex_hash : simple_hash, size);

    fclose(f);

    if (*table == NULL)
    {
        LOG_ERROR("не получилось сохранить данные в хеш таблицу%s", "");
        fclose(f);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

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

    tree_t *default_tree;

    if (read_tree_from_file(filename, &default_tree))
    {
        LOG_ERROR("не получилось построить ДДП из файла%s", "");
        return EXIT_FAILURE;
    }

    printf(YEL "ДДП из файла:\n\n" RESET);
    print_tree(default_tree);
    printf("\n");

    tree_t *balanced_tree;

    if (copy_tree(default_tree, &balanced_tree))
    {
        LOG_ERROR("не получилось копировать ДДП%s", "");
        free_tree(default_tree);
        return EXIT_FAILURE;
    }

    balanced_tree = balance_tree(balanced_tree);

    printf(YEL "Сбалансированное дерево:\n\n" RESET);
    print_tree(balanced_tree);
    printf("\n");

    hashtable_t *table;

    if (read_hashtable_from_file(filename, &table))
    {
        free_tree(default_tree);
        free_tree(balanced_tree);
        return EXIT_FAILURE;
    }

    printf(YEL "Хеш таблица:\n" RESET);
    print_hashtable(table);

    free_tree(default_tree);
    free_tree(balanced_tree);
    free_hashtable(table);


    return EXIT_SUCCESS;
}
