A2.1 – Recursive Design (2 Marks)
The blood bank uses recursion for two operations:
1. Finding the digital root of a donor ID.
2. Calculating the power of a number using recursion.
A. Digital Root
A digital root is obtained by repeatedly adding the digits of a number until only one digit remains.

Example:
Input = 9875

9 + 8 + 7 + 5 = 29
2 + 9 = 11
1 + 1 = 2

Digital Root = 2


Recursive pseudocode:
FUNCTION SUM_DIGITS(n)

    IF n == 0
        RETURN 0
    END IF

    RETURN (n MOD 10) + SUM_DIGITS(n / 10)

END FUNCTION


FUNCTION DIGITAL_ROOT(n)

    IF n < 10
        RETURN n
    END IF

    RETURN DIGITAL_ROOT(SUM_DIGITS(n))

END FUNCTION


Base case: When the number is less than 10, return the number.

Recursive case: Add the digits and call the function again until one digit remains.
B. Calculating Base Power (baseⁿ)
The power of a number can be calculated using:
\[
base^n = (base^{n/2})^2
\]

For an odd exponent:
\[
base^n = base \times (base^{n/2})^2
\]

Recursive pseudocode:
FUNCTION POWER(base, n)

    IF n == 0
        RETURN 1
    END IF

    half = POWER(base, n / 2)

    IF n MOD 2 == 0
        RETURN half * half
    ELSE
        RETURN base * half * half
    END IF

END FUNCTION


Base case: When n = 0, return 1.

Recursive case: Divide the exponent by 2 and calculate the smaller power.

A2.2 – Trace the Calls (2 Marks)
A. Digital Root Call Trace
Input: 9875

DIGITAL_ROOT(9875)
       |
       v
SUM_DIGITS(9875)
       |
       v
9 + 8 + 7 + 5 = 29
       |
       v
DIGITAL_ROOT(29)
       |
       v
SUM_DIGITS(29)
       |
       v
2 + 9 = 11
       |
       v
DIGITAL_ROOT(11)
       |
       v
SUM_DIGITS(11)
       |
       v
1 + 1 = 2
       |
       v
DIGITAL_ROOT(2)
       |
       v
Return 2 (Base Case)



Deepest call stack:
DIGITAL_ROOT(2)
DIGITAL_ROOT(11)
DIGITAL_ROOT(29)
DIGITAL_ROOT(9875)


Final answer = 2.
B. Linear Power vs Halving Power
Take:
\[
2^5 = 32
\]

Method 1 – Linear recursion
POWER(2,5)
  = 2 * POWER(2,4)
  = 2 * 2 * POWER(2,3)
  = 2 * 2 * 2 * POWER(2,2)
  = 2 * 2 * 2 * 2 * POWER(2,1)
  = 2 * 2 * 2 * 2 * 2 * POWER(2,0)

  = 32


Method 2 – Halving recursion
POWER(2,5)
       |
       v
POWER(2,2)
       |
       v
POWER(2,1)
       |
       v
POWER(2,0)
       |
       v
Return 1


Returning values:
POWER(2,0) = 1
POWER(2,1) = 2
POWER(2,2) = 4
POWER(2,5) = 32


C. Recurrence and Complexity
Method	Recurrence	Time complexity	Stack space
Linear power	T(n) = T(n-1) + O(1)	O(n)	O(n)
Halving power	T(n) = T(n/2) + O(1)	O(log n)	O(log n)

Conclusion: Halving recursion is faster because it reduces the exponent by half during every recursive call.

A2.3 – Recursion Risks (1 Mark)
If the base case is missing:
- The recursive function will continue calling itself.
- The call stack will keep growing.
- It may cause stack overflow.
- The program may crash or terminate abnormally.
When should recursion be replaced by a loop?

Recursion should be replaced by a loop when:
- The problem requires many repeated operations.
- Recursion creates excessive stack usage.
- A simple iterative solution is easier to understand.
- Stack overflow is a concern.

Alternative Concept and Complexity Analysis
The digital root can also be calculated using a loop or a mathematical formula.
Method 1 – Using a loop
Repeatedly add the digits until one digit remains.
- Time complexity: O(d), where d is the number of digits, for fixed-width integer input.
- Space complexity: O(1).
Method 2 – Using a mathematical formula
For a positive integer n:
\[
DigitalRoot(n)=1+(n-1)\bmod 9
\]

For zero, the digital root is zero.
- Time complexity: O(1).
- Space complexity: O(1).
Comparison
Method	Time complexity	Space complexity
Recursive	Depends on recursive calls	O(d) worst case for digit processing
Loop	O(d)	O(1)
Mathematical formula	O(1)	O(1)

Conclusion: The mathematical formula is the fastest method. However, recursion is useful for understanding function calls and problem decomposition. Loops are preferable when memory efficiency is important.
