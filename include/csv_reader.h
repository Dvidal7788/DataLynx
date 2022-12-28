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

// Double Linked Node
typedef struct node
{
    char *s;
    struct node *prev;
    struct node *next;
}
node;

// Double Linked Node for Dict
typedef struct dict_node
{
    char *s;
    char *column_name;
    struct dict_node *prev;
    struct dict_node *next;
}
dict_node;

// Function Prototypes
char *inf_buffer(char *prompt);
void build_dblink_list(char **s_ptr, node **head, node **last);
void print_list(node *head);
void free_list(node *head);
void if_error(int16_t error_num);
void free_null(char **filename);
void fclose_null(FILE **file);
void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name);
void free_dict_list(dict_node **main_array, uintmax_t row_count);

char *read_file_v1(FILE *file);
char **string_into_2d_array(char *s, uintmax_t *row_count);
char **read_file_v2(FILE *file, uintmax_t *row_count);
char *csv_reader_index(FILE *file, uintmax_t row, uintmax_t col, bool skip_header);
char *csv_dictreader_index(FILE *file, uintmax_t row, char *desired_column);
uint64_t get_uint(char *prompt);
bool is_ext(char *filename, char *ext);
char *index_2darray_csv(char **main_array, uintmax_t row_num, uintmax_t index);
node **split_by(char **orig_array, uintmax_t row_count, char split);
char **get_csv_header(FILE *file, uintmax_t *column_count);
dict_node **csv_dict_reader(FILE *file, uintmax_t *row_count);
char *index_into_dict(dict_node **main_array, uintmax_t row_count, uintmax_t row, char *desired_column);
void print_dict_list(dict_node **main_array, uintmax_t row_count);

#endif /* CSV_READER_H */