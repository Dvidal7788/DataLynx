#include <dataLynx.h>

//          ----- UTILITY FUCTIONS -----


//      CREATE SPACES STRING()
void create_spaces_string(char buffer[], size_t buffer_size, bool with_divider, char space) {

    for (uint8_t i = 0; i < buffer_size; i++) {

        if (with_divider && i == buffer_size-3) {
            buffer[i] = '|';
            continue;
        }

        buffer[i] = (i == buffer_size-1) ? '\0' : space;
    }

    return;
}


//          TRUNCATE WITH ELLIPSES()
char truncate_with_ellipses(char *original, char truncated[], uint32_t truncation_point) {

    char removed_char = original[truncation_point-3];

    original[truncation_point-3] = '\0';

    strcpy(truncated, original);
    strcpy(&truncated[truncation_point-3], "...");

    return removed_char;
}


//         ___ IS_NUMBER() ___
bool is_number(char *s) {

    // THIS FUNCTION calls 2 other internal functions to determine if a string is a number or not (includinng bothe decimal and hexadecimal)

    if (!is_numeric(s) && !is_hex(s)) return false;
    else return true;
}

//      ___ IS_NUMERIC() ____
bool is_numeric(char *s) {

    if (s == NULL) return false;
    if (s[0] == '\n') return false;

    uint32_t length = strlen(s);
    bool dot_found = false;

    for (uint32_t i = 0; i < length; i++) {

        if (!isdigit(s[i]) && s[i] != '.' && s[i] != '-') return false;
        else if (s[i] == '.' && !dot_found) dot_found = true;
        else if (s[i] == '.' && dot_found) return false;
        else if (s[i] == '-' && i != 0) return false;
        // else if (s[i] == '-' && !sign_found) sign_found = true;
        // else if ((s[i] == '-' && sign_found) || (s[i] == '-' && i != 0)) return false;
    }


    return true;
}

//     __ IS_HEX() __
bool is_hex(char *s) {

    if (s == NULL) return false;

    size_t length = strlen(s);

    // Must start with 0x or 0X - I wanted to be able to accept hex numbers wuhtout the 0x prefix, however this would mean that ANY string of letters that did not have anything higher than an f/F would be considered hex (i.e. the string 'abcdef' would be a hex number)
    if (s[0] != '0' && tolower(s[1]) != 'x') return false;

    for (uint32_t i = 0; i < length; i++) {

        if (i == 1 && (s[i] == 'x' || s[i] == 'X') && s[0] == '0') continue;
        else if ( (tolower(s[i]) >= 'a' && tolower(s[i]) <= 'f') || (s[i] >= '0' && s[i] <= '9') ) continue;
        else return false;
    }

    return true;
}



