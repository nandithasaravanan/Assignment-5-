A1.1 – Identify the Functions (2 Marks)
The District Blood Bank Network can be divided into smaller functions. Each function performs one specific operation.
A. List of functions
Function	Responsibility
registerDonor()	Registers a new donor
checkEligibility()	Checks whether a donor is eligible
findDonors()	Finds donors belonging to a given blood group
updateStock()	Updates available blood units
recordDonation()	Records a donation and updates donor details
issueBlood()	Issues blood and reduces stock
saveData()	Saves donor and stock records into files
loadData()	Loads previously saved records
displayDonors()	Displays donor information
displayStock()	Displays available blood stock

B. Structure chart
The following chart shows the function calling hierarchy.

                    MAIN()
                      |
          +-----------+-----------+
          |           |           |
       loadData()  displayMenu()  saveData()
                      |
          +-----------+-----------+
          |           |           |
    registerDonor() findDonors() issueBlood()
          |           |           |
   checkEligibility() checkEligibility() updateStock()
          |           |           |
          +-----------+-----------+
                      |
                 recordDonation()
                      |
                 updateStock()
                      |
                   saveData()



Explanation:
- main() controls the entire program.
- loadData() retrieves previously saved information.
- registerDonor() stores new donor information.
- checkEligibility() verifies donor eligibility.
- findDonors() searches donors based on blood group.
- recordDonation() records donations and calls updateStock().
- issueBlood() checks availability and reduces stock.
- saveData() permanently stores updated information.
A1.2 – Design the Interfaces (3 Marks)
Function prototypes define the function name, return type and parameters.
A. Structure definitions
struct Date
{
    int day, month, year;
};

struct Donor
{
    int donorID;
    char name[50];
    char bloodGroup[5];
    int age;
    char phone[11];
    struct Date lastDonation;
};

struct BloodStock
{
    char bloodGroup[5];
    int units;
};

struct EligibilityPolicy
{
    int minimumAge;
    int minimumGapDays;
};

struct EligibilitySummary
{
    int eligibleCount;
    int youngestDonorID;
};


B. Function prototypes
void registerDonor(struct Donor *d);

int checkEligibility(
    const struct Donor *d,
    const struct EligibilityPolicy *policy
);

int findDonors(
    const struct Donor donors[],
    int count,
    const char *group,
    struct Donor result[]
);

void updateStock(
    struct BloodStock *stock,
    int units,
    int operation
);

void recordDonation(
    struct Donor *d,
    struct BloodStock *stock
);

int issueBlood(
    struct BloodStock *stock,
    int units
);

int saveData(
    const struct Donor donors[],
    int donorCount,
    const struct BloodStock stock[],
    int stockCount
);

int loadData(
    struct Donor donors[],
    int *donorCount,
    struct BloodStock stock[],
    int *stockCount
);

void displayDonors(
    const struct Donor donors[],
    int count
);

void displayStock(
    const struct BloodStock stock[],
    int count
);


C. Parameter justification
Parameter type	Reason
struct Donor *d	Pointer allows modification of donor details
const struct Donor *d	Prevents accidental modification during eligibility checking
const char *group	Reads the requested blood group without modifying it
struct BloodStock *stock	Allows stock quantities to be updated
const struct Donor donors[]	Reads donor records without modifying the original array
int *donorCount	Allows the function to update the donor count
const struct EligibilityPolicy *policy	Reads eligibility rules without changing them

Conclusion: Pointers are used when functions must modify original data. Const pointers are used when functions only need to read data. Values are passed directly for simple variables such as counts and quantities.
A1.3 – Return More Than One Value (2 Marks)
The function must return:
1. Number of eligible donors.
2. ID of the youngest eligible donor.
There are two approaches.
Method 1 – Using pointer parameters
The function returns the eligible donor count normally and stores the youngest donor ID through a pointer.
int findSummary(
    const struct Donor donors[],
    int n,
    int *youngestID
);


Advantages:
- Simple to implement.
- Can return multiple values through pointers.
- Avoids creating an additional structure.
Disadvantage:
- Requires pointer parameters.
- The function must ensure that output pointers are valid.
Method 2 – Returning a structure
A structure can hold both results and return them together.
struct EligibilitySummary
{
    int eligibleCount;
    int youngestDonorID;
};

struct EligibilitySummary findSummary(
    const struct Donor donors[],
    int n
);


Advantages:
- Returns multiple values together.
- Easier to understand and maintain.
- No output pointer is required.
Disadvantage:
- Requires an additional structure definition.
- Returns a structure by value.
Comparison
Feature	Pointer method	Structure method
Number of outputs	Multiple	Multiple
Complexity	Simple	Simple
Additional structure	Not required	Required
Safety	Requires valid pointers	Avoids output pointers
Readability	Moderate	High

Best choice: Returning a structure is preferable because both results are logically related and can be returned together.
Alternative Concept and Complexity Analysis
If global variables were used instead of parameters and pointers:
- All functions could directly access donor and stock data.
- The running time would remain the same.
- Memory usage would not significantly change.
- However, functions would become tightly coupled.
- Testing and debugging would become difficult.
- Reusing functions in another program would be harder.
Conclusion: Passing data through parameters and pointers is better than using global variables because it improves modularity, reusability and maintainability.
