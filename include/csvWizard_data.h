#ifndef CSVWIZARD_DATA_H
#define CSVWIZARD_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <time.h>
#include <math.h>



//          ---- Error Codes----
enum ErrorCodes {MALLOC_FAILED=1, REALLOC_FAILED, SCANF_FAILED, FOPEN_FAILED, FREAD_FAILED};
#define NUM_OF_FUNCTIONS 25 /* For use in if_error()*/

#define MISSING_VALUE "---" /* This is the default missing value, which gets assigned to the char * */

#define FILENAME_BUFFER_SIZE 255


// Doubly Linked Node
typedef struct node {
    char *s;
    struct node *prev;
    struct node *next;

} node;

// Doubly Linked Node for Dict
typedef struct dict_node {
    char *s;
    char *column_name;
    struct dict_node *prev;
    struct dict_node *next;

} dict_node;


typedef struct dict {
    char *column_name;
    char *field;
} dict;


typedef struct ValueCount {
    char *value;
    uintmax_t count;
    struct ValueCount *next;
} ValueCount;

typedef struct Aggregate {
    char *column_name; /* This will point to the same char *column_name in the header array */
    bool is_number;
    double min;
    double lower_qrt;
    double median;
    double upper_qrt;
    double max;
    double sum;
    double mean;

    double std;

    uintmax_t is_null;
    uintmax_t not_null;
    struct ValueCount **value_counts;

} Aggregate;


//      -- Main csvWizard 'Object' --
typedef struct csvWizard {

    char *filename;
    FILE *file_ptr;
    uintmax_t file_size;
    uintmax_t header_size;
    bool skip_header;
    bool with_spaces;
    char *missingValue;
    bool csv_write_permission;
    bool printRowIndex;
    uintmax_t maxPrintRows;
    bool print_truncated_rows;
    uint32_t longest_field_strlen;
    bool in_place_sort;
    intmax_t row_to_drop; /* Will be set as -1 by default */
    bool generic_header;

    uint8_t max_row_digits;
    bool destructive_mode;
    bool find_rows_at_file_read;
    bool case_sensitive_value_counts;
    bool case_sensitive_sort;
    uintmax_t number_of_rows_to_print;
    bool print_tail;
    char if_error_buffer[50];

    char **header;
    Aggregate *aggregate;
    char **tmp_column;

    // Data structures
    char *raw; /* One long string */
    char **rows; /* Array of strings */
    char ***grid_v3; /* 3D */
    struct node **grid; /* Array of linked lists */
    struct dict_node **dict_grid; /* Array of dict-style linked lists (i.e. each node also contains the header name)*/

    struct node *node_last;
    struct dict_node *dict_last;


    uintmax_t columnCount;
    uintmax_t rowCount;

    char *last_retrieved_field;
    node *last_retrieved_fields;

    // -- Function pointers --

    char *(*userInputFilename)(struct csvWizard *self, char *prompt);
    // .csv - (Functions that read/write directly to CSV file)
    struct {
        bool (*openFile)(struct csvWizard *self, char *filename);
        char **(*headerReader)(struct csvWizard *self);
        char *(*fileReader)(struct csvWizard *self);
        char **(*fileRowReader)(struct csvWizard *self);

        char ***(*reader_v3)(struct csvWizard *self);
        node **(*reader)(struct csvWizard *self);
        dict_node **(*dictReader)(struct csvWizard *self);

        char *(*fieldReader)(struct csvWizard *self, uintmax_t desired_row, uintmax_t desired_colum);
        char *(*fieldReader2)(struct csvWizard *self, uintmax_t desired_row, char *desired_column);

        bool (*fieldWriter)(struct csvWizard *self, uintmax_t row, char *column, char *new_field);

        bool (*backup)(struct csvWizard *self);
        bool (*writeData)(struct csvWizard *self, char *new_filename);

    } csv;



    // Get Field  (from data structure in memory)
    char *(*getField)(struct csvWizard *self, uintmax_t desired_row, uintmax_t desired_column);
    char *(*getField2)(struct csvWizard *self, uintmax_t desired_row, char *desired_column);

    bool (*printColumnCond)(struct csvWizard *self, char *column_name, char *condition_operator, char *condition_num);

    bool (*stripAll)(struct csvWizard *self);
    void (*formatHeader)(struct csvWizard *self);
    bool (*changeColumnName)(struct csvWizard *self, char *old_column_name, char *new_column_name);
    bool (*printHeader)(struct csvWizard *self);
    bool (*printColumn)(struct csvWizard *self, char *column_name);
    bool (*printData)(struct csvWizard *self);
    bool (*printStats)(struct csvWizard *self, char *column_name);
    bool (*printShape)(struct csvWizard *self);
    void (*freeAll)(struct csvWizard *self);
    char *(*changeMissingValue)(struct csvWizard *self, char *missingValue);

    bool (*replace)(struct csvWizard *self, char *to_replace, char *replace_with);
    bool (*replaceInColumn)(struct csvWizard *self, char *column_name, char *to_replace, char *replace_with);
    bool (*dropColumn)(struct csvWizard *self, char *column_name);
    bool (*dropRow)(struct csvWizard *self, uintmax_t row_to_dop);

    bool (*createHeader)(struct csvWizard *self, char *header[], uint32_t header_size);
    bool (*insertRow)(struct csvWizard *self, char *values[]);
    bool (*insertRow2)(struct csvWizard *self, dict values[]);

    bool (*sortRowsByColumn)(struct csvWizard *self, const char *column_name, const char *asc_desc);

    uint16_t (*valueCount)(struct csvWizard *self, char *value, char *column_name);
    bool (*isInColumn)(struct csvWizard *self, char *value, char *column_name);
    double (*getStat)(struct csvWizard *self, char *column_name, char *operation);
    double (*min)(struct csvWizard *self, char *column_name);
    double (*max)(struct csvWizard *self, char *column_name);
    double (*sum)(struct csvWizard *self, char *column_name);
    double (*mean)(struct csvWizard *self, char *column_name);
    uintmax_t (*isNull)(struct csvWizard *self, char *column_name);
    uintmax_t (*notNull)(struct csvWizard *self, char *column_name);


    // Convert data structures
    char **(*string_into_2d_array)(struct csvWizard *myCSV);
    node **(*split_2darray_by)(struct csvWizard *self, char split_by);
    char *(*index_into_dict)(struct csvWizard *self, uintmax_t desired_row, char *desired_column);

    // Update
    bool (*update_dict_index)(struct csvWizard *self, uintmax_t desired_row, char *desired_column, char *new_field);


} csvWizard;



