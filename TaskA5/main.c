
#include <stdio.h>
#include <string.h>

#define SIZE 4

struct donor
{
    int id;
    char name[30];
    char blood_group[4];
    int age;
    char phone[15];
    char date[15];
};

struct stock
{
    char blood_group[4];
    int units;
};

/* Create donor text file */
void create_donor_file(void)
{
    FILE *fp = fopen("donors.txt", "w");

    if (fp == NULL)
    {
        printf("Cannot create donor file!\n");
        return;
    }

    struct donor d[3] = {
        {101, "Arun", "O+", 21, "9876543210", "10/10/2026"},
        {102, "Priya", "A+", 25, "9876543211", "12/10/2026"},
        {103, "Rahul", "B+", 23, "9876543212", "13/10/2026"}
    };

    for (int i = 0; i < 3; i++)
    {
        fprintf(fp, "%d %s %s %d %s %s\n",
                d[i].id, d[i].name,
                d[i].blood_group, d[i].age,
                d[i].phone, d[i].date);
    }

    fclose(fp);

    printf("Donor text file created successfully.\n");
}

/* Create binary stock file */
int create_stock_file(void)
{
    FILE *fp = fopen("stock.dat", "wb");

    if (fp == NULL)
    {
        printf("Cannot create stock file!\n");
        return 0;
    }

    struct stock s[SIZE] = {
        {"O+", 25},
        {"O-", 10},
        {"A+", 15},
        {"B+", 30}
    };

    if (fwrite(s, sizeof(struct stock), SIZE, fp) != SIZE)
    {
        printf("Stock file writing failed!\n");
        fclose(fp);
        return 0;
    }

    fclose(fp);

    printf("Binary stock file created successfully.\n");

    return 1;
}

/* Update one stock record in place */
void update_stock(const char group[], int change)
{
    FILE *fp = fopen("stock.dat", "rb+");

    struct stock s;
    int found = 0;

    if (fp == NULL)
    {
        printf("Cannot open stock file!\n");
        return;
    }

    for (int i = 0; i < SIZE; i++)
    {
        long position = (long)i * sizeof(struct stock);

        if (fseek(fp, position, SEEK_SET) != 0)
        {
            printf("File positioning failed!\n");
            fclose(fp);
            return;
        }

        if (fread(&s, sizeof(struct stock), 1, fp) != 1)
        {
            printf("File reading failed!\n");
            fclose(fp);
            return;
        }

        if (strcmp(s.blood_group, group) == 0)
        {
            if (s.units + change < 0)
            {
                printf("Insufficient stock!\n");
                fclose(fp);
                return;
            }

            s.units = s.units + change;

            if (fseek(fp, position, SEEK_SET) != 0)
            {
                printf("File positioning failed!\n");
                fclose(fp);
                return;
            }

            if (fwrite(&s, sizeof(struct stock), 1, fp) != 1)
            {
                printf("File writing failed!\n");
                fclose(fp);
                return;
            }

            printf("Stock updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Blood group not found!\n");
    }

    fclose(fp);
}

/* Display binary stock records */
void display_stock(void)
{
    FILE *fp = fopen("stock.dat", "rb");

    struct stock s;

    if (fp == NULL)
    {
        printf("Cannot open stock file!\n");
        return;
    }

    printf("\nBlood Stock Records\n");

    while (fread(&s, sizeof(struct stock), 1, fp) == 1)
    {
        printf("%s : %d units\n",
               s.blood_group, s.units);
    }

    fclose(fp);
}

int main(void)
{
    create_donor_file();

    if (!create_stock_file())
        return 1;

    display_stock();

    printf("\nUpdating O+ stock by 5 units...\n");

    update_stock("O+", 5);

    display_stock();

    return 0;
}
