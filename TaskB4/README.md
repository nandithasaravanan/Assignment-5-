# Assignment 5 – Part B
## Task B4: Final Engineering Decision

### B4.1 – Implement a Practical Strategy (2 Marks)

**Objective:** Implement a nightly donor eligibility report using a growable donor list loaded from a file.

**Data Structure:**

The program uses a dynamically allocated array of donor structures.

Each donor contains:
- Donor ID
- Blood group
- Last donation date

The donor list contains:
- `data` – Pointer to the dynamically allocated donor array.
- `size` – Number of donors currently stored.
- `capacity` – Total allocated storage capacity.

**Dynamic Memory Allocation:**

The program starts with a small capacity of 2 records.

When the list becomes full:
1. Calculate the new capacity by doubling the existing capacity.
2. Use `realloc()` to increase the allocated memory.
3. Store the result in a temporary pointer.
4. Update the original pointer only if allocation succeeds.
5. Update the capacity.
6. Add the new donor record.

**Why use a temporary pointer?**

The result of `realloc()` is stored in a temporary pointer because `realloc()` can fail. If it fails, the original memory block remains valid. Directly assigning the result to the original pointer could lose access to the allocated memory and cause a memory leak.

**Memory Measurement:**

Allocated donor-array memory is calculated using:

`Memory = capacity × sizeof(Donor)`

The program also measures the execution time required to load donor records and generate the eligibility report.

**Eligibility Report:**

The program:
1. Loads donor records from a text file.
2. Calculates the next eligibility date using the configured 90-day interval.
3. Identifies donors who are eligible now or within the next seven days.
4. Prints their donor IDs and blood groups.

### B4.2 – Check Memory Safety (1 Mark)

**Objective:** Verify that all dynamically allocated memory is released.

The program uses `free()` to release the donor array after completing the report.

**Memory Safety Measures:**

1. Check whether `malloc()` and `realloc()` return NULL.
2. Use a temporary pointer when calling `realloc()`.
3. Avoid accessing the array outside its allocated capacity.
4. Free allocated memory before program termination.
5. Set the pointer to NULL after freeing it.

**AddressSanitizer Command:**

`gcc -std=c11 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer taskB4.c -o taskB4`

Execute:

`./taskB4`

A clean execution without AddressSanitizer errors indicates that no detected memory errors occurred during the test.

### B4.3 – Production Decision (2 Marks)

**Objective:** Decide how donor information should be stored and accessed when the district network expands to 25 hospitals and the donor count increases 100 times.

Three strategies are considered.

**1. Keep All Donors in Memory**

- Load all donor records into a growable array.
- Provides fast sequential processing.
- Requires O(n) memory.
- Suitable when the complete donor list fits comfortably in available RAM.

**2. Read the File in Batches**

- Load a limited number of records at a time.
- Process each batch and release its memory.
- Requires O(b) memory, where b is the batch size.
- Suitable for systems with limited RAM.

**3. Maintain a Separate Index File**

- Store donor IDs and file offsets in a separate index.
- Sort the index by donor ID.
- Use binary search to locate records.
- Requires O(n) index storage but avoids loading every full donor record.

**Recommended Decision:**

For the district blood bank, a separate sorted index file combined with batch processing is a suitable production design when the donor database becomes very large.

For smaller databases, a growable array is simpler and provides fast access.

The final choice should be based on measured memory consumption, report execution time and available system resources.

### Alternative Concept and Complexity Analysis

If donors were searched using a separate sorted index file instead of loading all records into memory:

| Feature | Growable Array | Sorted Index File |
|---|---|---|
| Memory usage | O(n) | O(n) for index |
| Full donor lookup | O(1) by known array index | O(log n) index search |
| Additional file reads | Not required after loading | Required for indexed access |
| Report processing | O(n) | O(n) index traversal or filtering |
| Initial loading | O(n) | Index construction required |

**Conclusion:**

A growable array provides simple and fast donor access but consumes O(n) memory. A sorted index reduces the need to load complete donor records into RAM, although it introduces additional file operations and index-management complexity.

### Final Conclusion

The growable donor list provides an efficient way to manage an increasing number of donor records. Doubling capacity reduces repeated memory allocation, while safe `realloc()` handling prevents loss of the original pointer. Memory measurement and sanitizer testing help determine whether the implementation is suitable for production use.
