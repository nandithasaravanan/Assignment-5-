
#include <stdio.h>

long long calls = 0;

/* Digital Root using recursion */
int digital_root(int n)
{
    int sum = 0;

    if (n < 10)
        return n;

    while (n > 0)
    {
        sum = sum + n % 10;
        n = n / 10;
    }

    return digital_root(sum);
}

/* Linear Recursive Power */
long long power_linear(long long base, int exp)
{
    calls++;

    if (exp == 0)
        return 1;

    return base * power_linear(base, exp - 1);
}

/* Fast Recursive Power */
long long power_fast(long long base, int exp)
{
    long long half;

    calls++;

    if (exp == 0)
        return 1;

    half = power_fast(base, exp / 2);

    if (exp % 2 == 0)
        return half * half;

    return base * half * half;
}

/* Iterative Power using Square and Multiply */
long long power_iterative(long long base, int exp)
{
    long long result = 1;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = result * base;

        base = base * base;
        exp = exp / 2;
    }

    return result;
}

int main(void)
{
    int n[] = {10, 100, 1000, 10000};
    int i;

    printf("DIGITAL ROOT TESTS\n");
    printf("Digital root of 0 = %d\n", digital_root(0));
    printf("Digital root of 1 = %d\n", digital_root(1));
    printf("Digital root of 9875 = %d\n", digital_root(9875));

    printf("\nPOWER FUNCTION TESTS\n");

    calls = 0;
    printf("Linear: %lld in %lld calls\n",
           power_linear(2, 30), calls);

    calls = 0;
    long long result = power_linear(2, 30);
    printf("Linear: %lld in %lld calls\n", result, calls);

    calls = 0;
    result = power_fast(2, 30);
    printf("Fast: %lld in %lld calls\n", result, calls);

    printf("\nCALL COUNT COMPARISON\n");
    printf("Exponent\tLinear\tFast\n");

    for (i = 0; i < 4; i++)
    {
        calls = 0;
        power_linear(1, n[i]);
        long long linear_calls = calls;

        calls = 0;
        power_fast(1, n[i]);
        long long fast_calls = calls;

        printf("%d\t\t%lld\t%lld\n",
               n[i], linear_calls, fast_calls);
    }

    printf("\nITERATIVE POWER\n");
    printf("2^30 = %lld\n", power_iterative(2, 30));

    return 0;
}
