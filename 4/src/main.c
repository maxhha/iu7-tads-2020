#include "../inc/main.h"

/*
# Поиск пути

- [x] загрузка карты из файла
    - [x] стены
    - [x] точки начала и конца
- [x] поиск пути с использованием выбранной структуры
    - [x] для списка
    - [x] для массива
    - [x] с выводом времени выполнения и объема памяти
    - [x] c выводом адресов памяти
- [x] вывод результата
    - [x] в файл
    - [x] в консоль
    - [x] координаты
*/

#define OPTKEY_FILEIN 'i'
#define OPTKEY_FILEOUT 'o'
#define OPTKEY_MAPOUTPUT 'm'
#define OPTKEY_STACK 's'
#define OPTKEY_LIMIT 'l'
#define OPTKEY_MEMADRS 'A'
#define OPTKEY_TIME 't'

static struct argp_option options[] = {
    {
        "in",
        OPTKEY_FILEIN,
        "FILEIN",
        OPTION_ARG_OPTIONAL,
        "Файл с лабиринтом",
    },
    {
        "out",
        OPTKEY_FILEOUT,
        "FILEOUT",
        OPTION_ARG_OPTIONAL,
        "Файл вывода",
    },
    {
        "map",
        OPTKEY_MAPOUTPUT,
        0,
        OPTION_ARG_OPTIONAL,
        "Записать путь в виде карты",
    },
    {
        "stack",
        OPTKEY_STACK,
        "STACK",
        OPTION_ARG_OPTIONAL,
        "Тип представления стэка;\nМожет быть 'list'(по умолч.) и 'array'",
    },
    {
        "limit",
        OPTKEY_LIMIT,
        "LIMIT",
        OPTION_ARG_OPTIONAL,
        "Ограничение на количество элементов в стеке;\nПо умолч. 0 - без ограничения",
    },
    {
        "addresses",
        OPTKEY_MEMADRS,
        0,
        0,
        "Выводить адреса памяти",
    },
    {
        "time",
        OPTKEY_TIME,
        "N",
        OPTION_ARG_OPTIONAL,
        "Произвести измерение времени работы стэка и выйти",
    },
    {
        0,
        0,
        0,
        OPTION_DOC,
        "Формат файла с лабиринтом.\n"
        "\n"
        "В первой строке 2 числа - количество строк и столбков карты.\n"
        "\n"
        "Карта содержит только следующие символы:\n"
        "'#' - стена;\n"
        "' ' - проход;\n"
        "'A' - начало пути;\n"
        "'B' - конец;\n"
        "Лабиринт обязательно должен иметь стены на краях прямоугольной карты; "
        "\n\n"
        "Пример:\n"
        "5 6\n"
        "######\n"
        "#    #\n"
        "#  # #\n"
        "#A#B #\n"
        "######\n",
    },
    { 0 }
};

struct arguments {
    char *filein;
    char *fileout;
    enum { FORMAT_COORDS, FORMAT_MAP } output_format;
    enum { STACK_LIST, STACK_ARRAY } stack_type;
    size_t stack_limit;
    bool log_mem;
    int time_measure_elements;
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = (struct arguments *) state->input;

    switch (key)
    {
        case OPTKEY_FILEIN:

        arguments->filein = arg;

        return 0;
        case OPTKEY_FILEOUT:

        arguments->fileout = arg;

        return 0;
        case OPTKEY_MAPOUTPUT:

        arguments->output_format = FORMAT_MAP;

        return 0;
        case OPTKEY_STACK:

        if (arg == NULL)
            return 0;

        if (strcmp(arg, "list") == 0)
        {
            arguments->stack_type = STACK_LIST;
        }
        else if (strcmp(arg, "array") == 0)
        {
            arguments->stack_type = STACK_ARRAY;
        }
        else
        {
            argp_error(state, "Неизвестный тип стэка '%s'", arg);
        }

        return 0;
        case OPTKEY_LIMIT:

        if (arg == NULL)
            return 0;

        int limit;

        if (sscanf(arg, "%d", &limit) != 1 || limit <= 0)
        {
            argp_error(state, "Неправильный лимит '%s'", arg);
        }

        arguments->stack_limit = limit;

        return 0;
        case OPTKEY_MEMADRS:

        arguments->log_mem = true;

        return 0;
        case OPTKEY_TIME:

        if (sscanf(arg, "%d", &arguments->time_measure_elements) != 1 || arguments->time_measure_elements <= 0)
        {
            argp_error(state, "Неправильное количество элементов '%s'", arg);
        }

        return 0;
        default: return ARGP_ERR_UNKNOWN;
    }
}

static struct argp argp = { options, parse_opt, 0, 0};

