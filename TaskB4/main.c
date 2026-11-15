
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdint.h>

#define DONATION_INTERVAL 90
#define REPORT_DAYS 7

typedef struct
{
    int id;
    char group[4];
    char last_date[11];
} Donor;

typedef struct
{
    Donor *data;
    int size;
    int capacity;
} DonorList;

/* Add a donor and double capacity when full */
int add_donor(DonorList *list, int id,
              const char *group, const char *date)
{
    if (list == NULL || group == NULL || date == NULL ||
        strlen(group) >= sizeof(list->data[0].group))
        return 0;

    if (list->size == list->capacity)
    {
        int new_capacity;

        if (list->capacity == 0)
            new_capacity = 2;
        else
        {
            if (list->capacity > INT32_MAX / 2)
                return 0;

            new_capacity = list->capacity * 2;
        }

        if ((size_t)new_capacity > SIZE_MAX / sizeof(Donor))
            return 0;

        Donor *p = realloc(list->data,
                           (size_t)new_capacity * sizeof(Donor));

        if (p == NULL)
            return 0;

        list->data = p;
        list->capacity = new_capacity;
    }

    list->data[list->size].id = id;

    strcpy(list->data[list->size].group, group);
    strcpy(list->data[list->size].last_date, date);

    list->size++;

    return 1;
}

/* Convert DD/MM/YYYY into time_t */
time_t parse_date(const char *date)
{
    int day, month, year;

    if (sscanf(date, "%2d/%2d/%4d",
               &day, &month, &year) != 3)
        return (time_t)-1;

    struct tm t = {0};

    t.tm_mday = day;
    t.tm_mon = month - 1;
    t.tm_year = year - 1900;
    t.tm_isdst = -1;

    time_t result = mktime(&t);

    if (result == (time_t)-1)
        return (time_t)-1;

    if (t.tm_mday != day ||
        t.tm_mon != month - 1 ||
        t.tm_year != year - 1900)
        return (time_t)-1;

    return result;
}

/* Add days to a date */
time_t add_days(time_t date, int days)
{
    struct tm *temp = localtime(&date);

    if (temp == NULL)
        return (time_t)-1;

    struct tm t = *temp;

    t.tm_mday += days;

    return mktime(&t);
}

/* Print date in DD/MM/YYYY format */
void print_date(time_t date)
{
    struct tm *t = localtime(&date);

    if (t != NULL)
    {
        printf("%02d/%02d/%04d",
               t->tm_mday,
               t->tm_mon + 1,
               t->tm_year + 1900);
    }
}

/* Load donors from a text file */
int load_donors(DonorList *list, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[100];

    if (fp == NULL)
    {
        perror("Donor file");
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int id;
        char group[4];
        char date[11];

        if (sscanf(line, "%d|%3[^|]|%10s",
                   &id, group, date) != 3)
        {
            printf("Invalid donor record found.\n");
            fclose(fp);
            return 0;
        }

        if (parse_date(date) == (time_t)-1)
        {
            printf("Invalid donation date.\n");
            fclose(fp);
            return 0;
        }

        if (!add_donor(list, id, group, date))
        {
            printf("Memory allocation failed.\n");
            fclose(fp);
            return 0;
        }
    }

    if (ferror(fp))
    {
        perror("Reading donor file");
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

/* Generate the next-seven-day eligibility report */
void eligibility_report(const DonorList *list)
{
    time_t today = time(NULL);
    time_t end_date = add_days(today, REPORT_DAYS);

    if (today == (time_t)-1 ||
        end_date == (time_t)-1)
    {
        printf("Date calculation failed.\n");
        return;
    }

    printf("\nDONOR ELIGIBILITY REPORT\n");
    printf("------------------------\n");

    printf("Donor ID   Blood Group   Next Eligible Date\n");

    int count = 0;

    for (int i = 0; i < list->size; i++)
    {
        time_t last = parse_date(list->data[i].last_date);
        time_t next = add_days(last, DONATION_INTERVAL);

        if (next == (time_t)-1)
            continue;

        if (next <= end_date)
        {
            printf("%-10d %-13s ",
                   list->data[i].id,
                   list->data[i].group);

            print_date(next);
            printf("\n");

            count++;
        }
    }

    printf("\nTotal donors eligible now or within 7 days: %d\n",
           count);
}

int main(void)
{
    DonorList list = {NULL, 0, 0};

    clock_t start = clock();

    /* Load donor records */
    if (!load_donors(&list, "donors.txt"))
    {
        free(list.data);
        return 1;
    }

    clock_t loaded = clock();

    /* Generate report */
    eligibility_report(&list);

    clock_t finished = clock();

    /* Memory measurement */
    size_t memory = (size_t)list.capacity * sizeof(Donor);

    printf("\nMEMORY AND PERFORMANCE\n");
    printf("----------------------\n");

    printf("Total donors loaded: %d\n", list.size);
    printf("Current capacity: %d\n", list.capacity);
    printf("Memory allocated: %zu bytes\n", memory);
    printf("Memory allocated: %.2f KB\n",
           memory / 1024.0);

    printf("Loading time: %.6f seconds\n",
           (double)(loaded - start) / CLOCKS_PER_SEC);

    printf("Report time: %.6f seconds\n",
           (double)(finished - loaded) / CLOCKS_PER_SEC);

    /* Release allocated memory */
    free(list.data);
    list.data = NULL;

    printf("\nAll allocated memory released.\n");

    return 0;
}
