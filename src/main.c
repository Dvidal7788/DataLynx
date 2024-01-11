#include <DataLynx.h>


// EXAMPLE CODE FOR DataLynx Library
int main(void)
{
    // ----- Set Up ------

    // Create main 'object'
    DataLynx myData = DataLynxConstructor();

    // User input filename until a valid file is given
    while (true) {

        // Ask user for filename
        // myData.userInputFilename(&myData, "Enter filename: ");

        // Quit if user chooses to do so
        // if (strcasecmp(myData.filename, "quit") == 0) {printf("\n\tGOODBYE!\n\n"); free_null(&myData.filename); return 1;}

        // Open
        if (myData.csv.openFile(&myData, "csv/autos.csv")) break;

    }
    // End of set up section

    // CSV Readers:
    // csv.headerReader(&myData);          /* header  */
    // fileReader(&myData);                /* Raw     */
    // fileRowReader(&myData);             /* Rows    */

    // myData.DELIMITER = '|';

    myData.csv.reader_v3(&myData);      /* Grid_v3 */
    // myData.csv.reader(&myData);         /* Grid    */
    // myData.csv.dictReader(&myData);        /* Dict    */

    //              Linear Regression
    myData.linearModel.fit(&myData, "highway-mpg", "price");
    printf("Linear Model:\ny = %.2fx + %.2f\n\n", myData.linearModel.slope_, myData.linearModel.intercept_);

    double *highway_mpg = getNumericColumn(&myData, "highway-mpg");

    double *yhat = myData.linearModel.predict(&myData, highway_mpg);

    for (uintmax_t i = 0; i < myData.rowCount; i++) {
        printf("%.2f\n", yhat[i]);
    }


    double new_x2[myData.rowCount];
    for (uintmax_t row = 0; row < myData.rowCount; row++) {
        new_x2[row] = atof(myData.getField(&myData, row, "highway-mpg")) * 2;
    }

    double *yhat2 = myData.linearModel.predict(&myData, new_x2);

    double *y = getNumericColumn(&myData, "price");
    printf("MSE: %.2f\n\n", mse(&myData, y, yhat));


    printf("R2: %.2f\n\n", r2_score(&myData, yhat));

    freeAll(&myData);
    return 0;



    //          JOIN
    // DataLynx salary = DataLynxConstructor();

    // salary.csv.openFile(&salary, "csv/salary.csv");

    // salary.csv.reader_v3(&salary );

    // salary.printDataTable(&salary);

    // DataLynx *merged = join(&myData, &salary, "emp_id", "emp_id", "inner");

    // This will seg fault if merged is return  as NULL on error
    // merged->freeAll(merged);

    //          -- PRINT --
    // myData.printData(&myData);
    myData.printDataTable(&myData);
    // myData.printStats(&myData, "all");

    // print one column / one column's stats
    // myData.printColumn(&myData, "Name");
    // myData.printStats(&myData, "Name");

    // // Get one specific stat
    // printf("Sum: %.2f\n\n", myData.getStat(&myData, "Ext", "sum"));
    // printf("Sum: %.2f\n\n", myData.sum(&myData, "Ext"));
    // myData.csv.write_permission = true;
    // myData.destructive_mode = true;

            //  -- Correlation --
    // replaceAll(&myData, "?", "");
    // printf("\nCorrelation: %.2f\n\n", corr(&myData, "city-mpg", "price"));


    // //      -- One Hot --
    // oneHot(&myData, "name");
    // printDataTable(&myData);

    // freeAll(&myData);
    // return 0;

    //          -- Bins -
    // char *bin_names[] = {"Low", "Medium", "High"};
    uint8_t num_bins = 4;
    myData.getBins(&myData, "salary, Salary & Salary", num_bins, NULL);

    // Example storing dividers
    // char *bin_names2[] = {"Low", "Medium-Low", "Medium-High", "High"};
    double *dividers = myData.getBins(&myData, "emp_id", ++num_bins, NULL);

    // Print bin dividers
    if (dividers != NULL) {
        uint8_t num_dividers = num_bins + 1;
        for (uint8_t d = 0; d < num_dividers; d++) (d == num_dividers-1) ? printf("%.2f\n", dividers[d]) : printf("%.2f | ", dividers[d]) ;
    }


    //          -- FIX HEADER --
    // myData.formatHeader(&myData); /* Format to title case (i.e. first letter of every work capitalized) */
    // myData.changeColumnName(&myData, "Dept", "Department");



    //      ---- REPLACE ----
    // printf("\nREPLACE:\n");

    // myData.replaceAll(&myData, "?", ""); /* Replace all instances if '?' with NULL */
    // myData.replaceInColumn(&myData, "Name", "?", "TEST");

    // myData.replaceInColumn(&myData, "salary, Salary & Salary ", "?", );



    // Sort by numberic column
    // myData.sortRowsByColumn(&myData, "Salary,  Data & Entries", "desc");

    // // Sort by non-numeric column
    // myData.sortRowsByColumn(&myData, "Name", "ASC");

    myData.printData(&myData);



    //      --- Get Field (from memory) ---

    // printf("Get field: %s\n", myData.getField(&myData, 2, "Department"));

    //          --- Filter ---
    // myData.printColumnCond(&myData, "Name", "==", "Annie");

    // dropRowsFilter(&myData, "emp_id", "<", "7777");
    // dropRow(&myData, 0);


    //          --- Filter into NEW Object/Data Structure ---
    // DataLynx filteredData = DataLynxConstructor();

    // filter(&myData, &filteredData, "emp_id", ">", "8888");

    // // printf("Filtered Data: %p %s\n", &filteredData.grid_v3, filteredData.grid_v3[0][0]);
    // filteredData.printData(&filteredData);

    // filteredData.freeAll(&filteredData);

    // Needed to make sales_data2.csv print correctly as of 9.8.23 (need to factor screen resolution into print functions)
    // dropColumn(&myData, "Department");
    // dropColumn(&myData, "product");
    // dropColumn(&myData, "product_category");


    // Update Field (in memory)

    // Get Field (from CSV)
    // printf("Get field from CSV: '%s'\n", myData.csv.fieldReader(&myData, 0, "Department"));
    // printf("Get field from CSV: '%s'\n", myData.csv.fieldReaderIdx(&myData, 0, 0));

    // Update Field (in CSV)
    // myData.csv.write_permission = true;
    // if (myData.csv.fieldWriter(&myData, 0, "Name", "Dave Vidal")) printf("Success writing to csv\n");
    // else printf("Failure writing to csv\n");
    // myData.csv.write_permission = false;

    // myData.csv.renameFile(&myData, "csv/sales_data3.csv");

    // // Backup CSV
    // myData.csv.backup(&myData);
    // myData.csv.writeData(&myData, "testfile.csv");

    // printf("Get Field: %s, %s\n", getFieldIdx(&myData, 0, 3), getFieldIdx(&myData, 0, 4));
    // printf("\n\tUPDATE\n");
    // updateField(&myData, 6, "Emp_ID", "100");
    // updateFieldIdx(&myData, 6, 0, "100");
    // printHead(&myData, 5);
    // printTail(&myData, 5);
    // printData(&myData);
    // myData.printStats(&myData);

    // printf("%s %s\n", myData.csv.fieldReaderIdx(&myData, 0, 1), myData.csv.fieldReaderIdx(&myData, 0, 90));
    // printf("%s %s\n", myData.csv.fieldReaderIdx(&myData, 1, 1), myData.csv.fieldReaderIdx(&myData, 1, 2));
    // sort_value_counts(&myData);
    // printStats(&myData, "all");


    //      ---  APPEND ROW ---
    //      --- INSERT ROW --- /* VALGRIND ERRORS!! */
    /* When inserting, must at very least set columnCount (whether insertRow ro insertRowDict) */

    myData.csv.write_permission = true;
    myData.destructive_mode = true;

    // Create your own header
    // can either set header or columnCount if do not want to set header
    // const char *head[] = {"Name", "", "Phone Number"};
    // createHeader(&myData, head, 3);
    // myData.printHeader(&myData);

    // myData.columnCount = 6;
    // char *values[] = {"1111", "Tupac Shakur", "Sales", "123", "100000", "s"};
    // myData.insertRow(&myData, values);
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
    // if (!myData.insertRowDict(&myData, values)) return 1;
    // myData.insertRowDict(&myData, values);

    // dict values2[] = {{"Name", "Anna"}, {"Address", "123 Main Apt 2"}, {"Phone Number", "215-777"}};
    // dict values2[] = {{"Name", "Annie"}, {"Emp ID", "2323"}, {"Department", "Sales"}, {"Salary,  Data & Entries", "65000"}, {"Test", "c"}, {"Ext", "452"}};

    // if (!insertRowDict(&myData, values)) return 1;
    // myData.insertRowDict(&myData, values2);


    //  Insert into existing data
    // char *values[] = {"8888", "Mr. Pebbles", "Administration", "333", "80000"};
    // insertRow(&myData, values);


    // Does not have to be in correct order
    // dict values[] = {{"Name", "Mr. Pebbles"}, {"DEPT", "\"Administration, and TONS\""}, {"Ext", "333"}, {"Salary,  dATA & ENtRies", "80000"}, {"emp_id", "8888"}, {"Test", "f"}};
    // myData.csv.rowDictWriter(&myData, values);
    // insertRowDict(&myData, values);

    // printf("Field: %s\n", myData.getFieldIdx(&myData, 3, 0));

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

    //      --- Strip ---
    // printf("'%s'\n", myData.getField(&myData, 1, "Name"));
    // stripField(&myData, 1, "Name");
    // printf("'%s'\n", myData.getField(&myData, 1, "Name"));
    // myData.stripColumn(&myData, "Name");
    // printStatsAll(&myData);
    // myData.stripColumnIdx(&myData, 2);
    // myData.stripAll(&myData);


    // myData.printData(&myData);
    // printStats(&myData, "all");

    // myData.printRowIndex = false;
    // printDataTable(&myData);

    // printf("Anna in Name: %d\n", myData.isInColumn(&myData, "Anna", "Name"));
    // printf("Anna in Name: %d\n", myData.valueCount(&myData, "Anna", "Name"));
    // myData.printColumnCond(&myData, "Name", "==", "Annie");

    // myData.csv.write_permission = true;
    // myData.csv.fieldWriter(&myData, 0, "Emp ID", "HELLO");
    // myData.csv.fieldWriterIdx(&myData, 1, 0, "TEST");

    // char *row_values[] = {NULL, "Rin,,,,go Starr", "S,,,,ales", NULL, "100000", "d"};
    // myData.csv.rowWriter(&myData, row_values);
    // myData.insertRow(&myData, row_values);
    // myData.csv.write_permission = true;

    // dict row_values[] = {{"Test", NULL}, {"\"Salary,  Data & Entries\"", "250000"}, {"Name", "John Lenn,,,on"}, {"Department", "Sales"}, {"Ext", NULL}, {"Emp ID", "3333"}};
    // myData.csv.rowDictWriter(&myData, row_values);
    // myData.insertRowDict(&myData, row_values);

    //      -- Drop Null --
    printf("Drop NULL\n\n");
    // dropNull(&myData, "name");
    // dropNullIdx(&myData, 1);
    dropNullAll(&myData);


    //          -- to JSON --
    // toJSONString(&myData);
    // writeJSON(&myData, NULL);
    // writeXML(&myData, NULL);

    // printf("\n\t\t---- JSON ----\n\n%s\n", myData.json);

    // //      -- PRINT --
    // myData.printData(&myData);
    myData.printDataTable(&myData);


    // myData.printStatsAll(&myData);
    myData.printStatsColumn(&myData, "salary, Salary & Salary_binned");

    // toXMLString(&myData);
    // printf("\tXML:\n%s\n", myData.xml);
    // myData.printPivotTable(&myData, "dept", "salary, Salary & Salary", "mean");

    //      --- End ---
    myData.freeAll(&myData);
    // salary.freeAll(&salary);

    return EXIT_SUCCESS;
}