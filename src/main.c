#include "../include/csv_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



int main(void)
{
    // Get name of file to read from
    char *filename = NULL;
    while (true) {

        // Ask user for filename
        filename = inf_buffer("Enter file path to open: ");

        // Check if csv
        if (!is_ext(filename, ".csv")) {
            printf("\n'%s' is not a .csv file. Try again.\n", filename);
            free(filename);
        }
        else break;
    }


    // Open File (Read mode)
    FILE *file = fopen(filename, "r");
    if (file == NULL) {printf("error: file could not open. Check if file exists.\n"); free_null(&filename); return 1;}



    // // V1 ----- ONE LONG STRING ----- (.txt or .csv)
    // char *s = read_file_v1(file);

    // // Print
    // printf("FILE CONTENTS:\n%s\n", s);

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

    // // -- Index into 2D Array like a csv ---
    // // SELCECT ROW
    // uint64_t row_num;
    // while (true) {

    //     // Ask User which row
    //     row_num = get_uint("\nSELECT ROW:\nWhich ROW to print? ");

    //     if (row_num <= row_count-1) {
    //         break;
    //     }
    //     else printf("Row out of range. List is %lu rows long (i.e 0 - %lu).\n", row_count, row_count-1);
    // }
    // printf("%s\n", main_array[row_num]);

    // // SELECT INDEX (i.e. spreadsheet cell)
    // char *cell = NULL;
    // while (cell == NULL) {
    //     uint64_t index = get_uint("\nSELECT INDEX: ");

    //     cell = index_2darray_csv(main_array, row_num, index);

    //     if (cell == NULL) printf("%lu out of range. Try again.\n", index);
    // }

    // printf("CELL:\n%s\n", cell);


    // // --- SPLIT 2D array by comma into array of doubly linked lists --
    // printf("2D ARRAY SPLIT BY ',' (into array of doubly linked lists")
    // node **main_dblink_array = split_by(file, main_array, row_count, ',');

    // // Print array of db link list
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     print_list(main_dblink_array[i]);
    // }

    // // Free array of db link lists
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     free_list(main_dblink_array[i]);
    // }

    // // Free 2D array
    // for (uintmax_t i = 0; i < row_count; i++) {
    //     free(main_array[i]);
    //     main_array[i] = NULL;
    // }

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


    // FINAL - CSV DICT READER
    uintmax_t row_count = 0;
    dict_node **main_array = csv_dict_reader(file, &row_count);

    printf("ROW COUNT: %lu\n", row_count);

    // PRINT DICT LIST
    // Iterate through every row
    for (uintmax_t i = 0; i < row_count; i++) {

        // Traverse linked list
        dict_node *tmp = main_array[i];
        while (tmp != NULL) {
            printf("%s: %s\n", tmp->column_name, tmp->s);
            tmp = tmp->next;
        }
        printf("\n");
    }

    free_dict_list(main_array, row_count);
    free(main_array);
    fclose_null(&file);
    free_null(&filename);


    return EXIT_SUCCESS;
}
//////////////////////////////////////
// READ FILE V1 - ONE LONG STRING
char *read_file_v1(FILE *file)
{
    /* This function reads text based file (i.e. .csv, .txt etc) into one long string */
    // Allocate initial char
    char *s = malloc(sizeof(char));
    if (s == NULL) {printf("error: buffer error (malloc()).\n"); exit(2);}

    // Iterate through file char by char
    uint64_t i = 0;
    while (fscanf(file, "%c", &s[i]) == 1) {

        // Reallocate for next char
        s = realloc(s, sizeof(char)*(i+2));
        if (s == NULL) {printf("error: buffer error (realloc()).\n"); exit(3);}
        i++;
    }
    s[i] = '\0';

    return s;
}

