# file_read_library
Library for reading from files, especially csv files.

This library contains functions with the intention of giving a C programmer close to the ease of use of Python, but with the control of C.


LIST OF FUNCTIONS:

___ INF_BUFFER() ____
<br>
char *inf_buffer(char *prompt)

- PARAMETERS: Takes string as input.
- 1. Prompts user with string given as input.
- 2. Mallocs initial char. Reallocs one char at a time until '\n' is reached, at which point that char will be changed to '\0'.
- RETURN: Will return dynamically allocated string. It is up to the programmer to free this string in the calling function.

___ IF_ERROR() ___
<br>
void if_error(int16_t error_num)

- PARAMETERS: Takes integer (i.e. error code) as input.
- 1. Opens file errorKey.txt (which contains all of the potential error codes), scans the error key file for the appropriate corresponding error message.
- 2. Reads error message into dynamically allocated buffer (for ultimate modularity and ability for expansion moving forward)
- 3. Creates timestamp
- 4. Opens errorlog.csv, prints error code, error message and timestamp to error log
- 5. Prints error code/error message to the terminal
- 6. Exits program, returning correct error message from main().
- Note: The 1st iteration of this function took an error message as an input, however, when creating a program in C and trying to make it as robust as possible, of course there are many NULL checks and error checks throughout the program, each of which has an if statement that calls this if_error() function, if triggered. Since each of those calls to if_error() initially took an error message string as input, this began to quickly make the code look very messy. So, for ultimate modularity, ease of use and maintenance moving forward (in addition to cleaning up messy code in the calling function), this function was redesigned to read the corresponding error message from an error key).
- RETURN: 


//          ---- FUNCTION PROTOTYPES ----
// Doubly linked list functions (for use inside of other functions)
void build_dblink_list(char **s_ptr, node **head, node **last);
void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name);

// Functions for reading/updating csv files and organizing data structures
char *read_file_v1(FILE *file);
char **string_into_2d_array(char *s, uintmax_t *row_count);
void update_csv_index(char *filename, uintmax_t row, uintmax_t column, char *new_cell);
char **read_file_v2(FILE *file, uintmax_t *row_count);
char *csv_reader_index(FILE *file, uintmax_t row, uintmax_t col, bool skip_header);
char *csv_dictreader_index(FILE *file, uintmax_t row, char *desired_column);
char *index_2darray_csv(char **main_array, uintmax_t row_num, uintmax_t index);
node **split_2darray_by(char **orig_array, uintmax_t row_count, char split);
char **get_csv_header(FILE *file, uintmax_t *column_count);
dict_node **csv_dict_reader(FILE *file, uintmax_t *row_count);
char *index_into_dict(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column);
char *update_dict_and_csv(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column, char *new_cell);

// User input/utility functions
char *inf_buffer(char *prompt);
uint64_t get_uint(char *prompt);
bool is_ext(char *filename, char *ext);
void print_list(node *head);
void print_dict_list(dict_node **main_array, uintmax_t row_count);
void free_list(node *head);
void free_dict_list(dict_node **main_array, uintmax_t row_count);
void free_null(char **filename);
void fclose_null(FILE **file);
void if_error(uint8_t error_code, const char *function_name);