//          ---- FUNCTION PROTOTYPES ----

bool createHeader(csvWizard *self, char *header[], uint32_t header_size);
bool insertRow(csvWizard *self, char *values[]);
bool insertRow2(csvWizard *self, dict values[]);

bool rearrange_dict_array(csvWizard *self, dict values[]);

csvWizard csvWizardConstructor(void);

char *userInputFilename(csvWizard *self, char *prompt);

intmax_t findColumnIndex(csvWizard *self, const char *desired_column);

bool stripField(csvWizard *self, uintmax_t row, char *column_name);
bool strip_internal_(csvWizard *self, uintmax_t row, uintmax_t column, char *field, node *grid_tmp, dict_node *dict_tmp);
bool stripAll(csvWizard *self);

// bool stripColumn(csvWizard *self, char *column_name);
// bool strip(csvWizard *self);
bool changeColumnName(csvWizard *self, char *old_column_name, char *new_column_name);
void formatHeader(csvWizard *self);
bool printHeader(csvWizard *self);
bool printColumn(csvWizard *self, char *column_name);
bool printData(csvWizard *self);
bool print_data_internal(csvWizard *self);
bool printHead(csvWizard *self, uintmax_t number_of_rows);
bool printTail(csvWizard *self, uintmax_t number_of_rows);
bool printStats(csvWizard *self, char *column_name);
bool printShape(csvWizard *self);
char *changeMissingValue(csvWizard *self, char *missingValue);

uintmax_t find_row_count(csvWizard *self);


//  UPDATE fields in memory

    /* user facing */
bool updateField(csvWizard *self, uintmax_t row, char *column, char *new_value);

    /* _internal_ */
bool update_grid_v3_index(csvWizard *self, uintmax_t row, uintmax_t column, char *new_field);
bool update_grid_index(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column, char *new_field);
bool update_dict_index(csvWizard *self, uintmax_t desired_row, char *desired_column, char *new_field);