// READ FILE V2 - 2D JAGGED ARRAY
char **read_file_v2(FILE *file, uintmax_t *row_count)
{
    /* This function: READS FILE INTO DYNAMICALLY ALLOCATED JAGGED 2D ARRAY */
    /* When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way */
    *row_count = 1;

    // Main Array
    char **main_array = (char**)malloc(sizeof(char*));
    if (main_array == NULL) {printf("error: buffer error (malloc)\n"); exit(1);}

    // Iterate through file
    while (true)
    {
        bool next_row = true;

        // Allocate 1st char of current row
        main_array[(*row_count)-1] = malloc(sizeof(char));
        if (main_array[(*row_count)-1] == NULL){printf("error: buffer error (malloc)\n"); exit(2);}

        int8_t tmp;
        uint64_t i = 0;
        while (true) {

            // Scan char:
                 /* - Not top tested, bc that would require duplicate realloc code after loop ends.
                    - Using tmp to test for EOF, bc whether char is signed or not is implementation dependent. */
            tmp = fgetc(file);

            // Check for end of row
            if (tmp == '\n') {
                main_array[(*row_count)-1][i] = '\0';
                break;
            }
            else if (tmp == EOF) {
                main_array[(*row_count)-1][i] = '\0';
                next_row = false;
                break;
            }
            else {
                // Assign value to current char in current row
                main_array[(*row_count)-1][i] = tmp;

                // Reallocate for next char
                main_array[(*row_count)-1] = realloc(main_array[(*row_count)-1], sizeof(char)*(i+2));
                if (main_array[(*row_count)-1] == NULL) {printf("error: buffer error (realloc)\n"); exit(3);}
                i++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            main_array = realloc(main_array, sizeof(char*)*((*row_count)+1));
            if (main_array == NULL) {printf("error: buffer error (realloc)\n"); exit(4);}
            (*row_count)++;
        }
        else break;
    }

    return main_array;
}
// --- CSV READER INDEX ----
char *csv_reader_index(FILE *file, uintmax_t row, uintmax_t col, bool skip_header)
{
    /* THIS FUNCTION: Allows you to index directly into a csv file, without reading the whole file into memory.
         - You can choose whether or not to skip the header (i.e. if header is not skipped, row 0 will be the header)
         - Returns NULL if nothing found at given index. */


    // Reset file stream
    fseek(file, 0, SEEK_SET);

    // Skip header or not
    if (skip_header) row++;

    // Declare variables
    int8_t tmp;
    bool reached_eof = false;

    // Get stream to correct row
    for (uintmax_t i = 0; i < row; i++)
    {
        while ((tmp = fgetc(file)) != '\n') if (tmp == EOF) {reached_eof = true; break;}

        if (reached_eof) break;
    }

    // Get stream to correct column
    bool end_of_row = false;
    for (uintmax_t j = 0; j < col; j++)
    {
        while ((tmp = fgetc(file)) != ',' && tmp != '\n') if (tmp == EOF) {reached_eof = true; break;}

        if (reached_eof) break;

        if (tmp == '\n') {end_of_row = true; break;}
    }

    // Create string from current index location
    char *s = NULL;
    if (!reached_eof && !end_of_row) {

        // Alocate 1st char of string
        s = malloc(sizeof(char));
        if (s == NULL) exit(1);

        uintmax_t i = 0;
        while ((s[i] = fgetc(file)) != ',' && s[i] != '\n' && s[i] != EOF) {

            // Realloc
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) exit(2);
            i++;
        }
        s[i] = '\0';
    }

    return s;
}
// CSV Dict Reader INDEX
char *csv_dictreader_index(FILE *file, uintmax_t row, char *desired_column)
{
    /* THIS FUNCTION: 1. Allows you to index directly into a csv file using the HEADER and without reading the whole file into memory.
                      2. Returns NULL if desired_column is not in header of file
                      3. This function will treat the 1st non-header row as row 0 (i.e. row 2 in the file = row 0, as it is the first non-header row and will be 0 indexed.) */


    // Reset file stream
    fseek(file, 0, SEEK_SET);

    // Allocate 1st char of string for current column
    char *cursor = malloc(sizeof(char));
    if (cursor == NULL) exit(1);

    uintmax_t i, correct_column = 0;
    int8_t tmp;
    bool end_of_header = false, found_column = true;

    // Iterate through CSV HEADER to find correct column
    while (true) {

        // Iterate until end of current column (i.e. until ',', '\n' or EOF)
        i = 0;
        while ((cursor[i] = fgetc(file)) != ',' && cursor[i] != '\n' && cursor[i] != EOF) {

            // Reallocate
            cursor = realloc(cursor, sizeof(char)*(i+2));
            if (cursor == NULL) exit(2);
            i++;
        }
        if (cursor[i] == '\n' || cursor[i] == EOF) end_of_header = true;

        cursor[i] = '\0';

        // Compare strings
        if (strcmp(cursor, desired_column) == 0) break;
        else if (end_of_header) {found_column = false; break;}

        correct_column++;
    }

    free_null(&cursor);

    // Now that we have correct column, we can use csv_reader_index() to index directly into csv without reading into memory
    if (found_column)
        return csv_reader_index(file, row, correct_column, true);
    else return NULL;
}

