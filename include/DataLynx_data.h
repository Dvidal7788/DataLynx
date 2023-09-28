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
enum ErrorCodes {MALLOC_FAILED=1, REALLOC_FAILED, FOPEN_FAILED, FREAD_FAILED, SCANF_FAILED};
#define NUM_OF_FUNCTIONS 25 /* For use in if_error()*/

#define MISSING_VALUE "---" /* This is the default missing value, which gets assigned to the char * */

#define FILENAME_BUFFER_SIZE 255
// #define MAX_FIELD_PRINT_LENGTH 50 /* This number INCLUDES the borders of the table (i.e. '|' and '+') */

/* This will be passed as the column_name parameter to tell print_stats_internal_ to print all stats as opposed to 1 column's stats (Unless somebody has a CSV with a column named this exact random code, in which case, if they request to print only that column's stats, it will print all stats, but that's a tiny risk I'm willing to take.) */
#define PRINT_ALL_STATS "-9!8?3$6*&()\"\";;:://8Vj49r4HU2@b"

// These macros will be used to inform the internal strip functions whether to strip from the left, right or both sides
#define STRIP_LEFT (-1)
#define STRIP_RIGHT 1
#define STRIP_BOTH 0



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


//      -- Main DataLynx 'Object' --
typedef struct DataLynx {

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

    char *(*header)(struct DataLynx *self, uint32_t column);

    char *(*userInputFilename)(struct DataLynx *self, char *prompt);
    bool (*changeFilename)(struct DataLynx *self, char *filename);

    // .csv - (Functions that read/write directly to CSV file)
    struct {
        bool (*openFile)(struct DataLynx *self, char *filename);
        char **(*headerReader)(struct DataLynx *self);
        char *(*fileReader)(struct DataLynx *self);
        char **(*fileRowReader)(struct DataLynx *self);

        char ***(*reader_v3)(struct DataLynx *self);
        node **(*reader)(struct DataLynx *self);
        dict_node **(*dictReader)(struct DataLynx *self);

        char *(*fieldReader)(struct DataLynx *self, uintmax_t desired_row, char *desired_column);
        char *(*fieldReaderIdx)(struct DataLynx *self, uintmax_t desired_row, uintmax_t desired_colum);

        bool (*fieldWriter)(struct DataLynx *self, uintmax_t row, char *column_name, char *new_field);
        bool (*fieldWriterIdx)(struct DataLynx *self, uintmax_t row, uintmax_t column, char *new_field);

        bool (*rowWriter)(struct DataLynx *self, char *values[]);
        bool (*rowDictWriter)(struct DataLynx *self, dict values[]);

        bool (*renameFile)(struct DataLynx *self, char *new_filename);
        bool (*backup)(struct DataLynx *self);
        bool (*writeData)(struct DataLynx *self, char *new_filename);

    } csv;



    // Get Field  (from data structure in memory)
    char *(*getField)(struct DataLynx *self, uintmax_t desired_row, char *desired_column);
    char *(*getFieldIdx)(struct DataLynx *self, uintmax_t desired_row, uintmax_t desired_column);


    bool (*updateField)(struct DataLynx *self, uintmax_t row, char *column_name, char *new_value);
    bool (*updateFieldIdx)(struct DataLynx *self, uintmax_t row, uintmax_t column, char *new_value);

    bool (*stripField)(struct DataLynx *self, uintmax_t row, char *column_name);
    bool (*stripFieldL)(struct DataLynx *self, uintmax_t row, char *column_name);
    bool (*stripFieldR)(struct DataLynx *self, uintmax_t row, char *column_name);

    bool (*stripFieldIdx)(struct DataLynx *self, uintmax_t row, uintmax_t column);
    bool (*stripFieldIdxL)(struct DataLynx *self, uintmax_t row, uintmax_t column);
    bool (*stripFieldIdxR)(struct DataLynx *self, uintmax_t row, uintmax_t column);

    bool (*stripColumn)(struct DataLynx *self, char *column_name);
    bool (*stripColumnL)(struct DataLynx *self, char *column_name);
    bool (*stripColumnR)(struct DataLynx *self, char *column_name);

    bool (*stripColumnIdx)(struct DataLynx *self, uintmax_t column);
    bool (*stripColumnIdxL)(struct DataLynx *self, uintmax_t column);
    bool (*stripColumnIdxR)(struct DataLynx *self, uintmax_t column);

    bool (*stripAll)(struct DataLynx *self);
    bool (*stripAllL)(struct DataLynx *self);
    bool (*stripAllR)(struct DataLynx *self);

    bool (*formatHeader)(struct DataLynx *self);
    bool (*changeColumnName)(struct DataLynx *self, char *old_column_name, char *new_column_name);
    bool (*printHeader)(struct DataLynx *self);
    bool (*printColumn)(struct DataLynx *self, char *column_name);
    bool (*printData)(struct DataLynx *self);
    bool (*printDataTable)(struct DataLynx *self);
    bool (*printStatsAll)(struct DataLynx *self);
    bool (*printStatsColumn)(struct DataLynx *self, char *column_name);
    bool (*printShape)(struct DataLynx *self);
    bool (*printColumnCond)(struct DataLynx *self, char *column_name, char *condition_operator, char *condition_num);
    void (*freeAll)(struct DataLynx *self);
    char *(*changeMissingValue)(struct DataLynx *self, char *missingValue);

    bool (*replaceAll)(struct DataLynx *self, char *to_replace, char *replace_with);
    bool (*replaceInColumn)(struct DataLynx *self, char *column_name, char *to_replace, char *replace_with);
    bool (*dropColumn)(struct DataLynx *self, char *column_name);
    bool (*dropRow)(struct DataLynx *self, uintmax_t row_to_dop);

    bool (*createHeader)(struct DataLynx *self, char *header[], uint32_t column_count);
    bool (*insertRow)(struct DataLynx *self, char *values[]);
    bool (*insertRowDict)(struct DataLynx *self, dict values[]);

    bool (*sortRowsByColumn)(struct DataLynx *self, const char *column_name, const char *asc_desc);

    uint16_t (*valueCount)(struct DataLynx *self, char *value, char *column_name);
    bool (*isInColumn)(struct DataLynx *self, char *value, char *column_name);
    bool (*isInData)(struct DataLynx *self, char *value);
    double (*getStat)(struct DataLynx *self, char *column_name, char *operation);
    double (*min)(struct DataLynx *self, char *column_name);
    double (*max)(struct DataLynx *self, char *column_name);
    double (*sum)(struct DataLynx *self, char *column_name);
    double (*mean)(struct DataLynx *self, char *column_name);
    uintmax_t (*isNull)(struct DataLynx *self, char *column_name);
    uintmax_t (*notNull)(struct DataLynx *self, char *column_name);


    // Convert data structures
    char **(*string_into_2d_array)(struct DataLynx *myCSV);
    node **(*split_2darray_by)(struct DataLynx *self, char split_by);
    char *(*index_into_dict)(struct DataLynx *self, uintmax_t desired_row, char *desired_column);

    // Update
    bool (*update_dict_index)(struct DataLynx *self, uintmax_t desired_row, char *desired_column, char *new_field);


} DataLynx;


