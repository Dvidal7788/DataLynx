// #include <dbl_lnk.h>
#include "../include/csv_reader.h"

// ___ INFINITE BUFFER (User Input) ___
char *inf_buffer(char *prompt)
{
    // Prompt user
    printf("%s", prompt);

    // Allocate 1st char of string
    char *s = malloc(sizeof(char));
    if (s == NULL) {if_error(1);}

    // Scan command line char by char
    uint64_t i = 0;
    int scanReturn;
    while ((scanReturn = scanf("%c", &s[i])) == 1)
    {
        // Check EOF
        if (scanReturn == EOF) {free_null(&s); if_error(3);}

        // Check if end of user input
        if (s[i] == '\n') {
            s[i] = '\0';
            break;
        }
        else {
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) {if_error(2);}
            i++;
        }
    }

    return s;
}

// ___ BUILD DOUBLE LINK LIST ___
void build_dblink_list(char **s_ptr, node **head, node **last)
{
    // string input changed to **:
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string
    // -- APPEND NODE --
    // Create new node
    node *n = malloc(sizeof(node));
    if (n == NULL) {free_list(*head); if_error(1);}

    // This string pointer in the node takes over the allocated string buffer (i.e. **s_ptr).
    n->s = *s_ptr;
    *s_ptr = NULL;
    s_ptr = NULL;
    n->prev = n->next = NULL;

    // Append node to linked list
    if (*head == NULL) {
        *head = *last = n;
    }
    else {
        (*last)->next = n;
        n->prev = *last;
        *last = n;
    }

    return;
}

// ___ LIST: REMOVE ITEM ___
char *list_remove_item(node **head, node **last, bool pop_or_not)
{
    // Needs to be ** bc I need to change ORIGINAL pointers back to NULL or anything else
    /* Will return NULL if list is empty (nothing to remove) OR if !pop_or_not  */

    // Create string to return removed item
    char *popped = NULL;

    // Check if list is empty first
    if (*head != NULL) {

        // POP OR NOT: Programmer must chose whether to pop or not. (So they don't have to free(popped) if not used.)
        if (pop_or_not) {

            // Allocate popped buffer
            popped = malloc((strlen((*last)->s)+1) * sizeof(char));
            if (popped == NULL) {free_list(*head); if_error(1);}
            strcpy(popped, (*last)->s);
        }
    }
    else {
        // Nothing in list
        return NULL;
    }

    // - REMOVE LAST NODE - (i.e. pop off stack)
    if (*head == *last) {
        // One node left - if (*head == *last) OR if ((*last)->prev == NULL), both work
        free((*last)->s);
        (*last)->s = NULL;
        (*last)->prev = (*last)->next = NULL;
        free(*last);
        *head = *last = NULL;
    }
    else
    {
        node *tmp = (*last)->prev;
        tmp->next = NULL;
        free((*last)->s);
        (*last)->s = NULL;
        (*last)->prev = (*last)->next = NULL;
        free(*last);
        *last = tmp;
    }

    return popped;
}

// ___ PRINT LIST ___
void print_list(node *head)
{
    node *tmp = head;
    printf("[");
    while (tmp != NULL) {
        if (tmp->next == NULL) {
            printf("'%s'", tmp->s);
        }
        else printf("'%s', ", tmp->s);

        tmp = tmp->next;
    }
    printf("]\n\n");

    return;
}


// ___ FREE ___
void free_list(node *head)
{
    node *tmp = NULL;
    while (head != NULL)
    {
        tmp = head->next;
        free(head->s);
        head->s = NULL;
        head->prev = head->next = NULL;
        free(head);
        head = tmp;
    }

    tmp = head = NULL;
    return;
}

