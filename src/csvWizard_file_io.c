#include <csvWizard_file_io.h>
#include <csvWizard_stats.h>
#include <csvWizard_util.h>


//      OPEN FILE()
bool openFile(csvWizard *self, char *filename) {

    /* This function will use the input parameter filename if one is given. Otherwise it will use self->filename. If both filename and self->filname are NULL, it will return false */

    if (self == NULL) return false;


    const char *func_name = "csv.openFile";


    if (filename == NULL && self->filename == NULL) return NULL;
    else if (filename != NULL) {

        /* Use input filename parameter */

        // Check if csv
        if (!is_ext(filename, ".csv") && !is_ext(filename, ".txt")) return false;

        freeAll(self);
        changeMissingValue(self, MISSING_VALUE);

        // Allocate buffer to store filename
        self->filename = malloc(sizeof(char) * (strlen(filename)+1));
        if (self->filename == NULL) {if_error(MALLOC_FAILED, func_name); free_null(&self->filename); return false;}

        strcpy(self->filename, filename);

    }

    // Attempt to Open File (Read mode)
    self->file_ptr = fopen(self->filename, "r");
    if (self->file_ptr == NULL) {if_error(FOPEN_FAILED, func_name); free_null(&self->filename); return false;}

    return true;

}



// ---------- CSV.HEADERREADER() ----------
char **headerReader(csvWizard *self) {

    if (self == NULL) return  NULL;

    if (self->header != NULL) return self->header;

    if (self->file_ptr == NULL) return NULL;

    // Function name (for use in if_error())
    const char *func_name = "csv.headerReader";

    /* THIS FUNCTION: 1. Reads csv header into dynamically allocated array of string pointers (i.e. 2D array)
                      2. Returns array of strings as well as psuedo "returns" column_count (via accepting a pointer to column_count as input) for ability to print/free etc array without overrunning buffer
                      3. The main purpose of this function is for use in calling function dictReader(), however, of course could be used anywhere.
                      4. Takes pointer to column_count only for ablility to psuedo return column_count. For this reason, programmer does NOT need to have correct column count prior to inputting parameter */

    // Set column count to zero incase programmer has not already done so in calling function
    self->columnCount = 0;

    // Reset file stream
    fseek(self->file_ptr, 0, SEEK_SET);

    // Allocate first pointer in array of strings
    char **header = (char**)malloc(sizeof(char*));
    if (header == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Create temp string (to be used for each column name)
    char *s = NULL;

    // Iterate through header
    bool end_of_header = false;
    while (true) {

        // Allocate first char of string for current column in header
        s = (char*)malloc(sizeof(char));
        if (s == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

        // Iterate until end of current index
        uintmax_t i = 0;
        bool inside_quotes = false;
        while (true) {

            // Scan char
            if (fscanf(self->file_ptr, "%c", &s[i]) != 1) {
                end_of_header = true;
                break;
            }

            if (!inside_quotes && s[i] == '"') inside_quotes = true;
            else if (inside_quotes && s[i] == '"') inside_quotes = false;

            // Check if at next column or end of header
            if ((!inside_quotes && s[i] == ',') || s[i] == '\n') {
                self->columnCount++;
                if (s[i] == '\n') {end_of_header = true;}
                break;
            }
            else {
                s = realloc(s, sizeof(char)*(i+2));
                if (s == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
                i++;
            }
        }

        // Change last char (i.e. ',' or '\n') to nul and add string to array of string pointers
        s[i] = '\0';
        header[(self->columnCount)-1] = s;
        // if (strlen(s) > 0) {
        //     header[(self->columnCount)-1] = s;
        // }
        // else {free_null(&s); free(header); return NULL;}

        // Realloc array
        if (!end_of_header) {
            header = realloc(header, sizeof(char*)*((self->columnCount)+1));
            if (header == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
        }
        else break;
    }

    self->header_size = ftell(self->file_ptr);

    self->header = header;
    return header;
}


//      ___ READ FILE ___ (V1 - ONE LONG STRING)
char *fileReader(csvWizard *self) {

    /* THIS FUNCTION: Reads a text based file (i.e. .csv, .txt etc) into one long string */

    // Check
    if (self == NULL) return NULL;

    if (self->file_ptr == NULL) return NULL;

    if (self->raw != NULL) free(self->raw);

    // Get file size
    fseek(self->file_ptr, 0L, SEEK_END);
    self->file_size = ftell(self->file_ptr);

    // Check
    if (self->file_size == 0) return NULL;

    // Function name (for use in if_error())
    const char *func_name = "fileReader";


    // Get header (also ensures we have column count)
    /* I could decide to only call headerReader() if self->header == NULL, but then I would need to still reset the file stream and count char by char through the file until \n anyway, so I might as well just get the header either way, which takes same time */
    headerReader(self);

    // --- Disk read: entire file ---

    // Allocate buffer
    uintmax_t buffer_size;
    if (self->skip_header) {
        buffer_size = self->file_size - self->header_size;

        // Get file stream to end of header
        fseek(self->file_ptr, self->header_size, SEEK_SET);
    }
    else {
        buffer_size = self->file_size;
        fseek(self->file_ptr, 0, SEEK_SET);
    }

    char *buffer = (char*)malloc(sizeof(char)*(buffer_size+1));
    if (buffer == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Read
    /* If this fails, try to read smaller size */
    if (fread(buffer, buffer_size, 1, self->file_ptr) != 1) {if_error(FREAD_FAILED, func_name); return NULL;}

    buffer[buffer_size] = '\0';

    // Assign buffer filled with raw data (i.e. one long string) to object
    self->raw = buffer;
    if (self->find_rows_at_file_read) find_row_count(self);

    return buffer;
}


//      ___ fileRowReader ___ (V2 - 2D JAGGED ARRAY)
char **fileRowReader(csvWizard *self) {

    /* THIS FUNCTION: READS FILE INTO DYNAMICALLY ALLOCATED JAGGED 2D ARRAY (Each row in file corresponds to string in array of strings) */

    if (self == NULL) return NULL;

    if (self->rows != NULL) free_2d_array(self);

    // Read whole file into 1 long string

    if (fileReader(self) == NULL) return NULL;

    // Convert to 2d array (array of strings, 1 row per string)
    string_into_2d_array(self);

    return self->rows;
}




//       _________ cs.reader_v3() __________
char ***reader_v3(csvWizard *self) {

    if (self == NULL) return NULL;

    // Read file from disk
    // self->find_rows_at_file_read = false; /* Do not find rows at file read, because we will count rows as we convert from one long string (raw) into dict-style linked list */
    if (fileReader(self) == NULL) return NULL;

    // Function name (for use in if_error())
    const char *func_name = "csv.reader_v3";

    /* THIS FUNCTION: - Reads file contents and parses into an array of doubly linked DICT nodes.
                      - Each row in file corresponds to each list in array
                      - Each field in file corresponds to each node in list.
                      - Each node contains not only the contents of the corresponding field, but also the name of the corresponding column.
                      - This makes searching for desired index easier, as you will need the correct row number and column name as opposed to row number/column number.*/

    // First free previous data structure
    if (self->grid_v3 != NULL) free_grid_v3(self);

    // Allocate: MAIN ARRAY POINTER (array of pointers to dict-style linked lists)
    self->grid_v3 = (char***)malloc(sizeof(char**));
    if (self->grid_v3 == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Variables
    uintmax_t row_count = 1;
    bool end_of_data = false;
    uintmax_t char_count = 0;
    create_stats(self);

    // ITERATE THROUGH REST OF CSV
    while (!end_of_data) {

        // self->grid_v3[row_count-1] = NULL;
        if (row_count-1 < self->rowCount) {
            self->grid_v3[row_count-1] = (char**)malloc(sizeof(char*));
            if (self->grid_v3[row_count-1] == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}
        }


        // Variables for each row
        bool end_of_row = false;
        uintmax_t current_column = 0;

        // Iterate until end of row
        while (true) {
            bool add_null = true;
            if (row_count > self->rowCount) {end_of_data = true; break;}

            // Allocate 1st char of string (this string will be taken over by linked list node, so do not free)
            char *s = (char*)malloc(sizeof(char));
            if (s == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

            uintmax_t i = 0;
            bool inside_quotes = false;

            // For Each Column, Iterate until next column found
            while (true) {

                // Scan char
                if (self->raw[char_count] == '\0') {
                    end_of_data = end_of_row = true;
                    break;
                }

                s[i] = self->raw[char_count];
                char_count++;

                // Check for quotes
                if (!inside_quotes && s[i] == '"') {
                    inside_quotes = true;
                }
                else if (inside_quotes && s[i] == '"') {
                    inside_quotes = false;
                }

                // Check if found next column or end of row
                if (!inside_quotes && s[i] == ',') {
                    break;
                }
                else if (s[i] == '\n') {
                    if (i == 0 && current_column == 0) add_null = false;
                    end_of_row = true;
                    break;
                }
                else {
                    // Reallocate next char in string
                    s = realloc(s, sizeof(char)*(i+2));
                    if (s == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
                    i++;
                }
            }

            if (add_null) {
                s[i] = '\0'; /* Do not free this string. It will be taken over by node in linked list and freed when linked list is freed. */

                // Longest field length for this column
                if (i > self->aggregate[current_column].longest_field_strlen) self->aggregate[current_column].longest_field_strlen = i;
            }

            // Skip breaks in the csv
            if (s[0] == '\n' && current_column == 0) {free(self->grid_v3[--row_count]); free_null(&s); break;}

            if (strcmp(s, self->header[current_column]) == 0) {
                free_null(&s);
                if (current_column == self->columnCount-1) {row_count--; self->rowCount--; end_of_row = true; break;}
                current_column++;
                continue;
            }
            // Append to linked list
            else if (strlen(s) >= 1){

                self->aggregate[current_column].not_null++;

                // Edit stats for this column
                if (is_number(s)) {

                    double field_as_float = atof(s);
                    self->aggregate[current_column].sum += field_as_float;

                    // Set min and max to field from 1st row
                    if (row_count == 1) self->aggregate[current_column].min = self->aggregate[current_column].max = field_as_float;

                    if (field_as_float < self->aggregate[current_column].min) self->aggregate[current_column].min = field_as_float;
                    if (field_as_float > self->aggregate[current_column].max) self->aggregate[current_column].max = field_as_float;

                    // Record that this column is a number field if has not already been recorded
                    if (!self->aggregate[current_column].is_number) self->aggregate[current_column].is_number = true;


                }
                else increment_decrement_value_count(self, self->header[current_column], s, true);


            }
            else {
                self->aggregate[current_column].is_null++;
                if (self->raw[char_count] == '\n' && char_count == strlen(self->raw)-1) {
                    row_count--;
                    free_null(&s);
                }
            }

            // Append node to linked list
            // if (s[0] == '\n' && current_column == 0) {row_count--; free_null(&s); break;}
            // build_dblink_list(&s, &self->grid_v3[(row_count)-1], &last)
            self->grid_v3[row_count-1][current_column] = s;

            if (end_of_row) break;
            else {
                self->grid_v3[row_count-1] = realloc(self->grid_v3[row_count-1], sizeof(char*)*(current_column+2));
                if (self->grid_v3[row_count-1] == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
            }
            current_column++;
        }

        // Reallocate array of strings for next string
        if (!end_of_data) {
            // Realloc
            self->grid_v3 = realloc(self->grid_v3, sizeof(char**)*(++row_count));
            if (self->grid_v3 == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
        }
        else break;
    }

    // Free raw data
    free_null(&self->raw);

    // Calculate means, now that we have sums. Calc stds once we have means. Find medians now that we have all the data.
    for (uintmax_t i = 0; i < self->columnCount; i++) self->aggregate[i].mean = self->aggregate[i].sum / self->aggregate[i].not_null;
    calc_std(self);
    find_median(self);


    return self->grid_v3;
}



//      _________ csv.reader() __________
node **reader(csvWizard *self) {

    if (self == NULL) return NULL;

    // Read file from disk
    // self->find_rows_at_file_read = false; /* Do not find rows at file read, because we will count rows as we convert from one long string (raw) into dict-style linked list */
    if (fileReader(self) == NULL) return NULL;

    // Function name (for use in if_error())
    const char *func_name = "csv.reader";

    /* THIS FUNCTION: - Reads file contents and parses into an array of doubly linked DICT nodes.
                      - Each row in file corresponds to each list in array
                      - Each field in file corresponds to each node in list.
                      - Each node contains not only the contents of the corresponding field, but also the name of the corresponding column.
                      - This makes searching for desired index easier, as you will need the correct row number and column name as opposed to row number/column number.*/

    // First free previous data structure
    if (self->grid != NULL) free_grid(self);

    // Allocate: MAIN ARRAY POINTER (array of pointers to dict-style linked lists)
    self->grid = (node**)malloc(sizeof(node*));
    if (self->grid == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Variables
    uintmax_t row_count = 1;
    bool end_of_data = false;
    uintmax_t char_count = 0;
    create_stats(self);

    // ITERATE THROUGH REST OF CSV
    while (!end_of_data) {

        self->grid[row_count-1] = NULL;

        // Variables for each row
        bool end_of_row = false;
        node *last = NULL;
        uintmax_t current_column = 0;

        // Iterate until end of row
        while (true) {
            bool add_null = true;
            if (row_count > self->rowCount) {end_of_data = true; break;}

            // Allocate 1st char of string (this string will be taken over by linked list node, so do not free)
            char *s = (char*)malloc(sizeof(char));
            if (s == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

            uintmax_t i = 0;
            bool inside_quotes = false;

            // For Each Column, Iterate until next column found
            while (true) {

                // Scan char
                if (self->raw[char_count] == '\0') {
                    end_of_data = end_of_row = true;
                    break;
                }

                s[i] = self->raw[char_count];
                char_count++;

                // Check for quotes
                if (!inside_quotes && s[i] == '"') {
                    inside_quotes = true;
                }
                else if (inside_quotes && s[i] == '"') {
                    inside_quotes = false;
                }

                // Check if found next column or end of row
                if (!inside_quotes && s[i] == ',') {
                    break;
                }
                else if (s[i] == '\n') {
                    if (i == 0 && current_column == 0) add_null = false;
                    end_of_row = true;
                    break;
                }
                else {
                    // Reallocate next char in string
                    s = realloc(s, sizeof(char)*(i+2));
                    if (s == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
                    i++;
                }
            }

            if (add_null) {
                s[i] = '\0'; /* Do not free this string. It will be taken over by node in linked list and freed when linked list is freed. */

                // Longest field length for this column
                if (i > self->aggregate[current_column].longest_field_strlen) self->aggregate[current_column].longest_field_strlen = i;
            }

            // Skip breaks in the csv
            if (s[0] == '\n' && current_column == 0) {row_count--; free_null(&s); break;}

            if (strcmp(s, self->header[current_column]) == 0) {
                free_null(&s);
                if (current_column == self->columnCount-1) {row_count--; self->rowCount--; end_of_row = true; break;}
                current_column++;
                continue;
            }
            // Append to linked list
            else if (strlen(s) >= 1){

                self->aggregate[current_column].not_null++;

                // Edit stats for this column
                if (is_number(s)) {

                    double field_as_float = atof(s);
                    self->aggregate[current_column].sum += field_as_float;

                    // Set min and max to field from 1st row
                    if (row_count == 1) self->aggregate[current_column].min = self->aggregate[current_column].max = field_as_float;

                    if (field_as_float < self->aggregate[current_column].min) self->aggregate[current_column].min = field_as_float;
                    if (field_as_float > self->aggregate[current_column].max) self->aggregate[current_column].max = field_as_float;

                    // Record that this column is a number field if has not already been recorded
                    if (!self->aggregate[current_column].is_number) self->aggregate[current_column].is_number = true;

                }
                else increment_decrement_value_count(self, self->header[current_column], s, true);


            }
            else {
                self->aggregate[current_column].is_null++;
                if (self->raw[char_count] == '\n' && char_count == strlen(self->raw)-1) {
                    row_count--;
                    free_null(&s);
                }
            }

            // Append node to linked list
            // if (s[0] == '\n' && current_column == 0) {printf("row COUNT: %ld\n", row_count);row_count--; free_null(&s); break;}
            build_dblink_list(&s, &self->grid[(row_count)-1], &last);

            if (end_of_row) break;
            current_column++;
        }

        // Reallocate array of strings for next string
        if (!end_of_data) {
            // Realloc
            self->grid = realloc(self->grid, sizeof(node*)*(++(row_count)));
            if (self->grid == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
        }
        else break;
    }


    // Free raw data
    free_null(&self->raw);

    // Calculate means, now that we have sums. Calc stds once we have means. Find medians now that we have all the data.
    for (uintmax_t i = 0; i < self->columnCount; i++) self->aggregate[i].mean = self->aggregate[i].sum / self->aggregate[i].not_null;
    calc_std(self);
    find_median(self);

    return self->grid;
}

//      _____ CSV DICT READER _____
dict_node **dictReader(csvWizard *self) {

    if (self == NULL) return NULL;

    // Read file from disk
    // self->find_rows_at_file_read = false;
    if (fileReader(self) == NULL) return NULL;


    // Function name (for use in if_error())
    const char *func_name = "csv.dictReader";

    /* THIS FUNCTION: - Reads file contents and parses into an array of doubly linked DICT nodes.
                      - Each row in file corresponds to each list in array
                      - Each field in file corresponds to each node in list.
                      - Each node contains not only the contents of the corresponding field, but also the name of the corresponding column.
                      - This makes searching for desired index easier, as you will need the correct row number and column name as opposed to row number/column number.*/

    // First free previous data structure if there was one
    if (self->dict_grid != NULL) free_dict_grid(self);

    // Allocate: MAIN ARRAY POINTER (array of pointers to dict-style linked lists)
    self->dict_grid = (dict_node**)malloc(sizeof(dict_node*));
    if (self->dict_grid == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Variables
    uintmax_t row_count = 1;
    bool end_of_data = false;
    uintmax_t char_count = 0;
    create_stats(self);

    // ITERATE THROUGH REST OF CSV
    while (!end_of_data) {

        self->dict_grid[row_count-1] = NULL;

        // Variables for each row
        bool end_of_row = false;
        dict_node *last = NULL;
        uintmax_t current_column = 0;


        // Iterate until end of row
        while (true) {
            bool add_null = true;
            if (row_count > self->rowCount) {end_of_data = true; break;}

            // Allocate 1st char of string (this string will be taken over by linked list node, so do not free)
            char *s = (char*)malloc(sizeof(char));
            if (s == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

            uintmax_t i = 0;
            bool inside_quotes = false;

            // For Each Column, Iterate until next column found
            while (true) {

                // Scan char
                if (self->raw[char_count] == '\0') {
                    end_of_data = end_of_row = true;
                    break;
                }

                s[i] = self->raw[char_count];
                char_count++;

                // Check for quotes
                if (!inside_quotes && s[i] == '"') {
                    inside_quotes = true;
                }
                else if (inside_quotes && s[i] == '"') {
                    inside_quotes = false;
                }

                // Check if found next column or end of row
                if (!inside_quotes && s[i] == ',') {
                    break;
                }
                else if (s[i] == '\n') {
                    if (i == 0 && current_column == 0) add_null = false;
                    end_of_row = true;
                    break;
                }
                else {
                    // Reallocate next char in string
                    s = realloc(s, sizeof(char)*(i+2));
                    if (s == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
                    i++;
                }
            }

            if (add_null) {
                s[i] = '\0'; /* Do not free this string. It will be taken over by node in linked list and freed when linked list is freed. */

                // Longest field length for this column
                if (i > self->aggregate[current_column].longest_field_strlen) self->aggregate[current_column].longest_field_strlen = i;
            }

            // Skip breaks in the csv
            if (s[0] == '\n' && current_column == 0) {;row_count--; free_null(&s); break;}

            if (strcmp(s, self->header[current_column]) == 0) {
                free_null(&s);
                if (current_column == self->columnCount-1) {row_count--; self->rowCount--; end_of_row = true; break;}
                current_column++;
                continue;
            }
            // Append to linked list
            else if (strlen(s) >= 1) {

                self->aggregate[current_column].not_null++;

                // Edit stats for this column
                if (is_number(s)) {

                    double field_as_float = atof(s);
                    self->aggregate[current_column].sum += field_as_float;

                    // Set min and max to field from 1st row
                    if (row_count == 1) self->aggregate[current_column].min = self->aggregate[current_column].max = field_as_float;

                    if (field_as_float < self->aggregate[current_column].min) self->aggregate[current_column].min = field_as_float;
                    if (field_as_float > self->aggregate[current_column].max) self->aggregate[current_column].max = field_as_float;

                    // Record that this column is a number field if has not already been recorded
                    if (!self->aggregate[current_column].is_number) self->aggregate[current_column].is_number = true;

                }
                else increment_decrement_value_count(self, self->header[current_column], s, true);


            }
            else {
                self->aggregate[current_column].is_null++;
                if (self->raw[char_count] == '\n' && char_count == strlen(self->raw)-1) {
                    row_count--;
                    free_null(&s);
                }
            }

            // Append node to linked list
            // if (s[0] == '\n' && current_column == 0) {row_count--; free_null(&s); break;}
            build_dict_link_list(&s, &self->dict_grid[(row_count)-1], &last, &self->header[current_column]);

            if (end_of_row) break;
            current_column++;
        }

        // Reallocate array of strings for next string
        if (!end_of_data) {
            // Realloc
            self->dict_grid = realloc(self->dict_grid, sizeof(dict_node*)*(++row_count));
            if (self->dict_grid == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
        }
        else break;
    }

    // Free raw data
    free_null(&self->raw);

    // Calculate means, now that we have sums. Calc stds once we have means. Find medians now that we have all the data.
    for (uintmax_t i = 0; i < self->columnCount; i++) self->aggregate[i].mean = self->aggregate[i].sum / self->aggregate[i].not_null;
    calc_std(self);
    find_median(self);

    return self->dict_grid;
}

        //  ---------- FIELD READERS ----------

//      ___ CSV.FIELDREADER() ___
char *fieldReader(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column) {

    // Function name (for use in if_error()) (formerly csv_reader_index)
    const char *func_name = "csv.fieldReader";

    /* THIS FUNCTION: Allows you to index directly into a csv file, without reading the whole file into memory.
         - You can choose whether or not to skip the header (i.e. if header is not skipped, row 0 will be the header)
         - Returns NULL if nothing found at given index. */

    // Reset file stream
    fseek(self->file_ptr, 0, SEEK_SET);

    // Skip header or not
    if (self->skip_header) desired_row++;

    // Declare variables
    int8_t tmp;
    bool reached_eof = false;

    // Get stream to correct row
    for (uintmax_t i = 0; i < desired_row; i++)
    {
        while ((tmp = fgetc(self->file_ptr)) != '\n') if (tmp == EOF) {reached_eof = true; break;}

        if (reached_eof) break;
    }

    // Get stream to correct column
    bool end_of_row = false, inside_quotes = false;

    for (uintmax_t j = 0; j < desired_column; j++)
    {
        while ((tmp = fgetc(self->file_ptr)) != '\n') {

            if (tmp == '"' && inside_quotes == false) inside_quotes = true;
            else if (tmp == '"') inside_quotes = false;

            // Break conditions
            if (tmp == ',' && inside_quotes == false) break;

            if (tmp == EOF) {reached_eof = true; break;}
        }

        if (reached_eof) break;

        if (tmp == '\n') {end_of_row = true; break;}
    }

    // Create string from current index location
    char *s = NULL;
    if (!reached_eof && !end_of_row) {

        // Alocate 1st char of string
        s = (char*)malloc(sizeof(char));
        if (s == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

        uintmax_t i = 0;

        // In a field without quotes
        while ((s[i] = fgetc(self->file_ptr)) != '\n' && s[i] != EOF) {

            if (!inside_quotes && s[i] == '"') inside_quotes = true;
            else if (s[i] == '"') inside_quotes = false;

            if (s[i] == ',' && inside_quotes == false) break;

            // Realloc
            s = realloc(s, sizeof(char)*(i+2));
            if (s == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
            i++;
        }

        s[i] = '\0';
    }

    /* s will be NULL if went through whole file and did not reach row/column */

    if (s == NULL) return s;
    else if (s[0] == '\0') {free_null(&s); return self->missingValue;}

    return append_last_retrieved_fields(self, &s);
}



//      ___ csv.fieldReader2() ___
char *fieldReader2(csvWizard *self, uintmax_t desired_row, char *desired_column) {

    // Function name (for use in if_error()) (formerly csv_dictreader_index())
    // const char *func_name = "csv.fieldReader2";

    /* THIS FUNCTION: 1. Allows you to index directly into a csv file using the column name and without reading the whole file into memory.
                      2. Returns NULL if desired_column is not in header of file
                      3. This function will treat the 1st non-header row as row 0 (i.e. row 2 in the file = row 0, as it is the first non-header row and will be 0 indexed.)
                      How it works: 1. Finds correct column number that corresponds with column name.
                                    2. Then calls csv.fieldReader() to find field at that index location */


    // Reset file stream
    fseek(self->file_ptr, 0, SEEK_SET);

    if (self->header == NULL) self->csv.headerReader(self);

    // If we found correct column, we can use fieldReader() to index directly into csv without reading into memory
    intmax_t column_index = findColumnIndex(self, desired_column);

    if (column_index != -1)
        return fieldReader(self, desired_row, column_index);
    else return NULL;
}


//      ___ UPDATE_CSV_INDEX() ___
bool update_csv_index(csvWizard *self, uintmax_t row, uintmax_t column, char *new_field) {

    if (self == NULL) return false;
    if (!self->csv_write_permission) return false;

    // Function name (for use in if_error())
    const char *func_name = "update_csv_index";


    // Open file
    /* I will not use self->file_ptr to open and close this file,
    because if the library user has already used that ptr to open the file b4 using this function, then that file ptr will be lost.
       - This only matters if I open file, so library user does not have to */
    // FILE *file = fopen(self->filename, "r");
    // if (file == NULL) {if_error(FOPEN_FAILED, func_name); return false;}

    // Check if file is empty
    fseek(self->file_ptr, 0L, SEEK_END);
    self->file_size = ftell(self->file_ptr);
    if (self->file_size == 0) {
        fclose_null(&self->file_ptr);
        return false;
    }
    fseek(self->file_ptr, 0, SEEK_SET);

    /* THIS FUNCTION: Updates csv file based on choice of index
            Note: This function takes filename string instead of file pointer as input, to be able to open in read mode, then close and open in write mode.
                    This would not be possible with a single file pointer. */

    // Make new_field into a dynamically allocated string (so I can pass the address of string through add_quotes() and resize if necessary)
    char *new_field_dyn = (char*)malloc(sizeof(char) * strlen(new_field)+1);
    if (new_field_dyn == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    strcpy(new_field_dyn, new_field);

    // Allocate initial char for buffer to read file contents into
    char *s = (char*)malloc(sizeof(char));
    if (s == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    // Iterate through HEADER char by char
    uint64_t i = 0;
    while (fscanf(self->file_ptr, "%c", &s[i]) == 1) {

        if (s[i] == '\n') break;

        // Reallocate for next char
        s = realloc(s, sizeof(char)*(i+2));
        if (s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
        i++;
    }

    // Reallocate for next char for priming of next loop
    s = realloc(s, sizeof(char)*(i+2));
    if (s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
    i++;

    // Variables to keep track of current row/column to match with input row/column
    uintmax_t current_row = 0, current_column = 0;
    bool found_correct_index = false, inside_quotes = false;

    // Iterate through REST OF FILE char by char
    while (fscanf(self->file_ptr, "%c", &s[i]) == 1) {

        // FOUND Correct index to update
        if (current_row == row && current_column == column && !found_correct_index) {

            found_correct_index = true;

            // Determine current field is inside quotes
            if (!inside_quotes && s[i] == '"') inside_quotes = true;

            // Add quotes to new cell if necessary
            if (!has_quotes(new_field) && has_comma(new_field)) add_quotes(&new_field_dyn);

            // Copy new_field into string
            s = realloc(s, sizeof(char)*(strlen(new_field_dyn)+i+1));
            if (s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

            uintmax_t len =  strlen(new_field_dyn);
            for (uintmax_t index = 0; index < len; index++) {
                s[i] = new_field_dyn[index];
                i++;
            }

            // Get file stream past old cell that we are replacing in file
            char tmp;
            while (fscanf(self->file_ptr, "%c", &tmp) == 1) {

                if (inside_quotes && tmp == '"') inside_quotes = false;
                else if ((!inside_quotes && tmp == ',' ) || tmp == '\n') break;
            }
            s[i] = tmp;
        }

        // Increment current row/column if appropriate
        if (!found_correct_index) {

            if (!inside_quotes && s[i] == '"') inside_quotes = true;
            else if (inside_quotes && s[i] == '"') inside_quotes = false;
            else if (s[i] == ',' && !inside_quotes) {
                current_column++;
            }
            else if (s[i] == '\n') {current_row++; current_column = 0;}
        }

        // Reallocate for next char
        s = realloc(s, sizeof(char)*(i+2));
        if (s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
        i++;

    }
    s[i] = '\0';

    // Close file in read mode
    fclose(self->file_ptr);

    // Reopen file in write mode
    self->file_ptr = fopen(self->filename, "w");
    if (self->file_ptr == NULL) {if_error(FOPEN_FAILED, func_name); return false;}

    // Print new data to file
    fprintf(self->file_ptr, "%s", s);

    // Free string/close file
    free_null(&s);
    fclose_null(&self->file_ptr);
    free_null(&new_field_dyn);

    return true;
}


//          ____ UPDATE_CSV_INDEX2()___
bool fieldWriter(csvWizard *self, uintmax_t row, char *column, char *new_field) {

    if (self == NULL) return false;
    if (!self->csv_write_permission) return false;

    intmax_t column_index = findColumnIndex(self, column);

    return (column_index >= 0) ? update_csv_index(self, row, column_index, new_field) : false;
}



//      _____ csv.backup() _____
bool backup(csvWizard *self) {

    if (self == NULL) return false;
    if (self->filename == NULL) return false;

    // Function name
    const char *func_name = "csv.backup";

    // Open file
    FILE *file = fopen(self->filename, "r");
    if (file == NULL) {if_error(FOPEN_FAILED, func_name); return false;}

    // Get file size
    fseek(file, 0L, SEEK_END);
    uintmax_t file_size = ftell(file);

    // Setup buffer
    char buffer[file_size+1];

    // Read file into buffer, then close file
    fseek(file, 0, SEEK_SET);
    fread(buffer, file_size, 1, file);
    fclose(file);


    //                   WRITE TO NEW BACKUP FILE


    //      -- Find where .ext starts ---

    // Setup variables and buffer to save file extension
    uint16_t filename_length = strlen(self->filename);
    uint8_t MAX_EXT_LENGTH = 10;
    char extension[MAX_EXT_LENGTH+1];

    uint16_t dot_location = 0;
    bool found_dot = false;
    uint8_t extension_length = 0;

    // Iterate through filename
    for (uint16_t i = 0; i < filename_length; i++) {

        // Find the dot (this only works if there is one dot in the filename)
        if (self->filename[i] == '.') {
            dot_location = i;
            found_dot = true;
        }

        // Keep track of how long extension is (including '.')
        if (found_dot) extension_length++;
    }

    // Save file extension in buffer
    if (extension_length <= MAX_EXT_LENGTH) {
        strcpy(extension, &self->filename[dot_location]);
    }
    else return false;

    // Create timestamp
    time_t t;
    time(&t);
    char *timestamp = ctime(&t);

    // Cut off '\n' and spaces that came with timestamp
    uint8_t timestamp_length = strlen(timestamp);
    for (uint16_t i = 0; i < timestamp_length; i++) {
        if (timestamp[i] == ' ')
            timestamp[i] = '_';
        else if (timestamp[i] == '\n')
            timestamp[i] = '\0';
    }

    // Create new backup filename
    uint16_t backup_filename_length = strlen(self->filename) + (strlen(timestamp)-1) + strlen("_backup_") + 1;
    char backup_filename[backup_filename_length];

    // Cut extension from filename (otherwise when we append timestamp, file extension will be in the middle of the backup filename)
    self->filename[dot_location] = '\0';

    // Copy
    sprintf(backup_filename, "%s_backup_%s%s", self->filename, timestamp, extension);

    // Fix filename (now that we don't need the truncated version anymore)
    self->filename[dot_location] = '.';

    backup_filename[backup_filename_length] = '\0';

    // Create new file
    FILE *backup_file = fopen(backup_filename, "w");
    if (backup_file == NULL) {if_error(FOPEN_FAILED, func_name); return false;}

    // Write to file
    fwrite(buffer, file_size, 1, backup_file);

    fclose(backup_file);

    return true;
}


//          --- CSV.WRITEDATA()  ---
bool writeData(csvWizard *self, char *new_filename) {

    if (self == NULL) return false;

    if (self->filename == NULL) return false;

    if (strcasecmp(self->filename, new_filename) == 0 && !self->csv_write_permission) return false;

    // Make this create default new filename
    if (strlen(new_filename) == 0) return false;

    const char *func_name = "csv.writeData";

    // Create (open) new file
    FILE *new_file = fopen(new_filename, "w");
    if (new_file == NULL) {if_error(FOPEN_FAILED, func_name); return false;}

    // Write Header to file
    if (self->skip_header && self->header != NULL) {

        for (uintmax_t i = 0; i < self->columnCount; i++) {

            // Print with comma if not the last column
            if (i < self->columnCount-1) {
                fprintf(new_file, "%s,", self->header[i]);
                if (self->with_spaces) printf(" ");
            }
            else fprintf(new_file, "%s\n", self->header[i]);
        }

    }

    // Data
    if (self->raw != NULL) {

        // Write raw data (one long string) to file
        fprintf(new_file, "%s", self->raw);

        fclose(new_file);
        return true;
    }

    // Iterate through every row
    uintmax_t i;
    if (!self->skip_header) i = 1;
    else i = 0;


    if (self->rows != NULL) {

        // Write array of strings (rows) to file
        while (i < self->rowCount) {
            fprintf(new_file, "%s\n", self->rows[i]);
            i++;
        }

        fclose(new_file);
        return true;
    }

    if (self->grid_v3 != NULL) {

        while (i < self->rowCount) {

            for (uint32_t column = 0; column < self->columnCount; column++) {

                // Write grid to file
                if (column != self->columnCount-1)
                    fprintf(new_file, "%s,", self->grid_v3[i][column]);
                else fprintf(new_file, "%s\n", self->grid_v3[i][column]);
            }
            i++;
        }


        fclose(new_file);
        return true;
    }

    if (self->grid != NULL) {

        // Write array of db link list (grid) to file
        while (i < self->rowCount) {

            node *tmp = self->grid[i];
            while (tmp != NULL) {

                // Write grid to file
                if (tmp->next != NULL)
                    fprintf(new_file, "%s,", tmp->s);
                else fprintf(new_file, "%s\n", tmp->s);

                tmp = tmp->next;
            }
            i++;
        }


        fclose(new_file);
        return true;

    }

    if (self->dict_grid != NULL) {

        // Write array of db link list (dict grid) to file

        while (i < self->rowCount) {

            // Traverse linked list
            dict_node *tmp = self->dict_grid[i];
            while (tmp != NULL) {
                if (tmp->next != NULL)
                    fprintf(new_file, "%s, ", tmp->s);
                else
                    fprintf(new_file, "%s\n", tmp->s);
                tmp = tmp->next;
            }
            i++;
        }


        fclose(new_file);
        return true;

    }

    // End
    fclose(new_file);

    return false;



}


