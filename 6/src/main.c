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
    size = next_prime(size + 1);

    rewind(f);
    *table = read_to_hashtable(f, simple_hash, size);

    fclose(f);

    if (*table == NULL)
    {
        LOG_ERROR("не получилось сохранить данные в хеш таблицу%s", "");
        fclose(f);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int restructure_hashtable_by_entered_collosions(hashtable_t **table)
{
    int target_avg_collisions = 10;

    #ifndef DEBUG
    printf(YEL "Введите среднее количество коллизий:" RESET "\n");

    if (scanf("%d", &target_avg_collisions) != 1 || target_avg_collisions < 0)
    {
        printf(RED "Неправильное число" RESET "\n");
        return EXIT_FAILURE;
    }
    #endif

    int avg_collisions = (*table)->n_collisions;
    avg_collisions /= get_hashtable_count_items(*table);

    if (target_avg_collisions >= avg_collisions)
    {
        printf(YEL "Реструктуризация не нужна" RESET "\n\n");
        return EXIT_SUCCESS;
    }

    int new_size = (*table)->size;

    while (target_avg_collisions < avg_collisions)
    {
        hashtable_t *new_table = restructure_hashtable(*table, complex_hash, new_size);

        if (new_table == NULL)
        {
            LOG_ERROR("не удалось реструктуризировать%s", "");
            return EXIT_FAILURE;
        }

        free_hashtable(*table);
        *table = new_table;
        avg_collisions = (*table)->n_collisions;
        avg_collisions /= get_hashtable_count_items(*table);
        new_size = next_prime((*table)->size);
    }

    printf(YEL "Хеш таблица c сложной функцией хэширования:\n" RESET);
    print_hashtable(*table);
    printf("Среднее число коллизий: %d\n", avg_collisions);

    return EXIT_SUCCESS;
}

int remove_element_from_file(char *filename, int val)
{
    FILE *f = fopen(filename, "r+");

    if (f == NULL)
    {
        LOG_ERROR("не получилось открыть файл%s", "");
        return EXIT_FAILURE;
    }

    int i = 0, j = 0, x;

    while (fscanf(f, "%d", &x) == 1)
    {
        if (x == val)
        {
            j = ftell(f) + 1;
            continue;
        }

        if (j == i)
        {
            i = j = ftell(f) + 1;
            continue;
        }

        j = ftell(f) + 1;

        fseek(f, i, SEEK_SET);
        fprintf(f, "%d\n", x);
        i = ftell(f);
        fseek(f, j, SEEK_SET);
    }

    if (fclose(f))
    {
        LOG_ERROR("не получилось сохранить изменения%s", "");
        return EXIT_FAILURE;
    }

    if (truncate(filename, i))
    {
        LOG_ERROR("не получилось уменьшить размер файла%s", "");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int remove_element(char *filename, tree_t **default_tree, tree_t **balanced_tree, hashtable_t **table)
{
    int x = 944;

    printf(YEL "Введите элемент для удаления:" RESET "\n");

    #ifndef DEBUG
    if (scanf("%d", &x) != 1)
    {
        printf(RED "Неправильное число" RESET "\n");
        return EXIT_FAILURE;
    }

    if (find_tree_val(*balanced_tree, x) == NULL)
    {
        printf(RED "Нет такого элемента в файле" RESET "\n");
        return EXIT_FAILURE;
    }
    #endif

    *default_tree = delete_element_from_tree(*default_tree, x);

    printf(YEL "ДДП из файла после удаления элемента:\n\n" RESET);
    print_tree(*default_tree);

    *balanced_tree = delete_element_from_balanced_tree(*balanced_tree, x);

    printf(YEL "Сбалансированное дерево после удаления элемента:\n\n" RESET);
    print_tree(*balanced_tree);

    printf(YEL "Хеш таблица после удаления элемента:\n\n" RESET);
    delete_element_from_hashtable(*table, x);
    print_hashtable(*table);

    int rc = remove_element_from_file(filename, x);

    if (rc)
    {
        printf(RED "Не получилось удалить элемент из файла" RESET "\n");
        return EXIT_FAILURE;
    }

    printf(YEL "Элемент удалён из файла" RESET "\n");

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

    printf(YEL "Хеш таблица с простой функцией хеширования:\n\n" RESET);
    print_hashtable(table);
    printf("Среднее число коллизий: %d\n", table->n_collisions / get_hashtable_count_items(table));

    if (restructure_hashtable_by_entered_collosions(&table))
    {
        free_tree(default_tree);
        free_tree(balanced_tree);
        free_hashtable(table);
        return EXIT_FAILURE;
    }

    if (remove_element(filename, &default_tree, &balanced_tree, &table))
    {
        free_tree(default_tree);
        free_tree(balanced_tree);
        free_hashtable(table);
        return EXIT_FAILURE;
    }

    free_tree(default_tree);
    free_tree(balanced_tree);
    free_hashtable(table);

    return EXIT_SUCCESS;
}
