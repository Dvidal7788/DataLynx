#include <csvWizard.h>


// EXAMPLE CODE FOR CSV LIBRARY
int main(void)
{
    // ----- Set Up ------

    // Create main 'object'
    csvWizard myData = csvWizardConstructor();

    // User input filename until a valid file is given
    while (true) {

        // Ask user for filename
        // myData.userInputFilename(&myData, "Enter filename: ");

        // Quit if user chooses to do so
        // if (strcasecmp(myData.filename, "quit") == 0) {printf("\n\tGOODBYE!\n\n"); free_null(&myData.filename); return 1;}

        // Open
        if (myData.csv.openFile(&myData, "csv/staff.csv")) break;

    }
    // End of set up section

    // CSV Readers:
    // csv.headerReader(&myData);          /* header  */
    // fileReader(&myData);                /* Raw     */
    // fileRowReader(&myData);             /* Rows    */
    myData.csv.reader_v3(&myData);      /* Grid_v3 */
    // myData.csv.reader(&myData);         /* Grid    */
    // myData.csv.dictReader(&myData);        /* Dict    */

    // Print
    // myData.printData(&myData);
    // myData.printStats(&myData, "all");

    // print one column / one column's stats
    // myData.printColumn(&myData, "Name");
    // myData.printStats(&myData, "Name");

    // // Get one specific stat
    // printf("Sum: %.2f\n\n", myData.getStat(&myData, "Ext", "sum"));
    // printf("Sum: %.2f\n\n", myData.sum(&myData, "Ext"));


    //          -- FIX HEADER --
    myData.formatHeader(&myData); /* Format to title case (i.e. first letter of every work capitalized) */
    myData.changeColumnName(&myData, "Dept", "Department");



    //      ---- REPLACE ----
    // printf("\nREPLACE:\n");

    myData.replace(&myData, "?", ""); /* Replace all instances if '?' with NULL */
    // myData.replaceInColumn(&myData, "Name", "?", "TEST");



    // // Sort by numberic column
    // // myData.sortRowsByColumn(&myData, "Salary, Data & Entries", "desc");

    // // Sort by non-numeric column
    // myData.sortRowsByColumn(&myData, "Name", "ASC");

    // myData.printData(&myData);



    //      --- Get Field (from memory) ---

    // printf("Get field: %s\n", myData.getField2(&myData, 2, "Department"));

    //          --- Filter ---
    // myData.printColumnCond(&myData, "Name", "==", "Annie");

    // dropRowsFilter(&myData, "emp_id", "<", "7777");
    // dropRow(&myData, 0);


    //          --- Filter into NEW Object/Data Structure ---
    // csvWizard filteredData = csvWizardConstructor();

    // filter(&myData, &filteredData, "emp_id", ">", "8888");

    // // printf("Filtered Data: %p %s\n", &filteredData.grid_v3, filteredData.grid_v3[0][0]);
    // filteredData.printData(&filteredData);

    // filteredData.freeAll(&filteredData);

    // Needed to make sales_data2.csv print correctly as of 9.8.23 (need to factor screen resolution into print functions)
    // dropColumn(&myData, "sub_category");
    // dropColumn(&myData, "product");
    // dropColumn(&myData, "product_category");


    // Update Field (in memory)

    // Get Field (from CSV)
    // printf("Get field from CSV: %s\n", myData.csv.fieldReader2(&myData, 2, "Department"));

    // Update Field (in CSV)
    // myData.csv_write_permission = true;
    // if (myData.csv.fieldWriter(&myData, 0, "Name", "Dave Vidal")) printf("Success writing to csv\n");
    // else printf("Failure writing to csv\n");
    // myData.csv_write_permission = false;


    // // Backup CSV
    // myData.csv.backup(&myData);
    // myData.csv.writeData(&myData, "testfile.csv");

    // printf("Get Field: %s, %s\n", getField(&myData, 0, 3), getField(&myData, 0, 4));
    // printf("\n\tUPDATE\n");
    // updateField(&myData, 6, "Emp_ID", "100");
    // printHead(&myData, 5);
    // printTail(&myData, 5);
    // printData(&myData);
    // myData.printStats(&myData);

    // printf("%s %s\n", myData.csv.fieldReader(&myData, 0, 1), myData.csv.fieldReader(&myData, 0, 90));
    // printf("%s %s\n", myData.csv.fieldReader(&myData, 1, 1), myData.csv.fieldReader(&myData, 1, 2));
    // sort_value_counts(&myData);
    // printStats(&myData, "all");


    //      ---  APPEND ROW ---
    //      --- INSERT ROW --- /* VALGRIND ERRORS!! */
    /* When inserting, must at very least set columnCount (whether insertRow ro insertRow2) */

    // Create your own header
    // can either set header or columnCount if do not want to set header
    // const char *head[] = {"Name", "", "Phone Number"};
    // createHeader(&myData, head, 3);
    // myData.printHeader(&myData);


    // char *values[] = {"value1", "value2", "value3"};
    // Auto-create generic header
    // myData.columnCount = 3;
    // myData.createHeader(&myData, NULL, 3); /* Same as setting columnCount. Will create generic header */

    // Create header
    // char *header[] = {"ID", "Name", "Status"};
    // myData.createHeader(&myData, header, 3);

    // myData.insertRow(&myData, values);

    //      - Insert Row 2 - (rearranged columns if out of order)
    // dict values[] = {{"Name", "David"}, {"Address", "123 Main"}, {"Phone Number", "215-555"}};
    // myData.columnCount = 3;
    // if (!myData.insertRow2(&myData, values)) return 1;
    // myData.insertRow2(&myData, values);

    // dict values2[] = {{"Name", "Anna"}, {"Address", "123 Main Apt 2"}, {"Phone Number", "215-777"}};
    dict values2[] = {{"Name", "Annie"}, {"emp_id", "2323"}, {"DEPARTmedNT", "Sales"}, {"Salary,  dATA & ENtRies", "65000"}, {"Test", "c"}, {"Edxt", "452"}};


    // if (!insertRow2(&myData, values)) return 1;
    myData.insertRow2(&myData, values2);


    //  Insert into existing data
    // char *values[] = {"8888", "Mr. Pebbles", "Administration", "333", "80000"};
    // insertRow(&myData, values);


    // Does not have to be in correct order
    // dict values[] = {{"Name", "Mr. Pebbles"}, {"DEPARTmeNT", "\"Administration, and TONS\""}, {"Ext", "333"}, {"Salary, dATA & ENtRies", "80000"}, {"emp_id", "8888"}};
    // insertRow2(&myData, values);

    // printf("Field: %s\n", myData.getField(&myData, 3, 0));

    // myData.printData(&myData);

    //      --- Backup Data / Write New Data ---
    // myData.csv.writeData(&myData, "test_filetest.csv");

    // myData.printStats(&myData, "all");



    //                            ------ DATA CLEANUP ------

    //      --- Sort Rows ---
    // myData.case_sensitive_sort = true;
    // myData.in_place_sort = false;
    // myData.sortRowsByColumn(&myData, "Name", "asc");

    // TODO: myData.printSortedColumn()
    // myData.printData(&myData);

    //      --- Drop Column ---
    // myData.dropColumn(&myData, "Name");

    //      --- Format Header ---
    // myData.formatHeader(&myData);


    //      --- Drop Row --- /* MAKE WORK FOR ALL DATA STRUCTURES. LINK TO FILTER */
    // myData.dropRow(&myData, 9);
    /*                           DOES UPDATE STATS WORK??
                                        - is/not NULL NOT WORKING. DONE
                                        - min not updating when min changed DONE
                                        - value counts not updating  DONE */
    // myData.dropRow(&myData, 5);

    // //      --- Strip ---
    // // stripField(&myData, "Name");
    // myData.stripAll(&myData);


    // myData.printData(&myData);
    // printStats(&myData, "all");

    myData.printRowIndex = false;
    printDataTable(&myData);

    // printf("Anna in Name: %d\n", myData.isInColumn(&myData, "Anna", "Name"));
    // printf("Anna in Name: %d\n", myData.valueCount(&myData, "Anna", "Name"));
    myData.printColumnCond(&myData, "Name", "==", "Annie");
    //      --- End ---
    myData.freeAll(&myData);

    return EXIT_SUCCESS;
}