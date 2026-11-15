# TASK B2 – ENGINEER ADVERSARIAL INPUTS

## Aim

To test the blood bank program using invalid and unexpected inputs, identify possible memory and file-handling errors, and improve the program's safety.

## B2.1 – Test Long Donor Names

### Test 1: Using scanf("%s")

A character array has a fixed capacity. When scanf("%s") reads a name longer than the available array, it can write beyond the array boundary.

Example:

    char name[20];
    scanf("%s", name);

If the input contains more than 19 characters, the input may overflow the array because scanf does not check the destination buffer size.

**Possible consequences:**
- Buffer overflow
- Corruption of nearby memory
- Unexpected program behaviour
- Program crash

This is an unsafe input method.

### Test 2: Using fgets()

    char name[20];
    fgets(name, sizeof(name), stdin);

The fgets() function reads at most 19 characters and reserves one byte for the null character '\0'.

The program should also check whether the complete input was received. If the input is too long, the remaining characters must be discarded and the input rejected.

**Observation:** fgets() is safer because it limits the number of characters written into the array.

## B2.2 – Test File Errors

The program uses files to preserve donor and stock information.

### Test 1: Missing data file

When the required file does not exist, fopen() returns NULL.

The program should use perror() to display the reason for the failure.

Example:

    FILE *fp = fopen("stock.dat", "rb");

    if (fp == NULL)
    {
        perror("stock.dat");
        return 1;
    }

Expected error:

    stock.dat: No such file or directory

### Test 2: Read-only data file

When the program tries to update a read-only stock file using "rb+", fopen() fails if the operating system denies write access.

The program reports the error using perror() instead of continuing with an invalid file pointer.

Expected error:

    stock.dat: Permission denied

The exact error message depends on the operating system.

## B2.3 – Test Invalid Command-Line Arguments

The program accepts commands such as:

    ./main find O+
    ./main issue A- 2

The following invalid inputs are tested:

| Test | Input | Expected behaviour |
|---|---|---|
| 1 | ./main | Display usage instructions |
| 2 | ./main find O+ extra | Reject too many arguments |
| 3 | ./main find X+ | Reject invalid blood group |
| 4 | ./main issue A- -2 | Reject negative units |
| 5 | ./main issue A+ abc | Reject non-numeric units |

The program validates argc, argv, blood group and quantity before performing any operation.

## B2.4 – Memory Safety Testing

The program is tested using GCC AddressSanitizer.

Compilation:

    gcc -std=c11 -Wall -Wextra -g -fsanitize=address main.c -o main_asan

Execution:

    ./main_asan find O+

AddressSanitizer checks for memory errors such as:

- Stack buffer overflow
- Heap buffer overflow
- Use-after-free
- Invalid memory access

The fixed-size fgets() input method prevents writing beyond the name array.

**Observation:** The corrected program should execute without AddressSanitizer memory errors for valid test inputs.

## Alternative Concept and Complexity Analysis

### Using getline() instead of fgets()

The getline() function dynamically allocates or resizes a buffer to accommodate an entire input line.

For a line containing L characters:

| Property | fgets() | getline() |
|---|---|---|
| Time complexity | O(L) | O(L) |
| Buffer size | Fixed | Dynamic |
| Memory usage | Fixed buffer | O(L) heap space |
| Long input | Requires rejection or multiple reads | Can accommodate long lines |
| Memory release | Automatic for local array | Must use free() |

The caller is responsible for releasing the dynamically allocated buffer using free() after getline() has finished.

### Conclusion

Adversarial testing helps identify weaknesses that ordinary inputs may not reveal. Using fgets(), checking file operations, validating command-line arguments and testing with AddressSanitizer improves the reliability and safety of the blood bank program.
