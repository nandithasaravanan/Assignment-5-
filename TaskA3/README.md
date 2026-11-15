A3.1 – Pointer Operations (2 Marks)
Given:
int stock[8] = {25, 40, 15, 30, 50, 20, 35, 10};
int *p = stock;


Here, p stores the address of the first element of the array.
Explanation of pointer expressions
Expression	Meaning
p + 3	Address of the fourth element (stock[3]).
*(p + 3)	Value stored in the fourth element (30).
p[3]	Same as *(p + 3), gives 30.
*p++	Accesses the current value and then moves the pointer to the next element.

Important: *p++ is equivalent to *(p++). It increments the pointer, not the value stored at that address.
Memory Diagram
Assume the base address of the array is 1000 and each integer occupies 4 bytes.
Memory representation
stock[0]

25
1000

stock[1]

40
1004

stock[2]

15
1008

stock[3]

30
1012

stock[4]

50
1016

stock[5]

20
1020

stock[6]

35
1024

stock[7]

10
1028


p initially points to stock[0] (address 1000).

p + 3 points to stock[3] (address 1012).


Address formula:
\[
Address(p+i)=BaseAddress+(i\times sizeof(int))
\]

For p + 3:
\[
1000+(3\times4)=1012
\]

Therefore, *(p+3) = 30 and p[3] = 30.

A3.2 – Pointers and Functions (2 Marks)
The blood bank needs to identify the blood group with the lowest available stock.
Function design
int *find_lowest_stock(int stock[], int n);


Working:
1. Assume the first element has the lowest stock.
2. Compare every remaining element.
3. Update the index whenever a smaller value is found.
4. Return the address of the element with the lowest stock.
Pseudocode:
FUNCTION FIND_LOWEST_STOCK(stock, n)

    min_index = 0

    FOR i = 1 TO n-1

        IF stock[i] < stock[min_index]
            min_index = i
        END IF

    END FOR

    RETURN ADDRESS OF stock[min_index]

END FUNCTION


Why should we return a pointer?
Returning a pointer allows the calling function to directly modify the original array element.

Example:
int *lowest = find_lowest_stock(stock, 8);

*lowest = *lowest + 10;


This directly updates the lowest stock value in the original array.
Why is returning a pointer to a local variable wrong?
A local variable exists only while its function is executing.

After the function returns, its local variables cease to exist. Therefore, returning their addresses creates a dangling pointer.

This can cause:
- Undefined behaviour.
- Incorrect values.
- Memory access errors.
Correct approach: Return a pointer to an element of the caller-owned array, whose lifetime continues after the function returns.

A3.3 – Command-Line Interface (1 Mark)
The blood bank program can use argc and argv to receive commands through the command line.
- argc – Number of command-line arguments.
- argv – Array of strings containing the arguments.
Example commands
1. Find blood group stock
bloodbank find O+


Expected action: Display the available stock of O+.

2. Issue blood units
bloodbank issue A- 2


Expected action: Reduce A- stock by 2 units.
Argument validation
Condition	Action
Correct command	Execute the operation
Invalid blood group	Display error message
Missing arguments	Display usage instructions
Invalid quantity	Reject the input
Quantity greater than available stock	Reject the transaction
Unknown command	Display invalid command message


Alternative Concept and Complexity Analysis
Instead of returning a pointer to the lowest-stock element, the function can return its array index.
Method 1 – Returning a pointer
int *lowest = find_lowest_stock(stock, 8);
*lowest = *lowest + 10;


Method 2 – Returning an index
int index = find_lowest_index(stock, 8);
stock[index] = stock[index] + 10;


Complexity comparison
Factor	Pointer method	Index method
Time complexity	O(n)	O(n)
Extra space	O(1)	O(1)
Modification	Directly through pointer	Using array index
Safety	Pointer may become invalid after reallocation	Index can be reused after reallocation
Memory usage	One pointer	One integer

Conclusion: Both methods require O(n) time and O(1) extra space. Returning an index is safer when an array may be reallocated because the pointer may become invalid. The index can be used to calculate a new address after reallocation, provided the array's logical contents and ordering remain unchanged.
