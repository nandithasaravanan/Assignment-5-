
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 2

struct donor
{
    int id;
    char name[40];
};

struct donor *list = NULL;
int count = 0;
int capacity = 0;

/* Add a donor to the dynamic array */
int add_donor(int id, const char name[])
{
    struct donor *temp;

    /* Check whether the array is full */
    if (count == capacity)
    {
        int new_capacity;

        if (capacity == 0)
            new_capacity = INITIAL_CAPACITY;
        else
            new_capacity = capacity * 2;

        temp = realloc(list,
                       new_capacity * sizeof(struct donor));

        /* Handle allocation failure */
        if (temp == NULL)
        {
            printf("Memory allocation failed!\n");
            return 0;
        }

        list = temp;
        capacity = new_capacity;

        printf("New capacity: %d\n", capacity);
    }

    /* Insert donor details */
    list[count].id = id;

    snprintf(list[count].name,
             sizeof(list[count].name), "%s", name);

    count++;

    return 1;
}

/* Display donor records */
void display_donors(void)
{
    printf("\nDonor Records\n");

    for (int i = 0; i < count; i++)
    {
        printf("ID: %d  Name: %s\n",
               list[i].id, list[i].name);
    }

    printf("\nTotal Donors: %d\n", count);
    printf("Current Capacity: %d\n", capacity);
}

int main(void)
{
    /* Add donor records */
    if (!add_donor(101, "Arun"))
        goto cleanup;

    if (!add_donor(102, "Priya"))
        goto cleanup;

    if (!add_donor(103, "Rahul"))
        goto cleanup;

    if (!add_donor(104, "Divya"))
        goto cleanup;

    if (!add_donor(105, "Karthik"))
        goto cleanup;

    /* Display records */
    display_donors();

cleanup:

    /* Release allocated memory */
    free(list);
    list = NULL;

    count = 0;
    capacity = 0;

    printf("\nMemory released successfully.\n");

    return 0;
}
