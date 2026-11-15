
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 8

char *groups[SIZE] = {
    "O+", "O-", "A+", "A-",
    "B+", "B-", "AB+", "AB-"
};

/* Find the lowest stock and return its address */
int *find_lowest_stock(int stock[], int n)
{
    int min_index = 0;

    for (int i = 1; i < n; i++)
    {
        if (stock[i] < stock[min_index])
        {
            min_index = i;
        }
    }

    return &stock[min_index];
}

/* Find the index of a blood group */
int find_group(char *name)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (strcmp(groups[i], name) == 0)
        {
            return i;
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    int stock[SIZE] = {25, 40, 15, 30, 50, 20, 35, 10};

    /* Pointer operations */
    int *p = stock;

    printf("Pointer Operations\n");
    printf("p + 3 value: %d\n", *(p + 3));
    printf("p[3] value: %d\n", p[3]);
    printf("*p++ value: %d\n", *p++);

    /* Find lowest stock */
    int *lowest = find_lowest_stock(stock, SIZE);

    printf("\nLowest Stock: %d\n", *lowest);

    /* Command-line validation */
    if (argc < 3)
    {
        printf("\nUsage:\n");
        printf("bloodbank find O+\n");
        printf("bloodbank issue A- 2\n");
        return 1;
    }

    int index = find_group(argv[2]);

    if (index == -1)
    {
        printf("Invalid blood group!\n");
        return 1;
    }

    /* Find command */
    if (strcmp(argv[1], "find") == 0 && argc == 3)
    {
        printf("Blood Group: %s\n", groups[index]);
        printf("Available Stock: %d\n", stock[index]);
    }

    /* Issue command */
    else if (strcmp(argv[1], "issue") == 0 && argc == 4)
    {
        char *end;
        long quantity = strtol(argv[3], &end, 10);

        if (*argv[3] == '\0' || *end != '\0' ||
            quantity <= 0 || quantity > 2147483647)
        {
            printf("Invalid quantity!\n");
            return 1;
        }

        if (quantity > stock[index])
        {
            printf("Insufficient stock!\n");
            return 1;
        }

        stock[index] -= (int)quantity;

        printf("Blood issued successfully!\n");
        printf("Blood Group: %s\n", groups[index]);
        printf("Remaining Stock: %d\n", stock[index]);
    }

    else
    {
        printf("Invalid command or arguments!\n");
        printf("Usage: bloodbank find O+\n");
        printf("       bloodbank issue A- 2\n");
        return 1;
    }

    return 0;
}
