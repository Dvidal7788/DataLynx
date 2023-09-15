#ifndef DATALYNX_DATA_H
#define DATALYNX_DATA_H

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
#define MAX_FIELD_PRINT_LENGTH 50 /* This number INCLUDES the borders of the table (i.e. '|' and '+') */


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

    // For numeric columns:
    bool is_number;
    double min;
    double lower_qrt;
    double median;
    double upper_qrt;
    double max;
    double sum;
    double mean;
    double std;

    // For numeric AND non-numeric columns:
    size_t longest_field_strlen;

    uintmax_t is_null;
    uintmax_t not_null;

    // For non-numeric columns:
    struct ValueCount **value_counts;

} Aggregate;


//      -- Main dataLynx 'Object' --
typedef struct dataLynx {

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
    bool in_place_sort;
    intmax_t row_to_drop; /* Will be set as -1 by default */
    bool generic_header;
    bool print_filter;

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

    char *(*userInputFilename)(struct dataLynx *self, char *prompt);
    bool (*changeFilename)(struct dataLynx *self, char *filename);

    // .csv - (Functions that read/write directly to CSV file)
    struct {
        bool (*openFile)(struct dataLynx *self, char *filename);
        char **(*headerReader)(struct dataLynx *self);
        char *(*fileReader)(struct dataLynx *self);
        char **(*fileRowReader)(struct dataLynx *self);

        char ***(*reader_v3)(struct dataLynx *self);
        node **(*reader)(struct dataLynx *self);
        dict_node **(*dictReader)(struct dataLynx *self);

        char *(*fieldReader)(struct dataLynx *self, uintmax_t desired_row, uintmax_t desired_colum);
        char *(*fieldReader2)(struct dataLynx *self, uintmax_t desired_row, char *desired_column);

        bool (*fieldWriter)(struct dataLynx *self, uintmax_t row, char *column, char *new_field);

        bool (*backup)(struct dataLynx *self);
        bool (*writeData)(struct dataLynx *self, char *new_filename);

    } csv;



    // Get Field  (from data structure in memory)
    char *(*getField)(struct dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);
    char *(*getField2)(struct dataLynx *self, uintmax_t desired_row, char *desired_column);

    bool (*printColumnCond)(struct dataLynx *self, char *column_name, char *condition_operator, char *condition_num);

    bool (*stripAll)(struct dataLynx *self);
    void (*formatHeader)(struct dataLynx *self);
    bool (*changeColumnName)(struct dataLynx *self, char *old_column_name, char *new_column_name);
    bool (*printHeader)(struct dataLynx *self);
    bool (*printColumn)(struct dataLynx *self, char *column_name);
    bool (*printData)(struct dataLynx *self);
    bool (*printDataTable)(struct dataLynx *self);
    bool (*printStats)(struct dataLynx *self, char *column_name);
    bool (*printShape)(struct dataLynx *self);
    void (*freeAll)(struct dataLynx *self);
    char *(*changeMissingValue)(struct dataLynx *self, char *missingValue);

    bool (*replace)(struct dataLynx *self, char *to_replace, char *replace_with);
    bool (*replaceInColumn)(struct dataLynx *self, char *column_name, char *to_replace, char *replace_with);
    bool (*dropColumn)(struct dataLynx *self, char *column_name);
    bool (*dropRow)(struct dataLynx *self, uintmax_t row_to_dop);

    bool (*createHeader)(struct dataLynx *self, char *header[], uint32_t header_size);
    bool (*insertRow)(struct dataLynx *self, char *values[]);
    bool (*insertRow2)(struct dataLynx *self, dict values[]);

    bool (*sortRowsByColumn)(struct dataLynx *self, const char *column_name, const char *asc_desc);

    uint16_t (*valueCount)(struct dataLynx *self, char *value, char *column_name);
    bool (*isInColumn)(struct dataLynx *self, char *value, char *column_name);
    double (*getStat)(struct dataLynx *self, char *column_name, char *operation);
    double (*min)(struct dataLynx *self, char *column_name);
    double (*max)(struct dataLynx *self, char *column_name);
    double (*sum)(struct dataLynx *self, char *column_name);
    double (*mean)(struct dataLynx *self, char *column_name);
    uintmax_t (*isNull)(struct dataLynx *self, char *column_name);
    uintmax_t (*notNull)(struct dataLynx *self, char *column_name);


    // Convert data structures
    char **(*string_into_2d_array)(struct dataLynx *myCSV);
    node **(*split_2darray_by)(struct dataLynx *self, char split_by);
    char *(*index_into_dict)(struct dataLynx *self, uintmax_t desired_row, char *desired_column);

    // Update
    bool (*update_dict_index)(struct dataLynx *self, uintmax_t desired_row, char *desired_column, char *new_field);


} dataLynx;



//          ---- FUNCTION PROTOTYPES ----

bool createHeader(dataLynx *self, char *header[], uint32_t header_size);
bool insertRow(dataLynx *self, char *values[]);
bool insertRow2(dataLynx *self, dict values[]);

bool rearrange_dict_array(dataLynx *self, dict values[]);

