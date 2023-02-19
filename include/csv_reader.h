#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <time.h>

// Doubly Linked Node
typedef struct node
{
    char *s;
    struct node *prev;
    struct node *next;
}
node;

// Doubly Linked Node for Dict
typedef struct dict_node
{
    char *s;
    char *column_name;
    struct dict_node *prev;
    struct dict_node *next;
}
dict_node;

// Error Codes
enum ErrorCodes {MALLOC_FAILED=1, REALLOC_FAILED, SCANF_FAILED, FOPEN_FAILED};

#define NUM_OF_FUNCTIONS 25 /* For use in if_error()*/

//          ---- FUNCTION PROTOTYPES ----
// Doubly linked list functions (for use inside of other functions)
void build_dblink_list(char **s_ptr, node **head, node **last);
void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name);

// Functions for reading/updating csv files and organizing data structures
char *read_file_v1(FILE *file, bool skip_header);
char **string_into_2d_array(char *s, uintmax_t *row_count);
char **read_file_v2(FILE *file, uintmax_t *row_count, bool skip_header);
char *index_2darray_csv(char **main_array, uintmax_t row, uintmax_t column);
node **split_2darray_by(char **orig_array, uintmax_t row_count, char split_by);
char *csv_reader_index(FILE *file, uintmax_t row, uintmax_t col, bool skip_header);
void update_csv_index(char *filename, uintmax_t row, uintmax_t column, char *new_cell);
bool has_quotes(char *s);
bool has_comma(char *s);
void add_quotes(char **s);
char **get_csv_header(FILE *file, uintmax_t *column_count);
bool verify_column(char **header, uintmax_t column_count, char *column);
char *csv_dictreader_index(FILE *file, uintmax_t row, char *desired_column);
dict_node **csv_dict_reader(FILE *file, uintmax_t *row_count);
char *index_into_dict(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column);
bool update_dict_index(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column, char *new_cell);

// User input/utility functions
char *inf_buffer(char *prompt);
uint64_t get_uint(char *prompt);
bool is_ext(char *filename, char *ext);
void print_header(char **header, uintmax_t *column_count, bool with_spaces);
void print_list(node *head);
void print_dict_list(dict_node **main_array, uintmax_t row_count);
void free_list(node *head);
void free_dict_list(dict_node **main_array, uintmax_t row_count);
void free_null(char **filename);
void fclose_null(FILE **file);
void if_error(uint8_t error_code, const char *function_name);


#endif /* CSV_READER_H */