#include "../inc/main.h"

/*
# Поиск пути

- [x] загрузка карты из файла
    - [x] стены
    - [x] точки начала и конца
- [ ] поиск пути с использованием выбранной структуры
    - [x] для списка
    - [ ] для массива
    - [ ] с выводом времени выполнения и объема памяти
    - [ ] c выводом адресов памяти
- [x] вывод результата
    - [x] в файл
    - [x] в консоль
    - [x] координаты
*/

static struct argp_option options[] = {
    {
        0,
        'i',
        "FILEIN",
        OPTION_ARG_OPTIONAL,
        "Файл с лабиринтом",
        //     "Карта содержит только следующие символы:\n"
        //     "  '#' - стена\n"
        //     "  ' ' - проход\n"
        //     "  'A' - начало пути\n"
        //     "  'B' - конец\n"
        // "Лабиринт обязательно должен имет на краях прямоугольной карты. \n "
        // "\n "
        // "Пример:\n "
        // "######\n "
        // "#    #\n "
        // "#  # #\n "
        // "#A#B #\n "
        // "######\n "
    },
    {
        0,
        'o',
        "FILEOUT",
        OPTION_ARG_OPTIONAL,
        "Имя файла, в который запишется карта с путем",
    },
    {
        0,
        'm',
        0,
        OPTION_ARG_OPTIONAL,
        "Записать в файл путь на карте",
    },
    { 0 }
};

struct arguments {
    char *filein;
    char *fileout;
    enum { FORMAT_COORDS, FORMAT_MAP } output_format;
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = (struct arguments *) state->input;

    switch (key)
    {
        case 'i':

        arguments->filein = arg;

        return 0;
        case 'o':

        arguments->fileout = arg;

        return 0;
        case 'm':

        arguments->output_format = FORMAT_MAP;

        return 0;
        default: return ARGP_ERR_UNKNOWN;
    }
}

static struct argp argp = { options, parse_opt, 0, 0};

int main(int argc, char **argv)
{
    struct arguments arguments = { 0, 0, FORMAT_COORDS };

    if (argp_parse(&argp, argc, argv, 0, 0, &arguments))
    {
        LOG_ERROR("Error on argp_parse%s", "");
        return EXIT_FAILURE;
    }

    arguments.filein = "sample.txt";
    arguments.fileout = NULL;
    arguments.output_format = FORMAT_MAP;

    map_t *map = read_map_from_file(arguments.filein);

    if (map == NULL)
    {
        LOG_ERROR("не получилось прочитать карту из файла '%s'", arguments.filein);
        return EXIT_FAILURE;
    }

    point_t *path;

    int path_len = get_path(map, &path);

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