// ---- SPLIT BY ----
node **split_by(FILE *file, char **orig_array, uintmax_t row_count, char split)
{
    /* THIS FUNCTION: 1. Takes dynamically allocated 2D array
                      2. Returns array of double link lists, separated by whatever char is stored in split (commonly split by ',' for csv-like behavior, but could be anything) */

    /* Note: When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way. */

    // Allocate array of double link lists
    node **main_dblnk_array = malloc(sizeof(node*));
    if (main_dblnk_array == NULL) exit(2);

    // Iterate through whole array
    uintmax_t i = 0;
    while (true)
    {
        bool next_row = true;

        // Double linked list main pointers for current row
        node *last = NULL;
        main_dblnk_array[i] = NULL;
        char *tmp_str = NULL;

        uintmax_t j = 0;
        uintmax_t ii = 0;

        // Iterate through each ROW
        while (true) {

            // Allocate 1st char of tmp string
            if (ii == 0) {
                tmp_str = malloc(sizeof(char));
                if (tmp_str == NULL) exit(1);
            }

            tmp_str[ii] = orig_array[i][j];

            // Check for end of row
            if (tmp_str[ii] == '\0') {
                // Check if end of 2d array (this is why row_count is required as a parameter for this function. Otherwise will touch unallocated memory)
                build_dblink_list(&tmp_str, &main_dblnk_array[i], &last);
                if (i == row_count-1) {next_row = false;}
                break;
            }
            else if (tmp_str[ii] == split) {
                tmp_str[ii] = '\0';
                build_dblink_list(&tmp_str, &main_dblnk_array[i], &last);
                ii = 0;
                j++;
            }
            else {

                // Reallocate for next char
                tmp_str = realloc(tmp_str, sizeof(char)*(ii+2));
                if (tmp_str == NULL) {printf("error: buffer error (realloc)\n"); exit(3);}
                j++; ii++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            main_dblnk_array = realloc(main_dblnk_array, sizeof(node*)*(i+2));
            if (main_dblnk_array == NULL) {printf("error: buffer error (realloc)\n"); exit(4);}
            i++;
        }
        else break;
    }

    return main_dblnk_array;
}

//      -----  GET_UINT()  -----
uint64_t get_uint(char *prompt)
{
    char *s = NULL;
    // Loop until unsigned integer is given
    while (true) {

        bool is_int = true;

        // Scan command line
        s = inf_buffer(prompt);

        // Check if unsigned integer
        for (uint64_t i = 0, length = strlen(s); i < length; i++) {
            if (!isdigit(s[i])) {
                is_int = false;
                printf("\nError: unsinged integers only please.\n");
                break;
            }
        }

        if (is_int) break;

        free(s);
    }

    uint64_t x = atoi(s);

    free(s);
    s = NULL;

    return x;
}
//      -----  IS_EXT()  -----
bool is_ext(char *filename, char *ext)
{
    /* THIS FUNCTION: Checks if filename has correct file extension of your choice */

    // Check that proper argument for file extension has been passed through
    if (ext[0] != '.') return false;

    // Make sure not to dereference NULL pointer
    if (filename == NULL) return false;

    // Check if filename is at least 5 chars long (ex: ".csv" plus at least 1 char before ".csv")
        //  This avoids buffer underrun once we backup 4 chars in next section
    if (strlen(filename) < 5) return false;

    // Return true if filename has correct extension
    if (strcmp(&filename[strlen(filename)-4], ext) == 0) {
        return true;
    }
    else return false;
}

//      -----  2darray_csv_index()  -----
char *index_2darray_csv(char **main_array, uintmax_t row_num, uintmax_t index)
{
    // This function helps you index into a 2d array as if indexes were seperated by comma (i.e. csv) instead of char by char

    // Get to correct index in row
    uint64_t j = 0, cursor = 0;
    while (cursor < index) {
        while (main_array[row_num][j] != ',' && main_array[row_num][j] != '\0') {j++;}
        if (main_array[row_num][j] != '\0') {
            cursor++;
            j++;
        }
        else break;
    }

    if (main_array[row_num][j] == '\0') return NULL;

    if (main_array[row_num][j] == ',') j++;

    // Put index into own string
    char *cell_str = malloc(sizeof(char));
    if (cell_str == NULL) exit(1);

    // Iterate until next cell (i.e ',') is found or end of row (i.e. '\0')
    uint64_t str_i = 0;
    while (main_array[row_num][j] != ',' && main_array[row_num][j] != '\0') {

        // Add to string char by char
        cell_str[str_i] = main_array[row_num][j];

        // Realloc
        cell_str = realloc(cell_str, sizeof(char)*(str_i+2));
        if (cell_str == NULL) exit(2);

        str_i++;
        j++;
    }
    cell_str[str_i] = '\0';

    return cell_str;
}

// ---------- GET CSV HEADER ----------
char **get_csv_header(FILE *file, uintmax_t *column_count)
{
    /* THIS FUNCTION: 1. Reads csv header into dynamically allocated array of string pointers
                      2. Returns array of strings as well as "returns" column_count (via accepting a pointer to column_count as input) for ability to print/free etc array without overrunning buffer
                      2. The main purpose of this function is for use in calling function csv_dict_reader(), however, of course could be used anywhere. */

    // Set column count to zero incase programmer has not already done so in calling function
    *column_count = 0;

    // Reset file stream
    fseek(file, 0, SEEK_SET);

    // Allocate first pointer in array of strings
    char **header = malloc(sizeof(char*));
    if (header == NULL) exit(1);

    char *s = NULL;

    // Iterate through WHOLE HEADER
    bool end_of_header = false;
    while (true) {

        // Allocate first char of string for current column in header
        s = malloc(sizeof(char));
        if (s == NULL) exit(2);

        // Iterate until end of current index
        uintmax_t i = 0;
        while (true) {

            // Scan char
            if (fscanf(file, "%c", &s[i]) != 1) {
                end_of_header = true;
                break;
            }

            // Check if at next column or end of header
            if (s[i] == ',' || s[i] == '\n') {
                (*column_count)++;
                if (s[i] == '\n') {end_of_header = true;}
                break;
            }
            else {
                s = realloc(s, sizeof(char)*(i+2));
                if (s == NULL) exit(3);
                i++;
            }
        }
        // Add nul to string and add string to array of string pointers
        s[i] = '\0';
        if (*column_count > 0) {
            header[(*column_count)-1] = s;
        }
        else free_null(&s);

        // Realloc array
        if (!end_of_header) {
            header = realloc(header, sizeof(char*)*((*column_count)+1));
            if (header == NULL) exit(4);
        }
        else break;
    }

    return header;
}

//      _____ CSV DICT READER _____
dict_node **csv_dict_reader(FILE *file, uintmax_t *row_count)
{
    // Get csv HEADER
    uintmax_t column_count = 0;
    char **header = get_csv_header(file, &column_count);

    // // Print Header
    // printf("COLUMN COUNT: %lu\n", column_count);
    // for (uintmax_t i = 0; i < column_count; i++) {
    //     printf("%s\n", header[i]);
    // }

    // MAIN ARRAY POINTER
    dict_node **main_array = (dict_node**)malloc(sizeof(dict_node*));
    if (main_array == NULL) exit(1);

    // Variables
    *row_count = 1;
    bool end_of_file = false;

    // ITERATE THROUGH REST OF CSV
    while (!end_of_file) {
        main_array[(*row_count)-1] = NULL;

        // Variables for each row
        bool end_of_row = false;
        dict_node *last = NULL;
        uintmax_t current_column = 0;

        // Iterate until end of row
        while (true) {

            // Allocate 1st char of string (this string will be taken over by double linked list, so do not free)
            char *s = malloc(sizeof(char));
            if (s == NULL) exit(1);

            uintmax_t i = 0;
            // For Each Column, Iterate until next column found
            while (true) {

                // Scan char
                if (fscanf(file, "%c", &s[i]) != 1) {
                    end_of_file = end_of_row = true;
                    break;
                }

                // Check if found next column or end of row
                if (s[i] == ',') {
                    break;
                }
                else if (s[i] == '\n') {
                    end_of_row = true;
                    break;
                }
                else {
                    // Reallocate next char in string
                    s = realloc(s, sizeof(char)*(i+2));
                    if (s == NULL) exit(2);
                    i++;
                }
            }
            s[i] = '\0'; // Do not free this string. It will be taken over by node in link list and freed when link list is freed.

            // Append to linked list
            if (strlen(s) >= 1)
                build_dict_link_list(&s, &main_array[(*row_count)-1], &last, header[current_column++]);
            else {
                (*row_count)--;
                free_null(&s);
            }
            if (end_of_row) break;
        }

        // Reallocate array of strings for next string
        if (!end_of_file) {
            // Realloc
            main_array = realloc(main_array, sizeof(dict_node*)*(++(*row_count)));
            if (main_array == NULL) exit(3);
        }
        else break;
    }
    // (*row_count)--; // this only works if csv ends with empty line

    // Free header array of strings
    for (uintmax_t i = 0; i < column_count; i++) {
        free_null(&header[i]);
    }

    free(header);
    header = NULL;

    return main_array;
}
