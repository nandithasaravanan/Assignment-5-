
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int id;
    char name[40];
    char blood[4];
    int age;
    char phone[16];
    char date[11];
} Donor;

/* Generate sample donor records */
void generate_donor(Donor *d, int id)
{
    char *groups[] = {
        "O+", "A+", "B+", "AB+",
        "O-", "A-", "B-", "AB-"
    };

    d->id = id;
    snprintf(d->name, sizeof(d->name),
             "Donor%06d", id);

    strcpy(d->blood, groups[id % 8]);

    d->age = 18 + id % 45;

    snprintf(d->phone, sizeof(d->phone),
             "9%09d", id);

    strcpy(d->date, "15/11/2026");
}

/* Compare two donor records */
int compare_donor(Donor a, Donor b)
{
    return a.id == b.id &&
           strcmp(a.name, b.name) == 0 &&
           strcmp(a.blood, b.blood) == 0 &&
           a.age == b.age &&
           strcmp(a.phone, b.phone) == 0 &&
           strcmp(a.date, b.date) == 0;
}

/* Write records into a text file */
int write_text(const char *filename,
               Donor data[], int n)
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
    {
        perror("Text file");
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d|%s|%s|%d|%s|%s\n",
                data[i].id,
                data[i].name,
                data[i].blood,
                data[i].age,
                data[i].phone,
                data[i].date);
    }

    fclose(fp);
    return 1;
}

/* Read records from a text file */
int read_text(const char *filename,
              Donor data[], int max)
{
    FILE *fp = fopen(filename, "r");
    char line[150];
    int count = 0;

    if (fp == NULL)
    {
        perror("Text read");
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count >= max ||
            sscanf(line, "%d|%39[^|]|%3[^|]|%d|%15[^|]|%10s",
                   &data[count].id,
                   data[count].name,
                   data[count].blood,
                   &data[count].age,
                   data[count].phone,
                   data[count].date) != 6)
        {
            fclose(fp);
            return -1;
        }

        count++;
    }

    fclose(fp);
    return count;
}

/* Write all records into a binary file */
int write_binary(const char *filename,
                 Donor data[], int n)
{
    FILE *fp = fopen(filename, "wb");

    if (fp == NULL)
    {
        perror("Binary file");
        return 0;
    }

    size_t written = fwrite(data, sizeof(Donor), n, fp);

    fclose(fp);

    return written == (size_t)n;
}

/* Read records from a binary file */
int read_binary(const char *filename,
                Donor data[], int max)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        perror("Binary read");
        return -1;
    }

    int count = (int)fread(data, sizeof(Donor), max, fp);

    fclose(fp);
    return count;
}

/* Find file size */
long file_size(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
        return -1;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    fclose(fp);
    return size;
}

