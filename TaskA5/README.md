A5.1 – Text or Binary? (2 Marks)
The blood bank needs to store donor details and blood stock information permanently so that the data remains available even after the computer is switched off.

We use two different file formats:
1. Donor records – Text file.
2. Blood stock records – Binary file.
A. Donor Records – Text File
File name: donors.txt

Each donor record is stored on a separate line using fprintf().

Example:
101 Arun O+ 21 9876543210 10/10/2026
102 Priya A+ 25 9876543211 12/10/2026
103 Rahul B+ 23 9876543212 13/10/2026


Writing donor records:
FILE *fp = fopen("donors.txt", "w");

fprintf(fp, "%d %s %s %d %s %s\n",
        id, name, blood_group, age, phone, date);

fclose(fp);


Why use a text file?
- Human-readable and easy to understand.
- Can be opened using Notepad.
- Easy to edit and verify donor information.
- Convenient for debugging and maintaining records.
B. Blood Stock Records – Binary File
File name: stock.dat

Each blood stock record contains a blood group and its available units.

Structure:
struct stock
{
    char blood_group[4];
    int units;
};


Writing binary records:
FILE *fp = fopen("stock.dat", "wb");

fwrite(&record, sizeof(struct stock), 1, fp);

fclose(fp);


Why use a binary file?
- Stores fixed-size records.
- Faster reading and writing.
- Uses less storage than a text representation in many cases.
- Supports direct access to individual records using fseek().
- Suitable for frequent stock updates.
C. Comparison of Text and Binary Files
Feature	Text file	Binary file
Format	Human-readable	Machine-readable
Function	fprintf()	fwrite()
Reading	fscanf()	fread()
Editing	Easy to edit manually	Requires a program
Record size	Variable	Fixed for a given structure and ABI
Direct access	Difficult	Easy with fixed-size records
Best use	Donor details	Blood stock

Conclusion: A text file is suitable for donor records because they are easy to read and maintain. A binary file is suitable for stock records because fixed-size records allow faster access and in-place updates.

A5.2 – Update in Place (2 Marks)
The blood bank must update the available units of a blood group without rewriting the entire stock file.

For this purpose, we use:
- fseek() – Moves the file pointer to a specific record.
- fread() – Reads the selected record.
- fwrite() – Writes the modified record back.
A. Pseudocode for Updating Stock
Example: Increase the O+ blood stock by 5 units.
FUNCTION UPDATE_STOCK(group, change)

    OPEN "stock.dat" IN READ-WRITE BINARY MODE

    IF file cannot be opened
        DISPLAY "File opening failed"
        RETURN FAILURE
    END IF

    FOR i = 0 TO number_of_records - 1

        SEEK TO i * SIZEOF(stock_record)

        READ one stock record

        IF record.blood_group == group

            record.units = record.units + change

            IF record.units < 0
                DISPLAY "Insufficient stock"
                CLOSE FILE
                RETURN FAILURE
            END IF

            SEEK BACK TO i * SIZEOF(stock_record)

            WRITE updated record

            DISPLAY "Stock updated successfully"

            CLOSE FILE
            RETURN SUCCESS

        END IF

    END FOR

    DISPLAY "Blood group not found"

    CLOSE FILE

    RETURN FAILURE

END FUNCTION


B. Why is this difficult for a text file?
Text files contain variable-length records.

For example:
O+ 25
A+ 15
AB+ 100


If the stock value changes from 25 to 125, the length of the record changes.

This creates problems:
- The following records may shift position.
- Existing file offsets become invalid.
- Rewriting the entire file may be necessary.
- Updating one record becomes more complicated.
Conclusion: Binary files are better for in-place updates because every record has a fixed size. The program can calculate the exact position of a record and update it without rewriting the entire file.

Alternative Concept and Complexity Analysis
Consider two methods of updating blood stock.
Method 1 – Binary File with In-place Update
The program directly accesses the required record using fseek() and updates it using fread() and fwrite().
- Time complexity: O(1) to access and update a known record.
- If the program must search for a blood group first, searching takes O(n).
- Only one fixed-size record needs to be rewritten.
Method 2 – Text File with Complete Rewrite
The program reads all records, modifies the required blood group and writes all records into a new file.
- Time complexity: O(n) per update.
- All n records must be read and rewritten.
- More file operations are required.
Comparison
Factor	Binary in-place	Text file rewrite
Record access	O(1) by index	O(n) search
Update time	O(1) by index	O(n)
Records rewritten	1	All n records
File size	Fixed-size records	Variable-size records
Storage	Usually compact	Depends on text representation
Implementation	Requires binary handling	Easier to read and debug

Conclusion: Binary storage with in-place updating is more efficient for frequently changing blood stock. Text storage is more convenient for donor information that needs to be manually inspected.
