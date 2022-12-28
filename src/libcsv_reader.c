#include <csv_reader.h>

// ___ INFINITE BUFFER (User Input) ___
char *inf_buffer(char *prompt)
{
    // Prompt user
    printf("%s", prompt);

    // Allocate 1st char of string
    char *s = malloc(sizeof(char));
    if (s == NULL) {if_error(1);}

    // Scan command line char by char
    uint64_t i = 0;
    int scanReturn;
    while ((scanReturn = scanf("%c", &s[i])) == 1)
    {
        // Check EOF
        if (scanReturn == EOF) {free_null(&s); if_error(3);}

        // Check if end of user input
        if (s[i] == '\n') {
            s[i] = '\0';
            break;
        }
        else {
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) {if_error(2);}
            i++;
        }
    }

    return s;
}

// ___ BUILD DOUBLE LINK LIST ___
void build_dblink_list(char **s_ptr, node **head, node **last)
{
    // string input changed to **:
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string
    // -- APPEND NODE --
    // Create new node
    node *n = malloc(sizeof(node));
    if (n == NULL) {free_list(*head); if_error(1);}

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

// ___ PRINT LIST ___
void print_list(node *head)
{
    node *tmp = head;
    printf("[");
    while (tmp != NULL) {
        if (tmp->next == NULL) {
            printf("'%s'", tmp->s);
        }
        else printf("'%s', ", tmp->s);

        tmp = tmp->next;
    }
    printf("]\n\n");

    return;
}


// ___ FREE ___
void free_list(node *head)
{
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

// ___ IF ERROR ___
void if_error(int16_t error_num)
{
    // Open error KEY to READ from
    FILE *key_file = fopen("src/resources/errorskey_master.txt", "r");
    if (key_file == NULL) {
        printf("\nError: -1\nFailed in if_error(), attemping to log error.\n");
        exit(-1);
    }

    // Get file stream to correct error message in key
    char c = 0;
    while ((c = fgetc(key_file)) != error_num+48);
    while ((c = fgetc(key_file)) != '\n');

    // Allocate string for error msg (this allows end user full modularity for any length error msg)
    char *error_msg = malloc(sizeof(char));
    if (error_msg == NULL) {
        fclose_null(&key_file);
        printf("\nError: -2\nFailed in if_error(), attemping to log error.\n");
        exit(-2);
        }

    // Read error msg from key
    uint8_t i = 0;
    while ((c = fgetc(key_file)) != '\n') {
        error_msg[i] = c;
        error_msg = realloc(error_msg, sizeof(char)*(i+2));
        if (error_msg == NULL) {
            fclose_null(&key_file);
            printf("\nError: -3\nFailed in if_error() attemping to log error.\n");
            exit(-3);
        }
        i++;
    }
    error_msg[i] = '\0';

    //  Open error LOG in Append Mode
    FILE *log_file = fopen("./src/resources/error_log.csv", "a+");
    if (log_file == NULL) {
        fclose_null(&key_file);
        free_null(&error_msg);
        printf("\nError: -4\nFailed in if_error(), attemping to log error.\n");
        exit(-4);
    }

    // TIMESTAMP
    time_t time_rtrn = time(0);
    uint8_t l = strlen(ctime(&time_rtrn));

    char timestamp[l];
    strcpy(timestamp, ctime(&time_rtrn));
    timestamp[l-1] = '\0';

    // Append to log
    fprintf(log_file, "%i,%s,%s\n", error_num, error_msg, timestamp);

    // End
    printf("\n\n\t** ERROR **\n\nError code: %i\n\n** %s **\n\n%s\n", error_num, error_msg, timestamp);
    fclose_null(&key_file);
    fclose_null(&log_file);
    free_null(&error_msg);
    exit(error_num);
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

// ___ BUILD - DICT - DOUBLE LINK LIST ___
void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name)
{
    // string input changed to **:
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string
    // -- APPEND NODE --
    // Create new node
    dict_node *n = malloc(sizeof(dict_node));
    if (n == NULL) {/*free_dict_list(*head);*/ if_error(1);}

    // This string pointer in the node takes over the allocated string buffer (i.e. **s_ptr).
    n->s = *s_ptr;
    *s_ptr = NULL;
    s_ptr = NULL;

    n->column_name = malloc(sizeof(char)*strlen(current_column_name)+1);
    if (n->column_name == NULL) exit(1);
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

// ___ FREE DICT LIST ___
void free_dict_list(dict_node **main_array, uintmax_t row_count)
{
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
//      ___ READ FILE ___ (V1 - ONE LONG STRING)
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

//      ___ STRING to 2D Array ___ (V2 - 2D JAGGED ARRAY)
char **string_into_2d_array(char *s, uintmax_t *row_count)
{
    /* This function: Turns string INTO DYNAMICALLY ALLOCATED JAGGED 2D ARRAY */
    /* When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way */
    *row_count = 1;

    // Main Array
    char **main_array = (char**)malloc(sizeof(char*));
    if (main_array == NULL) {printf("error: buffer error (malloc)\n"); exit(1);}

    // Iterate through file
    uintmax_t str_index = 0;
    while (true)
    {
        bool next_row = true;

        // Allocate 1st char of current row
        main_array[(*row_count)-1] = malloc(sizeof(char));
        if (main_array[(*row_count)-1] == NULL){printf("error: buffer error (malloc)\n"); exit(2);}

        uint64_t i = 0;
        while (true) {

            // Check for end of row
            if (s[str_index] == '\n') {
                main_array[(*row_count)-1][i] = '\0';
                break;
            }
            else if (s[str_index] == '\0') {
                main_array[(*row_count)-1][i] = '\0';
                next_row = false;
                break;
            }
            else {
                // Assign value to current char in current row
                main_array[(*row_count)-1][i] = s[str_index];

                // Reallocate for next char
                main_array[(*row_count)-1] = realloc(main_array[(*row_count)-1], sizeof(char)*(i+2));
                if (main_array[(*row_count)-1] == NULL) {printf("error: buffer error (realloc)\n"); exit(3);}
                i++;
                str_index++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            main_array = realloc(main_array, sizeof(char*)*((*row_count)+1));
            if (main_array == NULL) {printf("error: buffer error (realloc)\n"); exit(4);}
            (*row_count)++;
            str_index++;
        }
        else break;
    }

    return main_array;
}

//      ___ READ FILE ___ (V2 - 2D JAGGED ARRAY)
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

//      ___ CSV READER INDEX ___
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
//      ___ CSV Dict Reader INDEX ___
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

//      ___ SPLIT BY ___
node **split_by(char **orig_array, uintmax_t row_count, char split)
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

//      ___  GET_UINT()  ___
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
                printf("\nError: unsigned integers only please.\n");
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

//      ____  IS_EXT()  ____
bool is_ext(char *filename, char *ext)
{
    /* THIS FUNCTION: Checks if filename has correct file extension of your choice */

    // Check that proper argument for file extension (char *ext) has been passed through
    if (ext[0] != '.') return false;

    // Make sure not to dereference NULL pointer
    if (filename == NULL) return false;

    // Check if filename is at least 5 chars long (e.g. ".csv" plus at least 1 char prior to ".csv")
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
    /* THIS FUNCTION: Helps you index into a 2D array as if indexes were seperated by comma (i.e. csv) instead of char by char */

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
    /* THIS FUNCTION: 1. Reads csv header into dynamically allocated array of string pointers (i.e. 2D array)
                      2. Returns array of strings as well as psuedo "returns" column_count (via accepting a pointer to column_count as input) for ability to print/free etc array without overrunning buffer
                      3. The main purpose of this function is for use in calling function csv_dict_reader(), however, of course could be used anywhere. */

    // Set column count to zero incase programmer has not already done so in calling function
    *column_count = 0;

    // Reset file stream
    fseek(file, 0, SEEK_SET);

    // Allocate first pointer in array of strings
    char **header = malloc(sizeof(char*));
    if (header == NULL) exit(1);

    char *s = NULL;

    // Iterate through HEADER
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

        // Change last char (i.e. ',' or '\n') to nul and add string to array of string pointers
        s[i] = '\0';
        if (strlen(s) > 0) {
            header[(*column_count)-1] = s;
        }
        else {free_null(&s); free(header); return NULL;}

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
    if (header == NULL) return NULL;

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

// _____ INDEX INTO DICT _____
char *index_into_dict(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column)
{
    /* This function allows you to index into array of dicts */
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
            if (cell == NULL) exit(1);
            strcpy(cell, tmp->s);
            break;
        }
        tmp = tmp->next;
    }

    return cell;
}
void print_dict_list(dict_node **main_array, uintmax_t row_count)
{
    // PRINT DICT LIST
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