// REPLACE all instances of a particular value

    /* user facing */
bool replace(csvWizard *self, char *to_replace, char *replace_with);
bool replaceInColumn(csvWizard *self, char *column_name, char *to_replace, char *replace_with);

bool dropColumn(csvWizard *self, char *column_name);
bool dropRow(csvWizard *self, uintmax_t row_to_drop);

    /* _internal_ */
bool grid_replace(csvWizard *self, char *to_replace, char *replace_with, intmax_t column);
bool grid_v3_replace(csvWizard *self, char *to_replace, char *replace_with, intmax_t column); /* column can not be unsinged, bc if -1 is passed, function will replace in all columns */
bool dict_grid_replace(csvWizard *self, char *to_replace, char *replace_with, intmax_t column);


// GET FIELD  (from data structure

    /* user facing */
char *getField(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column);
char *getField2(csvWizard *self, uintmax_t desired_row, char *desired_column);

bool printColumnCond(csvWizard *self, char *column_name, char *condition_operator, char *condition_num);
bool get_fields_cond_grid_v3(csvWizard *self, uintmax_t desired_column, char *condition_operator, char *condition_value);
bool get_fields_cond_grid(csvWizard *self, uintmax_t desired_column, char *condition_operator, char *condition_value);
bool get_fields_cond_dict(csvWizard *self, char *desired_column, char *condition_operator, char *condition_value);


    /* _internal_ */
char *get_field_raw(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column);
char *get_field_rows(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column);
// char *index_2darray_csv(char **main_array, uintmax_t row, uintmax_t column);
char *get_field_grid_v3(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num);
char *get_field_grid(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num);
char *get_field_dict(csvWizard *self, uintmax_t desired_row, char *desired_column);


// Convert to other data structures
char **string_into_2d_array(csvWizard *self);
node **split_2darray_by(csvWizard *self, char split_by);
dict_node **grid_into_dict_grid(csvWizard *self);

// Doubly linked list functions (for use inside of other functions)
bool build_dblink_list(char **s_ptr, node **head, node **last);
bool build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char **current_column_name); // TODO RENAME TO APPEND

bool sortRowsByColumn(csvWizard *self, const char *column_name, const char *asc_desc); /* self.in_place_sort & self.case_sensitive_sort will affect this function */
bool compare_for_sort_(char *field1, char *field2, bool ascending, bool case_sensitive, bool is_number);


// Util
bool is_number(char *s);
bool is_numeric(char *s);
bool is_hex(char *s);
bool has_quotes(char *s);
bool has_comma(char *s);
bool add_quotes(char **s);
char *remove_quotes(csvWizard *self, char *s);
bool verify_column(char **header, uintmax_t column_count, char *column);
int strcmp_quotes(const char *s1, const char *s2, bool case_sensitive);
void get_ptr_to_correct_column(intmax_t correct_column, node **node_ptr, dict_node **dict_ptr);

char *inf_buffer(char *prompt);
uint64_t get_uint(char *prompt);
bool is_ext(char *filename, char *ext);
bool convert_to_csv(csvWizard *self, char *filename);

char *append_last_retrieved_fields(csvWizard *self, char **field);

void calc_max_row_digits(csvWizard *self);
void print_grid_v3(csvWizard *self);
void print_grid(csvWizard *self);
bool print_lnk_list(csvWizard *self, node *head, uintmax_t current_row);
bool print_dict_grid(csvWizard *self);
bool print_dict_grid2(csvWizard *self);
void stat_print_(char *stat_name, double stat, uint8_t column_strlen);


void freeAll(csvWizard *self);
bool free_header(csvWizard *self);
bool free_2d_array(csvWizard *self);
bool free_grid_v3(csvWizard *self);
bool free_grid(csvWizard *self);
bool free_list(node *head);
bool free_dict_grid(csvWizard *self);
bool free_dict_list(dict_node *head);
bool free_value_counts(csvWizard *self);
void free_last_retrieved_fields(csvWizard *self);
bool free_null(char **s);
bool fclose_null(FILE **file);
uint8_t if_error(uint8_t error_code, const char *function_name);


#endif /* CSVWIZARD_DATA_H */