//          ---- FUNCTION PROTOTYPES ----

DataLynx DataLynxConstructor(void);
char *header(DataLynx *self, uint32_t column);

bool createHeader(DataLynx *self, char *header[], uint32_t columnCount);
bool insertRow(DataLynx *self, char *values[]);
bool insertRowDict(DataLynx *self, dict values[]);

char *userInputFilename(DataLynx *self, char *prompt);
bool changeFilename(DataLynx *self, char *filename);

intmax_t findColumnIndex(DataLynx *self, const char *desired_column);

bool stripField(DataLynx *self, uintmax_t row, char *column_name);
bool stripFieldL(DataLynx *self, uintmax_t row, char *column_name);
bool stripFieldR(DataLynx *self, uintmax_t row, char *column_name);

bool stripFieldIdx(DataLynx *self, uintmax_t row, uintmax_t column);
bool stripFieldIdxL(DataLynx *self, uintmax_t row, uintmax_t column);
bool stripFieldIdxR(DataLynx *self, uintmax_t row, uintmax_t column);

bool strip_field_helper_(DataLynx *self, uintmax_t row, uintmax_t column, int8_t strip_side);

bool stripColumn(DataLynx *self, char *column_name);
bool stripColumnL(DataLynx *self, char *column_name);
bool stripColumnR(DataLynx *self, char *column_name);

bool stripColumnIdx(DataLynx *self, uintmax_t column);
bool stripColumnIdxL(DataLynx *self, uintmax_t column);
bool stripColumnIdxR(DataLynx *self, uintmax_t column);

bool strip_column_internal_(DataLynx *self, uintmax_t column, int8_t strip_side);

bool stripAll(DataLynx *self);
bool stripAllL(DataLynx *self);
bool stripAllR(DataLynx *self);

bool strip_all_internal_(DataLynx *self, int8_t strip_side);
bool strip_field_internal_(DataLynx *self, uintmax_t row, uintmax_t column, char *field, node *grid_tmp, dict_node *dict_tmp, int8_t strip_side);


