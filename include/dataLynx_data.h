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
// #define MAX_FIELD_PRINT_LENGTH 50 /* This number INCLUDES the borders of the table (i.e. '|' and '+') */

/* This will tell print_stats_internal_ to print all stats as opposed to 1 column's stats (Unless somebody has a CSV with a column named this exact random code, in which case, if they request to print only that column's stats, it will print all stats, but that's a risk I'm willing to take lol) */
#define PRINT_ALL_STATS "-9!8?3$6*&()\"\";;:://8Vj49r4HU2@b"


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
    uint16_t maxFieldPrintLength;
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

    char **__header__;
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

    char *(*header)(struct dataLynx *self, uint32_t column);

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

        char *(*fieldReader)(struct dataLynx *self, uintmax_t desired_row, char *desired_column);
        char *(*fieldReader2)(struct dataLynx *self, uintmax_t desired_row, uintmax_t desired_colum);

        bool (*fieldWriter)(struct dataLynx *self, uintmax_t row, char *column_name, char *new_field);
        bool (*fieldWriter2)(struct dataLynx *self, uintmax_t row, uintmax_t column, char *new_field);

        bool (*rowWriter)(struct dataLynx *self, char *values[]);
        bool (*rowDictWriter)(struct dataLynx *self, dict values[]);

        bool (*backup)(struct dataLynx *self);
        bool (*writeData)(struct dataLynx *self, char *new_filename);

    } csv;



    // Get Field  (from data structure in memory)
    char *(*getField)(struct dataLynx *self, uintmax_t desired_row, char *desired_column);
    char *(*getField2)(struct dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);


    bool (*updateField)(struct dataLynx *self, uintmax_t row, char *column_name, char *new_value);
    bool (*updateField2)(struct dataLynx *self, uintmax_t row, uintmax_t column, char *new_value);


    bool (*stripAll)(struct dataLynx *self);
    bool (*formatHeader)(struct dataLynx *self);
    bool (*changeColumnName)(struct dataLynx *self, char *old_column_name, char *new_column_name);
    bool (*printHeader)(struct dataLynx *self);
    bool (*printColumn)(struct dataLynx *self, char *column_name);
    bool (*printData)(struct dataLynx *self);
    bool (*printDataTable)(struct dataLynx *self);
    bool (*printStatsAll)(struct dataLynx *self);
    bool (*printStatsColumn)(struct dataLynx *self, char *column_name);
    bool (*printShape)(struct dataLynx *self);
    bool (*printColumnCond)(struct dataLynx *self, char *column_name, char *condition_operator, char *condition_num);
    void (*freeAll)(struct dataLynx *self);
    char *(*changeMissingValue)(struct dataLynx *self, char *missingValue);

    bool (*replaceAll)(struct dataLynx *self, char *to_replace, char *replace_with);
    bool (*replaceInColumn)(struct dataLynx *self, char *column_name, char *to_replace, char *replace_with);
    bool (*dropColumn)(struct dataLynx *self, char *column_name);
    bool (*dropRow)(struct dataLynx *self, uintmax_t row_to_dop);

    bool (*createHeader)(struct dataLynx *self, char *header[], uint32_t column_count);
    bool (*insertRow)(struct dataLynx *self, char *values[]);
    bool (*insertRowDict)(struct dataLynx *self, dict values[]);

    bool (*sortRowsByColumn)(struct dataLynx *self, const char *column_name, const char *asc_desc);

    uint16_t (*valueCount)(struct dataLynx *self, char *value, char *column_name);
    bool (*isInColumn)(struct dataLynx *self, char *value, char *column_name);
    bool (*isInData)(struct dataLynx *self, char *value);
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

dataLynx dataLynxConstructor(void);
char *header(dataLynx *self, uint32_t column);

bool createHeader(dataLynx *self, char *header[], uint32_t columnCount);
bool insertRow(dataLynx *self, char *values[]);
bool insertRowDict(dataLynx *self, dict values[]);

char *userInputFilename(dataLynx *self, char *prompt);
bool changeFilename(dataLynx *self, char *filename);

intmax_t findColumnIndex(dataLynx *self, const char *desired_column);

bool stripField(dataLynx *self, uintmax_t row, char *column_name);
bool strip_internal_(dataLynx *self, uintmax_t row, uintmax_t column, char *field, node *grid_tmp, dict_node *dict_tmp);
bool stripAll(dataLynx *self);

// bool stripColumn(dataLynx *self, char *column_name);
// bool strip(dataLynx *self);
bool changeColumnName(dataLynx *self, char *old_column_name, char *new_column_name);
bool formatHeader(dataLynx *self);
bool printHeader(dataLynx *self);
bool printColumn(dataLynx *self, char *column_name);
bool printDataTable(dataLynx *self);
bool printData(dataLynx *self);
bool print_data_internal(dataLynx *self);
bool printHead(dataLynx *self, uintmax_t number_of_rows);
bool printTail(dataLynx *self, uintmax_t number_of_rows);
bool printStatsAll(dataLynx *self);
bool printStatsColumn(dataLynx *self, char *column_name);
bool print_stats_internal_(dataLynx *self, char *column_name);
void print_stats_is_not_null_(dataLynx *self, size_t column_strlen, uint32_t column_index, bool is_null);
bool printShape(dataLynx *self);
char *changeMissingValue(dataLynx *self, char *missingValue);

uintmax_t find_row_count(dataLynx *self);


//  UPDATE fields in memory

    /* User facing */
bool updateField(dataLynx *self, uintmax_t row, char *column_name, char *new_value);
bool updateField2(dataLynx *self, uintmax_t row, uintmax_t column, char *new_value);

    /* _internal_ */
bool update_field_internal_(dataLynx *self, uintmax_t row, uintmax_t column, char *new_value);
bool update_grid_v3_index(dataLynx *self, uintmax_t row, uintmax_t column, char *new_field);
bool update_grid_index(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *new_field);
bool update_dict_index(dataLynx *self, uintmax_t desired_row, char *desired_column, char *new_field);


// REPLACE all instances of a particular value

    /* user facing */
bool replaceAll(dataLynx *self, char *to_replace, char *replace_with);
bool replaceInColumn(dataLynx *self, char *column_name, char *to_replace, char *replace_with);

bool dropColumn(dataLynx *self, char *column_name);
bool dropRow(dataLynx *self, uintmax_t row_to_drop);

    /* _internal_ */
bool grid_replace(dataLynx *self, char *to_replace, char *replace_with, intmax_t column);
bool grid_v3_replace(dataLynx *self, char *to_replace, char *replace_with, intmax_t column); /* column can not be unsinged, bc if -1 is passed, function will replace in all columns */
bool dict_grid_replace(dataLynx *self, char *to_replace, char *replace_with, intmax_t column);


// GET FIELD  (from data structure

    /* user facing */
char *getField(dataLynx *self, uintmax_t desired_row, char *desired_column);
char *getField2(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);

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