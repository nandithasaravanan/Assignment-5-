# Assignment 5 – Part B
## Task B3: Text File vs Binary File

### B3.1 – Correctness on Small Data (2 Marks)

**Objective:** Write and read back 10 donor records in both text and binary formats and verify that the retrieved data is identical.

**Text File:**
- Donor records are stored using `fprintf()`.
- Each donor is written on a separate line.
- Records are retrieved using `fgets()` and `sscanf()`.

**Binary File:**
- Donor records are stored using `fwrite()`.
- Records are retrieved using `fread()`.
- Each record has a fixed structure size.

**Verification:**

The original donor records are compared with the retrieved records field by field.

If all fields match, the verification is successful.

### B3.2 – Increase the Data Size (2 Marks)

**Objective:** Measure file size and execution time for 1,000, 10,000 and 100,000 donor records.

| Number of Donors | Text File | Binary File |
|---:|---|---|
| 1,000 | Measure size and time | Measure size and time |
| 10,000 | Measure size and time | Measure size and time |
| 100,000 | Measure size and time | Measure size and time |

**Observations:**

1. Text files generally require more storage because numbers and characters are stored as readable text.
2. Binary files store records in their internal memory representation.
3. Binary files generally provide faster fixed-record access.
4. Actual performance depends on the compiler, operating system, storage device and buffering.

### B3.3 – Update One Record (2 Marks)

**Objective:** Update the 50,000th donor record.

**Binary File Update:**

1. Open the binary file in read and write mode (`rb+`).
2. Calculate the record position using:

   `position = (record_index * sizeof(Donor))`

3. Use `fseek()` to move to the required record.
4. Read the record using `fread()`.
5. Modify the required field.
6. Move back using `fseek()`.
7. Write the modified record using `fwrite()`.

Only one record is modified.

**Time Complexity:** O(1) for fixed-size record access.

**Text File Update:**

1. Open the original text file for reading.
2. Create a temporary text file.
3. Read every record.
4. Modify the 50,000th record.
5. Write all records into the temporary file.
6. Replace the original file with the updated file.

**Time Complexity:** O(n), because the entire file must be processed.

### B3.4 – Engineering Trade-Off (2 Marks)

| Feature | Text File | Binary File |
|---|---|---|
| Readability | Human-readable | Not directly readable |
| Storage | Generally larger | Generally smaller |
| Writing | Requires formatting | Direct memory representation |
| Reading | Requires parsing | Direct record reading |
| Updating | Usually requires rewriting | Supports in-place updates |
| Portability | High | May depend on system representation |

**Recommendation:**

Text files are recommended for donor records because they are readable, portable and easy to inspect or modify.

Binary files are recommended for stock records because they contain fixed-size records and support efficient direct access and in-place updates.

### Alternative Concept and Complexity Analysis

If all stock records were written using one `fwrite()` call instead of one call per record:

- The number of library calls would decrease from n to 1.
- The total amount of data written would remain the same.
- The overall time complexity would remain O(n).
- An array of n records would need to be stored in memory before writing.
- The memory requirement would be O(n).

**Conclusion:**

Writing the complete array in one operation can reduce function-call overhead. However, it requires additional memory to hold all records before writing.

### Final Conclusion

Both text and binary files are useful for permanent data storage. Text files provide readability and portability, whereas binary files provide compact storage and efficient fixed-record updates. The appropriate format depends on the application requirements.
