#ifndef DBL_LNK_H
#define DBL_LNK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
void append_csv(char *file_name, char *s);
void csv_rewrite(char *file_path, node *head);
char *inf_buffer(char *prompt);
void build_dblink_list(char **s_ptr, node **head, node **last);
char *list_remove_item(node **head, node **last, bool pop_or_not);
void print_list(node *head);
void free_list(node *head);
void if_error(int16_t error_num);
void free_null(char **filename);
void fclose_null(FILE **file);
void underline(uint64_t n);
void print_list_and_listname(node *head, char *list_name);
void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name);
void free_dict_list(dict_node **main_array, uintmax_t row_count);

#endif /* DBL_LNK_H */