//      ___ HAS_QUOTES() ___
bool has_quotes(char *s)
{

    if (s == NULL) return false;
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
bool add_quotes(char **s)
{
    /* THIS FUNCTION:
        - is destructive
        - Adds quotes to a dynamically allocated string. */

    // Don't add quotes if unnecessary
    if (has_quotes(*s)) return false;

    // Function name for if_error()
    char *func_name = "add_quotes";

    // Original string length
    uint16_t length = strlen(*s);

    // Make copy of string without quotes
    char s_no_quotes[length+1];
    strcpy(s_no_quotes, *s);

    if ((*s)[0] != '"' && (*s)[length-1] != '"') {

        // Resize s to be long enough for quotes
        *s = realloc(*s, sizeof(char)*(strlen(*s)+3));
        if (*s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

        //
        strcpy(&((*s)[1]), s_no_quotes);

        // Add quotes and nul
        (*s)[0] = '"';
        (*s)[length+1] = '"';
        (*s)[length+2] = '\0';

    }

    return true;

}


//      REMOVE QUOTES()
char *remove_quotes(dataLynx *self, char *s) {

    /* This function is NON destructive (unlike add_quotes) */

    const char * func_name = "remove_quotes";

    // Length of new string 2 less than original
    uint16_t new_length = strlen(s)-2;

    char *s_sans_quotes = malloc(sizeof(char)*(new_length+1));
    if (s == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Copy, excluding first double quote
    strncpy(s_sans_quotes, &s[1], new_length);

    // Change last char to nul
    s_sans_quotes[new_length] = '\0';

    if (self->last_retrieved_field != NULL) free(self->last_retrieved_field);

    self->last_retrieved_field = s_sans_quotes;

    return self->last_retrieved_field;


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


// ___ INFINITE BUFFER (User Input) ___
char *inf_buffer(char *prompt)
{
    // Function name (for use in if_error())
    const char *func_name = "inf_buffer";

    /* THIS FUNCTION: allows the user to input a dynamically allocated string, of "any" length (i.e. length dictated by size of heap of course)*/

    // Prompt user
    printf("%s", prompt);

    // Allocate 1st char of string
    char *s = (char*)malloc(sizeof(char));
    if (s == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Scan command line char by char
    uint64_t i = 0;
    int8_t scanReturn;
    while ((scanReturn = scanf("%c", &s[i])) == 1)
    {
        // Check EOF
        if (scanReturn == EOF) {free_null(&s); if_error(SCANF_FAILED, func_name); return NULL;}

        // Check if end of user input
        if (s[i] == '\n') {
            s[i] = '\0';
            break;
        }
        else {
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) {free_null(&s); if_error(REALLOC_FAILED, func_name); return NULL;}
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

bool convert_to_csv(dataLynx *self, char *filename) {
    /*DOES NOT WORK YET*/
    if (is_ext(filename, ".csv")) return true;

    char *possible_extensions[5] = {
        ".xls",
        ".xlsx",
        ".xlsm",
        ".xlsb",
        ".docx"
    };


    bool correct_ext = false;
    for (uint8_t i = 0; i < 5; i++) {
        if (is_ext(filename, possible_extensions[i])) {
            correct_ext = true;
            break;
        }
    }

    if (!correct_ext) return false;

    const char *func_name = "convert_to_csv";

    FILE *file = fopen(filename, "r");
    if (file == NULL) {if_error(FOPEN_FAILED, func_name); return false;}

    fseek(file, 0, SEEK_END);
    uintmax_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char buffer[file_size+1];

    fread(buffer, file_size, 1, file);
    buffer[file_size] = '\0';

    fclose(file);

    // replace '.' with  '\0'
    uint8_t filename_length = strlen(filename);

    char *new_filename = malloc(sizeof(char)*filename_length+1);
    if (new_filename == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    for (uint8_t i = 0; i < filename_length; i++) {

        new_filename[i] = filename[i];

        if (new_filename[i] == '.') {
            new_filename[i+1] = 'c';
            new_filename[i+2] = 's';
            new_filename[i+3] = 'v';
            new_filename[i+4] = '\0';
            break;
        }
    }


    FILE *new_file = fopen(new_filename, "w");
    if (new_file == NULL) {if_error(FOPEN_FAILED, func_name); return false;}

    fwrite(buffer, 0, file_size, new_file);

    self->file_ptr = new_file;
    self->filename = new_filename;

    fseek(file, 0, SEEK_SET);
    return true;

}

char *append_last_retrieved_fields(dataLynx *self, char **field) {

    if (field == NULL) return false;
    if (*field == NULL) return false;

    const char *func_name = "append_last_retrieved_fields";

    // Allocate for new node
    node *n = (node *)malloc(sizeof(node));
    if (n ==  NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    n->next = self->last_retrieved_fields;
    self->last_retrieved_fields = n;

    n->s = *field;

    return n->s;
}


//      ____  IS_EXT()  ____
bool is_ext(char *filename, char *ext) {

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


void calc_max_row_digits(dataLynx *self) {

    uintmax_t temp = self->rowCount;

    self->max_row_digits = 1;

    while (temp >= 10) {
        temp /= 10;
        self->max_row_digits++;
    }
}


bool rearrange_dict_array(dataLynx *self, dict values[]) {

    /* THIS FUNCTION:
                - Rearranges a dict array IF columns are not in correct order according to header */

    if (self->__header__ == NULL) {
        if (!headerReader(self)) return false;
    }

    bool case_sensitive = true;
    bool rearranged = false;

    // Iterate through array
    for (uint32_t column = 0; column < self->columnCount; column++) {

        // Mismatch found
        if (strcmp_quotes(values[column].column_name, self->__header__[column], case_sensitive) != 0) {

            // Search rest of columns for match
            for (uint32_t c = column+1; c < self->columnCount; c++) {

                if (strcmp_quotes(values[c].column_name, self->__header__[column], case_sensitive) == 0) {

                    // Swap
                    dict tmp = values[column];
                    values[column] = values[c];
                    values[c] = tmp;

                    rearranged = true;
                    break;

                }
            }

        }
    }

    return rearranged;
}


//      ___ GET FILE SIZE() ___
size_t get_file_size_(dataLynx *self) {

    // Get file stream to beginning (to get header size)
    fseek(self->file_ptr, 0L, SEEK_SET);

    // Get header size
    char tmp = 'a'; /* Must be initialized */
    size_t header_size = 0;

    // Iterate until end of first line
    while (tmp != '\n' && tmp != EOF) {
        tmp = getc(self->file_ptr);
        header_size++;
    }

    self->header_size = header_size;

    // Get file stream to end of file
    fseek(self->file_ptr, 0L, SEEK_END);

    // Get file stream position (i.e. file size) - store file size in dataLynx object
    self->file_size = ftell(self->file_ptr);

    return self->file_size;
}


//      ___ IF_ERROR() ___
uint8_t if_error(uint8_t error_code, const char *function_name)
{
    /* THIS FUNCTION: 1. Takes error code and function name
                      2. Creates error_log.csv if doesn't already exist
                      3. Concatontates error_code with function code to create final error code
                      4. Generates error message from error code and function name
                      5. Appends error code, error message and timestamp to error log, prints message to terminal. */

    // Declare array of strings of failed function
    const char *FAILED_FUNCTIONS[5] = {"malloc()", "scanf()", "realloc()", "fopen()", "fread()"};

    // Decalre array of string of function names, inside of which above functions might fail (for now, simply a list of all functions in this library)
    const char *LIST_OF_CSV_FUNCTIONS[NUM_OF_FUNCTIONS] = {"build_dblink_list", "build_dict_link_list", "read_file_v1", "string_into_2d_array", "update_csv_index",
                                            "read_file_v2", "csv_reader_index", "add_quotes", "index_2darray_csv", "split_2darray_by", "headerReader", "csv_dictreader_index",
                                            "dictReader", "get_field_dict", "update_dict_and_csv", "inf_buffer", "get_uint", "is_ext", "print_lnk_list",
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
    if (file == NULL) {printf("Error: fopen failed while trying to log error.\n"); exit(1);}

    // Create timestamp
    time_t t;
    time(&t);
    const char *timestamp = ctime(&t);

    // Append to error log - (timestamp has '\n' at end so no need for '\n' in fprintf())
    // Note: error messages start indexing from 1 (of course 0 is success), so index into ERROR_MESSAGES using error_code - 1
    fprintf(file, "%i,%s failed in %s(),%s", final_error_code, FAILED_FUNCTIONS[error_code-1], function_name, timestamp);

    // Print to terminal
    printf("ERROR: %i,%s failed in %s(),%s", final_error_code, FAILED_FUNCTIONS[error_code-1], function_name, timestamp);

    fclose_null(&file);
    // exit(final_error_code);

    return final_error_code;
}
