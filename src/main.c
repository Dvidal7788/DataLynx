// CSV READER
#include <csv_reader.h>

int main(void)
{
    // ----- GET FILE NAME & OPEN FILE (Read Mode) ------

    // Declare filename and file pointers
    char *filename = NULL;
    FILE *file = NULL;

    // User input filename until a valid file is given
    while (true) {

        // Ask user for filename
        filename = inf_buffer("Enter file path to open: ");

        // Quit if user chooses to do so
        if (strcasecmp(filename, "quit") == 0) {printf("\nGOODBYE!\n\n"); free_null(&filename); return 1;}

        // Check if csv
        if (!is_ext(filename, ".csv")) {
            printf("\n'%s' is not a .csv file. Try again.\n", filename);
            free(filename);
            continue;
        }

        // Attempt to Open File (Read mode)
        file = fopen(filename, "r");
        if (file == NULL) {printf("error: file could not open. Check if file exists. Try again.\n"); free(filename);}
        else break;
    }



    // // V1 ----- ONE LONG STRING ----- (.txt or .csv)
    // // Read file into string
    // char *s = read_file_v1(file);

    // // Print
    // printf("FILE CONTENTS:\n%s\n", s);

    // // Convert string into 2D Array of strings (One row per string)
    // uintmax_t row_count;
    // char **main_array = string_into_2d_array(s, &row_count);

    // /* From here I can use split_by() to split 2D array by ',' into array of doubly linked lists, but for the sake of less redundancy, will do that in next section. */

    // // Print 2D Array
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     printf("%s\n", main_array[i]);
    // }

    //  // Free 2D array
    // for (uintmax_t i = 0; i < row_count; i++) {free_null(&main_array[i]);}

    // free(main_array);
    // main_array = NULL;

    // // --- END ---
    // fclose_null(&file);
    // free_null(&s);
    // free_null(&filename);


    // // V2 ----- 2D Dynamically Allocated JAGGED ARRAY (.txt or .csv) -----
    // uintmax_t row_count;
    // char **main_array = read_file_v2(file, &row_count);

    // // Print
    // printf("FILE CONTENTS:\n");
    // for (uint64_t i = 0; i < row_count; i++) {
    //     printf("%s\n", main_array[i]);
    // }

    // // Print Row
    // uintmax_t row, column;
    // while (true) {

    //     // Ask User which row
    //     printf("-- PRINT WHOLE ROW --\n");
    //     row = get_uint("\nSELECT ROW: ");

    //     if (row <= row_count-1) {
    //         break;
    //     }
    //     else printf("Row out of range. List is %lu rows long (i.e 0 - %lu).Try again.\n", row_count, row_count-1);
    // }
    // printf("%s\n\n", main_array[row]);

    // // -- Index into 2D Array like a csv ---
    // char *cell = NULL;
    // while (cell == NULL) {

    //     printf("-- INDEX INTO ARRAY --\n");
    //     row = get_uint("Row: ");

    //     if (row > row_count-1) {
    //         printf("Row out of range. List is %lu rows long (i.e 0 - %lu).\n", row_count, row_count-1);
    //         continue;
    //     }

    //     column = get_uint("Column: ");

    //     cell = index_2darray_csv(main_array, row, column);

    //     if (cell == NULL) printf("Index[%lu][%lu] out of range. Try again.\n", row, column);
    // }

    // printf("Index[%lu][%lu]: '%s'\n\n", row, column, cell);


    // // --- SPLIT 2D array by comma into array of doubly linked lists --
    // printf("-- 2D ARRAY SPLIT BY ',' (into array of doubly linked lists --\n");
    // node **main_dblink_array = split_by(main_array, row_count, ',');

    // // Print array of db link list
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     print_list(main_dblink_array[i]);
    // }

    // // Free array of db link lists
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     free_list(main_dblink_array[i]);
    // }

    // // Free 2D array
    // for (uintmax_t i = 0; i < row_count; i++) free_null(&main_array[i]);

    // // FREE --
    // free(main_dblink_array); /* cannot use free_null() to free node** or char**, bc would need node*** and char***, which does not exist */
    // main_dblink_array = NULL;

    // free(main_array);
    // main_array = NULL;

    // fclose_null(&file);
    // free_null(&cell);
    // free_null(&filename);


    // // ----- V3 - CSV READER INDEX -----
    //     // Index directly into csv without reading whole file into memory

    // // Loop until correct index is given
    // char *cell = NULL;
    // while (cell == NULL) {

    //     // User input row/column
    //     uintmax_t row = get_uint("Enter row: ");
    //     uintmax_t col = get_uint("Enter column: ");

    //     // Index into csv at correct row/column
    //     cell = csv_reader_index(file, row, col, true);

    //     // Print
    //     if (cell == NULL) printf("Index[%lu][%lu]: does not exist in '%s'. Try again.\n", row, col, filename);
    //     else printf("Index[%lu][%lu]: %s\n", row, col, cell);
    // }


    // // Free
    // fclose_null(&file);
    // free_null(&filename);
    // free_null(&cell);


    // // V4 - CSV DICT READER INDEX -
    //     // Index directly into csv like a dict (without reading whole file into memory)
    //     // Row is integer, Column is STRING (i.e. index into csv like a dict)

    // // Index directly into correct location in csv using row and column name/title
    // uintmax_t row;
    // char *col = NULL, *cell = NULL;

    // while (true) {

    //     // User input row and name/title of column to index into
    //     row = get_uint("Enter Row: ");
    //     col = inf_buffer("Enter Column Title: ");

    //     cell = csv_dictreader_index(file, row, col);

    //     if (cell != NULL) break;
    //     else printf("Index[%lu]['%s'] does not exist in '%s'. Try again.\n", row, col, filename);

    //     // Free current column string before looping back around and trying again
    //     free(col);
    // }

    // printf("\nIndex[%lu]['%s']: %s\n", row, col, cell);


    // // Free
    // fclose_null(&file);
    // free_null(&filename);
    // free_null(&col);
    // free_null(&cell);


    // --- V5 - FINAL - CSV DICT READER ----

    // Read csv into array of linked lists (i.e. dict behavior)
    uintmax_t row_count = 0;
    dict_node **main_array = csv_dict_reader(file, &row_count);
    if (main_array == NULL) {printf("Check if '%s' is blank.\n", filename);}

    // Set up row variable/ strings
    uintmax_t row;
    char *desired_column = NULL, *cell = NULL;

    // INDEX INTO ARRAY OF DICTS
    if (main_array != NULL)
    {
        // Print Whole List
        print_dict_list(main_array, row_count);

        // Loop until valid row/column is given
        while (cell == NULL) {

            // User Input Row/Column
            while ((row = get_uint("Enter Row: ")) > row_count-1) if (row > row_count-1) printf("Row %lu out of range. Try again.\n", row);
            desired_column = inf_buffer("Enter Column: ");

            // Attempt to index into array of dicts at given index
            cell = index_into_dict(main_array, row_count, row, desired_column);

            if (cell != NULL) break;
            else  {
                // Print error message and free cell before looping back around
                printf("Index[%lu]['%s'] does not exist. Try again.\n", row, desired_column);
                free_null(&cell);
                free_null(&desired_column);
            }

        }

        // Print
        printf("%s\n", cell);
    }


    // Free
    free_dict_list(main_array, row_count);
    free(main_array);
    fclose_null(&file);
    free_null(&filename);
    free_null(&cell);
    free_null(&desired_column);


    return EXIT_SUCCESS;
}