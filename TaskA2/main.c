
#include <stdio.h>

/* Recursively calculate sum of digits */
int sum_digits(int n)
{
    if (n == 0)
        return 0;

    return (n % 10) + sum_digits(n / 10);
}

/* Calculate digital root */
int digital_root(int n)
{
    if (n < 10)
        return n;

    return digital_root(sum_digits(n));
}

/* Linear recursive power */
int power_linear(int base, int n)
{
    if (n == 0)
        return 1;

    return base * power_linear(base, n - 1);
}

/* Halving recursive power */
int power_fast(int base, int n)
{
    int half;

    if (n == 0)
        return 1;

    half = power_fast(base, n / 2);

    if (n % 2 == 0)
        return half * half;

    return base * half * half;
}

int main(void)
{
    int donorID = 9875;
    int base = 2;
    int exponent = 5;

    printf("Donor ID: %d\n", donorID);

    printf("Digital Root: %d\n",
           digital_root(donorID));

    printf("\nBase: %d\n", base);
    printf("Exponent: %d\n", exponent);

    printf("Linear Power: %d\n",
           power_linear(base, exponent));

    printf("Halving Power: %d\n",
           power_fast(base, exponent));

    return 0;
}
