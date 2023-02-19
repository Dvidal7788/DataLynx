#include <csv_reader.h>

// ___ BUILD DOUBLE LINK LIST ___
void build_dblink_list(char **s_ptr, node **head, node **last)
{
    /* THIS FUNCTION: Builds doubly linked list. It is called in split_2darray_by() */
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string

    // -- APPEND NODE --
    // Create new node
    node *n = malloc(sizeof(node));
    if (n == NULL) {if_error(MALLOC_FAILED, "build_dblink_list");}

    // This string pointer in the node takes over the allocated string buffer (i.e. **s_ptr).
    n->s = *s_ptr;
    *s_ptr = NULL;
    s_ptr = NULL;
    n->prev = n->next = NULL;

    // Append node to linked list
    if (*head == NULL) {
        *head = *last = n;
    }
    else {
        (*last)->next = n;
        n->prev = *last;
        *last = n;
    }

    return;
}


// ___ BUILD - DICT - DOUBLE LINK LIST ___
void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name)
{
    // Function name for use in if_error()
    const char *func_name = "build_dict_link_list";

    /* THIS FUNCTION: Builds an array of doubly linked lists that acts an array of dicts. It is called from csv_dict_reader() */
    // string input changed to **:
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string
    // -- APPEND NODE --
    // Create new node
    dict_node *n = malloc(sizeof(dict_node));
    if (n == NULL) {if_error(MALLOC_FAILED, func_name);}

    // This string pointer in the node takes over the allocated string buffer (i.e. **s_ptr).
    n->s = *s_ptr;
    *s_ptr = NULL;
    s_ptr = NULL;

    n->column_name = malloc(sizeof(char)*strlen(current_column_name)+1);
    if (n->column_name == NULL) {if_error(MALLOC_FAILED, func_name);}
    strcpy(n->column_name, current_column_name);

    n->prev = n->next = NULL;

    // Append node to linked list
    if (*head == NULL) {
        *head = *last = n;
    }
    else {
        (*last)->next = n;
        n->prev = *last;
        *last = n;
    }

    return;
}


