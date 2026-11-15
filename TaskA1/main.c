
#include <stdio.h>

struct Donor
{
    int donorID;
    char name[50];
    char bloodGroup[5];
    int age;
};

struct EligibilitySummary
{
    int eligibleCount;
    int youngestDonorID;
};

/* Method 1: Using pointer parameters */
int findSummaryPointer(
    const struct Donor donors[],
    int n,
    int *youngestID)
{
    int count = 0;
    int youngestAge = 1000;

    *youngestID = -1;

    for (int i = 0; i < n; i++)
    {
        count++;

        if (donors[i].age < youngestAge)
        {
            youngestAge = donors[i].age;
            *youngestID = donors[i].donorID;
        }
    }

    return count;
}

/* Method 2: Returning a structure */
struct EligibilitySummary findSummaryStruct(
    const struct Donor donors[],
    int n)
{
    struct EligibilitySummary result;
    int youngestAge = 1000;

    result.eligibleCount = 0;
    result.youngestDonorID = -1;

    for (int i = 0; i < n; i++)
    {
        result.eligibleCount++;

        if (donors[i].age < youngestAge)
        {
            youngestAge = donors[i].age;
            result.youngestDonorID = donors[i].donorID;
        }
    }

    return result;
}

int main(void)
{
    struct Donor eligible[3] =
    {
        {101, "Anu", "O+", 25},
        {102, "Ravi", "O+", 22},
        {103, "Meena", "O+", 20}
    };

    int youngestID;

    /* Method 1 */
    int count = findSummaryPointer(
        eligible, 3, &youngestID);

    printf("METHOD 1: POINTER PARAMETERS\n");
    printf("Eligible Donors: %d\n", count);
    printf("Youngest Donor ID: %d\n", youngestID);

    /* Method 2 */
    struct EligibilitySummary result =
        findSummaryStruct(eligible, 3);

    printf("\nMETHOD 2: STRUCTURE RETURN\n");
    printf("Eligible Donors: %d\n",
           result.eligibleCount);

    printf("Youngest Donor ID: %d\n",
           result.youngestDonorID);

    return 0;
}