// ___ IF ERROR ___
void if_error(int16_t error_num)
{
    // Open error KEY to READ from
    FILE *key_file = fopen("src/resources/errorskey_master.txt", "r");
    if (key_file == NULL) {
        printf("\nError: -1\nFailed in if_error(), attemping to log error.\n");
        exit(-1);
    }

    // Get file stream to correct error message in key
    char c = 0;
    while ((c = fgetc(key_file)) != error_num+48);
    while ((c = fgetc(key_file)) != '\n');

    // Allocate string for error msg (this allows end user full modularity for any length error msg)
    char *error_msg = malloc(sizeof(char));
    if (error_msg == NULL) {
        fclose_null(&key_file);
        printf("\nError: -2\nFailed in if_error(), attemping to log error.\n");
        exit(-2);
        }

    // Read error msg from key
    uint8_t i = 0;
    while ((c = fgetc(key_file)) != '\n') {
        error_msg[i] = c;
        error_msg = realloc(error_msg, sizeof(char)*(i+2));
        if (error_msg == NULL) {
            fclose_null(&key_file);
            printf("\nError: -3\nFailed in if_error() attemping to log error.\n");
            exit(-3);
        }
        i++;
    }
    error_msg[i] = '\0';

    //  Open error LOG in Append Mode
    FILE *log_file = fopen("./src/resources/error_log.csv", "a+");
    if (log_file == NULL) {
        fclose_null(&key_file);
        free_null(&error_msg);
        printf("\nError: -4\nFailed in if_error(), attemping to log error.\n");
        exit(-4);
    }

    // TIMESTAMP
    time_t time_rtrn = time(0);
    uint8_t l = strlen(ctime(&time_rtrn));

    char timestamp[l];
    strcpy(timestamp, ctime(&time_rtrn));
    timestamp[l-1] = '\0';

    // Append to log
    fprintf(log_file, "%i,%s,%s\n", error_num, error_msg, timestamp);

    // End
    printf("\n\n\t** ERROR **\n\nError code: %i\n\n** %s **\n\n%s\n", error_num, error_msg, timestamp);
    fclose_null(&key_file);
    fclose_null(&log_file);
    free_null(&error_msg);
    exit(error_num);
}

// ___ FREE NULL ___
void free_null(char **filename)
{
    /* The only purpose of this function is to condense these 2 lines of code down to 1 in calling function. */
    free(*filename);
    *filename = NULL;
    return;
}

// __ FCLOSE NULL ___
void fclose_null(FILE **file)
{
    /* The only purpose of this function is to condense these 2 lines of code down to 1. Like free_null(), but for file pointers. */
    fclose(*file);
    *file = NULL;
    return;
}
void underline(uint64_t n)
{
    for (uint16_t i = 0; i < n; i++) {
        printf("-");
    }
}
void print_list_and_listname(node *head, char *list_name)
{
    printf("----   ");
    underline(strlen(list_name));
    printf("\nLIST: '%s'\n----   ", list_name);
    underline(strlen(list_name));
    printf("\n\n");
    print_list(head);
}

// ___ BUILD - DICT - DOUBLE LINK LIST ___
void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name)
{
    // string input changed to **:
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string
    // -- APPEND NODE --
    // Create new node
    dict_node *n = malloc(sizeof(dict_node));
    if (n == NULL) {/*free_dict_list(*head);*/ if_error(1);}

    // This string pointer in the node takes over the allocated string buffer (i.e. **s_ptr).
    n->s = *s_ptr;
    *s_ptr = NULL;
    s_ptr = NULL;

    n->column_name = malloc(sizeof(char)*strlen(current_column_name)+1);
    if (n->column_name == NULL) exit(1);
    strcpy(n->column_name, current_column_name);

    n->prev = n->next = NULL;

    // Append node to linked list
    if (*head == NULL) {
        *head = *last = n;
    }
    else {
        (*last)->next = n;
        n->prev = *last;
        *last = n;
    }

    return;
}

// ___ FREE DICT LIST ___
void free_dict_list(dict_node **main_array, uintmax_t row_count)
{
    // Iterate through each row (i.e thorugh main array of dict_node pointers)
    dict_node *tmp = NULL;
    for (uintmax_t i = 0; i < row_count; i++)
    {
        // Traverse linked list at current index in main array
        // tmp = NULL;
        while (main_array[i] != NULL)
        {
            tmp = main_array[i]->next;
            free_null(&main_array[i]->s);
            free_null(&main_array[i]->column_name);
            main_array[i]->prev = main_array[i]->next = NULL;
            free(main_array[i]);
            main_array[i] = tmp;
        }

    }
    tmp = NULL;
    main_array = NULL;
    return;
}