/* Update one binary record directly */
int update_binary(const char *filename,
                  int index)
{
    FILE *fp = fopen(filename, "rb+");
    Donor d;

    if (fp == NULL)
    {
        perror("Binary update");
        return 0;
    }

    if (fseek(fp, (long)index * sizeof(Donor),
              SEEK_SET) != 0)
    {
        fclose(fp);
        return 0;
    }

    if (fread(&d, sizeof(Donor), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    strcpy(d.name, "UpdatedDonor");

    if (fseek(fp, -(long)sizeof(Donor), SEEK_CUR) != 0)
    {
        fclose(fp);
        return 0;
    }

    if (fwrite(&d, sizeof(Donor), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

/* Update text file by rewriting all records */
int update_text(const char *filename, int index)
{
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp_text.txt", "w");

    char line[150];
    Donor d;
    int count = 0;
    int success = 1;

    if (fp == NULL || temp == NULL)
    {
        perror("Text update");
        if (fp != NULL)
            fclose(fp);
        if (temp != NULL)
            fclose(temp);
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%d|%39[^|]|%3[^|]|%d|%15[^|]|%10s",
                   &d.id, d.name, d.blood,
                   &d.age, d.phone, d.date) != 6)
        {
            success = 0;
            break;
        }

        if (count == index)
            strcpy(d.name, "UpdatedDonor");

        if (fprintf(temp, "%d|%s|%s|%d|%s|%s\n",
                    d.id, d.name, d.blood,
                    d.age, d.phone, d.date) < 0)
        {
            success = 0;
            break;
        }

        count++;
    }

    if (ferror(fp) || fclose(fp) != 0)
        success = 0;

    if (fclose(temp) != 0)
        success = 0;

    if (!success || count <= index)
    {
        remove("temp_text.txt");
        return 0;
    }

    if (remove(filename) != 0)
    {
        remove("temp_text.txt");
        return 0;
    }

    if (rename("temp_text.txt", filename) != 0)
    {
        perror("Rename");
        return 0;
    }

    return 1;
}

/* Verify all records */
int verify(Donor original[], Donor result[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (!compare_donor(original[i], result[i]))
            return 0;
    }

    return 1;
}

int main(void)
{
    /* B3.1: Verify 10 records */

    Donor small[10], text_read[10], binary_read[10];

    for (int i = 0; i < 10; i++)
        generate_donor(&small[i], i + 1);

    write_text("small.txt", small, 10);
    write_binary("small.dat", small, 10);

    int tc = read_text("small.txt", text_read, 10);
    int bc = read_binary("small.dat", binary_read, 10);

    printf("B3.1 - Verification\n");

    printf("Text file: %s\n",
           tc == 10 && verify(small, text_read, 10)
           ? "PASS" : "FAIL");

    printf("Binary file: %s\n",
           bc == 10 && verify(small, binary_read, 10)
           ? "PASS" : "FAIL");

    /* B3.2: Performance measurement */

    int sizes[] = {1000, 10000, 100000};

    printf("\nB3.2 - Performance Comparison\n");

    printf("Records\tFormat\tFile Size\tWrite(s)\tRead(s)\n");

    for (int k = 0; k < 3; k++)
    {
        int n = sizes[k];

        Donor *data = malloc(n * sizeof(Donor));
        Donor *result = malloc(n * sizeof(Donor));

        if (data == NULL || result == NULL)
        {
            printf("Memory allocation failed\n");
            free(data);
            free(result);
            return 1;
        }

        for (int i = 0; i < n; i++)
            generate_donor(&data[i], i + 1);

        char textfile[40], binfile[40];

        snprintf(textfile, sizeof(textfile),
                 "text_%d.txt", n);

        snprintf(binfile, sizeof(binfile),
                 "binary_%d.dat", n);

        clock_t start, end;
        double wt, rt;

        /* Text file writing */

        start = clock();
        int ok1 = write_text(textfile, data, n);
        end = clock();

        wt = (double)(end - start) / CLOCKS_PER_SEC;

        /* Text file reading */

        start = clock();
        int nr = read_text(textfile, result, n);
        end = clock();

        rt = (double)(end - start) / CLOCKS_PER_SEC;

        if (!ok1 || nr != n || !verify(data, result, n))
        {
            printf("Text verification failed\n");
            free(data);
            free(result);
            return 1;
        }

        printf("%d\tText\t%ld\t%.6f\t%.6f\n",
               n, file_size(textfile), wt, rt);

        /* Binary file writing */

        start = clock();
        int ok2 = write_binary(binfile, data, n);
        end = clock();

        wt = (double)(end - start) / CLOCKS_PER_SEC;

        /* Binary file reading */

        start = clock();
        nr = read_binary(binfile, result, n);
        end = clock();

        rt = (double)(end - start) / CLOCKS_PER_SEC;

        if (!ok2 || nr != n || !verify(data, result, n))
        {
            printf("Binary verification failed\n");
            free(data);
            free(result);
            return 1;
        }

        printf("%d\tBinary\t%ld\t%.6f\t%.6f\n",
               n, file_size(binfile), wt, rt);

        free(data);
        free(result);
    }

    /* B3.3: Update the 50,000th record */

    printf("\nB3.3 - Update 50000th Record\n");

    clock_t start, end;
    double bt, tt;

    start = clock();
    int b = update_binary("binary_100000.dat", 49999);
    end = clock();

    bt = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    int t = update_text("text_100000.txt", 49999);
    end = clock();

    tt = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Binary update: %s, %.6f seconds\n",
           b ? "SUCCESS" : "FAILED", bt);

    printf("Text update: %s, %.6f seconds\n",
           t ? "SUCCESS" : "FAILED", tt);

    printf("\nTask B3 completed.\n");

    return 0;
}
