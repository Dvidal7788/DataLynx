#include <stdio.h>
#include <string.h>

void parseCSVLine(char *line) {
    char *token;
    int inside_quoted_field = 0;
    char field[1024];
    int field_index = 0;

    token = strtok(line, ",");
    while (token != NULL) {
        // Check if we are inside a quoted field
        if (inside_quoted_field) {
            // Append the current token to the current field
            strcat(field, ",");
            strcat(field, token);

            // Check if the current token ends with a double quote
            if (token[strlen(token) - 1] == '"') {
                inside_quoted_field = 0;

                // Remove the trailing double quote from the field
                field[strlen(field) - 1] = '\0';

                // Print the field
                printf("%s\n", field);
                field[0] = '\0';
                field_index = 0;
            }
        } else {
            // Check if the current token starts with a double quote
            if (token[0] == '"') {
                inside_quoted_field = 1;

                // Copy the token to the field, excluding the leading double quote
                strcpy(field, token + 1);
                field_index = strlen(token) - 1;
            } else {
                // Print the token as it is (not inside a quoted field)
                printf("%s\n", token);
            }
        }

        token = strtok(NULL, ",");
    }
}

int main() {
    // FILE *file = fopen("csv/staff.csv", "r"); // Replace "example.csv" with the path to your CSV file

    // if (file == NULL) {
    //     printf("Error opening the file.\n");
    //     return 1;
    // }

    // char line[1024];

    // while (fgets(line, sizeof(line), file)) {
    //     // Remove the trailing newline character, if present
    //     if (line[strlen(line) - 1] == '\n') {
    //         line[strlen(line) - 1] = '\0';
    //     }

    //     parseCSVLine(line);
    // }

    // fclose(file);

    // Bitwise
    int x = 129;

    if (x | 1) printf("evn\n");
    else printf("oddd\n");

    return 0;
}
