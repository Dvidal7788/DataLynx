# CSV Library
Library for reading and updating files, especially csv files.

This is a library of functions written in C, for use in C programs, inspired by Python's csv library, giving you similiar functionality to dicts among several other data structures read from csv files.

LIST OF FUNCTIONS:

<h5 align="center">BUILD_DBLINK_LIST()</h5>
<h6 align="center">void build_dblink_list(char **s_ptr, node **head, node **last)</h6>
<ul>
  <li>PARAMETERS: Takes pointer to string that will is intended to be in node, pointer to head node pointer and pointer to last node pointer. All 3 need to be pointers to pointers, so I can changed what they are pointing to inside the function. NOTE: String input must be dyamically allocated string, as this buffer will be taken over by linked list node. Otherwise buffer will be popped off the stack in calling function. Can not be string literal, otherwise will be popped off the stack when this function returns.</li>
  <ol>
    <li>Mallocs new node.</li>
    <li>New node takes over string (i.e. string pointer in node now points to string).</li>
    <li>New node is appended to linked list or added as first node if head is NULL.</li>
  </ol>
  <li>RETURN: No return value.</li>
</ul>

<hr>
<h5 align="center">BUILD_DICT_LINK_LIST</h5>
<h6 align="center">void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name);</h6>
<ul>
  <li>PARAMETERS: Takes pointer to string that will appear in new node, pointer to head node pointer, pointer to last node pointer and string of column name. NOTE: String input must be dyamically allocated string, as this buffer will be taken over by linked list node. Otherwise buffer will be popped off the stack in calling function. Can not be string literal, otherwise will be popped off the stack when this function returns. Column_name string input CAN be string literal.</li>
  <ol>
    <li>Mallocs new node.</li>
    <li>New node takes over string (i.e. string pointer in node now points to string).</li>
    <li>String for column_name of node is malloced and copied from current_column_name given as input.</li>
    <li>New node is appended to linked list or added as first node if head is NULL.</li>
  </ol>
  <li>RETURN: No return value.</li>
</ul>

<hr>
<h5 align="center">READ_FILE_V1()</h5>
<h6 align="center">char *read_file_v1(FILE *file, bool skip_header)</h6>
<ul>
    <h5>Purpose:</h5>
    <h6>READS FILE CONTENTS FROM BEGINNING TO END INTO ONE LONG STRING. (Can be .txt or .csv file.)  Also takes bool to determine if function should skip header or not.</h6>
  <li>PARAMETERS: File pointer.</li>
  <ol>
    <li>File is opened in read mode, using file pointer given as input.</li>
    <li>Initial char is malloced.</li>
    <li>File is read, one char at a time, reallocing after each char.</li>
    <li>When end of file reached, final char is changed to '\0', and string is returned from fucntion.</li>
  </ol>
  <li>RETURN: Returns string of contents read from file.</li>
</ul>

<hr>
<h5 align="center">STRING_INTO_2D_ARRAY()</h5>
<h6 align="center">char **string_into_2d_array(char *s, uintmax_t *row_count)</h6>
<ul>
   <h5>Purpose:</h5>
   <h6>TURNS STRING CONTAINING MULTIPLE LINES INTO 2D ARRAY (i.e. ARRAY OF STRINGS), ONE STRING FOR EACH LINE</h6>
  <li>PARAMETERS: Takes string as input that you wish to turn into 2D array. Input a pointer to (or address of) an unused unsigned integer for the purposes of remembering how many rows (lines) the new array will have. This row_count int will be necessary for printing and freeing 2D array.</li>
  <ol>
    <li>Mallocs first string pointer in array of strings.</li>
    <li>Mallocs 1st char in first string of the array of strings</li>
    <li>Copies char from current index of input string to current char in new string.</li>
    <li>Keeps reallocing one char at a time until '\n' or '\0' is found.</li>
    <li>If '\n' is found, this char is chagned to '\0' and reallocs next pointer in array of strings and repeats process.</li>
    <li>if '\0' is found, this is the end of the original string and process ends.</li>
  </ol>
  <li>RETURN: Returns pointer to array of strings.</li>
</ul>

