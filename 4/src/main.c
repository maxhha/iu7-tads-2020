#include "../inc/main.h"

/*
# Поиск пути

- [x] загрузка карты из файла
    - [x] стены
    - [x] точки начала и конца
- [ ] поиск пути с использованием выбранной структуры
    - [x] для списка
    - [x] для массива
    - [ ] с выводом времени выполнения и объема памяти
    - [ ] c выводом адресов памяти
- [x] вывод результата
    - [x] в файл
    - [x] в консоль
    - [x] координаты
*/

#define OPTKEY_FILEIN 'i'
#define OPTKEY_FILEOUT 'o'
#define OPTKEY_MAPOUTPUT 'm'
#define OPTKEY_STACK 's'

static struct argp_option options[] = {
    {
        0,
        OPTKEY_FILEIN,
        "FILEIN",
        OPTION_ARG_OPTIONAL,
        "Файл с лабиринтом",
    },
    {
        0,
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
        "Записать путь в виде карты, а не списком координат",
    },
    {
        "stack",
        OPTKEY_STACK,
        "STACK",
        OPTION_ARG_OPTIONAL,
        "Тип представления стэка;\nМожет быть 'list'(по умолч.) и 'array'",
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
        default: return ARGP_ERR_UNKNOWN;
    }
}

static struct argp argp = { options, parse_opt, 0, 0};

int main(int argc, char **argv)
{
    struct arguments arguments = { 0, 0, FORMAT_COORDS, STACK_LIST };

    if (argp_parse(&argp, argc, argv, 0, 0, &arguments))
    {
        return EXIT_FAILURE;
    }

    arguments.filein = "sample.txt";
    arguments.fileout = NULL;
    arguments.output_format = FORMAT_MAP;
    arguments.stack_type = STACK_ARRAY;

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
        stack_list_t *stack = create_stack_list();
        if (stack == NULL)
        {
            LOG_ERROR("не получилось создать стэк%s", "");
            free_map(map);
            return EXIT_FAILURE;
        }
        path_len = get_path_using_stack_list(map, stack, &path);
        free_stack_list(stack);
    }
    else
    {
        stack_array_t *stack = create_stack_array(0);
        if (stack == NULL)
        {
            LOG_ERROR("не получилось создать стэк%s", "");
            free_map(map);
            return EXIT_FAILURE;
        }
        path_len = get_path_using_stack_array(map, stack, &path);
        free_stack_array(stack);
    }

    if (path_len < 0)
    {
        LOG_ERROR("не получилось найти путь%s", "");

        free_map(map);
        return EXIT_FAILURE;
    }

    if (arguments.output_format == FORMAT_COORDS)
    {
        if (arguments.fileout == NULL)
        {
            for (int i = 0; i < path_len; i++)
                printf("%d %d\n", path[i].x, path[i].y);
        }
        else
        {
            FILE *f = fopen(arguments.fileout, "w");

            if (f == NULL)
            {
                LOG_ERROR("не получилось открыть файл для записи%s", "");
                LOG_ERROR("не получилось записать координаты в файл%s", "");

                free(path);
                free_map(map);
                return EXIT_FAILURE;
            }

            for (int i = 0; i < path_len; i++)
                fprintf(f, "%d %d\n", path[i].x, path[i].y);

            fclose(f);
        }
    }
    else
    {
        if (arguments.fileout == NULL)
        {
            print_map_with_path(stdout, map, path, path_len);
        }
        else
        {
            if (print_map_with_path_to_file(arguments.filein, map, path, path_len))
            {
                LOG_ERROR("не получилось записать карту в файл%s", "");

                free(path);
                free_map(map);
                return EXIT_FAILURE;
            }
        }
    }

    free(path);
    free_map(map);

    return EXIT_SUCCESS;
}
