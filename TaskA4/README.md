A4.1 – Growable Donor List (2 Marks)
The number of donors in the blood bank is not known in advance. Therefore, a dynamically allocated array is better than a fixed-size array.

We use:
- malloc() – To allocate initial memory.
- realloc() – To increase the array capacity.
- free() – To release allocated memory.
A. Data Structure Design
struct donor
{
    int id;
    char name[40];
};

struct donor *list = NULL;
int count = 0;
int capacity = 0;


Here:
Variable	Purpose
list	Pointer to the donor array
count	Number of donors currently stored
capacity	Total available spaces
id	Unique donor identification number
name	Donor name

B. Dynamic Memory Allocation
Initially, the array starts with a capacity of 2 donors.

Whenever the array becomes full, its capacity doubles.
Capacity growth illustration
Initial allocation
2 records

Donor 1

Donor 2



First expansion
4 records

Slot 1


Slot 2


Slot 3


Slot 4




Second expansion
8 records

Slot 1


Slot 2


Slot 3


Slot 4


Slot 5


Slot 6


Slot 7


Slot 8





Formula:
\[
NewCapacity = OldCapacity \times 2
\]

C. Pseudocode for Adding a Donor
FUNCTION ADD_DONOR(donor)

    IF count == capacity THEN

        IF capacity == 0 THEN
            newCapacity = 2
        ELSE
            newCapacity = capacity * 2
        END IF

        temp = REALLOC(list, newCapacity)

        IF temp == NULL THEN
            DISPLAY "Memory allocation failed"
            RETURN FAILURE
        END IF

        list = temp
        capacity = newCapacity

    END IF

    list[count] = donor
    count = count + 1

    RETURN SUCCESS

END FUNCTION


D. What happens if realloc() fails?
- realloc() returns NULL if memory allocation fails.
- The original allocated memory remains valid.
- Therefore, a temporary pointer must be used.
- The original pointer must not be overwritten with NULL.
- The program should display an error and safely handle the failed insertion.
Important: Never write list = realloc(list, newSize) directly because it can lose the original pointer when allocation fails.

A4.2 – Memory Errors (2 Marks)
Three common memory errors that can occur in the blood bank program are:
1. Memory Leak
A memory leak occurs when dynamically allocated memory is not released using free() after it is no longer required.

Example:
int *p = malloc(100 * sizeof(int));

p = malloc(200 * sizeof(int));


Here, the original memory allocated for 100 integers is lost because the pointer is overwritten.

Prevention:
free(p);
p = NULL;


In the donor list, release the allocated memory when the program finishes.
2. Dangling Pointer
A dangling pointer occurs when a pointer refers to memory that has already been freed.

Example:
int *p = malloc(sizeof(int));

free(p);

*p = 100;


Here, p refers to freed memory. Accessing it causes undefined behaviour.

Prevention:
free(p);
p = NULL;


Set the pointer to NULL after releasing memory.
3. Double Free
A double free occurs when the same memory is released more than once.

Example:
int *p = malloc(sizeof(int));

free(p);
free(p);


This can cause undefined behaviour or program crashes.

Prevention:
free(p);
p = NULL;

if (p != NULL)
{
    free(p);
}


Summary of Memory Errors
Memory error	Cause	Prevention
Memory leak	Forgetting to release allocated memory	Use free()
Dangling pointer	Accessing freed memory	Set pointer to NULL
Double free	Releasing the same memory twice	Check pointer and set it to NULL


Alternative Concept and Complexity Analysis
Instead of doubling the capacity, the donor list could grow by a fixed 10 records every time it becomes full.
Method 1 – Doubling Strategy
Capacity increases as follows:
2 → 4 → 8 → 16 → 32 → 64


- Total elements copied during reallocations: O(n).
- Time complexity for n insertions: O(n) overall (amortized O(1) per insertion).
- Maximum unused space after expansion: approximately half the new capacity.
Method 2 – Fixed Increment Strategy
Capacity increases as follows:
10 → 20 → 30 → 40 → 50 → 60


- Total elements copied during reallocations: O(n²).
- Time complexity for n insertions: O(n²) overall.
- Maximum unused space: 9 records.
Comparison
Factor	Doubling	Fixed increment (10)
Capacity growth	Multiplies by 2	Adds 10
Total copying	O(n)	O(n²)
Insertion (amortized)	O(1)	O(n)
Maximum unused slots	Less than half capacity after growth	9
Memory efficiency	May have more unused space	Less unused space
Performance	Faster for large lists	Slower for large lists

Conclusion: Doubling is preferable for a district blood bank because the number of donors can increase significantly. It reduces repeated memory copying and provides better overall performance.
