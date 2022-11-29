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
        if (scanReturn == EOF) {free_null(&s); if_error(3);}

        if (s[i] == '\n') {
            s[i] = '\0';
            break;
        }
        else {
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) {free_null(&s); if_error(2);}
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
    if (n == NULL) {free_list(*head); if_error(1);}

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
char *list_remove_item(node **head, node **last)
{
    // Needs to be ** bc I need to change ORIGINAL pointers back to NULL or anything else

    // Create string to return
    char *popped = NULL;
    if (*head != NULL) {
        popped = malloc((strlen((*last)->s)+1) * sizeof(char));
        if (popped == NULL) {if_error(1);}
        strcpy(popped, (*last)->s);
    }
    else {
        // Nothing in list
        return NULL;
    }

    if (*head == *last) {
        // One node left - (checking if ((*last)->prev == NULL) also works)
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
            free_null(&error_msg);
            printf("\nError: -3\nFailed in if_error() attemping to log error.\n");
            exit(-3);
        }
        i++;
    }
    error_msg[i] = '\0';

    // TIMESTAMP
    time_t timestamp = time(0);

    //  Open error LOG in Append Mode
    FILE *log_file = fopen("./src/resources/error_log.csv", "a");
    if (log_file == NULL) {
        fclose_null(&key_file);
        free_null(&error_msg);
        printf("\nError: -4\nFailed in if_error(), attemping to log error.\n");
        exit(-4);
    }

    // Append to log
    fprintf(log_file, "%i,%s,%s\n", error_num, error_msg, ctime(&timestamp));

    printf("\n\n\t** ERROR **\n\nError code: %i\n** %s **\n\n", error_num, error_msg);
    fclose_null(&key_file);
    fclose_null(&log_file);
    free_null(&error_msg);
    exit(error_num);
}
// ___ FREE NULL ___
void free_null(char **s)
{
    /* The only purpose of this function is to condense these 2 lines of code down to 1 in main.c or other calling functions. */
    free(*s);
    *s = NULL;
    return;
}

// __ FCLOSE NULL ___
void fclose_null(FILE **file)
{
    /* The only purpose of this function is to condense these 2 lines of code down to 1. Like free_null(), for file pointers. */
    fclose(*file);
    *file = NULL;
    return;
}