//      ___ READ FILE ___ (V1 - ONE LONG STRING)
char *read_file_v1(FILE *file, bool skip_header)
{
    // Function name for use in if_error()
    const char *func_name = "read_file_v1";

    // Get file stream to beginning, so this function works no matter what happened before
    fseek(file, SEEK_SET, 0);

    // Get passed header if programmer decided to do so
    if (skip_header) {
        int8_t tmp = 0;
        while (tmp != '\n') fscanf(file, "%c", &tmp);
    }

    /* THIS FUNCTION: Reads a text based file (i.e. .csv, .txt etc) into one long string */

    // Allocate initial char
    char *s = malloc(sizeof(char));
    if (s == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Iterate through file char by char
    uint64_t i = 0;
    while (fscanf(file, "%c", &s[i]) == 1) {

        // Reallocate for next char
        s = realloc(s, sizeof(char)*(i+2));
        if (s == NULL) {if_error(REALLOC_FAILED, func_name);}
        i++;
    }
    s[i] = '\0';

    return s;
}

//      ___ STRING to 2D Array ___ (V2 - 2D JAGGED ARRAY)
char **string_into_2d_array(char *s, uintmax_t *row_count)
{
    // Function name for use in if_error()
    const char *func_name = "string_into_2d_array";

    /* THIS FUNCTION: Turns string INTO DYNAMICALLY ALLOCATED JAGGED 2D ARRAY (splits by '\n') */
    /* When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way */
    *row_count = 1;

    // Main Array
    char **main_array = (char**)malloc(sizeof(char*));
    if (main_array == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Iterate through string
    uintmax_t str_index = 0;
    while (true)
    {
        bool next_row = true;

        // Allocate 1st char of current row
        main_array[(*row_count)-1] = malloc(sizeof(char));
        if (main_array[(*row_count)-1] == NULL) {if_error(MALLOC_FAILED, func_name);}

        uintmax_t i = 0;
        while (true) {

            // Check for end of row
            if (s[str_index] == '\n') {
                main_array[(*row_count)-1][i] = '\0';
                break;
            }

            // Check end of string
            else if (s[str_index] == '\0') {
                main_array[(*row_count)-1][i] = '\0';

                // Get rid of empty last row
                if (main_array[(*row_count)-1][0] == '\0') free_null(&main_array[(*row_count)-1]);
                (*row_count)--;
                next_row = false;
                break;
            }
            else {
                // Assign value to current char in current row
                main_array[(*row_count)-1][i] = s[str_index];

                // Reallocate for next char
                main_array[(*row_count)-1] = realloc(main_array[(*row_count)-1], sizeof(char)*(i+2));
                if (main_array[(*row_count)-1] == NULL) {if_error(REALLOC_FAILED, func_name);}
                i++;
                str_index++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            main_array = realloc(main_array, sizeof(char*)*((*row_count)+1));
            if (main_array == NULL) {if_error(REALLOC_FAILED, func_name);}
            (*row_count)++;
            str_index++;
        }
        else break;
    }

    return main_array;
}

//      ___ READ FILE ___ (V2 - 2D JAGGED ARRAY)
char **read_file_v2(FILE *file, uintmax_t *row_count, bool skip_header)
{
    /* THIS FUNCTION: READS FILE INTO DYNAMICALLY ALLOCATED JAGGED 2D ARRAY (Each row in file corresponds to string in array of strings) */

    // Function name for use in if_error()
    const char *func_name = "read_file_v2";

    // Get file stream to beginning, so this function works no matter what happened before
    fseek(file, SEEK_SET, 0);

    // Get passed header if programmer decided to do so
    if (skip_header) {
        int8_t tmp = 0;
        while (tmp != '\n') fscanf(file, "%c", &tmp);
    }

    /* When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way */
    *row_count = 1;

    // Main Array
    char **main_array = (char**)malloc(sizeof(char*));
    if (main_array == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Iterate through file
    while (true)
    {
        bool next_row = true;

        // Allocate 1st char of current row
        main_array[(*row_count)-1] = malloc(sizeof(char));
        if (main_array[(*row_count)-1] == NULL) {if_error(MALLOC_FAILED, func_name);}

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

                // Get rid of empty row
                if (main_array[(*row_count)-1][0] == '\0') {
                    free_null(&main_array[(*row_count)-1]);
                    (*row_count)--;
                }

                next_row = false;
                break;
            }
            else {
                // Assign value to current char in current row
                main_array[(*row_count)-1][i] = tmp;

                // Reallocate for next char
                main_array[(*row_count)-1] = realloc(main_array[(*row_count)-1], sizeof(char)*(i+2));
                if (main_array[(*row_count)-1] == NULL) {if_error(REALLOC_FAILED, func_name);}
                i++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            main_array = realloc(main_array, sizeof(char*)*((*row_count)+1));
            if (main_array == NULL) {if_error(REALLOC_FAILED, func_name);}
            (*row_count)++;
        }
        else break;
    }

    return main_array;
}


//      -----  2darray_csv_index()  -----
char *index_2darray_csv(char **main_array, uintmax_t row, uintmax_t column)
{
    // Function name for use in if_error()
    const char *func_name = "index_2darray_csv";

    /* THIS FUNCTION: Helps you index into a 2D array as if indexes were seperated by comma (i.e. csv) instead of char by char */

    // Get to correct index in row
    uint64_t j = 0, cursor = 0;
    bool inside_quotes = false;
    while (cursor < column) {

        // Get to next column
        // while (main_array[row][j] != ',' && main_array[row][j] != '\0') {j++;}

        // Determine if inside quotes
        if (!inside_quotes && main_array[row][j] == '"') {inside_quotes = true; j++;}
        else if (inside_quotes && main_array[row][j] == '"') {inside_quotes = false; j++;}

        // Get to next column
        if (!inside_quotes) {while (main_array[row][j] != ',') j++;}
        else if (inside_quotes) {while (main_array[row][j] != '"') j++;}

        if (main_array[row][j] != '\0') {
            cursor++;
            j++;
        }
        else break;
    }

    if (main_array[row][j] == '\0') return NULL;

    if (main_array[row][j] == ',') j++;

    // Put index into own string
    char *cell_str = malloc(sizeof(char));
    if (cell_str == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Iterate until next cell (i.e ',') is found or end of row (i.e. '\0')
    uint64_t str_i = 0;
    if (main_array[row][j] == '"') inside_quotes = true;
    bool break_after_realloc = false;
    while (main_array[row][j] != '\0') {

        // Determine whether to break
        if (!inside_quotes && main_array[row][j] == ',') break;
        else if (inside_quotes && main_array[row][j] == '"' && str_i != 0) break_after_realloc = true;

        // Add to string char by char
        cell_str[str_i] = main_array[row][j];

        // Realloc
        cell_str = realloc(cell_str, sizeof(char)*(str_i+2));
        if (cell_str == NULL) {if_error(REALLOC_FAILED, func_name);}

        str_i++;
        j++;

        if (break_after_realloc) break;
    }
    cell_str[str_i] = '\0';

    return cell_str;
}


//      ___ SPLIT BY ___
node **split_2darray_by(char **orig_array, uintmax_t row_count, char split_by)
{
    // Function name for use in if_error()
    const char *func_name = "split_2daray_by";

    /* THIS FUNCTION: 1. Takes dynamically allocated 2D array
                      2. Returns array of double link lists, separated by whatever char is stored in split (commonly split by ',' for csv-like behavior, but could be anything) */

    /* Note: When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way. */

    // Allocate main array of double link lists
    node **main_dblnk_array = malloc(sizeof(node*));
    if (main_dblnk_array == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Iterate through whole 2D array
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

        bool inside_quotes = false;

        // Iterate through each ROW
        while (true) {

            // Allocate 1st char of tmp string
            if (ii == 0) {
                tmp_str = malloc(sizeof(char));
                if (tmp_str == NULL) if_error(MALLOC_FAILED, func_name);
            }

            tmp_str[ii] = orig_array[i][j];

            if (!inside_quotes && orig_array[i][j] == '"') inside_quotes = true;
            else if (inside_quotes && orig_array[i][j] == '"') inside_quotes = false;

            // Check for end of row
            if (tmp_str[ii] == '\0') {
                build_dblink_list(&tmp_str, &main_dblnk_array[i], &last);

                // Check if end of 2d array (this is why row_count is required as a parameter for this function. Otherwise will touch unallocated memory)
                if (i == row_count-1) {next_row = false;}
                break;
            }
            else if (!inside_quotes && split_by == ',' && tmp_str[ii] == split_by) {
                tmp_str[ii] = '\0';
                build_dblink_list(&tmp_str, &main_dblnk_array[i], &last);
                ii = 0;
                j++;
            }
            else if (split_by != ',' && tmp_str[ii] == split_by) {
                tmp_str[ii] = '\0';
                build_dblink_list(&tmp_str, &main_dblnk_array[i], &last);
                ii = 0;
                j++;
            }
            else {

                // Reallocate for next char
                tmp_str = realloc(tmp_str, sizeof(char)*(ii+2));
                if (tmp_str == NULL) {if_error(REALLOC_FAILED, func_name);}
                j++; ii++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            main_dblnk_array = realloc(main_dblnk_array, sizeof(node*)*(i+2));
            if (main_dblnk_array == NULL) {if_error(REALLOC_FAILED, func_name);}
            i++;
        }
        else break;
    }

    return main_dblnk_array;
}


//      ___ CSV READER INDEX ___
char *csv_reader_index(FILE *file, uintmax_t row, uintmax_t col, bool skip_header)
{
    // Function name for use in if_error()
    const char *func_name = "csv_reader_index";

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
    bool end_of_row = false, inside_quotes = false;

    for (uintmax_t j = 0; j < col; j++)
    {
        while ((tmp = fgetc(file)) != '\n') {

            if (tmp == '"' && inside_quotes == false) inside_quotes = true;
            else if (tmp == '"') inside_quotes = false;

            // Break conditions
            if (tmp == ',' && inside_quotes == false) break;

            if (tmp == EOF) {reached_eof = true; break;}
        }

        if (reached_eof) break;

        if (tmp == '\n') {end_of_row = true; break;}
    }

    // Create string from current index location
    char *s = NULL;
    if (!reached_eof && !end_of_row) {

        // Alocate 1st char of string
        s = malloc(sizeof(char));
        if (s == NULL) {if_error(MALLOC_FAILED, func_name);}

        uintmax_t i = 0;

        // In a field without quotes
        while ((s[i] = fgetc(file)) != '\n' && s[i] != EOF) {

            if (!inside_quotes && s[i] == '"') inside_quotes = true;
            else if (s[i] == '"') inside_quotes = false;

            if (s[i] == ',' && inside_quotes == false) break;

            // Realloc
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) {if_error(REALLOC_FAILED, func_name);}
            i++;
        }

        s[i] = '\0';
    }

    return s;
}


//      ___ UPDATE_CSV_INDEX() ___
void update_csv_index(char *filename, uintmax_t row, uintmax_t column, char *new_cell)
{
    // Function name for use in if_error()
    const char *func_name = "update_csv_index";

    /* THIS FUNCTION: Updates csv file based on choice of index
            Note: This function takes filename string instead of file pointer as input, to be able to open in read mode, then close and open in write mode.
                    This would not be possible with a single file pointer. */

    // Make new_cell into a dynamically allocated string (so I can pass the address of string through add_quotes() and resize if necessary)
    char *new_cell_dyn = malloc(sizeof(char) * strlen(new_cell)+1);
    if (new_cell_dyn == NULL) if_error(MALLOC_FAILED, func_name);
    strcpy(new_cell_dyn, new_cell);

    // Open file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {if_error(FOPEN_FAILED, func_name);}

    // Allocate initial char for buffer to read file contents into
    char *s = malloc(sizeof(char));
    if (s == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Iterate through HEADER char by char
    uint64_t i = 0;
    while (fscanf(file, "%c", &s[i]) == 1) {

        if (s[i] == '\n') break;

        // Reallocate for next char
        s = realloc(s, sizeof(char)*(i+2));
        if (s == NULL) {if_error(REALLOC_FAILED, func_name);}
        i++;
    }

    // Reallocate for next char for priming of next loop
    s = realloc(s, sizeof(char)*(i+2));
    if (s == NULL) {if_error(REALLOC_FAILED, func_name);}
    i++;

    // Variables to keep track of current row/column to match with input row/column
    uintmax_t current_row = 0, current_column = 0;
    bool found_correct_index = false, inside_quotes = false;

    // Iterate through REST OF FILE char by char
    while (fscanf(file, "%c", &s[i]) == 1) {

        // FOUND Correct index to update
        if (current_row == row && current_column == column && !found_correct_index) {

            found_correct_index = true;

            // Determine current field is inside quotes
            if (!inside_quotes && s[i] == '"') inside_quotes = true;

            // Add quotes to new cell if necessary
            if (!has_quotes(new_cell) && has_comma(new_cell)) add_quotes(&new_cell_dyn);

            // Copy new_cell into string
            s = realloc(s, sizeof(char)*(strlen(new_cell_dyn)+i+1));
            if (s == NULL) {if_error(REALLOC_FAILED, func_name);}

            uintmax_t len =  strlen(new_cell_dyn);
            for (uintmax_t index = 0; index < len; index++) {
                s[i] = new_cell_dyn[index];
                i++;
            }

            // Get file stream past old cell that we are replacing in file
            char tmp;
            while (fscanf(file, "%c", &tmp) == 1) {

                if (inside_quotes && tmp == '"') inside_quotes = false;
                else if ((!inside_quotes && tmp == ',' ) || tmp == '\n') break;
            }
            s[i] = tmp;
        }

        // Increment current row/column if appropriate
        if (!found_correct_index) {

            if (!inside_quotes && s[i] == '"') inside_quotes = true;
            else if (inside_quotes && s[i] == '"') inside_quotes = false;
            else if (s[i] == ',' && !inside_quotes) {
                current_column++;
            }
            else if (s[i] == '\n') {current_row++; current_column = 0;}
        }

        // Reallocate for next char
        s = realloc(s, sizeof(char)*(i+2));
        if (s == NULL) {if_error(REALLOC_FAILED, func_name);}
        i++;

    }
    s[i] = '\0';

    // Close file in read mode
    fclose(file);

    // Reopen file in write mode
    file = fopen(filename, "w");
    if (file == NULL) {if_error(FOPEN_FAILED, func_name);}

    // Print new data to file
    fprintf(file, "%s", s);

    // Free string/close file
    free_null(&s);
    fclose_null(&file);
    free_null(&new_cell_dyn);

    return;
}


//      ___ HAS_QUOTES() ___
bool has_quotes(char *s)
{
    /* THIS FUNCTION: Checks if a string has quotes */
    if (s[0] == '"' && s[strlen(s)-1] == '"') return true;
    else return false;
}

//      __ HAS_COMMA()___
bool has_comma(char *s)
{
    /* THIS FUNCTION: Checks if a string has 1 or more commas */
    uintmax_t length = strlen(s);

    for (uintmax_t i = 0; i < length; i++) {
        if (s[i] == ',') return true;
    }

    return false;
}

//      __ ADD_QUOTES()___
void add_quotes(char **s)
{
    /* THIS FUNCTION: Adds quotes to a dynamically allocated string. */

    // Don't add quotes if unnecessary
    if (has_quotes(*s)) return;

    // Fucntion name for if_error()
    char *func_name = "add_quotes";

    // Original string length
    uint16_t length = strlen(*s);

    // Make copy of string without quotes
    char s_no_quotes[length+1];
    strcpy(s_no_quotes, *s);

    if ((*s)[0] != '"' && (*s)[length-1] != '"') {

        // Resize s to be long enough for quotes
        *s = realloc(*s, sizeof(char)*(strlen(*s)+3));
        if (*s == NULL) if_error(REALLOC_FAILED, func_name);

        //
        strcpy(&((*s)[1]), s_no_quotes);

        // Add quotes and nul
        (*s)[0] = '"';
        (*s)[length+1] = '"';
        (*s)[length+2] = '\0';

    }

    return;

}


// ---------- GET CSV HEADER ----------
char **get_csv_header(FILE *file, uintmax_t *column_count)
{
    // Function name for use in if_error()
    const char *func_name = "get_csv_header";

    /* THIS FUNCTION: 1. Reads csv header into dynamically allocated array of string pointers (i.e. 2D array)
                      2. Returns array of strings as well as psuedo "returns" column_count (via accepting a pointer to column_count as input) for ability to print/free etc array without overrunning buffer
                      3. The main purpose of this function is for use in calling function csv_dict_reader(), however, of course could be used anywhere.
                      4. Takes pointer to column_count only for ablility to psuedo return column_count. For this reason, programmer does NOT need to have correct column count prior to inputting parameter */

    // Set column count to zero incase programmer has not already done so in calling function
    *column_count = 0;

    // Reset file stream
    fseek(file, 0, SEEK_SET);

    // Allocate first pointer in array of strings
    char **header = malloc(sizeof(char*));
    if (header == NULL) {if_error(MALLOC_FAILED, func_name);}

    char *s = NULL;

    // Iterate through HEADER
    bool end_of_header = false;
    while (true) {

        // Allocate first char of string for current column in header
        s = malloc(sizeof(char));
        if (s == NULL) {if_error(MALLOC_FAILED, func_name);}

        // Iterate until end of current index
        uintmax_t i = 0;
        bool inside_quotes = false;
        while (true) {

            // Scan char
            if (fscanf(file, "%c", &s[i]) != 1) {
                end_of_header = true;
                break;
            }

            if (!inside_quotes && s[i] == '"') inside_quotes = true;
            else if (inside_quotes && s[i] == '"') inside_quotes = false;

            // Check if at next column or end of header
            if ((!inside_quotes && s[i] == ',') || s[i] == '\n') {
                (*column_count)++;
                if (s[i] == '\n') {end_of_header = true;}
                break;
            }
            else {
                s = realloc(s, sizeof(char)*(i+2));
                if (s == NULL) {if_error(REALLOC_FAILED, func_name);}
                i++;
            }
        }

        // Change last char (i.e. ',' or '\n') to nul and add string to array of string pointers
        s[i] = '\0';
        if (strlen(s) > 0) {
            header[(*column_count)-1] = s;
        }
        else {free_null(&s); free(header); return NULL;}

        // Realloc array
        if (!end_of_header) {
            header = realloc(header, sizeof(char*)*((*column_count)+1));
            if (header == NULL) {if_error(REALLOC_FAILED, func_name);}
        }
        else break;
    }

    return header;
}

//      ___ VERIFY_COLUMN() ___
bool verify_column(char **header, uintmax_t column_count, char *column)
{
    /* THIS FUNCTION: Verifies if a column name is in the header of a csv, mainly for the purpose of tightening code in calling function. */

    for (uint16_t i = 0; i < column_count; i++) {
        if (strcasecmp(header[i], column) == 0)
            return true;
    }

    return false;

}

//      ___ CSV Dict Reader INDEX ___
char *csv_dictreader_index(FILE *file, uintmax_t row, char *desired_column)
{
    // Function name for use in if_error()
    const char *func_name = "csv_dictreader_index";

    /* THIS FUNCTION: 1. Allows you to index directly into a csv file using the column name and without reading the whole file into memory.
                      2. Returns NULL if desired_column is not in header of file
                      3. This function will treat the 1st non-header row as row 0 (i.e. row 2 in the file = row 0, as it is the first non-header row and will be 0 indexed.)
                      How it works: 1. Finds correct column number that corresponds with column name.
                                    2. Then calls csv_reader_index() to find field at that index location */


    // Reset file stream
    fseek(file, 0, SEEK_SET);

    // Allocate 1st char of string for current column
    char *cursor = malloc(sizeof(char));
    if (cursor == NULL) {if_error(MALLOC_FAILED, func_name);}

    uintmax_t i, correct_column = 0;
    bool end_of_header = false, found_column = true;

    // Iterate through CSV HEADER to find correct column
    while (true) {

        // Iterate until end of current column (i.e. until ',', '\n' or EOF)
        i = 0;
        while ((cursor[i] = fgetc(file)) != ',' && cursor[i] != '\n' && cursor[i] != EOF) {

            // Reallocate
            cursor = realloc(cursor, sizeof(char)*(i+2));
            if (cursor == NULL) {if_error(REALLOC_FAILED, func_name);}
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

//      _____ CSV DICT READER _____
dict_node **csv_dict_reader(FILE *file, uintmax_t *row_count)
{
    // Function name for use in if_error()
    const char *func_name = "csv_dict_reader";

    /* THIS FUNCTION: - Reads file contents and parses into an array of doubly linked DICT nodes.
                      - Each row in file corresponds to each list in array
                      - Each field in file corresponds to each node in list.
                      - Each node contains not only the contents of the corresponding field, but also the name of the corresponding column.
                      - This makes searching for desired index easier, as you will need the correct row number and column name as opposed to row number/column number.*/
    // Get csv HEADER
    uintmax_t column_count = 0;
    char **header = get_csv_header(file, &column_count);
    if (header == NULL) return NULL;

    // MAIN ARRAY POINTER
    dict_node **main_array = (dict_node**)malloc(sizeof(dict_node*));
    if (main_array == NULL) {if_error(MALLOC_FAILED, func_name);}

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

            // Allocate 1st char of string (this string will be taken over by linked list node, so do not free)
            char *s = malloc(sizeof(char));
            if (s == NULL) {if_error(MALLOC_FAILED, func_name);}

            uintmax_t i = 0;
            bool inside_quotes = false;

            // For Each Column, Iterate until next column found
            while (true) {

                // Scan char
                if (fscanf(file, "%c", &s[i]) != 1) {
                    end_of_file = end_of_row = true;
                    break;
                }

                // Check for quotes
                if (!inside_quotes && s[i] == '"') {
                    inside_quotes = true;
                }
                else if (inside_quotes && s[i] == '"') {
                    inside_quotes = false;
                }

                // Check if found next column or end of row
                if (!inside_quotes && s[i] == ',') {
                    break;
                }
                else if (s[i] == '\n') {
                    end_of_row = true;
                    break;
                }
                else {
                    // Reallocate next char in string
                    s = realloc(s, sizeof(char)*(i+2));
                    if (s == NULL) {if_error(REALLOC_FAILED, func_name);}
                    i++;
                }
            }
            s[i] = '\0'; /* Do not free this string. It will be taken over by node in linked list and freed when linked list is freed. */

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
            if (main_array == NULL) {if_error(REALLOC_FAILED, func_name);}
        }
        else break;
    }
    // (*row_count)--; // this only works if csv ends with empty line

    // Free header array of strings
    for (uintmax_t i = 0; i < column_count; i++) {
        free_null(&header[i]);
    }

    /* Can not use free_null() because takes char** (i.e. pointer to char*) but header is char**, so would need to pass char***.
    If I pass header, which is char **, that would pass the address of the first string in the array, not the address of the char ** header, and if I pass &header, it will be char*** */
    free(header);
    header = NULL;

    return main_array;
}

// _____ INDEX INTO DICT _____
char *index_into_dict(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column)
{
    // Function name for use in if_error()
    const char *func_name = "index_into_dict";

    /* THIS FUNCTION: allows you to index into array of dicts (i.e. array of doubly linked lists acting as dicts) */
        // Will return NULL if no column matches desired_column or if row is out of range.

    if (row > row_count-1) return NULL;

    char *cell = NULL;

    // Traverse Doubly linked list to find correct column
    dict_node *tmp = main_array[row];
    while (tmp != NULL)
    {
        if (strcmp(tmp->column_name, desired_column) == 0)
        {
            cell = malloc( sizeof(char) * (strlen(tmp->s)+1) );
            if (cell == NULL) {if_error(MALLOC_FAILED, func_name);}
            strcpy(cell, tmp->s);
            break;
        }
        tmp = tmp->next;
    }

    return cell;
}

bool update_dict_index(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column, char *new_cell)
{
    /* THIS FUNCTION: Allows you to update specific index in array of dicts and csv index */
        // Will return false if no column matches desired_column or if row is out of range.

    if (row > row_count-1) return NULL;

    // Add quotes if new_cell has comma(s)
    if (has_comma(new_cell) && !has_quotes(new_cell)) {
        add_quotes(&new_cell);
    }

    // Traverse Doubly linked dict list to find correct column
    dict_node *tmp = main_array[row];
    while (tmp != NULL)
    {
        if (strcmp(tmp->column_name, desired_column) == 0)
        {
            free_null(&tmp->s);
            tmp->s = new_cell;
            return true;
        }
        tmp = tmp->next;
    }

    return false;
}


// ___ INFINITE BUFFER (User Input) ___
char *inf_buffer(char *prompt)
{
    // Function name for use in if_error()
    const char *func_name = "inf_buffer";

    /* THIS FUNCTION: allows the user to input a dynamically allocated string, of "any" length (i.e. length dictated by size of heap of course)*/

    // Prompt user
    printf("%s", prompt);

    // Allocate 1st char of string
    char *s = malloc(sizeof(char));
    if (s == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Scan command line char by char
    uint64_t i = 0;
    int8_t scanReturn;
    while ((scanReturn = scanf("%c", &s[i])) == 1)
    {
        // Check EOF
        if (scanReturn == EOF) {free_null(&s); if_error(SCANF_FAILED, func_name);}

        // Check if end of user input
        if (s[i] == '\n') {
            s[i] = '\0';
            break;
        }
        else {
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) {free_null(&s); if_error(REALLOC_FAILED, func_name);}
            i++;
        }
    }

    return s;
}


//      ___  GET_UINT()  ___
uint64_t get_uint(char *prompt)
{
    /* THIS FUNCTION: 1. Uses inf_buffer() to get input from user.
                      2. Keeps looping until unsigned integer is given by user
                      2. Returns unsigned integer */
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
                printf("\nError: unsigned integers only please.\n");
                break;
            }
        }

        if (is_int) break;

        free(s);
    }

    // Convert string to integer. (Can not simply return atoi(s), because I need to free(s) first.)
    uint64_t x = atoi(s);

    free(s);
    s = NULL;

    return x;
}


//      ____  IS_EXT()  ____
bool is_ext(char *filename, char *ext)
{
    /* THIS FUNCTION: Checks if filename has correct file extension of your choice */

    // Check that proper argument for file extension (char *ext) has been passed through
    if (ext[0] != '.') return false;

    // Make sure not to dereference NULL pointer
    if (filename == NULL || ext == NULL) return false;

    // Check if filename is at least 5 chars long (e.g. ".csv" plus at least 1 char prior to ".csv")
        //  This avoids buffer underrun once we backup 4 chars in next section
    if (strlen(filename) < 5) return false;

    // Return true if filename has correct extension
    if (strcmp(&filename[strlen(filename)-4], ext) == 0) {
        return true;
    }
    else return false;
}


// ___ PRINT_HEADER() ___
void print_header(char **header, uintmax_t *column_count, bool with_spaces)
{
    /* THIS FUNCTION: Makes printing the header of the csv easier and tightens up the code of the calling function. */

    for (uintmax_t i = 0; i < *column_count; i++) {

        // Print with comma if not the last column
        if (i < (*column_count)-1) {
            printf("%s,", header[i]);
            if (with_spaces) printf(" ");
        }
        else printf("%s\n", header[i]);
    }
    return;
}


// ___ PRINT LIST ___
void print_list(node *head)
{
    /* THIS FUNCTION: Prints the string in each node of a standard doubly linked list */

    node *tmp = head;
    printf("[");
    while (tmp != NULL) {

        // Dont print single quotes if field has double quotes
        if (!has_quotes(tmp->s)) printf("'%s'", tmp->s);
        else printf("%s", tmp->s);

        // Print comma & space after field if not end of row
        if (tmp->next != NULL) printf(", ");

        tmp = tmp->next;
    }
    printf("]\n\n");

    return;
}


// ____ PRINT_DICT_LIST() ____
void print_dict_list(dict_node **main_array, uintmax_t row_count)
{
    // PRINT DICT LIST
    /* THIS FUNCTION: Prints the string in each dict_node of a doubly linked list acting as a DICT. Prints the index (i.e. integer row, string column name in addition to cell value string) */

    // Iterate through every row
    for (uintmax_t i = 0; i < row_count; i++) {

        // Traverse linked list
        dict_node *tmp = main_array[i];
        while (tmp != NULL) {
            printf("Index[%lu]['%s']: %s\n", i, tmp->column_name, tmp->s);
            tmp = tmp->next;
        }
        printf("\n");
    }
}


// ___ FREE ___
void free_list(node *head)
{
    /* THIS FUNCTION: Frees a doubly linked list */
    node *tmp = NULL;
    while (head != NULL)
    {
        tmp = head->next;
        free(head->s);
        head->s = NULL;
        head->prev = head->next = NULL;
        free(head);
        head = tmp;
    }

    tmp = head = NULL;
    return;
}


// ___ FREE DICT LIST ___
void free_dict_list(dict_node **main_array, uintmax_t row_count)
{
    /* THIS FUNCTION: Frees douby linked list of dict_nodes */

    // Iterate through each row (i.e thorugh main array of dict_node pointers)
    dict_node *tmp = NULL;
    for (uintmax_t i = 0; i < row_count; i++)
    {
        // Traverse linked list at current index in main array
        // tmp = NULL;
        while (main_array[i] != NULL)
        {
            tmp = main_array[i]->next;
            free_null(&main_array[i]->s);
            free_null(&main_array[i]->column_name);
            main_array[i]->prev = main_array[i]->next = NULL;
            free(main_array[i]);
            main_array[i] = tmp;
        }

    }
    tmp = NULL;
    main_array = NULL;
    return;
}


// ___ FREE NULL ___
void free_null(char **filename)
{
    /* The only purpose of this function is to condense these 2 lines of code down to 1 in calling function. */
    free(*filename);
    *filename = NULL;
    return;
}

// __ FCLOSE NULL ___
void fclose_null(FILE **file)
{
    /* The only purpose of this function is to condense these 2 lines of code down to 1. Like free_null(), but for file pointers. */
    fclose(*file);
    *file = NULL;
    return;
}


//      ___ IF_ERROR() ___
void if_error(uint8_t error_code, const char *function_name)
{
    /* THIS FUNCTION: 1. Takes error code and function name
                      2. Creates error_log.csv if doesn't already exist
                      3. Concatontates error_code with function code to create final error code
                      4. Generates error message from error code and function name
                      5. Appends error code, error message and timestamp to error log, prints message to terminal. */

    // Declare array of strings of failed function
    const char *FAILED_FUNCTIONS[4] = {"malloc failed", "scanf failed", "realloc failed", "fopen failed"};

    // Decalre array of string of function names, inside of which above functions might fail (for now, simply a list of all functions in this library)
    const char *LIST_OF_CSV_FUNCTIONS[NUM_OF_FUNCTIONS] = {"build_dblink_list", "build_dict_link_list", "read_file_v1", "string_into_2d_array", "update_csv_index",
                                            "read_file_v2", "csv_reader_index", "add_quotes", "index_2darray_csv", "split_2darray_by", "get_csv_header", "csv_dictreader_index",
                                            "csv_dict_reader", "index_into_dict", "update_dict_and_csv", "inf_buffer", "get_uint", "is_ext", "print_list",
                                            "print_dict_list", "free_list", "free_dict_list", "free_null", "fclose_null", "if_error"};

    // Find corresponding number for function that failed
    uint8_t libcsv_function_code = 0;
    for (uint8_t i = 0; i < NUM_OF_FUNCTIONS; i++) {
        if (strcasecmp(LIST_OF_CSV_FUNCTIONS[i], function_name) == 0) {
            libcsv_function_code = i;
            break;
        }
    }

    // Concatonate error codes
    char concatonated_error_code[5];
    sprintf(concatonated_error_code, "%i", error_code);
    sprintf(&concatonated_error_code[1], "%i", libcsv_function_code);
    uint8_t final_error_code = atoi(concatonated_error_code);

    // Open error_log to append
    FILE *file = fopen("error_log.csv", "a");
    if (file == NULL) {printf("Error: fopen failed\n"); exit(1);}

    // Create timestamp
    time_t t;
    time(&t);
    const char *timestamp = ctime(&t);

    // Append to error log - (timestamp has '\n' at end so no need for '\n' in fprintf())
    // Note: error messages start indexing from 1 (of course 0 is success), so index into ERROR_MESSAGES using error_code - 1
    fprintf(file, "%i,%s() in %s(),%s", final_error_code, FAILED_FUNCTIONS[error_code-1], function_name, timestamp);

    // Print to terminal
    printf("ERROR: %i,%s() in %s(),%s", final_error_code, FAILED_FUNCTIONS[error_code-1], function_name, timestamp);

    fclose_null(&file);
    exit(final_error_code);
}