<hr>
<h5 align="center">READ_FILE_V2</h5>
<h6 align="center">char **read_file_v2(FILE *file, uintmax_t *row_count, bool skip_header)</h6>
<ul>
  <h5>Purpose:</h5>
  <h6>READS FILE INTO DYNAMICALLY ALLOCATED 2D ARRAY (i.e. Array of strings. One row from file per string</h6>
  <li>PARAMETERS: Takes file pointer and pointer to unsigned integer for purposes of keeping track of number of rows, so this can be pseudo "returned" to calling function, as it will be necessary for printing and freeing 2D array. Also takes bool to determine if function should skip header or not.</li>
  <ol>
    <li>Opens file in read mode using file pointer given as input.</li>
    <li>Mallocs first string pointer in array of strings.</li>
    <li>Mallocs 1st char in first string of the array of strings</li>
    <li>Reads one char from file.</li>
    <li>Keeps reallocing one char at a time until '\n' or EOF is found.</li>
    <li>If '\n' is found, this char is chagned to '\0' and reallocs next pointer in array of strings and repeats process.</li>
    <li>if '\0' is found, this is the end of the original string and process ends.</li>
  </ol>
  <li>RETURN: Returns pointer array of strings.</li>
</ul>

<hr>
<h5 align="center">CSV_READER_INDEX()</h5>
<h6 align="center">char *csv_reader_index(FILE *file, uintmax_t row, uintmax_t col, bool skip_header)</h6>
<ul>
  <h5>Purpose:</h5>
  <h6>Allows you to index directly into a csv, read the field at that specific index, without reading the whole file into memory.</h6>
  <h5>How to use function:</h5>
  <li>Input file pointer, desired row number, column number and whether or not you want to count the header as the first row (i.e. skip_header = true if you do not want to count header as 1st row).</li>
  <li>The function will read the field at that index and return it back to you as a string.</li> 
</ul>

<hr>
<h5 align="center">UPDATE_CSV_INDEX()</h5>
<h6 align="center">void update_csv_index(char *filename, uintmax_t row, uintmax_t column, char *new_cell)</h6>
<h5>Purpose:</h5>
<h6>Updates specific field in csv file.</h6>
<h5>How to use:</h5>
<ul>
  <li>Input filename, desired row number, column number and string you wish to replace the field at the given index with.</li>
  <li>The function will open the file, read through it, copying everything into a temporary string until it arrives at the desired row/column.</li>
  <li>Once file stream is at desired row/column, the new string you wish to update the csv with, will be copied into the temporary string instead of the text at the current index.</li>
  <li>The file stream will then move past the current index (i.e. to the next column), at which point it will continue to copy everything into the temporary string until EOF is reached.</li>
  <li>The file will be closed in read mode and opened in write mode, at which point the entire file will be overwritten with the contents of the temporary string, effectively updating only the desired index (i.e. the row/column given as input).</li>
</ul>

<hr>
<h5 align="center">CSV_DICT_READER()</h5>
<h6 align="center">char *csv_dictreader_index(FILE *file, uintmax_t row, char *desired_column)</h6>
<ul>
  <h5>Purpose:</h5>
  <h6>Allows you to index directly into a csv file, without reading the whole file into memory, by using the name of a column instead of the number of the number. When done this way, your code will still work even if columns are moved around in csv.</h6>
  <h5>How to use function:</h5>
  <ol>
    <li>Input file pointer, desired row number and name of desired column.</li>
    <li>The function will return back to you the field at the given row number/column name in the form of a string.</li>
  </ol>
</ul>

<hr>
<h5 align="center">SPLIT_2DARRAY_BY()</h5>
<h6 align="center">node **split_2darray_by(char **orig_array, uintmax_t row_count, char split)</h6>
<h5>Purpose: To take an array of strings (typical use case is an array of strings in which each string is equivalent to 1 row in a csv or text file) and split it by the ',' (or any other desired char) into an array of doubly linked lists.</h5>
<h5>How to use function:</h5>
  <li>Input the main pointer to an array of strings. Input the row_count of the (i.e. the same one that was "psuedo returned" (i.e. given as input in the form of a pointer of address of) from string_into_2d_array()). Input char to split the array by.</li>
  <li>Function will take 2D array and split it by the desired char, creating an array of doubly linked lists, which will be returned.</li>


<hr>
<h5 align="center">INDEX_2DARRAY_CSV()</h5>
<h6 align="center">char *index_2darray_csv(char **main_array, uintmax_t row_num, uintmax_t index)</h6>
<h5>Purpose:</h5>
<h6>To allow you index into an arary of strings (in which each string equates to a line in a csv file) at a specific row and column.</h6>
<h5>How to use function:</h5>
<h6>Input the pointer to main array (i.e. the pointer to the array of strings), the desired row and desired column you wish to index into.</h6>
<h6>The function will return back to you a string of the field contents at that specific row/column.</h6>

<hr>
<h5 align="center">GET_CSV_HEADER()</h5>
<h6 align="center">char **get_csv_header(FILE *file, uintmax_t *column_count)</h6>
<h5>Purpose:</h5>
<h6>Allows you to recieve an array of strings of the header (i.e. column name) of a csv.</h6>
<h5>How to use function:</h5>
<h6>Input pointer to (or address of) unused unsiged integer to keep track of the number of columns.</h6>
<h6>The function will return back to you an array of strings, each string corresponding to the name of each column.</h6>

<hr>
<h5 align="center">CSV_DICTREADER_INDEX()</h5>
<h6 align="center">char *csv_dictreader_index(FILE *file, uintmax_t row, char *desired_column)</h6>
<h5>Purpose:</h5>
<h6>Allows you to index directly into a csv, read the field at that specific index, without reading the whole file into memory. This is similar to csv_reader_index(), however instead of using a desired row number/column number, you can use the desired row number and column name.</h6>
<h5>How to use function:</h5>
<h6>Inut file pointer, desired row number and desired column name.</h6>
<h6>Function will return back to you a string of the contents at that specific row number/column name.</h6>

<hr>
<h5 align="center">CS_DICT_READER()</h5>
<h6 align="center">dict_node **csv_dict_reader(FILE *file, uintmax_t *row_count)</h6>
<h5>Purpose:</h5>
<h6>- To read csv file contents and parse into an array of dicts (i.e. doubly linked dict nodes)<br>
  - Each row in file corresponds to each list in array.<br>
  - Each field in file corresponds to each node in list.<br>
 - Each node contains not only the contents of the corresponding field, but also the name of the corresponding column.<br>
 - This makes searching for desired index easier, as you will need the correct row number and column name as opposed to row number/column number.</h6>
<h5>How to use function:</h5>
<h6>Input file pointer and pointer to (or address of) unused unsigned integer to keep track of number of rows.</h6>
<h6>Funciton will return back to you a pointer to an array of doubly linked DICT nodes (i.e. each node contains field contents as well as corresponding column name).</h6>

<hr>
<h5 align="center">INDEX_INTO_DICT()</h5>
<h6 align="center">char *index_into_dict(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column)</h6>
<h5>Purpose:</h5>
<h6>Allows you to index into specific row number and column name in an array of dicts (i.e. doubly linked dict nodes).</h6>
<h5>How to use function:</h5>
<h6>Input pointer to arary of dicts, row_count (protects you against yourself from inputting a desired row number that is beyond the buffer), the desired row number and desired column name.</h6>
<h6>Functino returns back to you a string of the contents at that specific row number/column name</h6>

<hr>
<h5 align="center">INF_BUFFER()</h5>
<h6 align="center">char *inf_buffer(char *prompt)</h6>
<h5>Purpose:</h5>
<h6>Allows you to prompt user for input and allows user to input a string as long as they desire (only limited by memory in the heap).</h6>
<h5>How to use function:</h5>
<h6>Input desired user prompt into function</h6>
<h6>Function will prompt user and return back to you a dynamically allocated string.</h6>
<h6>It is up to the programmer to free the returned string.</h6>


<hr>
<h5 align="center">GET_UINT()</h5>
<h6 align="center">uint64_t get_uint(char *prompt)</h6>
<h5>Purpose:</h5>
<h6>To prompt user for input and only return once user has given an unsigned integer. (Useful for getting desired row/column numbers.)</h6>
<h5>How to use function:</h5>
<h6>Input desired prompt.</h6>
<h6>Function will prompt user and keep looping until an unsigned integer is given by the user.</h6>
<h6>The unsigned integer is then returned from the function.</h6>


<hr>
<h5 align="center">IS_EXT()</h5>
<h6 align="center">bool is_ext(char *filename, char *ext)</h6>
<h5>Purpose:</h5>
<h6>To check if a filename has the correct file extension.</h6>
<h5>How to use function:</h5>
<h6>Input filename and desired file extension. Make sure to include '.' in desired file extension (i.e. ".csv" or ".txt").</h6>
<h6>If filename has correct file extension, the fuction will return true.</h6>
<h6>If filename does not have correct file extension, the fuction will return false.</h6>

<hr>
<h5 align="center">PRINT_LIST(</h5>
<h6 align="center">void print_list(node *head)</h6>
<h5>Purpose:</h5>
<h6>Prints the string in each node of a standard doubly linked list.</h6>
<h5>How to use function:</h5>
<h6>Input pointer to first node in list.</h6>
<h6>Each string will printed in a csv-esque format (i.e. with ',' after each item, except for last items in each row).</h6>
<h6>Note: This function is meant to be used in a loop. Therefore if you are printing an array of doubly linked lists, you must create a loop iterateing through every list head in the array, calling this function on every iteration of the loop. This function will then traverse each node of the current list in the array of doubly linked lists.</h6>

<hr>
<h5 align="center">PRINT_DICT_LIST()</h5>
<h6 align="center">void print_dict_list(dict_node **main_array, uintmax_t row_count)</h6>
<h5>Purpose:</h5>
<h6>Prints the string in each node of a doubly linked DICT list</h6>
<h5>How to use function:</h5>
<h6></h6>
<h6></h6>


<h5 align="center"></h5>
<h6 align="center"></h6>
<h5>Purpose:</h5>
<h6></h6>
<h5>How to use function:</h5>
<h6></h6>
<h6></h6>

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
