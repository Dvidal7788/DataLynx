# CSV Library
Library for reading from files, especially csv files.

This library contains functions with the intention of giving a C programmer close to the ease of use of Python, but with the control of C.


LIST OF FUNCTIONS:

<h5>INF_BUFFER()</h5>
<h6>char *inf_buffer(char *prompt)</h6>
<ul>
  <li><h6>PARAMETERS:</h6> Takes string as input.</li>
  <ol>
    <li>Prompts user with string given as input.</li>
    <li>Mallocs initial char. Reallocs one char at a time until '\n' is reached, at which point that char will be changed to '\0'.</li>
  </ol>
  <li><h6>RETURN:</h6> Will return dynamically allocated string. It is up to the programmer to free this string in the calling function.</li>
</ul>
<center>TEST</center>
<h5>BUILD_DBLINK_LIST()</h5>
<h6>void build_dblink_list(char **s_ptr, node **head, node **last)</h6>
<ul>
  <li><h6>PARAMETERS:</h6> Takes pointer to string that will is intended to be in node, pointer to head node pointer and pointer to last node pointer. All 3 need to be pointers to pointers, so I can changed what they are pointing to inside the function.</li>
  <ol>
    <li>Mallocs new node.</li>
    <li>New node takes over string (i.e. string pointer in node now points to string).</li>
    <li>New node is appended to linked list or added as first node if head is NULL.</li>
  </ol>
  <li><h6>RETURN:</h6> No return value.</li>
</ul>


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
