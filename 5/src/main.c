#include "../inc/main.h"

/*
- [x] установить тип очереди (массив/стек)
- [ ] установить время
- [ ] замерить ли время работы (и сколько раз мерять)
- [ ] выводить ли адреса
- [x] запустить
*/

#define PROMPT_ACTION(act, desc) \
    printf("  %-*s" YEL " - " desc RESET "\n", MENU_ACTION_LEN, act)

#define PROMPT_ACTIONF(act, desc, ...) \
    printf("  %-*s" YEL " - "  desc RESET "\n", MENU_ACTION_LEN, act, __VA_ARGS__)

#define ACT_IF_EQUALS(action, val, callback, ...) \
    if (strcmp(action, val) == 0) \
    { \
        callback(__VA_ARGS__); \
        continue; \
    } \

void print_prompt(action_params_t params)
{
    printf("\n");
    printf("**************************************************\n");
    printf("*                                                *\n");
    printf("*             ЛР5 Обработка очередей             *\n");
    printf("*                                                *\n");
    printf("**************************************************\n");
    printf("\n");
    print_action_params(params);
    printf("\n");
    printf("Действия:\n");
    PROMPT_ACTION(MENU_ACTION_TYPE, "изменить тип очереди");
    PROMPT_ACTION(MENU_ACTION_ADDRESS, "изменить тип очереди");
    PROMPT_ACTION(MENU_ACTION_PROCESS, "запустить моделирование");
    PROMPT_ACTION(MENU_ACTION_QUIT, "выйти");
}

int main(void)
{
    srand(time(NULL));

    char *action = NULL;
    size_t action_size = 0;

    action_params_t params = init_action_params();

    while (!action || strcmp(action, MENU_ACTION_QUIT))
    {
        print_prompt(params);

        int readed = getline(&action, &action_size, stdin);

        if (readed < 0)
        {
            LOG_ERROR("readed < 0%s", "");

            if (action)
                free(action);

            return EXIT_FAILURE;
        }

        action[readed - 1] = '\0';

        LOG_DEBUG("action = \"%s\"", action);

        ACT_IF_EQUALS(action, MENU_ACTION_TYPE, action_change_type, &params)
        ACT_IF_EQUALS(action, MENU_ACTION_ADDRESS, action_set_address, &params)
        ACT_IF_EQUALS(action, MENU_ACTION_PROCESS, action_process, params)
        if (strcmp(action, MENU_ACTION_QUIT) == 0)
            continue;

        printf(RED "Неизвестное действие\n" RESET);
    }

    if (action)
        free(action);

    return EXIT_SUCCESS;
}
