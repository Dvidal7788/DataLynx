#include <csv_reader.h>

// EXAMPLE CODE FOR CSV LIBRARY
int main(void)
{
    // ----- GET FILE NAME & OPEN FILE (Read Mode) ------
    // Setup section will be the same for all example code versions

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
    // End of set up section



    // // EXAMPLE CODE V1 ----- ONE LONG STRING ----- (.txt or .csv)

    // // Get header 1st, so I can keep file contents separate
    // uint64_t column_count;
    // char **header = get_csv_header(file, &column_count);

    // // Read file into string
    // char *s = read_file_v1(file, true);

    // // PRINT
    // printf("FILE CONTENTS:\n");

    // // Print header
    // print_header(header, &column_count, false);

    // // Print file contents
    // printf("%s\n", s);

    // // Convert string into 2D Array of strings (One row per string)
    // uintmax_t row_count;
    // char **main_array = string_into_2d_array(s, &row_count);

    // /* From here I can use split__2d_array_by() to split 2D array by ',' into array of doubly linked lists, but for the sake of less redundancy, will do that in next section. */

    // // Print header
    // print_header(header, &column_count, true);

    // // Print 2D Array
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     printf("%s\n", main_array[i]);
    // }

    // //      --- FREE ---

    // // Free header
    // for (uint16_t i = 0; i < column_count; i++) free_null(&header[i]);
    // free(header);
    // header = NULL;

    //  // Free 2D array
    // for (uintmax_t i = 0; i < row_count; i++) {free_null(&main_array[i]);}

    // free(main_array);
    // main_array = NULL;

    // fclose_null(&file);
    // free_null(&s);
    // free_null(&filename);


    // // EXAMPLE CODE V2 ----- Dynamically Allocated 2D ARRAY (.txt or .csv) -----
    // uintmax_t row_count, column_count;
    // char **header = get_csv_header(file, &column_count);
    // char **main_array = read_file_v2(file, &row_count, true);

    // // PRINT
    // printf("FILE CONTENTS:\n");

    // // Print header
    // for (uint16_t i = 0; i < column_count; i++) {
    //     if (i < column_count - 1) printf("%s, ", header[i]);
    //     else printf("%s\n", header[i]);
    // }

    // // Print file contents
    // for (uint64_t i = 0; i < row_count; i++) printf("%s\n", main_array[i]);

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

    // // Print individual requested field (do not print single quotes around field if field has quotes)
    // if (!has_quotes(cell)) printf("Index[%lu][%lu]: '%s'\n\n", row, column, cell);
    // else printf("Index[%lu][%lu]: %s\n\n", row, column, cell);


    // // --- SPLIT 2D array by comma into array of doubly linked lists --
    // printf("-- 2D ARRAY SPLIT BY ',' (into array of doubly linked lists --\n");
    // node **main_dblink_array = split_2darray_by(main_array, row_count, ',');

    // // Print array of db link list
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     print_list(main_dblink_array[i]);
    // }


    // //      -- FREE --
    // // Free header
    // for (uint16_t i = 0; i < column_count; i++) free_null(&header[i]);

    // free(header); /* cannot use free_null() to free node** or char**, bc would need node*** and char***, which does not exist */
    // header = NULL;

    // // Free array of db link lists
    // for (uintmax_t i = 0; i < row_count; i++) free_list(main_dblink_array[i]);

    // free(main_dblink_array); /* cannot use free_null() to free node** or char**, bc would need node*** and char***, which does not exist */
    // main_dblink_array = NULL;

    // // Free 2D array
    // for (uintmax_t i = 0; i < row_count; i++) free_null(&main_array[i]);

    // free(main_array);
    // main_array = NULL;

    // fclose_null(&file);
    // free_null(&cell);
    // free_null(&filename);


    // // ----- EXAMPLE CODE V3 - CSV READER INDEX -----
    //     // Index directly into csv without reading whole file into memory

    // // Loop until correct index is given
    // char *cell = NULL;
    // uintmax_t row, column;
    // while (cell == NULL) {

    //     // User input row/column
    //     row = get_uint("Enter row: ");
    //     column = get_uint("Enter column: ");

    //     // Index into csv at correct row/column
    //     cell = csv_reader_index(file, row, column, true);

    //     // Print
    //     if (cell == NULL) printf("Index[%lu][%lu]: does not exist in '%s'. Try again.\n", row, column, filename);
    //     else printf("Index[%lu][%lu]: %s\n", row, column, cell);
    // }

    // // ----- UPDATE INDEX -----
    // char *yes_or_no = NULL;
    // while (true) {
    //     // User decides if they want to update cell or not
    //     yes_or_no = inf_buffer("Would you like to update this index? (yes or no): ");

    //     if (strcasecmp(yes_or_no, "no") == 0) {
    //         break;
    //     }
    //     else if (strcasecmp(yes_or_no, "yes") == 0) {

    //         // Close file before passing filename to update_csv_index(). This is not technically neccessary, but best practice.
    //         fclose_null(&file);

    //         // User input new cell
    //         printf("UPDATE INDEX[%lu][%lu]: ", row, column);
    //         char *new_cell = inf_buffer("");

    //         update_csv_index(filename, row, column, new_cell);
    //         free_null(&new_cell);
    //         break;
    //     }
    //     else {
    //         // Free yes_or_no before looping back around
    //         free_null(&yes_or_no);
    //         printf("\nPlease type 'yes' or 'no' only.\n");
    //     }
    // }

    // // Free
    // if (strcasecmp(yes_or_no, "no") == 0) fclose_null(&file);
    // free_null(&filename);
    // free_null(&cell);
    // free_null(&yes_or_no);


    // // EXAMPLE CODE V4 - CSV DICT READER INDEX -
    //     // Index directly into csv like a dict (without reading whole file into memory)
    //     // Row is integer, Column is STRING (i.e. index into csv like a dict)

    // // Index directly into correct location in csv using row and column name/title
    // uintmax_t row;
    // char *column = NULL, *cell = NULL;

    // while (true) {

    //     // User input row and name/title of column to index into
    //     row = get_uint("Enter Row: ");
    //     column = inf_buffer("Enter Column Title: ");

    //     cell = csv_dictreader_index(file, row, column);

    //     if (cell != NULL) break;
    //     else {

    //         // Error message
    //         printf("\nIndex[%lu]['%s'] does not exist in '%s'. Try again.\n", row, column, filename);

    //         // Get correct column names
    //         uintmax_t column_count;
    //         char **header = get_csv_header(file, &column_count);

    //         if (verify_column(header, column_count, column)) {
    //             printf("Row is out of range.\n\n");
    //         }
    //         else {
    //             // Display correct column names - (print/free strings in header)
    //             printf("\n** Incorrect column name ** THE CORRECT COLUMN NAMES ARE:\n");
    //             for (uint16_t i = 0; i < column_count; i++) {
    //                 if (i < column_count-1) {printf("%s,", header[i]); free_null(&header[i]);}
    //                 else {printf("%s\n\n", header[i]); free_null(&header[i]);}
    //             }
    //             free(header);
    //             header = NULL;
    //         }
    //     }

    //     // Free current column string before looping back around and trying again
    //     free(column);
    // }

    // printf("\nIndex[%lu]['%s']: %s\n", row, column, cell);


    // // Free
    // fclose_null(&file);
    // free_null(&filename);
    // free_null(&column);
    // free_null(&cell);


    // --- EXAMPLE CODE V5 - FINAL - CSV DICT READER ----

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
            while ((row = get_uint("Enter Row: ")) > row_count-1) if (row > row_count-1) printf("\nError: Row %lu out of range. Try again.\n\n", row);
            desired_column = inf_buffer("Enter Column Title: ");

            // Attempt to index into array of dicts at given index
            cell = index_into_dict(main_array, row_count, row, desired_column);

            if (cell != NULL) break;
            else  {

                // Error message - invalid column name
                printf("\n*Error: Index[%lu]['%s'] does not exist. Try again.\n\n", row, desired_column);

                // Display correct column names
                uintmax_t column_count;
                char **header = get_csv_header(file, &column_count);

                printf("*THE CORRECT COLUMN NAMES ARE:\n");
                for (uintmax_t i = 0; i < column_count; i++) {
                    if (i < column_count-1) printf("%s,", header[i]);
                    else printf("%s\n\n", header[i]);
                }

                // Free before looping back around
                for (uintmax_t i = 0; i < column_count; i++) free_null(&header[i]);
                free(header); header = NULL;
                free_null(&cell);
                free_null(&desired_column);
            }

        }

        // Print
        printf("Index:[%lu]['%s']: %s\n", row, desired_column, cell);
    }

    // ----- UPDATE INDEX in DICT AND CSV -----
    char *yes_or_no = NULL, *new_cell = NULL;
    while (true) {

        // User decides if they want to update cell or not
        yes_or_no = inf_buffer("Would you like to update this index? (yes or no): ");

        if (strcasecmp(yes_or_no, "no") == 0) {
            break;
        }
        else if (strcasecmp(yes_or_no, "yes") == 0) {

            // User input new cell
            printf("UPDATE INDEX[%lu]['%s']: ", row, desired_column);
            new_cell = inf_buffer("");

            // Update dict
            update_dict_index(main_array, row_count, row, desired_column, new_cell);

            // REWRITE TO FILE
            // Get header 2D array while still in read mode
            uintmax_t column_count = 0;
            char **header = get_csv_header(file, &column_count);

            fclose(file);
            file = fopen(filename, "w");
            if (file == NULL) {printf("Error: fopen failed."); return 4;}

            // Rewrite header to file & free header
            for (uintmax_t i = 0; i < column_count; i++) {
                if (i < column_count-1) fprintf(file, "%s,", header[i]);
                else fprintf(file, "%s\n", header[i]);

                free_null(&header[i]);
            }
            // Can not use free_null() on char ** (would need char ***)
            free(header);
            header = NULL;


            // Each row
            for (uintmax_t i = 0; i < row_count; i++) {

                // Traverse linked list (columns)
                dict_node *tmp = main_array[i];
                while (tmp != NULL) {
                    if (tmp->next != NULL) fprintf(file, "%s,", tmp->s);
                    else fprintf(file, "%s\n", tmp->s);
                    tmp = tmp->next;
                }
            }
            break;
        }
        else {
            // Free yes_or_no before looping back around
            free_null(&yes_or_no);
            printf("\nPlease type 'yes' or 'no' only.\n");
        }
    }

    // Free
    free_dict_list(main_array, row_count);
    free(main_array);
    fclose_null(&file);
    free_null(&filename);
    free_null(&cell);
    free_null(&desired_column);
    free_null(&yes_or_no);


    return EXIT_SUCCESS;
}