int process_task(struct arguments arguments, memwatch_t *memwatch)
{
    map_t *map = read_map_from_file(arguments.filein);

    if (map == NULL)
    {
        LOG_ERROR("не получилось прочитать карту из файла '%s'", arguments.filein);
        return EXIT_FAILURE;
    }

    point_t *path;
    int path_len;

    if (arguments.stack_type == STACK_LIST)
    {
        stack_list_t *stack = create_stack_list(arguments.stack_limit, memwatch);
        path_len = get_path_using_stack_list(map, stack, &path);
        free_stack_list(stack);
    }
    else
    {
        stack_array_t *stack = create_stack_array(arguments.stack_limit, memwatch);
        path_len = get_path_using_stack_array(map, stack, &path);
        free_stack_array(stack);
    }

    if (path_len < 0)
    {
        LOG_ERROR("не получилось найти путь%s", "");

        free_map(map);
        return EXIT_FAILURE;
    }

    if (arguments.fileout == NULL)
    {
        for (int i = 0; i < path_len; i++)
            printf("%d %d\n", path[i].x, path[i].y);

        if (arguments.output_format == FORMAT_MAP)
        {
            printf("\n");
            print_map_with_path(stdout, map, path, path_len);
        }
    }
    else
    {
        FILE *f = fopen(arguments.fileout, "w");

        if (f == NULL)
        {
            LOG_ERROR("не получилось открыть файл для записи%s", "");
            LOG_ERROR("не получилось записать координаты в файл%s", "");

            wfree(memwatch, path);
            free_map(map);
            return EXIT_FAILURE;
        }

        for (int i = 0; i < path_len; i++)
            fprintf(f, "%d %d\n", path[i].x, path[i].y);

        if (arguments.output_format == FORMAT_MAP)
        {
            fprintf(f, "\n");
            print_map_with_path(f, map, path, path_len);
        }

        fclose(f);
    }

    wfree(memwatch, path);
    free_map(map);

    return EXIT_SUCCESS;
}

int process_measure_time(struct arguments arguments, memwatch_t *memwatch)
{
    int rc = EXIT_SUCCESS;
    double time_push = 0, time_pop = 0;
    clock_t start, end;

    if (arguments.stack_type == STACK_LIST)
    {
        int data = 1;

        for (int m = 0; m < TIME_MEASURE_REPEATS; m++)
        {
            stack_list_t *stack = create_stack_list(arguments.stack_limit, memwatch);

            start = clock();
            for (int i = 0; i < arguments.time_measure_elements && rc == 0; i++)
            {
                rc = stack_list_push(stack, &data);
            }
            end = clock();

            time_push += (double) (end - start) / CLOCKS_PER_SEC;

            if (rc != EXIT_SUCCESS)
            {
                LOG_ERROR("не получилось добавить элемент%s", "");
                while (stack_list_pop(stack) != NULL) {}
                free_stack_list(stack);
                return EXIT_FAILURE;
            }

            start = clock();
            for (int i = 0; i < arguments.time_measure_elements; i++)
            {
                stack_list_pop(stack);
            }
            end = clock();
            time_pop += (double) (end - start) / CLOCKS_PER_SEC;

            free_stack_list(stack);
        }


    }
    else
    {
        int data = 1;

        for (int m = 0; m < TIME_MEASURE_REPEATS; m++)
        {
            stack_array_t *stack = create_stack_array(arguments.stack_limit, memwatch);

            start = clock();
            for (int i = 0; i < arguments.time_measure_elements && rc == 0; i++)
            {
                rc = stack_array_push(stack, &data);
            }
            end = clock();

            time_push += (double) (end - start) / CLOCKS_PER_SEC;

            if (rc != EXIT_SUCCESS)
            {
                LOG_ERROR("не получилось добавить элемент%s", "");
                while (stack_array_pop(stack) != NULL) {}
                free_stack_array(stack);
                return EXIT_FAILURE;
            }

            start = clock();
            for (int i = 0; i < arguments.time_measure_elements; i++)
            {
                stack_array_pop(stack);
            }
            end = clock();
            time_pop += (double) (end - start) / CLOCKS_PER_SEC;

            free_stack_array(stack);
        }

    }

    time_push /= TIME_MEASURE_REPEATS;
    time_pop /= TIME_MEASURE_REPEATS;

    printf("push: %.6lf s\npop:  %.6lf s\n", time_push, time_pop);

    return rc;
}

int main(int argc, char **argv)
{
    struct arguments arguments = { 0, 0, FORMAT_COORDS, STACK_LIST, 0, false, -1 };

    if (argp_parse(&argp, argc, argv, 0, 0, &arguments))
    {
        return EXIT_FAILURE;
    }

    // arguments.filein = "sample.txt";
    // arguments.fileout = NULL;
    // arguments.output_format = FORMAT_MAP;
    // arguments.stack_type = STACK_LIST; //STACK_ARRAY; STACK_LIST
    // arguments.log_mem = true;

    memwatch_t *memwatch = NULL;

    if (arguments.log_mem)
    {
        memwatch = create_memory_watch();
        if (memwatch == NULL)
        {
            LOG_ERROR("не получилось создать наблюдателя за памятью%s", "");
            return EXIT_FAILURE;
        }
    }

    int rc;

    if (arguments.time_measure_elements > 0)
    {
        rc = process_measure_time(arguments, memwatch);
    }
    else
    {
        rc = process_task(arguments, memwatch);
    }

    if (memwatch != NULL)
        free_memory_watch(memwatch);

    return rc;
}
