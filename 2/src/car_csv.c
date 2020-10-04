#include "../inc/car_csv.h"

int write_cars_to_csv(const char *filename, const car_t *car_table, size_t car_table_size)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf(RED "Невозможно сохранить в этот файл\n" RESET);
        return EWRITE;
    }

    const car_t *end = car_table + car_table_size;

    for (const car_t *car = car_table; car < end; car++)
    {
        fprintf(f, "%s;", car->brand);
        fprintf(f, "%s;", car->country);
        fprintf(f, "%ld;", car->price);
        fprintf(f, "%s;", car->color);
        fprintf(f, "%s;", car->is_new ? "1" : "0");

        if (car->is_new)
        {
            fprintf(f, "%d;", car->state.new_info.guarantee);
            fprintf(f, "-;-;-;-;");
        }
        else
        {
            fprintf(f, "-;");
            fprintf(f, "%d;", car->state.old_info.year);
            fprintf(f, "%d;", car->state.old_info.mileage);
            fprintf(f, "%d;", car->state.old_info.repairs_n);
            fprintf(f, "%d;", car->state.old_info.owners_n);
        }

        fprintf(f, "\n");
    }

    fclose(f);

    return OK;
}

#define RETURN_ERROR_IF_IS_END() \
    do { \
        if (c == EOF || c == '\n') \
        { \
            fclose(f); \
            printf(RED "Недостаточно полей\n" RESET); \
            printf(RED "%d строкa\n" RESET, (int) (car - car_table) + 1); \
            return EREAD; \
        } \
    } while (0)

#define READ_STR_FIELD(field, maxlen, fieldname) \
    do { \
        int i = 0; \
        while(c != ';' && c != EOF && c != '\n' && i < maxlen) \
        { \
            field[i] = c; \
            c = fgetc(f); \
            i++; \
        } \
        field[i] = '\0'; \
        if (c != ';' && c != EOF && c != '\n' && i == maxlen) \
        { \
            fclose(f); \
            printf(RED "Превышение максимально допустимого размера поля\n" RESET); \
            printf(RED "%d строка, поле - " fieldname "\n" RESET, (int) (car - car_table) + 1); \
            return EREAD; \
        } \
    } while (0)

#define READ_UINT_FIELD(field, fieldname) \
    do { \
        field = 0; \
        do { \
            if (strchr("1234567890", c) == NULL) \
            { \
                fclose(f); \
                printf(RED "Неправильное число\n" RESET); \
                printf(RED "%d строка, поле - " fieldname "\n" RESET, (int) (car - car_table) + 1); \
                return EREAD; \
            } \
            field = field * 10 + c - '0'; \
            c = fgetc(f); \
        } while(c != EOF && c != ';' && c != '\n'); \
    } while(0)

int read_cars_from_csv(const char *filename, car_t *car_table, size_t *car_table_size)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        printf(RED "Файл не найден\n" RESET);
        return EREAD;
    }

    *car_table_size = 0;
    car_t *car = car_table;
    int c = 0;

    for(;;)
    {
        c = fgetc(f);
        if (c == EOF)
            break;

        if (*car_table_size == MAX_TABLE_SIZE)
        {
            fclose(f);
            printf(RED "Превышение максимально допустимого размера таблицы\n" RESET);
            return EREAD;
        }

        // read brand
        READ_STR_FIELD(car->brand, CAR_BRAND_LEN, "марка");
        RETURN_ERROR_IF_IS_END();

        // read country
        c = fgetc(f);
        RETURN_ERROR_IF_IS_END();

        READ_STR_FIELD(car->country, CAR_COUNTRY_LEN, "страна");
        RETURN_ERROR_IF_IS_END();

        // read price
        c = fgetc(f);
        RETURN_ERROR_IF_IS_END();

        READ_UINT_FIELD(car->price, "цена");
        RETURN_ERROR_IF_IS_END();

        // read color
        c = fgetc(f);
        RETURN_ERROR_IF_IS_END();

        READ_STR_FIELD(car->color, CAR_COLOR_LEN, "цвет");
        RETURN_ERROR_IF_IS_END();

        // read state
        c = fgetc(f);
        RETURN_ERROR_IF_IS_END();

        car->is_new = c - '0';

        c = fgetc(f);
        RETURN_ERROR_IF_IS_END();

        c = fgetc(f);
        RETURN_ERROR_IF_IS_END();

        if (car->is_new)
        {
            READ_UINT_FIELD(car->state.new_info.guarantee, "гарантия");
        }
        else
        {
            while (c != ';' && c != '\n' && c != EOF)
                c = fgetc(f);
            RETURN_ERROR_IF_IS_END();

            c = fgetc(f);
            RETURN_ERROR_IF_IS_END();

            READ_UINT_FIELD(car->state.old_info.year, "год");
            RETURN_ERROR_IF_IS_END();

            c = fgetc(f);
            RETURN_ERROR_IF_IS_END();

            READ_UINT_FIELD(car->state.old_info.mileage, "пробег");
            RETURN_ERROR_IF_IS_END();

            c = fgetc(f);
            RETURN_ERROR_IF_IS_END();

            READ_UINT_FIELD(car->state.old_info.repairs_n, "количество ремонтов");
            RETURN_ERROR_IF_IS_END();

            c = fgetc(f);
            RETURN_ERROR_IF_IS_END();

            READ_UINT_FIELD(car->state.old_info.owners_n, "количество собственников");
        }

        if (c != '\n')
            fwait_new_line(f);
        car++;
        *car_table_size += 1;
    }

    fclose(f);

    return OK;
}
