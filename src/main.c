// ~~~~~ DOUBLE LINK CSV LIBRARY ~~~~~
// (STACK)

// TO DO :
    // remove
    // if error read from txt
    // csv rewrite()
    // Hitting enter creates blank item in csv, but not link list
#include "../include/header.h"

int main(void)
{
    uint64_t node_count = 0;
    // TO DO:
    // Add quit as item in list
    // REMOVE FUNCTION
    // write_list_csv() - TEST when make REMOVE FUNCTION

    // User input: Name List
    char *list_name = inf_buffer("Name of list: ");

    // Prepend "csv/" and append ".csv" to list name, to create file path
    char file_path[(strlen(list_name) + strlen("csv/.csv") + 1) * sizeof(char)];
    sprintf(file_path, "csv/%s.csv", list_name);

    // Declare Main Pointers
    node *head = NULL;
    node *last = NULL;

    //  _____ BUILD DOUBLE LINKED LIST _____

    // Declare string pointers
    char *s = NULL, *choice = NULL;

    // Loop until user quits
    while (true)
    {
        bool append_item = false;

        // User Input
        s = inf_buffer("Enter item: ");

        // -- QUIT --
        if (strcasecmp(s, "quit") == 0) {
            break;
        }
        else if (strcasecmp(s, "remove") == 0) {
            // -- REMOVE --
            while (true) {
                choice = inf_buffer("\n\t ~ REMOVE ~\nDo you want to REMOVE the last item? ");
                if (strcasecmp(choice, "yes") == 0) {
                    list_remove_item(&head, &last, 0);
                    csv_rewrite(file_path, head);
                    free_null(&s);
                    free_null(&choice);
                    break;
                }
                else if (strcasecmp(choice, "no") == 0) {
                    free(choice);
                    choice = inf_buffer("Do you want to ADD 'remove' as an item to the list? ");
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

        if (append_item == true) {
            // Append Item to List
            build_dblink_list(s, &head, &last);
            append_csv(file_path, last->s);
        }

        // Print
        printf("\nList: '%s':\n\n", list_name);
        print_list(head);
    }

    //      _____ END _____

    print_list(head);
    printf("\n** A file named '%s.csv' was made for you. **\n", list_name);
    printf("\nFile path: '%s'\n\n", file_path);

    // Free Pointers
    free_list(head);
    free(list_name);
    free(s);
    free(choice);
    list_name = s = choice = NULL;
    head = last = NULL;

    return EXIT_SUCCESS;
}