bool changeColumnName(DataLynx *self, char *old_column_name, char *new_column_name);
bool formatHeader(DataLynx *self);
bool printHeader(DataLynx *self);
bool printColumn(DataLynx *self, char *column_name);
bool printDataTable(DataLynx *self);
bool printData(DataLynx *self);
bool print_data_internal(DataLynx *self);
bool printHead(DataLynx *self, uintmax_t number_of_rows);
bool printTail(DataLynx *self, uintmax_t number_of_rows);
bool printStatsAll(DataLynx *self);
bool printStatsColumn(DataLynx *self, char *column_name);
bool print_stats_internal_(DataLynx *self, char *column_name);
void print_stats_is_not_null_(DataLynx *self, size_t column_strlen, uint32_t column_index, bool is_null);
bool printShape(DataLynx *self);
char *changeMissingValue(DataLynx *self, char *missingValue);

uintmax_t find_row_count(DataLynx *self);


//  UPDATE fields in memory

    /* User facing */
bool updateField(DataLynx *self, uintmax_t row, char *column_name, char *new_value);
bool updateFieldIdx(DataLynx *self, uintmax_t row, uintmax_t column, char *new_value);

    /* _internal_ */
bool update_field_internal_(DataLynx *self, uintmax_t row, uintmax_t column, char *new_value);
bool update_grid_v3_index(DataLynx *self, uintmax_t row, uintmax_t column, char *new_field);
bool update_grid_index(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *new_field);
bool update_dict_index(DataLynx *self, uintmax_t desired_row, char *desired_column, char *new_field);


// REPLACE all instances of a particular value

    /* user facing */
bool replaceAll(DataLynx *self, char *to_replace, char *replace_with);
bool replaceInColumn(DataLynx *self, char *column_name, char *to_replace, char *replace_with);

bool dropColumn(DataLynx *self, char *column_name);
bool dropRow(DataLynx *self, uintmax_t row_to_drop);

    /* _internal_ */
bool grid_replace(DataLynx *self, char *to_replace, char *replace_with, intmax_t column);
bool grid_v3_replace(DataLynx *self, char *to_replace, char *replace_with, intmax_t column); /* column can not be unsinged, bc if -1 is passed, function will replace in all columns */
bool dict_grid_replace(DataLynx *self, char *to_replace, char *replace_with, intmax_t column);


// GET FIELD  (from data structure

    /* user facing */
char *getField(DataLynx *self, uintmax_t desired_row, char *desired_column);
char *getFieldIdx(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column);

bool dropRowsFilter(DataLynx *self, char *column_name, char *condition_operator, char *condition_value);
bool filter(DataLynx *self, DataLynx *filteredData, char *column_name, char *condition_operator, char *condition_value);
bool printColumnCond(DataLynx *self, char *column_name, char *condition_operator, char *condition_num);
bool filter_internal_(DataLynx *self, uintmax_t desired_column, char *condition_operator, char *condition_value, DataLynx *new_data, bool print, bool drop_rows);



    /* _internal_ */
char *get_field_raw(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column);
char *get_field_rows(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column);
// char *index_2darray_csv(char **main_array, uintmax_t row, uintmax_t column);
char *get_field_grid_v3(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num);
char *get_field_grid(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num);
char *get_field_dict(DataLynx *self, uintmax_t desired_row, char *desired_column);


// Convert to other data structures
char **string_into_2d_array(DataLynx *self);
node **split_2darray_by(DataLynx *self, char split_by);
dict_node **grid_into_dict_grid(DataLynx *self);

// Doubly linked list functions (for use inside of other functions)
bool build_dblink_list(char **s_ptr, node **head, node **last);
bool build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char **current_column_name); // TODO RENAME TO APPEND

bool sortRowsByColumn(DataLynx *self, const char *column_name, const char *asc_desc); /* self.in_place_sort & self.case_sensitive_sort will affect this function */
bool compare_for_sort_(char *field1, char *field2, bool ascending, bool case_sensitive, bool is_number);



void print_grid_v3(DataLynx *self);
void print_grid(DataLynx *self);
bool print_lnk_list(DataLynx *self, node *head, uintmax_t current_row);
bool print_dict_grid(DataLynx *self);
bool print_dict_grid2(DataLynx *self);
void stat_print_(char *stat_name, double stat, uint8_t column_strlen);


void freeAll(DataLynx *self);
bool free_header(DataLynx *self);
bool free_2d_array(DataLynx *self);
bool free_grid_v3(DataLynx *self);
bool free_grid(DataLynx *self);
bool free_list(node *head);
bool free_dict_grid(DataLynx *self);
bool free_dict_list(dict_node *head);
bool free_value_counts(DataLynx *self);
void free_last_retrieved_fields(DataLynx *self);
bool free_null(char **s);
bool fclose_null(FILE **file);


#endif /* DATALYNX_DATA_H */
