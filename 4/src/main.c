#include "../inc/main.h"

/*
# Поиск пути

- [ ] загрузка карты из файла
    - [ ] стены
    - [ ] точки начала и конца
- [ ] поиск пути с использованием выбранной структуры
    - [ ] для стэка и массива
    - [ ] с выводом времени выполнения и объема памяти
    - [ ] c выводом адресов памяти
- [ ] вывод результата
    - [ ] в файл
    - [ ] в консоль
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
    { 0 }
};

struct arguments {
    char *filein;
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = (struct arguments *) state->input;

    switch (key)
    {
        case 'i':

        arguments->filein = arg;

        return 0;
        default: return ARGP_ERR_UNKNOWN;
    }
}

static struct argp argp = { options, parse_opt, 0, 0};

int main(int argc, char **argv)
{
    struct arguments arguments = { 0 };

    if (argp_parse(&argp, argc, argv, 0, 0, &arguments))
    {
        LOG_ERROR("Error on argp_parse%s", "");
        return EXIT_FAILURE;
    }

    arguments.filein = "sample.txt";

    map_t *map = read_map_from_file(arguments.filein);

    if (map == NULL)
    {
        LOG_ERROR("не получилось прочитать карту из файла '%s'", arguments.filein);
        return EXIT_FAILURE;
    }

    free_map(map);

    return EXIT_SUCCESS;
}
