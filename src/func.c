#include "../include/header.h"


// ___ APPEND TO CSV ___
void append_csv(char *file_name, char *s)
{
    FILE *file = fopen(file_name, "a");
    if (file == NULL) {if_error(4);}

    fprintf(file, "%s\n", s);

    fclose(file);
    file = NULL;
    return;
}

// ___ CSV: REWRITE ___
void csv_rewrite(char *file_path, node *head)
{
    // *This funcion is typically called after list_remove_item() to rewrite list to csv

    // Open File: (Write Mode)
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {if_error(4);}

    node *tmp = head;
    while (tmp != NULL)
    {
        fprintf(file, "%s\n", tmp->s);
        tmp = tmp->next;
    }

    fclose(file);
    file = NULL;
    return;
}

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
    while (true)
    {
        int scanReturn = scanf("%c", &s[i]);
        if (scanReturn == EOF) {if_error(3);}

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
void build_dblink_list(char *s, node **head, node **last)
{
    // char *s input can NOT be string literal (maybe change to **)
        // Must be dynamically allocated string
    // -- APPEND NODE --
    // Create new node
    node *n = malloc(sizeof(node));
    if (n == NULL) {if_error(1);}

    n->s = s;
    s = NULL;
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
char *list_remove_item(node **head, node **last, uint8_t return_or_not)
{
    // Needs to be ** bc I need to chagen ORIGINAL pointers nack to NULL or anything else

    // Create string to return
    char *popped = NULL;
    if (return_or_not == 1) {
        popped = malloc((strlen((*last)->s)+1) * sizeof(char));
        if (popped == NULL) {if_error(1);}

        sprintf(popped, "%s", (*last)->s);
    }

    // V2
    if (*head == NULL) {
        printf("\n\nNothing to remove. List empty.\n");
    }
    else if (*head == *last) {
        // Does last->prev == NULL not work?
        // One node left

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


    // Return string
    if (return_or_not == 1) {
        return popped;
    }

    return NULL;
}

// ___ PRINT LIST ___
void print_list(node *head)
{
    node *tmp = head;
    while (tmp != NULL)
    {
        printf("%s ", tmp->s);
        tmp = tmp->next;
    }
    printf("\n\n");

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
    // TO DO: READ FROM resources/error_key
    // Use: error message (input to this function) should not have "error:" or '\n'
    // Fill in later
    FILE *file = fopen("src/resources/errors_key.txt", "r");
    if (file == NULL) {exit(-1);}

    // Get file stream to correct error message in key
    char c = 0;
    while ((c = fgetc(file)) != error_num+48);
    while ((c = fgetc(file)) != '\n');

    char error_msg[250];

    uint8_t i = 0;
    while ((c = fgetc(file)) != '\n') {
        error_msg[i] = c;
        i++;
    }
    error_msg[i] = '\0';

    printf("\n\n\t** ERROR **\n\nError code: %i\n** %s **\n\n", error_num, error_msg);

    fclose(file);
    exit(error_num);
}
// ___ FREE NULL ___
void free_null(char **s)
{
    // The only purpose of this function is to condense these 2 lines of code down to 1 in main.c
    free(*s);
    *s = NULL;
    return;
}