dataLynx dataLynxConstructor(void);

char *userInputFilename(dataLynx *self, char *prompt);
bool changeFilename(dataLynx *self, char *filename);

intmax_t findColumnIndex(dataLynx *self, const char *desired_column);

bool stripField(dataLynx *self, uintmax_t row, char *column_name);
bool strip_internal_(dataLynx *self, uintmax_t row, uintmax_t column, char *field, node *grid_tmp, dict_node *dict_tmp);
bool stripAll(dataLynx *self);

// bool stripColumn(dataLynx *self, char *column_name);
// bool strip(dataLynx *self);
bool changeColumnName(dataLynx *self, char *old_column_name, char *new_column_name);
void formatHeader(dataLynx *self);
bool printHeader(dataLynx *self);
bool printColumn(dataLynx *self, char *column_name);
bool printDataTable(dataLynx *self);
bool printData(dataLynx *self);
bool print_data_internal(dataLynx *self);
bool printHead(dataLynx *self, uintmax_t number_of_rows);
bool printTail(dataLynx *self, uintmax_t number_of_rows);
bool printStats(dataLynx *self, char *column_name);
void print_stats_is_not_null_(dataLynx *self, size_t column_strlen, uint32_t column_index, bool is_null);
bool printShape(dataLynx *self);
char *changeMissingValue(dataLynx *self, char *missingValue);

uintmax_t find_row_count(dataLynx *self);


//  UPDATE fields in memory

    /* user facing */
bool updateField(dataLynx *self, uintmax_t row, char *column, char *new_value);

    /* _internal_ */
bool update_grid_v3_index(dataLynx *self, uintmax_t row, uintmax_t column, char *new_field);
bool update_grid_index(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *new_field);
bool update_dict_index(dataLynx *self, uintmax_t desired_row, char *desired_column, char *new_field);


// REPLACE all instances of a particular value

    /* user facing */
bool replace(dataLynx *self, char *to_replace, char *replace_with);
bool replaceInColumn(dataLynx *self, char *column_name, char *to_replace, char *replace_with);

bool dropColumn(dataLynx *self, char *column_name);
bool dropRow(dataLynx *self, uintmax_t row_to_drop);

    /* _internal_ */
bool grid_replace(dataLynx *self, char *to_replace, char *replace_with, intmax_t column);
bool grid_v3_replace(dataLynx *self, char *to_replace, char *replace_with, intmax_t column); /* column can not be unsinged, bc if -1 is passed, function will replace in all columns */
bool dict_grid_replace(dataLynx *self, char *to_replace, char *replace_with, intmax_t column);


// GET FIELD  (from data structure

    /* user facing */
char *getField(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);
char *getField2(dataLynx *self, uintmax_t desired_row, char *desired_column);

bool dropRowsFilter(dataLynx *self, char *column_name, char *condition_operator, char *condition_value);
bool filter(dataLynx *self, dataLynx *filteredData, char *column_name, char *condition_operator, char *condition_value);
bool printColumnCond(dataLynx *self, char *column_name, char *condition_operator, char *condition_num);
bool filter_internal_(dataLynx *self, uintmax_t desired_column, char *condition_operator, char *condition_value, dataLynx *new_data, bool print, bool drop_rows);



    /* _internal_ */
char *get_field_raw(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);
char *get_field_rows(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);
// char *index_2darray_csv(char **main_array, uintmax_t row, uintmax_t column);
char *get_field_grid_v3(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num);
char *get_field_grid(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num);
char *get_field_dict(dataLynx *self, uintmax_t desired_row, char *desired_column);


// Convert to other data structures
char **string_into_2d_array(dataLynx *self);
node **split_2darray_by(dataLynx *self, char split_by);
dict_node **grid_into_dict_grid(dataLynx *self);

// Doubly linked list functions (for use inside of other functions)
bool build_dblink_list(char **s_ptr, node **head, node **last);
bool build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char **current_column_name); // TODO RENAME TO APPEND

bool sortRowsByColumn(dataLynx *self, const char *column_name, const char *asc_desc); /* self.in_place_sort & self.case_sensitive_sort will affect this function */
bool compare_for_sort_(char *field1, char *field2, bool ascending, bool case_sensitive, bool is_number);



void print_grid_v3(dataLynx *self);
void print_grid(dataLynx *self);
bool print_lnk_list(dataLynx *self, node *head, uintmax_t current_row);
bool print_dict_grid(dataLynx *self);
bool print_dict_grid2(dataLynx *self);
void stat_print_(char *stat_name, double stat, uint8_t column_strlen);


void freeAll(dataLynx *self);
bool free_header(dataLynx *self);
bool free_2d_array(dataLynx *self);
bool free_grid_v3(dataLynx *self);
bool free_grid(dataLynx *self);
bool free_list(node *head);
bool free_dict_grid(dataLynx *self);
bool free_dict_list(dict_node *head);
bool free_value_counts(dataLynx *self);
void free_last_retrieved_fields(dataLynx *self);
bool free_null(char **s);
bool fclose_null(FILE **file);


#endif /* DATALYNX_DATA_H */