// ~~~~~ DOUBLE LINK LIBRARY ~~~~~
// (STACK)

#include <dbl_lnk.h>
// #include "../include/dbl_lnk.h"

int main(void)
{
    uint64_t node_count = 0;

    // User input: Name List
    char *list_name = inf_buffer("Name of list: ");
    if (strcasecmp(list_name, "quit") == 0) {free_null(&list_name); return 0;}

    // Prepend "csv/" and append ".csv" to list name, to create file path
    char file_path[(strlen(list_name) + strlen("csv/.csv") + 1) * sizeof(char)];
    sprintf(file_path, "./csv/%s.csv", list_name);

    // Declare Main Pointers
    node *head = NULL;
    node *last = NULL;

    //  _____ BUILD DOUBLE LINKED LIST _____

    // Declare string pointers
    char *s = NULL, *choice = NULL;

    // Loop until user quits
    while (true)
    {
        bool append_item = false, quit_choice = false;

        // User Input
        s = inf_buffer("Enter item: ");

        // -- QUIT --
        if (strcasecmp(s, "quit") == 0) {
               // -- QUIT --
            while (true) {
                choice = inf_buffer("\n\t ~ QUIT ~\nDo you want to QUIT? (yes/no): ");

                if (strcasecmp(choice, "yes") == 0) {
                    quit_choice = true;
                    break;
                }
                else if (strcasecmp(choice, "no") == 0) {

                    printf("Do you want to ADD '%s' as an item to the list? (yes/no): ", s);
                    free(choice);
                    choice = inf_buffer("");

                    if (strcasecmp(choice, "yes") == 0) {
                        append_item = true;
                        free_null(&choice);
                        break;
                    }
                    else if (strcasecmp(choice, "no") == 0) {
                        free_null(&s);
                        free_null(&choice);
                        break;
                    }
                }
                printf("\n\n*** Enter 'yes' or 'no' only. ***\n");
                free_null(&choice);
            }
        }
        else if (strcasecmp(s, "remove") == 0) {
            // -- REMOVE --
            while (true) {
                choice = inf_buffer("\n\t ~ REMOVE ~\nDo you want to REMOVE the last item? (yes/no): ");

                if (strcasecmp(choice, "yes") == 0) {

                    // Attempt to Remove last item from Double Linked List
                    char *popped = list_remove_item(&head, &last, true);

                    if (popped == NULL) {
                        printf("\n\n* Nothing to remove. List empty.\n");
                    }
                    else {
                        csv_rewrite(file_path, head);
                        printf("\n* You just REMOVED: '%s'\n", popped);
                        node_count--;
                    }
                    free_null(&s);
                    free_null(&choice);
                    if (popped != NULL) {free_null(&popped);}
                    break;
                }
                else if (strcasecmp(choice, "no") == 0) {
                    free(choice);
                    printf("Do you want to ADD '%s' as an item to the list? (yes/no): ", s);

                    choice = inf_buffer("");
                    if (strcasecmp(choice, "yes") == 0) {
                        append_item = true;
                        free_null(&choice);
                        break;
                    }
                    else if (strcasecmp(choice, "no") == 0) {
                        free_null(&s);
                        free_null(&choice);
                        break;
                    }
                }
                printf("\n\n*** Enter 'yes' or 'no' only. ***\n");
                free_null(&choice);
            }
        }
        else append_item = true;

        if (quit_choice) break;

        if (append_item == true) {
            // Append Item to List
            build_dblink_list(&s, &head, &last);
            append_csv(file_path, last->s);
            node_count++;
        }

        // Print
        printf("\nLIST: '%s'\n----\n", list_name);
        print_list(head);

        // I do not need to free(s) because the buffer that s points to has been taken over by node->s
    }


    //      _____ END _____

    // Print
    printf("\nLIST: '%s'\n----\n", list_name);
    print_list(head);

    // End message
    printf("\n** A file named '%s.csv' was made for you. **\n", list_name);
    printf("\nFile path: '%s'\n\n", file_path);

    // Free Pointers
    free_null(&list_name);
    free_null(&s);
    free_null(&choice);
    free_list(head);
    head = last = NULL;

    return EXIT_SUCCESS;
}


