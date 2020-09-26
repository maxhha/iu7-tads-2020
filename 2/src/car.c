#include "../inc/car.h"

/*
    Делает fprintf если f не NULL
*/
#define xfprintf(f, ...) if (f) fprintf(f, __VA_ARGS__)

/*
    Работает как fgets, только не добавляет конец строки
*/
char *fgetline(char *dest, int maxlen, FILE *f)
{
    int c;
    char *i = dest;
    char *end = dest + maxlen - 1;

    while(i < end && (c = fgetc(f)) != EOF && c != '\n')
        *i++ = c;

    if (c != EOF && c != '\n')
        return NULL;

    *i = '\0';

    return c == EOF ? NULL : dest;
}

/*
    Считывает из потока информацию о состоянии новой машины
*/
int read_car_state_new(FILE *fin, FILE *fout, car_state_new_t *info)
{
    xfprintf(fout, "Гарантия (в целых годах):\n");
    if (fscanf(fin, "%d", &info->guarantee) != 1 || info->guarantee < 0)
    {
        xfprintf(fout, "Неправильный ввод.\n");
        return EREAD;
    }

    return OK;
}

/*
    Считывает из потока информацию о состоянии старой машины
*/
int read_car_state_old(FILE *fin, FILE *fout, car_state_old_t *new_info)
{
    xfprintf(fout, "Год выпуска:\n");
    if (fscanf(fin, "%d", &new_info->year) != 1 || new_info->year < 0)
        return EREAD;

    xfprintf(fout, "Пробег (в целых км):\n");
    if (fscanf(fin, "%d", &new_info->mileage) != 1 || new_info->mileage < 0)
        return EREAD;

    xfprintf(fout, "Количество ремонтов:\n");
    if (fscanf(fin, "%d", &new_info->repairs_n) != 1 || new_info->repairs_n < 0)
        return EREAD;

    xfprintf(fout, "Количество собственников:\n");
    if (fscanf(fin, "%d", &new_info->owners_n) != 1 || new_info->owners_n < 0)
        return EREAD;

    return OK;
}
/*
    Заполняет поля car из fin. выводит приглашения в fout
*/
int read_car(FILE *fin, FILE *fout, car_t *car)
{
    xfprintf(fout, "Марка:\n");
    if (fgetline(car->brand, CAR_BRAND_LEN, fin) == NULL)
        return EREAD;

    xfprintf(fout, "Страна-производитель:\n");
    if (fgetline(car->country, CAR_COUNTRY_LEN, fin) == NULL)
        return EREAD;

    xfprintf(fout, "Цена:\n");
    if (fscanf(fin, "%d", &car->price) != 1 || car->price < 0)
        return EREAD;

    xfprintf(fout, "Цвет:\n");
    if (fgetline(car->country, CAR_COLOR_LEN, fin) == NULL)
        return EREAD;

    xfprintf(fout, "Состояние (0 - старая, 1 - новая):\n");
    if (fscanf(fin, "%d", (int *) &car->is_new) != 1)
        return EREAD;

    if (car->is_new)
    {
        if (read_car_state_new(fin, fout, &car->state.new_info) != OK)
            return EREAD;
    }
    else
    {
        if (read_car_state_old(fin, fout, &car->state.old_info) != OK)
            return EREAD;
    }

    return OK;
}
