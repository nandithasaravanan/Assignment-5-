
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define NAME_SIZE 20
#define GROUP_SIZE 4
#define LINE_SIZE 150
#define GROUP_COUNT 8

const char *groups[GROUP_COUNT] =
{
    "O+", "O-", "A+", "A-",
    "B+", "B-", "AB+", "AB-"
};

/* Find blood group index */
int group_index(const char *group)
{
    int i;

    for (i = 0; i < GROUP_COUNT; i++)
    {
        if (strcmp(group, groups[i]) == 0)
            return i;
    }

    return -1;
}

/* Read donor name safely */
void read_name(void)
{
    char name[NAME_SIZE];
    int ch;
    size_t len;

    printf("Enter donor name: ");

    if (fgets(name, sizeof(name), stdin) == NULL)
    {
        printf("Input error.\n");
        return;
    }

    len = strlen(name);

    if (len > 0 && name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
    }
    else if (!feof(stdin))
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            /* Clear remaining input */
        }

        printf("Name too long. Maximum 19 characters.\n");
        return;
    }

    printf("Donor name: %s\n", name);
}

/* Create binary stock file */
int initialize_stock(void)
{
    int stock[GROUP_COUNT] =
    {
        25, 10, 15, 12, 20, 8, 7, 5
    };

    FILE *fp = fopen("stock.dat", "wb");

    if (fp == NULL)
    {
        perror("stock.dat");
        return 1;
    }

    if (fwrite(stock, sizeof(int), GROUP_COUNT, fp)
        != GROUP_COUNT)
    {
        perror("Writing stock.dat");
        fclose(fp);
        return 1;
    }

    fclose(fp);

    printf("Stock file created successfully.\n");
    return 0;
}

/* Find donors by blood group */
int find_donors(const char *group)
{
    FILE *fp;
    char line[LINE_SIZE];
    int id, age, found = 0;
    char name[NAME_SIZE];
    char blood[GROUP_SIZE];
    char phone[15];
    char date[11];

    fp = fopen("donors.txt", "r");

    if (fp == NULL)
    {
        perror("donors.txt");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%d|%19[^|]|%3[^|]|%d|%14[^|]|%10s",
                   &id, name, blood, &age, phone, date) == 6)
        {
            if (strcmp(blood, group) == 0)
            {
                printf("%d %s %s %d %s %s\n",
                       id, name, blood, age, phone, date);
                found = 1;
            }
        }
    }

    if (!found)
        printf("No matching donors found.\n");

    if (ferror(fp))
    {
        perror("Reading donors.txt");
        fclose(fp);
        return 1;
    }

    fclose(fp);
    return 0;
}

/* Update stock using fseek, fread and fwrite */
int issue_stock(const char *group, int quantity)
{
    FILE *fp;
    int index, current;

    index = group_index(group);

    if (index == -1)
    {
        printf("Invalid blood group.\n");
        return 1;
    }

    if (quantity <= 0)
    {
        printf("Quantity must be positive.\n");
        return 1;
    }

    fp = fopen("stock.dat", "rb+");

    if (fp == NULL)
    {
        perror("stock.dat");
        return 1;
    }

    if (fseek(fp, (long)index * sizeof(int), SEEK_SET) != 0)
    {
        perror("fseek");
        fclose(fp);
        return 1;
    }

    if (fread(&current, sizeof(int), 1, fp) != 1)
    {
        if (ferror(fp))
            perror("Reading stock.dat");
        else
            printf("Invalid or incomplete stock file.\n");

        fclose(fp);
        return 1;
    }

    if (current < quantity)
    {
        printf("Insufficient stock.\n");
        fclose(fp);
        return 1;
    }

    current = current - quantity;

    if (fseek(fp, (long)index * sizeof(int), SEEK_SET) != 0)
    {
        perror("fseek");
        fclose(fp);
        return 1;
    }

    if (fwrite(&current, sizeof(int), 1, fp) != 1)
    {
        perror("Writing stock.dat");
        fclose(fp);
        return 1;
    }

    if (fclose(fp) != 0)
    {
        perror("Closing stock.dat");
        return 1;
    }

    printf("Stock updated successfully.\n");
    printf("%s remaining units: %d\n", group, current);

    return 0;
}

/* Convert command-line quantity safely */
int read_quantity(const char *text, int *quantity)
{
    char *end;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);

    if (text == end || *end != '\0' ||
        errno == ERANGE || value <= 0 || value > INT_MAX)
    {
        return 0;
    }

    *quantity = (int)value;
    return 1;
}

/* Display usage instructions */
void usage(void)
{
    printf("Blood Bank Commands:\n");
    printf("./main init\n");
    printf("./main name-test\n");
    printf("./main find O+\n");
    printf("./main issue A- 2\n");
}

int main(int argc, char *argv[])
{
    int quantity;

    if (argc == 2 && strcmp(argv[1], "init") == 0)
        return initialize_stock();

    if (argc == 2 && strcmp(argv[1], "name-test") == 0)
    {
        read_name();
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "find") == 0)
    {
        if (group_index(argv[2]) == -1)
        {
            printf("Invalid blood group.\n");
            return 1;
        }

        return find_donors(argv[2]);
    }

    if (argc == 4 && strcmp(argv[1], "issue") == 0)
    {
        if (group_index(argv[2]) == -1)
        {
            printf("Invalid blood group.\n");
            return 1;
        }

        if (!read_quantity(argv[3], &quantity))
        {
            printf("Invalid quantity.\n");
            return 1;
        }

        return issue_stock(argv[2], quantity);
    }

    usage();
    return 1;
}
