# TASK B1 – IMPLEMENT AND INSTRUMENT THE RECURSIVE FUNCTIONS

## B1.1 – Validate the Implementation

### Aim
To implement and test the digital root function, linear recursive power function and fast recursive power function.

### 1. Digital Root

The digital root is calculated by repeatedly adding the digits of a number until a single digit remains.

**Algorithm:**
1. If the number is less than 10, return the number.
2. Otherwise, calculate the sum of its digits.
3. Call the function recursively with the digit sum.
4. Repeat until one digit remains.

**Test cases:**

| Input | Digital Root |
|---|---:|
| 0 | 0 |
| 1 | 1 |
| 9875 | 2 |

### 2. Linear Recursive Power

Formula:

power(base, exp) = base × power(base, exp - 1)

Base case: If exp = 0, return 1.

Recursive case: Multiply the base by the result of power(base, exp - 1).

### 3. Fast Recursive Power

Formula:

power(base, exp) = (power(base, exp/2))²

If the exponent is odd, multiply the result by the base.

Base case: If exp = 0, return 1.

Recursive case: Calculate power(base, exp/2) once and square the result.

### Test cases

| Base | Exponent | Expected Result |
|---|---:|---:|
| 2 | 0 | 1 |
| 2 | 1 | 2 |
| 2 | 30 | 1073741824 |

These tests verify the base case, smallest positive exponent and ordinary recursive calculation.

## B1.2 – Compare with Your Design

The implemented C program follows the pseudocode developed in Part A.

- The base case stops recursion when the exponent becomes zero.
- Linear recursion decreases the exponent by one during each call.
- Fast recursion divides the exponent by two during each call.
- Both functions return the correct power for valid non-negative exponents.

No major design changes were required. The implementation adds a global call counter to measure recursive function calls.

## B1.3 – Count the Calls

The recursive calls are measured for exponents 10, 100, 1,000 and 10,000.

| Exponent (n) | Linear Calls (n + 1) | Fast Calls |
|---:|---:|---:|
| 10 | 11 | 5 |
| 100 | 101 | 8 |
| 1,000 | 1,001 | 11 |
| 10,000 | 10,001 | 15 |

**Observation:**

- Linear recursion makes n + 1 calls because it decreases the exponent by one.
- Fast recursion makes approximately log₂(n) + 2 calls for positive n.
- Fast recursion requires significantly fewer function calls for large exponents.

### Recurrence Relations

Linear recursion:

T(n) = T(n - 1) + 1

Time complexity: O(n)

Fast recursion:

T(n) = T(n/2) + 1

Time complexity: O(log n)

## B1.4 – Find the Limit

Linear recursion creates a new stack frame for every recursive call.

When the exponent becomes sufficiently large, the program may exceed the available stack memory, resulting in a stack overflow and program crash.

An iterative version uses a loop instead of recursive calls.

**Comparison:**

| Method | Time Complexity | Stack Space |
|---|---|---|
| Linear recursion | O(n) | O(n) |
| Fast recursion | O(log n) | O(log n) |
| Iterative power | O(log n) | O(1) |

The exact recursion limit depends on the compiler, operating system, stack size and execution environment. Therefore, the largest successful exponent and first failing exponent must be measured on the system used for execution.

### Conclusion

Fast recursive power is more efficient than linear recursion because it reduces the exponent by half in every recursive call. Iterative square-and-multiply provides the same logarithmic time complexity without using additional recursive stack space.
