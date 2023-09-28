#include <DataLynx.h>

DataLynx DataLynxConstructor(void) {

    DataLynx self;

    self.filename = NULL;
    self.file_ptr = NULL;
    self.file_size = 0;
    self.header_size = 0;
    self.skip_header = true;
    self.with_spaces = true;
    self.csv_write_permission = false;
    self.printRowIndex = true;
    self.maxPrintRows = 25;
    self.maxFieldPrintLength = 50;
    self.print_truncated_rows = false;
    self.in_place_sort = true;
    self.row_to_drop = -1;
    self.generic_header = false;
    self.print_filter = false;

    self.destructive_mode = false;
    self.find_rows_at_file_read = true;
    self.missingValue = NULL; /* Must make this NULL first, so changeMissingValue() behaves correctly. Also, if has garbage values, changeMissingValue() will try to free it. */
    changeMissingValue(&self, MISSING_VALUE);
    self.case_sensitive_value_counts = false;
    self.case_sensitive_sort = false;


    self.__header__ = NULL;
    self.aggregate = NULL;
    self.tmp_column = NULL;

    self.raw = NULL;
    self.rows = NULL;
    self.grid_v3 = NULL;
    self.grid = NULL;
    self.dict_grid = NULL;

    self.node_last = NULL;
    self.dict_last = NULL;

    self.rowCount = 0;
    self.columnCount = 0;
    self.number_of_rows_to_print = 0;
    self.max_row_digits = 0;
    self.print_tail = false;

    self.last_retrieved_field = NULL;
    self.last_retrieved_fields = NULL;

    //  --- Assign function pointers ---
    self.header = &header;

    self.userInputFilename = &userInputFilename;
    self.changeFilename = &changeFilename;

    // Read/Write to CSV Functions
    self.csv.openFile = &openFile;
    self.csv.headerReader = &headerReader;
    self.csv.fileReader = &fileReader;
    self.csv.fileRowReader = &fileRowReader;
    self.csv.reader_v3 = &reader_v3;
    self.csv.reader = &reader;
    self.csv.dictReader = &dictReader;

    self.csv.fieldReader = &fieldReader;
    self.csv.fieldReaderIdx = &fieldReaderIdx;

    self.csv.fieldWriter = &fieldWriter;
    self.csv.fieldWriterIdx = &fieldWriterIdx;

    self.csv.rowWriter = &rowWriter;
    self.csv.rowDictWriter = &rowDictWriter;

    self.csv.renameFile = &renameFile;
    self.csv.backup = &backup;
    self.csv.writeData = &writeData;

    // Functions that operate on the data structure in memory

    self.getField = &getField;
    self.getFieldIdx = &getFieldIdx;

    self.updateField = &updateField;
    self.updateFieldIdx = &updateFieldIdx;


    // Convert into other data structures
    self.string_into_2d_array = &string_into_2d_array;
    self.split_2darray_by = &split_2darray_by;

    self.replaceAll = &replaceAll;
    self.replaceInColumn = &replaceInColumn;

    self.dropColumn = &dropColumn;
    self.dropRow = &dropRow;

    self.createHeader = &createHeader;
    self.insertRow = &insertRow;
    self.insertRowDict = &insertRowDict;

    self.sortRowsByColumn = &sortRowsByColumn;

    // Can call min, max etc from .min()/.max() etc or .stats()
    self.valueCount = &valueCount;
    self.isInColumn = &isInColumn;
    self.getStat = &getStat;
    self.min = &min;
    self.max = &max;
    self.sum = &sum;
    self.mean = &mean;
    self.isNull = &isNull;
    self.notNull = &notNull;

    self.freeAll = &freeAll;

    self.stripField = &stripField;
    self.stripFieldL = &stripFieldL;
    self.stripFieldR = &stripFieldR;

    self.stripFieldIdx = &stripFieldIdx;
    self.stripFieldIdxL = &stripFieldIdxL;
    self.stripFieldIdxR = &stripFieldIdxR;

    self.stripColumn = &stripColumn;
    self.stripColumnL = &stripColumnL;
    self.stripColumnR = &stripColumnR;

    self.stripColumnIdx = &stripColumnIdx;
    self.stripColumnIdxL = &stripColumnIdxL;
    self.stripColumnIdxR = &stripColumnIdxR;

    self.stripAll = &stripAll;
    self.stripAllL = &stripAllL;
    self.stripAllR = &stripAllR;

    self.formatHeader = &formatHeader;
    self.changeColumnName = &changeColumnName;
    self.printHeader = &printHeader;
    self.printColumn = &printColumn;
    self.printData = &printData;
    self.printDataTable = &printDataTable;
    self.printStatsAll = &printStatsAll;
    self.printStatsColumn = &printStatsColumn;
    self.printShape = &printShape;
    self.printColumnCond = &printColumnCond;
    self.changeMissingValue = &changeMissingValue;


    return self;
}


char *header(DataLynx *self, uint32_t column) {

    /*  THIS FUNCTION only exists to ensure no buffer over runs when accessing the header.
        The library user should use this function instead of accessing the header directly (self.__header__) */

    if (self == NULL) return NULL;

    if (self->__header__ == NULL) return NULL;

    if (column > self->columnCount-1) return NULL;

    return self->__header__[column];

}


bool createHeader(DataLynx *self, char *header[], uint32_t column_count) {

    /* - This function creates header from list input parameter
       - This function does NOT read from a file. */

    /* NOTE: In order for this function to work, I have to trust that the library user inputs the correct header_size for the array they input as header[] */
    if (self == NULL) return false;

    if (self->__header__ != NULL) free_header(self);

    self->columnCount = column_count;

    const char *func_name = "createHeader";

    // Allocate array of strings
    self->__header__ = (char **)malloc(sizeof(char *) * column_count);
    if (self->__header__ == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    for (uint32_t column = 0; column < column_count; column++) {

        // Allocate for string
        if (header == NULL) {

            // If no column names, create header of generic column names

            // Allocate each string
            self->__header__[column] = (char*)malloc(sizeof(char)*16);
            if (self->__header__[column] == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

            sprintf(self->__header__[column], "Column %d", column+1);


        }
        else if (header[column][0] == '\0') {

            self->__header__[column] = (char *)malloc(sizeof(char)*16);
            if (self->__header__[column] == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

            sprintf(self->__header__[column], "Column %d", column+1);
        }
        else {
            self->__header__[column] = (char *)malloc( sizeof(char) * (strlen(header[column])+1) );
            if (self->__header__[column] == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
            strcpy(self->__header__[column], header[column]);
        }



    }


    if (header == NULL) self->generic_header = true;

    return true;
}



bool insertRow(DataLynx *self, char *values[]) {

    // Safety checks
    if (self == NULL || values == NULL) return false;
    if (self->columnCount == 0) return false;

    const char *func_name = "insertRow";

    // Create Generic Header (i.e 'Column1', 'Column2'...)
    /* Note: Library user must set columnCount. Ensuring a header exists will prevent issues when trying to dereference the header in other functions */
    if (self->__header__ == NULL && self->columnCount > 0) createHeader(self, NULL, self->columnCount);

    dict values2[self->columnCount];

    // Copy values into new array in order to call insertRowDict()
    for (uint32_t column = 0; column < self->columnCount; column++) {

        // Get string length (protect against NULL string)
        size_t length = (values[column] != NULL) ? strlen(values[column]) : 0;

        // Allocate for field string
        values2[column].field = (char*)malloc(sizeof(char) * (length+1));
        if (values2[column].field == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        // Copy string (protect against NULL string)
        if (values[column] != NULL) strcpy(values2[column].field, values[column]);
        else values2[column].field[0] = '\0';

        // Point column name to header
        values2[column].column_name = self->__header__[column];

    }

    // Save function return instead of returng directly because we need to free values2
    bool function_return = insertRowDict(self, values2);

    // Free values2
    for (uint32_t column = 0; column < self->columnCount; column++) free_null(&values2[column].field);

    return function_return;
}

// NULL valuees in insertROw

bool insertRowDict(DataLynx *self, dict values[]) {

    // If inserting rows from scratch (i.e. not into existing data structure), will create grid_v3

    // Library user can input dict values[] out of order. If header exists, tests against header and rearranges array if necessary

    // If header does not exist and columnCount is set, creates header

    // Safety checks
    if (self == NULL || values == NULL) return false;
    if (self->columnCount == 0) return false;

    // Check if columns are in correct order. Rearraange if not.
    rearrange_dict_array(self, values);

    const char *func_name = "insertRowDict";

    // Create header (Library user must set columnCount)
    if (self->__header__ == NULL && self->columnCount > 0) {

        // Allocate array of pointers
        self->__header__ = (char**)malloc(sizeof(char*) * self->columnCount);
        if (self->__header__ == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        for (uint32_t column = 0; column < self->columnCount; column++) {

            // Allocate each string
            self->__header__[column] = (char*)malloc(sizeof(char) * (strlen(values[column].column_name)+1));
            if (self->__header__[column] == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

            strcpy(self->__header__[column], values[column].column_name);

        }

    }


    // Malloc new data structure or realloc current data structure + 1 row
    if (self->rowCount == 0) {

        // Inserting from scratch (i.e. no existing data structure)
        self->grid_v3 = (char***)malloc(sizeof(char**));
        if (self->grid_v3 == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
        self->rowCount++;
        create_stats(self);
    }
    // Realloc main array for 1 more row pointer
    else if (self->grid_v3 != NULL) {

        self->grid_v3 = (char***)realloc(self->grid_v3, sizeof(char**) * (++self->rowCount));
        if (self->grid_v3 == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
    }
    else if (self->grid != NULL) {

        self->grid = (node**)realloc(self->grid, sizeof(node*) * (++(self->rowCount)));
        if (self->grid == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
    }
    else if (self->dict_grid != NULL) {

        self->dict_grid = (dict_node**)realloc(self->dict_grid, sizeof(dict_node*) * (++(self->rowCount)));
        if (self->dict_grid == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
    }


    // Allocate / setup new row
    if (self->grid_v3 != NULL) {
        self->grid_v3[self->rowCount-1] = (char**)malloc(sizeof(char*) * self->columnCount);
        if (self->grid_v3 == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    }
    // Make head row pointer NULL, so linked list builder functions handle it prooerly
    else if (self->grid != NULL) {
        self->grid[self->rowCount-1] = self->node_last = NULL;
    }
    else if (self->grid_v3 != NULL) {
        self->dict_grid[self->rowCount-1] = self->dict_last = NULL;
    }


    // Create array of strings for update_stats_new_row
    char *values2[self->columnCount];

    // Iterate through values
    for (uint32_t column = 0; column < self->columnCount; column++) {

        bool add_quotes = false;
        bool field_not_null = (values[column].field != NULL) ? true : false;

        // Buffer size
        size_t buffer_size = (field_not_null) ? strlen(values[column].field) + 1 : 1;

        // Add quotes to field if necessary
        if (field_not_null && has_comma(values[column].field) && !has_quotes(values[column].field)) {
            buffer_size += 2;
            add_quotes = true;
        }

        //  Allocate for string
        char *new_field = (char*)malloc(sizeof(char) * buffer_size);
        if (new_field == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        // Copy string to new buffer
        if (field_not_null) {
            !add_quotes ? strcpy(new_field, values[column].field) : strcpy(&new_field[1], values[column].field);
        }
        else new_field[0] = '\0';


        // Add quotes
        if (add_quotes) new_field[0] = new_field[buffer_size-2] = '"';
        new_field[buffer_size-1] = '\0';

        // Allocate next string for values2 (for input into update_stats_new_row())
        values2[column] = (char*)malloc(sizeof(char) * buffer_size);
        if (values2[column] == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        strcpy(values2[column], new_field);

        // Assign new field to data structure
        if (self->grid_v3 != NULL) self->grid_v3[self->rowCount-1][column] = new_field;
        else if (self->grid != NULL) {
            if (!build_dblink_list(&new_field, &self->grid[self->rowCount-1], &self->node_last)) return false;
        }
        else if (self->dict_grid != NULL) {
            if (!build_dict_link_list(&new_field, &self->dict_grid[self->rowCount-1], &self->dict_last, &self->__header__[column])) return false;
        }

    }

    // Update object data
    self->number_of_rows_to_print = self->rowCount;
    calc_max_row_digits(self);
    update_stats_new_row(self, values2);

    // Simultaneously write this row to the CSV file if permissions allows
    if (self->csv_write_permission && self->destructive_mode) rowWriter(self, values2);

    // Free
    for (uint32_t column = 0; column < self->columnCount; column++) free_null(&values2[column]);

    return true;
}





// bool createRow(DataLynx *self, uintmax_t row, const char *values[]) {

//     /* - This function will NOT update/change data if field already exists. This is ONLY for creating/inserting fields
//        - Row must be rowCount-1 or rowCount, in which case we start new row, only if column is 0th column. Otherwise return false
//        - Creates hedaer dynamically as reads in row 0 */

//     if (self == NULL || values == NULL) return false;

//     // I either need to require header_size as input for this function or I have to require that the caller has already created the heaaer
//     // if (row == 0 && self->__header__ == NULL) createHeader(self, header, header_size);


//     if (self->__header__ == NULL) return false;

//     const char *func_name = "createRow";

//     if (row == 0 && self->grid_v3 != NULL) free_grid_v3(self);

//     // Musst insert next row
//     if (row != self->rowCount)  return false;

//     // Allocate Next Row

//     if (row == 0) {
//         self->grid_v3 = (char ***)malloc(sizeof(char **) * self->columnCount);
//         if (self->grid_v3 == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
//     }
//     else {
//         self->grid_v3 == (char ***)realloc(self->grid_v3, (sizeof(char **) * self->columnCount * (++self->rowCount)));
//         if (self->grid_v3 == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
//     }


//     for (uint32_t column = 0; column < self->columnCount; column++) {

//         // Allocate string
//         self->grid_v3[row][column] = (char*)malloc(sizeof(char) * (strlen(values[column])+1));
//         if (self->grid_v3[row][column] == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

//         strcpy(self->grid_v3[row][column], values[column]);
//     }

//     // self->rowCount++;

//     return true;
// }

// bool createData(DataLynx *self,  uintmax_t row, const char *header[], ) {

//     /* - This function will NOT update/change data if field already exists. This is ONLY for creating/inserting fields
//        - Row must be rowCount-1 or rowCount, in which case we start new row, only if column is 0th column. Otherwise return false
//        - Creates hedaer dynamically as reads in row 0 */

//     if (self == NULL) return false;
// // USE STATIC UNTIL HITS header_size

//     return true;
// }


// ___ BUILD DOUBLE LINK LIST ___
bool build_dblink_list(char **s_ptr, node **head, node **last) {

    /* THIS FUNCTION: Builds doubly linked list. It is called in split_2darray_by() */
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string

    // -- APPEND NODE --
    // Create new node
    node *n = (node*)malloc(sizeof(node));
    if (n == NULL) {if_error(MALLOC_FAILED, "build_dblink_list"); return false;}

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

    return true;
}


// ___ BUILD - DICT - DOUBLE LINK LIST ___
bool build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char **current_column_name) {

    // Function name (for use in if_error())
    const char *func_name = "build_dict_link_list";

    /* THIS FUNCTION: Builds an array of doubly linked lists that acts an array of dicts. It is called from dictReader() */
    // string input changed to **:
            // It makes it more obvious to the programmer what the intent of the input is (i.e. that the buffer will be taken over by the node)

                  // This does not solve the problem of not being able to handle a string literal.
            // input can NOT be string literal bc this will be popped off the stack

        // Must be dynamically allocated string
    // -- APPEND NODE --
    // Create new node
    dict_node *n = (dict_node*)malloc(sizeof(dict_node));
    if (n == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    // This string pointer in the node takes over the allocated string buffer (i.e. **s_ptr).
    n->s = *s_ptr;
    *s_ptr = NULL;
    s_ptr = NULL;

    // Allocate buffer column name and copy string
    // n->column_name = (char*)malloc(sizeof(char)*strlen(*current_column_name)+1);
    // if (n->column_name == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    // strcpy(n->column_name, current_column_name);
    n->column_name = *current_column_name;

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

    return true;
}

char *changeMissingValue(DataLynx *self, char *missingValue) {

    if (self == NULL || missingValue == NULL) return NULL;

    /* For simplicity's sake, this function will be called changeMissingValue(), although I will use it to create the missingValue in the constructor using the preprocessor #define MISSING_VALUE */
    const char *func_name = "changeMissingValue";

    // Free previous string if it exists
    if (self->missingValue != NULL) {

        if (strcmp(self->missingValue, missingValue) == 0) return self->missingValue;
        else free(self->missingValue);
    }

    // Allocate
    self->missingValue = malloc(sizeof(char)*strlen(missingValue)+1);
    if (self->missingValue == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Assign
    strcpy(self->missingValue, missingValue);

    return self->missingValue;
}

bool changeFilename(DataLynx *self, char *filename) {

    if (self == NULL || filename == NULL) return false;

    if (self->filename != NULL) free(self->filename);

    const char *func_name = "changeFilename";

    // Allocate buffer for filename
    self->filename = (char*)malloc(sizeof(char) * (strlen(filename)+1));
    if (self->filename == NULL)  {if_error(MALLOC_FAILED, func_name); return false;}

    strcpy(self->filename, filename);

    return true;
}

//          _______ USER INPUT FILENAME() ________
char *userInputFilename(DataLynx *self, char *prompt) {

    if (self == NULL) return NULL;

    const char *func_name = "userInputFilename";

    // Print prompt to terminal
    printf("%s", prompt);

    if (self->filename != NULL) free_null(&self->filename);

    // Allocate buffer
    self->filename = (char *)malloc(sizeof(char)*FILENAME_BUFFER_SIZE+1);
    if (self->filename == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Read from command line
    if (fgets(self->filename, FILENAME_BUFFER_SIZE, stdin) == NULL) {if_error(FREAD_FAILED, func_name); return NULL;}

    for (uintmax_t i = 0; i < FILENAME_BUFFER_SIZE; i++) {
        if (self->filename[i] == '\n') {self->filename[i] = '\0'; break;}
    }
    self->filename[FILENAME_BUFFER_SIZE] = '\0';

    // Check if csv
    if (!is_ext(self->filename, ".csv") && !is_ext(self->filename, ".txt")) return NULL;

    return self->filename;

}



// -- REPLACE --
bool replaceAll(DataLynx *self, char *to_replace, char *replace_with) {

    if (self == NULL || to_replace == NULL || replace_with == NULL) return false;

    if (self->grid_v3 != NULL) return grid_v3_replace(self, to_replace, replace_with, -1);
    else if (self->grid != NULL) return grid_replace(self, to_replace, replace_with, -1);
    else if (self->dict_grid != NULL) return dict_grid_replace(self, to_replace, replace_with, -1);

    return false;
}

bool replaceInColumn(DataLynx *self, char *column_name, char *to_replace, char *replace_with) {

    if (self == NULL || column_name == NULL || to_replace == NULL || replace_with == NULL) return false;

    intmax_t column_index = findColumnIndex(self, column_name);

    if (column_index < 0) return false;

    if (self->grid_v3 != NULL) return grid_v3_replace(self, to_replace, replace_with, column_index);
    else if (self->grid != NULL) return grid_replace(self, to_replace, replace_with, column_index);
    else if (self->dict_grid != NULL) return dict_grid_replace(self, to_replace, replace_with, column_index);

    return false;
}


// GRID REPLACE_v3
bool grid_v3_replace(DataLynx *self, char *to_replace, char *replace_with, intmax_t column) {

    /* column can not be unsinged, bc if -1 is passed, function will replace in all columns */

    const char *func_name = "grid_v3_replace";

    for (uintmax_t i = 0; i < self->rowCount; i++) {

        uintmax_t j = 0;
        uintmax_t column_stop = self->columnCount;

        // Set j and column_stop accordingly if only to replace in one column
        if (column >= 0) {
            j = column;
            column_stop = column+1;
        }


        while (j < column_stop) {

            // if (column >= 0 && j != column) continue; /* Although cleaner code, this seems to be less optimized than if statement to adjust j and column_stop */

            if (strcmp(self->grid_v3[i][j], to_replace) == 0) {

                update_stats(self, j, to_replace, replace_with);

                // Allocate for new string
                self->grid_v3[i][j] = realloc(self->grid_v3[i][j], sizeof(char)*(strlen(replace_with)+1));
                if (self->grid_v3[i][j] == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

                strcpy(self->grid_v3[i][j], replace_with);

            }

            j++;

        }

    }


    return true;
}


// GRID REPLACE
bool grid_replace(DataLynx *self, char *to_replace, char *replace_with, intmax_t column) {

    const char *func_name = "grid_replace";

    for (uintmax_t i = 0; i < self->rowCount; i++) {

        node *tmp = self->grid[i];
        intmax_t current_column = 0;

        while (tmp != NULL) {

            // For replacing only in 1 column - get to correct column
            if (current_column != column && column >= 0) {tmp=tmp->next; current_column++; continue;}

            if (strcmp(tmp->s, to_replace) == 0) {

                update_stats(self, current_column, to_replace, replace_with);

                // Allocate for new string
                tmp->s = realloc(tmp->s, sizeof(char)*(strlen(replace_with)+1));
                if (tmp->s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

                strcpy(tmp->s, replace_with);

            }

            tmp = tmp->next;
            current_column++;
        }

    }


    return true;
}


bool dict_grid_replace(DataLynx *self, char *to_replace, char *replace_with, intmax_t column) {

    const char *func_name = "dict_grid_replace";

    for (uintmax_t i = 0; i < self->rowCount; i++) {

        dict_node *tmp = self->dict_grid[i];
        intmax_t current_column = 0;

        while (tmp != NULL) {

            // For replacing only in 1 column - get to correct column
            if (current_column != column && column >= 0) {tmp=tmp->next; current_column++; continue;}

            if (strcmp(tmp->s, to_replace) == 0) {

                update_stats(self, current_column, to_replace, replace_with);

                // Allocate for new string
                tmp->s = realloc(tmp->s, sizeof(char)*(strlen(replace_with)+1));
                if (tmp->s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

                strcpy(tmp->s, replace_with);

            }

            tmp = tmp->next;
            current_column++;
        }

    }


    return true;
}



//          DROP COLUMN()
bool dropColumn(DataLynx *self, char *column_name) {

    if (self == NULL) return false;

    int32_t column_index_signed;
    if ((column_index_signed = findColumnIndex(self, column_name)) < 0) return false;

    uint32_t column_index = column_index_signed;

    const char *func_name = "dropColumn";

    // Allocate new header
    char **new_header = (char**)malloc(sizeof(char*) * (self->columnCount-1));
    if (new_header == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    // Allocate new aggregate array
    Aggregate *new_aggregate = (Aggregate*)malloc(sizeof(Aggregate) * (self->columnCount-1));
    if (new_aggregate == NULL) {if_error(MALLOC_FAILED, func_name); return false;}


    for (uintmax_t row = 0; row < self->rowCount; row++) {

        // Allocate new row (1 less column)
        char **new_row = NULL;
        if (self->grid_v3 != NULL) {
            new_row = (char**)malloc(sizeof(char*) * (self->columnCount-1));
            if (new_row == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
        }


        // Copy data from old row to new row (skip column to drop)
        uint32_t new_row_column = 0; /* Must use separate column counter or else will get off track after drop column is reached (i.e. will be one index off)*/
        for (uint32_t column = 0; column < self->columnCount; column++) {

            if (column != column_index) {

                if (self->grid_v3 != NULL) {new_row[new_row_column] = self->grid_v3[row][column];}

                new_header[new_row_column] = self->__header__[column];
                new_aggregate[new_row_column] = self->aggregate[column];

                new_row_column++;
            }
            else {
                if (self->grid_v3 != NULL) free_null(&self->grid_v3[row][column]);
            }


        }

        if (self->grid_v3 != NULL) {
            // Free old row and reassign
            free(self->grid_v3[row]);
            self->grid_v3[row] = new_row;
        }
        else if (self->grid != NULL) {

            if (column_index == 0) {
                node *tmp = self->grid[row];
                self->grid[row] = self->grid[row]->next;
                free_null(&tmp->s);
                free(tmp);
            }
            else {
                node *tmp_prev = self->grid[row];
                get_ptr_to_correct_column(column_index-1, &tmp_prev, NULL);
                node *tmp_to_drop = tmp_prev->next;
                tmp_prev->next = tmp_to_drop->next;
                free_null(&tmp_to_drop->s);
                free(tmp_to_drop);
            }

        }
        else if (self->dict_grid != NULL) {

            if (column_index == 0) {
                dict_node *tmp = self->dict_grid[row];
                self->dict_grid[row] = self->dict_grid[row]->next;
                free_null(&tmp->s);
                free(tmp);
            }
            else {
                dict_node *tmp_prev = self->dict_grid[row];
                get_ptr_to_correct_column(column_index-1, NULL, &tmp_prev);
                dict_node *tmp_to_drop = tmp_prev->next;
                tmp_prev->next = tmp_to_drop->next;
                free_null(&tmp_to_drop->s);
                free(tmp_to_drop);
            }

        }

    }


    // Free Value Counts
    if (self->aggregate[column_index].value_counts != NULL) {
        ValueCount *tmp = NULL;
        for (uint8_t i = 0; i < 27; i++) {

            while (self->aggregate[column_index].value_counts[i] != NULL) {
                // Remember next node
                tmp = self->aggregate[column_index].value_counts[i]->next;

                // Free
                free_null(&(self->aggregate[column_index].value_counts[i]->value)); /* Will only need to free this if a replace has been done and no longer points to value in data structure. free_null() will bounce back right away if already null, so no double frees */
                self->aggregate[column_index].value_counts[i]->next = NULL;
                free(self->aggregate[column_index].value_counts[i]);
                self->aggregate[column_index].value_counts[i] = tmp;
            }
        }

        // Free array of value counts
        free(self->aggregate[column_index].value_counts);
        self->aggregate[column_index].value_counts = NULL;
    }


    // Free old header/aggregate arrays and reassign
    free_null(&self->__header__[column_index]);
    free(self->__header__);
    self->__header__ = new_header;

    free(self->aggregate);
    self->aggregate = new_aggregate;

    self->columnCount--;

    return true;
}


//      DROP ROW()
bool dropRow(DataLynx *self, uintmax_t row_to_drop) {

    // Safety checks
    if (self == NULL || row_to_drop > self->rowCount-1) return false;

    const char *func_name = "dropRow";

    // Allocate new grid
    char ***new_grid_v3 = NULL;
    node **new_grid = NULL;
    dict_node **new_dict_grid = NULL;

    if (self->grid_v3 != NULL) {
        new_grid_v3 = (char***)malloc(sizeof(char**) * self->rowCount-1);
        if (new_grid_v3 == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    }
    else if (self->grid != NULL) {
        new_grid = (node**)malloc(sizeof(node*) * self->rowCount-1);
        if (new_grid == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    }
    else if (self->dict_grid != NULL) {
        new_dict_grid = (dict_node**)malloc(sizeof(dict_node*) * self->rowCount-1);
        if (new_dict_grid == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    }

    // Iterate through row and assign all rows (except row to drop) to new grid
    uintmax_t new_row_count = 0;
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        if (row == row_to_drop) continue;

        if (self->grid_v3 != NULL) {

            new_grid_v3[new_row_count] = self->grid_v3[row];
        }
        else if (self->grid != NULL) {
             new_grid[new_row_count] = self->grid[row];
        }
        else if (self->dict_grid != NULL) {
             new_dict_grid[new_row_count] = self->dict_grid[row];
        }

        new_row_count++;
    }


    // Free fields/update stats - Iterate through columns of row to drop and free strings (fields) in dropped row, as well as update stats for that row
    self->row_to_drop = row_to_drop; /* Used in find_new_min/max() */

    // // Decrement row count
    // self->rowCount--;
    // self->number_of_rows_to_print = self->rowCount;

    node *grid_tmp = NULL;
    dict_node *dict_tmp = NULL;
    if (self->grid != NULL) grid_tmp = self->grid[row_to_drop];
    else if (self->dict_grid != NULL) dict_tmp = self->dict_grid[row_to_drop];

    for (uint32_t column = 0; column < self->columnCount; column++) {
        char *field = NULL;

        if (self->grid_v3 != NULL) field = self->grid_v3[row_to_drop][column];
        else if (self->grid != NULL) field = grid_tmp->s;
        else if (self->dict_grid != NULL) field = dict_tmp->s;

        update_stats(self, column, field, NULL);

        if (self->grid_v3 != NULL) free(field); /* The fields in grid and dict_grid will be freed in free_list() and free_dict_list() */

        if (self->grid != NULL)grid_tmp = grid_tmp->next;
        else if (self->dict_grid != NULL) dict_tmp = dict_tmp->next;
    }
    self->row_to_drop = -1; /* Reset to default */

    // Free dropped row & old grid. Assign to grid to 'object'
    if (self->grid_v3 != NULL) {

        // Free dropped row
        free(self->grid_v3[row_to_drop]);

        // Free old grid
        free(self->grid_v3);

        // Assign new grid
        self->grid_v3 = new_grid_v3;
    }
    else if (self->grid != NULL) {

        // Free dropped row
        free_list(self->grid[row_to_drop]);

        // Free old grid
        free(self->grid);

        // Assign new grid
        self->grid = new_grid;
    }
    else if (self->dict_grid != NULL) {

        // Free dropped row
        free_dict_list(self->dict_grid[row_to_drop]);

        // Free old grid
        free(self->dict_grid);

        // Assign new grid
        self->dict_grid = new_dict_grid;
    }



    // Decrement row count
    self->rowCount--;
    self->number_of_rows_to_print = self->rowCount;


    return true;
}




//          FIND ALPHA INDEX()
uint8_t find_alpha_index(char *value) {

    // Find alphabetical index of 1st char fo string (i.e. a-z/A-Z : 0-25)
    if (value[0] >= 'A' && value[0] <= 'Z') return value[0] - 65;
    else if (value[0] >= 'a' && value[0] <= 'z') return value[0] - 97;

    return 26;

}



//      _____ FIND_ROW_COUNT() _____
uintmax_t find_row_count(DataLynx *self) {

    if (self == NULL) return 0;

    if (self->raw == NULL) return 0;

    uintmax_t i = 0;

    self->rowCount = 0;

    while (self->raw[i] != '\0') {

        if (self->raw[i] == '\n' && self->raw[i-1] != '\n') {
            self->rowCount++;
        }

        i++;
    }

    // This accounts for CSVs in which end has not been carriage returned (i.e. the last char of the file is the last char of the last row, as opposed to being on the following line)
    if (self->raw[strlen(self->raw)-1] != '\n') self->rowCount++;

    calc_max_row_digits(self);

    self->number_of_rows_to_print = self->rowCount;

    return self->rowCount;
}

bool formatHeader(DataLynx *self) {

    // Safety checks
    if (self == NULL) return false;
    if (self->__header__ == NULL) return false;

    bool formatted = false;
    char **header = self->__header__; /* This is simply to make if statements slightly more readable */

    // Iterate through each column name
    for (uintmax_t i = 0; i < self->columnCount; i++) {

        uintmax_t length = strlen(self->__header__[i]);

        // Iterate through each char in column name
        for (uintmax_t j = 0; j < length; j++) {

            // Capitalize first char
            if (j == 0 && (header[i][0] >= 'a' && header[i][0] <= 'z')) {header[i][0] -= 32; formatted = true;} /* capitalize first letter of string */
            else if (header[i][j] == '_') {header[i][j] = ' '; formatted = true;} /* replace _ with space */
            else if ((j != 0 && header[i][j-1] == ' ') && (header[i][j] >= 'a' && header[i][j] <= 'z')) {header[i][j] -=32; formatted = true;} /* capitalize first letter that comes after space (i.e new word) */
            else if ((j != 0 && header[i][j-1] != ' ' && header[i][j-1] != '"') && (header[i][j] >= 'A' && header[i][j] <= 'Z')) {
                /* Lower case any letters that are not the first letter of a word */

                // Do not lowercase 'D' in ID
                if (header[i][j] == 'D' && (header[i][j+1] == '_' || header[i][j+1] == ' ' || header[i][j+1] == '\0')) continue;

                header[i][j] += 32; formatted = true;
            }
            else if (j >= 1 && header[i][j-1] == 'I' && header[i][j] == 'd') {header[i][j] -= 32; formatted = true;} /* capitalize ID */

        }
    }

    // Simultaneously update CSV file if permissions allow
    if (self->csv_write_permission && self->destructive_mode) write_csv_header_(self);

    return formatted;
}


//      CHANGE COLUMN NAME
bool changeColumnName(DataLynx *self, char *old_column_name, char *new_column_name) {

    if (self == NULL || old_column_name == NULL || new_column_name == NULL) return false;
    if (self->__header__ == NULL) return false;

    char *func_name = "changeColumnName";

    intmax_t column_index = findColumnIndex(self, old_column_name);

    if (column_index < 0) return false;

    free(self->__header__[column_index]);

    // Allocate buffer for new column name
    self->__header__[column_index] = (char*)malloc(sizeof(char)*(strlen(new_column_name)+1));
    if (self->__header__[column_index] == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    strcpy(self->__header__[column_index], new_column_name);


    // Re-assign all corresponding column names in data structure to point to new string in header
    if (self->dict_grid != NULL) {

        for (uint32_t row = 0; row < self->rowCount; row++) {

            dict_node *tmp = self->dict_grid[row];

            for (uint32_t col = 0; col < column_index && tmp != NULL; col++) tmp = tmp->next;

            if (tmp != NULL) tmp->column_name = self->__header__[column_index];
        }
    }

    // Simultaneously update CSV file if permissions allow
    if (self->csv_write_permission && self->destructive_mode) write_csv_header_(self);

    return true;

}




//      STRIP FIELD
bool stripField(DataLynx *self, uintmax_t row, char *column_name) {

    // Safety checks
    if (self == NULL || column_name == NULL) return false;
    if (row > self->rowCount-1) return false;

    // Get column integer index
    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    return strip_field_helper_(self, row, column_index, STRIP_BOTH);
}



//      STRIP FIELD LEFT
bool stripFieldL(DataLynx *self, uintmax_t row, char *column_name) {

    // Safety checks
    if (self == NULL || column_name == NULL) return false;
    if (row > self->rowCount-1) return false;

    // Get column integer index
    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    return strip_field_helper_(self, row, column_index, STRIP_LEFT);
}


//      STRIP FIELD RIGHT
bool stripFieldR(DataLynx *self, uintmax_t row, char *column_name) {

    // Safety checks
    if (self == NULL || column_name == NULL) return false;
    if (row > self->rowCount-1) return false;

    // Get column integer index
    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    return strip_field_helper_(self, row, column_index, STRIP_RIGHT);
}



//      STRIP FIELD INDEX
bool stripFieldIdx(DataLynx *self, uintmax_t row, uintmax_t column) {

    // Safety checks
    if (self == NULL) return false;
    if (row > self->rowCount-1 || column > self->columnCount-1) return false;

    return strip_field_helper_(self, row, column, STRIP_BOTH);
}


//      STRIP FIELD INDEX LEFT
bool stripFieldIdxL(DataLynx *self, uintmax_t row, uintmax_t column) {

    // Safety checks
    if (self == NULL) return false;
    if (row > self->rowCount-1 || column > self->columnCount-1) return false;

    return strip_field_helper_(self, row, column, STRIP_LEFT);
}


//      STRIP FIELD INDEX RIGHT
bool stripFieldIdxR(DataLynx *self, uintmax_t row, uintmax_t column) {

    // Safety checks
    if (self == NULL) return false;
    if (row > self->rowCount-1 || column > self->columnCount-1) return false;

    return strip_field_helper_(self, row, column, STRIP_RIGHT);
}




//          STRIP FIELD HELPER
bool strip_field_helper_(DataLynx *self, uintmax_t row, uintmax_t column, int8_t strip_side) {


    // Setup pointers
    char *field = NULL;

    node *grid_tmp = NULL;
    dict_node *dict_tmp = NULL;

    // Access correct field
    if (self->grid_v3 != NULL) field = self->grid_v3[row][column];
    else if (self->grid != NULL) {
        grid_tmp = self->grid[row];
        get_ptr_to_correct_column(column, &grid_tmp, NULL);
        field = grid_tmp->s;
    }
    else if (self->dict_grid != NULL) {
        dict_tmp = self->dict_grid[row];
        get_ptr_to_correct_column(column, NULL, &dict_tmp);
        field = dict_tmp->s;
    }

    // Strip field
    return strip_field_internal_(self, row, column, field, grid_tmp, dict_tmp, strip_side);
}



//          STRIP COLUMN
bool stripColumn(DataLynx *self, char *column_name) {

    if (self == NULL) return false;

    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    return strip_column_internal_(self, column_index, STRIP_BOTH);

}


//          STRIP COLUMN LEFT
bool stripColumnL(DataLynx *self, char *column_name) {

    if (self == NULL) return false;

    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    return strip_column_internal_(self, column_index, STRIP_LEFT);

}



//          STRIP COLUMN RIGHT
bool stripColumnR(DataLynx *self, char *column_name) {

    if (self == NULL) return false;

    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    return strip_column_internal_(self, column_index, STRIP_RIGHT);

}


//          STRIP COLUMN INDEX BOTH
bool stripColumnIdx(DataLynx *self, uintmax_t column) {

    if (self == NULL) return false;

    if (column > self->columnCount-1) return false;

    return strip_column_internal_(self, column, STRIP_BOTH);

}


//          STRIP COLUMN INDEX LEFT
bool stripColumnIdxL(DataLynx *self, uintmax_t column) {

    if (self == NULL) return false;

    if (column > self->columnCount-1) return false;

    return strip_column_internal_(self, column, STRIP_LEFT);

}


//          STRIP COLUMN INDEX RIGHT
bool stripColumnIdxR(DataLynx *self, uintmax_t column) {

    if (self == NULL) return false;

    if (column > self->columnCount-1) return false;

    return strip_column_internal_(self, column, STRIP_RIGHT);

}


//          STRIP COLUMN INTERNAL
bool strip_column_internal_(DataLynx *self, uintmax_t column, int8_t strip_side) {

    // Iterate through every row in column
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        char *field = NULL;

        // Create tmp cursor pointers for grid & dict_grid
        node *grid_tmp = NULL;
        dict_node *dict_tmp = NULL;
        if (self->grid != NULL) grid_tmp = self->grid[row];
        else if (self->dict_grid != NULL) dict_tmp = self->dict_grid[row];

        // Access field
        if (self->grid_v3 != NULL) field = self->grid_v3[row][column];
        else if (self->grid != NULL) {
            grid_tmp = self->grid[row];
            get_ptr_to_correct_column(column, &grid_tmp, NULL);
        }
        else if (self->dict_grid != NULL) {
            dict_tmp = self->dict_grid[row];
            get_ptr_to_correct_column(column, NULL, &dict_tmp);
        }


        // Strip current field
        if (!strip_field_internal_(self, row, column, field, grid_tmp, dict_tmp, strip_side)) return false;

    }

    return true;
}



//      STRIP ALL BOTH
bool stripAll(DataLynx *self) {

    if (self == NULL) return false;

    return strip_all_internal_(self, STRIP_BOTH);
}


//      STRIP ALL LEFT
bool stripAllL(DataLynx *self) {

    if (self == NULL) return false;

    return strip_all_internal_(self, STRIP_LEFT);
}


//      STRIP ALL RIGHT
bool stripAllR(DataLynx *self) {

    if (self == NULL) return false;

    return strip_all_internal_(self, STRIP_RIGHT);
}



//      STRIP ALL INTERNAL
bool strip_all_internal_(DataLynx *self, int8_t strip_side) {

    /* NOTE: I could rearrange this code to iterate through every column 1st, then call stripColumn() within that loop to iterate through every row in the current column, which would make the code look cleaner, however with grid and dict_grid, this would change the time complexity from O(n) to O(n^2), as the pointer would have to iterate through the columns to get to the next column in every row. */

    // Iterate through each row
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        // Create tmp cursor pointers for grid & dict_grid
        node *grid_tmp = NULL;
        dict_node *dict_tmp = NULL;
        if (self->grid != NULL) grid_tmp = self->grid[row];
        else if (self->dict_grid != NULL) dict_tmp = self->dict_grid[row];

        for (uintmax_t column = 0; column < self->columnCount; column++) {

            // Assign current field to temporary pointer
            char *field = NULL;
            if (self->grid_v3 != NULL) field = self->grid_v3[row][column];
            else if (self->grid != NULL) field = grid_tmp->s;
            else if (self->dict_grid != NULL) field = dict_tmp->s;

            // Strip current field
            if (!strip_field_internal_(self, row, column, field, grid_tmp, dict_tmp, strip_side)) return false;

            // Next column
            if (self->grid != NULL) grid_tmp = grid_tmp->next;
            else if (self->dict_grid != NULL) dict_tmp = dict_tmp->next;
        }
    }

    return true;
}



//      STRIP FIELD INTERNAL
bool strip_field_internal_(DataLynx *self, uintmax_t row, uintmax_t column, char *field, node *grid_tmp, dict_node *dict_tmp, int8_t strip_side) {

    const char *func_name = "strip_field_internal_";

    uint32_t len = strlen(field);

    // Left Strip
    uint16_t chars_to_strip_left = 0;
    if (strip_side == STRIP_LEFT || strip_side == STRIP_BOTH) {

        // Iterate through string (i.e. field) from the LEFT
        for (uint32_t i = 0; i < len; i++) {

            if (field[i] != ' ' && field[i] != '\n' && field[i] != '\t' && field[i] != '\r') break;
            else chars_to_strip_left++;

        }
    }

    // Right Strip
    uint16_t chars_to_strip_right = 0;
    if (strip_side == STRIP_RIGHT || strip_side == STRIP_BOTH) {

        // Iterate through string (i.e. field) from the RIGHT
        for (int32_t i = len-1; i >= 0; i--) {

            if (field[i] != ' ' && field[i] != '\n' && field[i] != '\t' && field[i] != '\r') break;
            else chars_to_strip_right++;

        }
    }

    // Strip string of current field
    if (chars_to_strip_left > 0 || chars_to_strip_right > 0) {

        // Alocate new buffer
        size_t new_length = len - chars_to_strip_left - chars_to_strip_right;
        char *stripped_field = (char *)malloc(sizeof(char) * (new_length+1));
        if (stripped_field == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        // Temporarily cut off right white space (doing this because strncpy() gives valgrind issues)
        char last_non_white_char = field[len-chars_to_strip_right];
        field[len-chars_to_strip_right] = '\0';

        // Copy string starting from 1st non-white space char on left side and with white space on right cut off by '\0'
        // strncpy(stripped_field, &field[chars_to_strip_left], new_length); /* strncpy gives issues, even when I manually null-terminate the string */
        // stripped_field[len] = '\0';
        strcpy(stripped_field, &field[chars_to_strip_left]);

        // Return original field to original state (free() should stil free it regardless, but this makes me feel better. Remove later?)
        field[len-chars_to_strip_right] = last_non_white_char;

        // Assign stripped field to data structure
        if (self->grid_v3 != NULL) self->grid_v3[row][column] = stripped_field;
        else if (self->grid != NULL && grid_tmp != NULL) grid_tmp->s = stripped_field;
        else if (self->dict_grid != NULL && dict_tmp != NULL) dict_tmp->s = stripped_field;

        update_stats(self, column, field, stripped_field);

        free_null(&field);
    }

    return true;

}



//          FILTER()
bool filter(DataLynx *self, DataLynx *filteredData, char *column_name, char *condition_operator, char *condition_value) {

    /* THIS FUNCTION: Filters into a NEW data structure (currently only into a grid_v3, but can filter FROM any data structure) */

    // Safety Checks
    if (self == NULL || column_name == NULL || condition_operator == NULL || condition_value == NULL || self == filteredData) return false;

    // Determine if column_name is valid / retrieve corresponding column index integer
    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    // Filter / create new data structure
    bool print = false;
    bool drop_rows = false;

    return filter_internal_(self, column_index, condition_operator, condition_value, filteredData, print, drop_rows);
}



// GET FIELD CONDITION
bool printColumnCond(DataLynx *self, char *column_name, char *condition_operator, char *condition_value) {

    // Safety Checks
    if (self == NULL || column_name == NULL || condition_operator == NULL || condition_value == NULL) return false;

    // Determine if column_name is valid / retrieve corresponding column index integer
    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    // Filter and print
    bool print = true;
    bool drop_rows = false;
    return filter_internal_(self, column_index, condition_operator, condition_value, NULL, print, drop_rows);

}

bool dropRowsFilter(DataLynx *self, char *column_name, char *condition_operator, char *condition_value) {

    // Safety Checks
    if (self == NULL || column_name == NULL || condition_operator == NULL || condition_value == NULL) return false;

    // Determine if column_name is valid / retrieve corresponding column index integer
    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    // Filter and Drop Rows
    bool print = false;
    bool drop_rows = true;

    return filter_internal_(self, column_index, condition_operator, condition_value, NULL, print, drop_rows);

}



//      GET FIELDS CONDITION DICT()
bool filter_internal_(DataLynx *self, uintmax_t desired_column, char *condition_operator, char *condition_value, DataLynx *new_data, bool print, bool drop_rows) {

    if (new_data != NULL && drop_rows) return false;

    const char *func_name = "filter_internal_";

    double condition_num = 0;

    // Print column head
    if (self->aggregate[desired_column].is_number) condition_num = atof(condition_value);

    if (print && self->aggregate[desired_column].is_number) {
        printf("\n'%s' <Conditon: %s %.2f>\n", self->__header__[desired_column], condition_operator, condition_num);
    }
    else if (print) printf("\n'%s' <Conditon: %s '%s'>\n", self->__header__[desired_column], condition_operator, condition_value);


    // Array to keep track of which rows met filter condition
    uint32_t rows_met_condition[self->rowCount];

    char *current_field = NULL;
    node *node_tmp = NULL;
    dict_node *dict_tmp = NULL;

    // Iterate through rows
    uint32_t total = 0;
    for (uint32_t current_row = 0; current_row < self->rowCount; current_row++) {

        bool condition_met = false;

        // Retrieve current field
        if (self->grid_v3 != NULL) current_field = self->grid_v3[current_row][desired_column];
        else if (self->grid != NULL) {
            node_tmp = self->grid[current_row];
            get_ptr_to_correct_column(desired_column, &node_tmp, NULL);
            current_field = node_tmp->s;
        }
        else if (self->dict_grid != NULL) {
            dict_tmp = self->dict_grid[current_row];
            get_ptr_to_correct_column(desired_column, NULL, &dict_tmp);
            current_field = dict_tmp->s;
        }

        // Skip empy fields
        if (current_field[0] == '\0') continue;

        // Check conditon
        if (strcmp(condition_operator, "<") == 0) {

            if (self->aggregate[desired_column].is_number && atof(current_field) < condition_num) {condition_met = true;}
            else if (!self->aggregate[desired_column].is_number) {
                if (strcasecmp(current_field, condition_value) < 0 || (has_quotes(current_field) && strcasecmp(&current_field[1], condition_value) < 0)) condition_met = true;
            }

        }
        else if (strcmp(condition_operator, ">") == 0) {

            if (self->aggregate[desired_column].is_number && atof(current_field) > condition_num) {condition_met = true;}
            else if (!self->aggregate[desired_column].is_number) {
                if (strcasecmp(current_field, condition_value) > 0 || (has_quotes(current_field) && strcasecmp(&current_field[1], condition_value) > 0)) condition_met = true;
            }

        }
        else if (strcmp(condition_operator, "<=") == 0) {

            if (self->aggregate[desired_column].is_number && atof(current_field) <= condition_num) {condition_met = true;}
            else if (!self->aggregate[desired_column].is_number) {
                if ((!has_quotes(current_field) && strcasecmp(current_field, condition_value) <= 0) || (has_quotes(current_field) && strcasecmp(&current_field[1], condition_value) <= 0) ) condition_met = true;
            }

        }
        else if (strcmp(condition_operator, ">=") == 0) {

            if (self->aggregate[desired_column].is_number && atof(current_field) >= condition_num) {condition_met = true;}
            else if (!self->aggregate[desired_column].is_number) {
                if ((!has_quotes(current_field) && strcasecmp(current_field, condition_value) >= 0) || (has_quotes(current_field) && strcasecmp(&current_field[1], condition_value) >= 0) ) condition_met = true;
            }

        }
        else if (strcmp(condition_operator, "==") == 0) {

            if (self->aggregate[desired_column].is_number && atof(current_field) == condition_num) {condition_met = true;}
            else if (!self->aggregate[desired_column].is_number) {
                if ((!has_quotes(current_field) && strcmp(current_field, condition_value) == 0) || (has_quotes(current_field) && strcmp(&current_field[1], condition_value) == 0) ) condition_met = true;
            }

        }

        // Print result
        if (condition_met && print) {
            printf("%*d: %s\n", self->max_row_digits, current_row, current_field);
        }

        if (condition_met) {
            rows_met_condition[total] = current_row; /* Keep track of which rows met the filter condition */
            total++;
        }

    }


    if (print) (total == 1) ? printf("\n%d field in this column met this condition\n\n", total) : printf("\n%d fields in this column met this condition\n\n", total);


    // For filtering into new object
    if (new_data != NULL) {

        size_t filename_length = strlen(self->filename);

        // Create appended portion of amended filename (i.e. filename plus conditon)
        size_t append_strlen = strlen(" (Filtered Data)") + strlen(self->__header__[desired_column]) + strlen(condition_value) + strlen(condition_operator);
        char new_filename_append[append_strlen+4];
        sprintf(new_filename_append, " (Filtered Data) %s %s %s", self->__header__[desired_column], condition_operator, condition_value);

        size_t new_filename_length = filename_length + strlen(new_filename_append);

        // Allocate for new filename
        new_data->filename = (char*)malloc(sizeof(char) * (new_filename_length + 1));
        if (new_data->filename == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        sprintf(new_data->filename, "%s%s", self->filename, new_filename_append);

        createHeader(new_data, self->__header__, self->columnCount);

        // Copy filtered rows
        for (uint32_t row = 0; row < total; row++) {

            uint32_t original_row_index = rows_met_condition[row];

            char *values[self->columnCount];

            // Assign current row to pointer
            if (self->grid != NULL) node_tmp = self->grid[original_row_index];
            else if (self->dict_grid != NULL) dict_tmp = self->dict_grid[original_row_index];

            // Copy field strings into values array
            for (uint16_t column = 0; column < self->columnCount; column++) {

                if (self->grid_v3 != NULL) values[column] = self->grid_v3[original_row_index][column];
                else if (self->grid != NULL) {
                    values[column] = node_tmp->s;
                    node_tmp = node_tmp->next;
                }
                else if (self->dict_grid != NULL) {
                    values[column] = dict_tmp->s;
                    dict_tmp = dict_tmp->next;
                }

            }

            // Append row
            insertRow(new_data, values);

        }

    }


    if (drop_rows) {

        // Iterate through rows that met conditon and drop them from data structure
        for (uint32_t row = 0; row < total; row++) {

            uint32_t original_row_index = rows_met_condition[row];

            dropRow(self, original_row_index);

            // Adjust index of remaining rows, now that a row was dropped
            for (uint32_t j = row+1; j < total; j++) rows_met_condition[j]--;
        }
    }


    return (total > 0) ? true : false;

}







//          _____ GET_FIELD2() ____
char *getField(DataLynx *self, uintmax_t desired_row, char *desired_column) {

    /* THIS FUNCTION:
        - Uses integer row/string column name to index into data and find field
            - 1st finds the integer column index corresponding to the string column name
            - Then calls getField() */
    if (self == NULL) return NULL;

    if (desired_row > self->rowCount) return NULL;

    // Dict Grid (we do not need to find column integer if the dict_grid is the present data structure)
    if (self->dict_grid != NULL) {
        return get_field_dict(self, desired_row, desired_column);

    }

    // Retrieve integer column index
    intmax_t column_index = findColumnIndex(self, desired_column);

    if (column_index < 0) return NULL;



    return getFieldIdx(self, desired_row, column_index);
}

//          _____ GET_FIELD() ____
char *getFieldIdx(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column) {

    /* THIS FUNCTION:
        - Uses integer row/column to index into data and find field */

    if (self == NULL) return NULL;

    if (desired_row > self->rowCount-1 || desired_column > self->columnCount-1) return NULL;

    // const char *func_name = "getField";


    //          -----  Retrieve Field  ----
    // Raw
    if (self->raw != NULL) {
        // return NULL;
        return get_field_raw(self, desired_row, desired_column);

    }
    // Rows
    else if (self->rows != NULL) {
        return get_field_rows(self, desired_row, desired_column);
    }
    else if (self->grid_v3 != NULL) {

        return get_field_grid_v3(self, desired_row, desired_column, NULL, 0);
    }
    // Grid
    else if (self->grid != NULL) {

        return get_field_grid(self, desired_row, desired_column, NULL, 0);
    }
    else if (self->dict_grid != NULL) {
        return get_field_dict(self, desired_row, self->__header__[desired_column]);
    }



    return NULL;
}


//          ____ GET_FIELD_RAW() ____
char *get_field_raw(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column) {

    /* THIS FUNCTION WILL CAUSE ERRORS IF CALLED TWICE FROM THE SAME FUNCTION -> printf("Get Field: %s, %s\n", myData.getField(&myData, 9,2), myData.getField(&myData, 9,2)); */

    /* THIS FUNCTION: Is intented to only be called from getField(),
        therefore, I will not be checking if self == NULL or if desired row/column are greater than rowCount-1/columnCount-1. */

    const char *func_name = "get_field_raw";

    // Find beginning of field
    uintmax_t cursor = 0, current_row = 0, current_column = 0, field_char_count = 0;
    bool inside_quotes = false, field_found = false;
    while (true) {


        if (current_row == desired_row && current_column == desired_column) {
            field_found = true;
        }


        if (self->raw[cursor] == '\0') break;
        else if (!inside_quotes && self->raw[cursor] == ',') {
            if (field_found) break;
            current_column++;
        }
        else if (!inside_quotes && self->raw[cursor] == '\n') {
            if (field_found) break;
            current_row++;
            current_column = 0;
        }
        else if (!inside_quotes && self->raw[cursor] == '"') inside_quotes = true;
        else if (inside_quotes && self->raw[cursor] == '"') inside_quotes = false;

        cursor++;

        if (field_found) field_char_count++;

    }

    if (!field_found) return NULL;


    // Allocate buffer
    char *buffer = malloc(sizeof(char)*(field_char_count+1));
    if (buffer == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Get cursor back to beginning of field
    cursor -= field_char_count;

    for (uintmax_t i = 0; i < field_char_count; i++) {
        buffer[i] = self->raw[cursor];
        cursor++;
    }

    buffer[field_char_count] = '\0';


    // Save in struct (for ability to return multiple strings in one line of code without making caller responsible for freeing strings i.e. printf("Get Field: %s, %s\n", myData.getField(&myData, 0, 3), myData.getField(&myData, 0, 4));)
    if (buffer == NULL) return buffer; /* This means the index does not exist (i.e. row/column integer out of range) */
    else if (buffer[0] == '\0') {free_null(&buffer); return self->missingValue;} /* This means the index exists, but field is blank (i.e. David,,123 : the middle field is blank) */

    return append_last_retrieved_fields(self, &buffer);

}


//          ____ GET_FIELD_ROWS() ____
char *get_field_rows(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column) {

    /* THIS FUNCTION WILL CAUSE ERRORS IF CALLED TWICE FROM THE SAME FUNCTION -> printf("Get Field: %s, %s\n", myData.getField(&myData, 9,2), myData.getField(&myData, 9,2)); */
    /* THIS FUNCTION: Is intented to only be called from getField(),
        therefore, I will not be checking if self == NULL or if desired row/column are greater than rowCount-1/columnCount-1. */

    const char *func_name = "get_field_rows";

    // Find beginning of field
    uintmax_t cursor = 0, current_column = 0, field_char_count = 0;
    bool inside_quotes = false, field_found = false;
    while (true) {


        if (current_column == desired_column) {
            field_found = true;
        }


        if (self->rows[desired_row][cursor] == '\0') break;
        else if (!inside_quotes && self->rows[desired_row][cursor] == ',') {
            if (field_found) break;
            current_column++;
        }
        else if (!inside_quotes && self->rows[desired_row][cursor] == '\n') {
            if (field_found) break;
            current_column = 0;
        }
        else if (!inside_quotes && self->rows[desired_row][cursor] == '"') inside_quotes = true;
        else if (inside_quotes && self->rows[desired_row][cursor] == '"') inside_quotes = false;

        cursor++;

        if (field_found) field_char_count++;

    }

    if (!field_found) return NULL;


    // Allocate buffer
    char *buffer = malloc(sizeof(char)*(field_char_count+1));
    if (buffer == NULL) {if_error(MALLOC_FAILED, func_name);}

    // Get cursor back to beginning of field
    cursor -= field_char_count;

    for (uintmax_t i = 0; i < field_char_count; i++) {
        buffer[i] = self->rows[desired_row][cursor];
        cursor++;
    }

    buffer[field_char_count] = '\0';


    // Save in struct (for ability to return multiple strings in one line of code without making caller responsible for freeing strings i.e. printf("Get Field: %s, %s\n", myData.getField(&myData, 0, 3), myData.getField(&myData, 0, 4));)
    if (buffer == NULL) return buffer;
    else if (buffer[0] == '\0') {free_null(&buffer); return self->missingValue;}

    return append_last_retrieved_fields(self, &buffer);

}


char *get_field_grid_v3(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num) {

    // NO condition (getField())
    if (condition_operator == NULL) return self->grid_v3[desired_row][desired_column];

    // CONDITION: (printColumnCond())
    printf("'%s' <Conditon: %s %f>\n", self->__header__[desired_column], condition_operator, condition_num);

    uint32_t total = 0;
    for (uint32_t current_row = 0; current_row < self->rowCount; current_row++) {

        // Do not print empy fields
        if (self->grid_v3[current_row][desired_column][0] == '\0') continue;

        bool condition_met = false;

        // Check conditon
        if (strcmp(condition_operator, "<") == 0) {
            if (atof(self->grid_v3[current_row][desired_column]) < condition_num) {condition_met = true;}
        }
        else if (strcmp(condition_operator, ">") == 0) {
            if (atof(self->grid_v3[current_row][desired_column]) > condition_num) {condition_met = true;}
        }
        else if (strcmp(condition_operator, "<=") == 0) {
            if (atof(self->grid_v3[current_row][desired_column]) <= condition_num) {condition_met = true;}
        }
        else if (strcmp(condition_operator, ">=") == 0) {
            if (atof(self->grid_v3[current_row][desired_column]) >= condition_num) {condition_met = true;}
        }

        // Print result
        if (condition_met) {
            printf("%*d: %s\n", self->max_row_digits, current_row, self->grid_v3[current_row][desired_column]);
            total++;
        }
    }

    printf("\n%d fields in this column met this condition\n\n", total);

    return "Successfully printed all fields meeting condition\n";
}


char *get_field_grid(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *condition_operator, double condition_num) {

    /* THIS FUNCTION: allows you to index into array of dicts (i.e. array of doubly linked lists acting as dicts) */
        // Will return NULL if no column matches desired_column or if row is out of range.

    uintmax_t current_column = 0;


    // NO condition (getField())
    if (condition_operator == NULL) {

        // Traverse Doubly linked list to find correct column
        node *tmp = self->grid[desired_row];
        while (tmp != NULL) {

            if (current_column == desired_column) return tmp->s;

            tmp = tmp->next;
            current_column++;
        }

        return NULL;
    }

    // Condition (printColumnCond())
    printf("'%s' <Conditon: %s %f>\n", self->__header__[desired_column], condition_operator, condition_num);

    uint32_t total = 0;
    for (uint32_t current_row = 0; current_row < self->rowCount; current_row++) {

        node *tmp = self->grid[current_row];
        bool condition_met = false;
        current_column = 0;
        // Get to correct column
        while (tmp != NULL) {
            if (current_column == desired_column) break;
            tmp = tmp->next;
            current_column++;
        }


        if (current_column == desired_column && tmp != NULL) {

            // Do not print empy fields
            if (tmp->s[0] == '\0') continue;

            // Check conditon
            if (strcmp(condition_operator, "<") == 0) {
                if (atof(tmp->s) < condition_num) {condition_met = true;}
            }
            else if (strcmp(condition_operator, ">") == 0) {
                if (atof(tmp->s) > condition_num) {condition_met = true;}
            }
            else if (strcmp(condition_operator, "<=") == 0) {

                if (atof(tmp->s) <= condition_num) {condition_met = true;}
            }
            else if (strcmp(condition_operator, ">=") == 0) {
                if (atof(tmp->s) >= condition_num) {condition_met = true;}
            }

            // Print result
            if (condition_met) {
                printf("%*d: %s\n", self->max_row_digits, current_row, tmp->s);
                total++;
            }
        }


    }

    printf("\n%d fields in this column met this condition\n\n", total);

    return "Successfully printed all fields meeting condition\n";

}




// _____ INDEX INTO DICT _____
char *get_field_dict(DataLynx *self, uintmax_t desired_row, char *desired_column) {

    // Checks
    // if (self == NULL) return NULL;
    if (desired_row > self->rowCount-1 || self->dict_grid == NULL) return NULL;

    // Function name (for use in if_error())
    const char *func_name = "get_field_dict";

    /* THIS FUNCTION: allows you to index into array of dicts (i.e. array of doubly linked lists acting as dicts) */
        // Will return NULL if no column matches desired_column or if row is out of range.

    char *desired_column_with_quotes = NULL;
    bool added_quotes = false;

    // Traverse Doubly linked list to find correct column
    dict_node *tmp = self->dict_grid[desired_row];
    while (tmp != NULL) {

        // Add quotes to desrired column name if neccessary
        if (has_quotes(tmp->column_name) && !has_quotes(desired_column)) {

            // Allocate buffer
            desired_column_with_quotes = (char *)malloc(sizeof(char)*(strlen(desired_column)+1));
            if (desired_column_with_quotes == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;};

            // Copy
            strcpy(desired_column_with_quotes, desired_column);
            add_quotes(&desired_column_with_quotes);
            added_quotes = true;
        }

        // Compare current column name with desired column name
        if (strcmp(tmp->column_name, desired_column) == 0 || (added_quotes && strcmp(tmp->column_name, desired_column_with_quotes) == 0)) {

            if (desired_column_with_quotes != NULL) free_null(&desired_column_with_quotes);

            return tmp->s;
        }

        if (desired_column_with_quotes != NULL) free_null(&desired_column_with_quotes);

        tmp = tmp->next;
    }

    return NULL;



}






//      ___ FIND_COLUMN_INDEX() ___
intmax_t findColumnIndex(DataLynx *self, const char *desired_column) {

    /* Will return the correct integer index location associated with desired_column string.
            - If no column name is found that matches desired_column, will return -1. This is why although index locations are not signed, I still need to return a signed int */

    // Find column integer index - Iterate through CSV Header to find column that matches desired_column

    if (self == NULL || desired_column == NULL) return -1;

    if (self->__header__ == NULL && self->file_ptr != NULL) headerReader(self);

    bool found_column = false;
    uintmax_t current_column = 0;
    while (current_column < self->columnCount) {

        // Compare strings
        if (strcmp(self->__header__[current_column], desired_column) == 0) {
            found_column = true;
            break;
        }
        else if (has_quotes(self->__header__[current_column])) {
            // Compare strings without quotes:

            if (strcmp(remove_quotes(self, self->__header__[current_column]), desired_column) == 0) {
                found_column = true;
                break;
            }
        }

        current_column++;
    }

    /* This connot be inline if statement, because current_column is unsiged, so the return becomes unsigned */
    if (found_column) return current_column;
    else return -1;

}





//        --- CONVERT DATA STRUCTURES in MEMORY FUNCTIONS ---

//      ___ STRING to 2D Array ___ (V2 - 2D JAGGED ARRAY)
char **string_into_2d_array(DataLynx *self) {

    if (self == NULL) return NULL;

    if (self->raw == NULL) return NULL;

    // Function name (for use in if_error())
    const char *func_name = "string_into_2d_array";

    /* THIS FUNCTION: Turns string INTO DYNAMICALLY ALLOCATED JAGGED 2D ARRAY (splits by '\n') */

    uintmax_t current_row = 0;

    // Main Array
    self->rows = (char**)malloc(sizeof(char*));
    if (self->rows == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Iterate through file
    uintmax_t char_count = 0;
    while (true)
    {
        bool next_row = true;

        // Allocate 1st char of current row
        self->rows[current_row] = (char*)malloc(sizeof(char));
        if (self->rows[current_row] == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

        int8_t tmp;
        uint64_t i = 0;
        while (true) {

            // Scan char:
                 /* - Not top tested, bc that would require duplicate realloc code after loop ends.
                    - Using tmp to test for EOF, bc whether char is signed or not is implementation dependent. */
            // tmp = fgetc(self->file_ptr);
            tmp = self->raw[char_count];
            char_count++;

            // Check for end of row
            if (tmp == '\n') {
                self->rows[current_row][i] = '\0';
                break;
            }
            else if (tmp == '\0' || current_row >= self->rowCount) {
                self->rows[current_row][i] = '\0';

                // Get rid of empty row
                if (self->rows[current_row][0] == '\0') {
                    free_null(&self->rows[current_row]);
                    current_row--;
                }
                next_row = false;
                break;
            }
            else {
                // Assign value to current char in current row
                self->rows[current_row][i] = tmp;

                // Reallocate for next char
                self->rows[current_row] = realloc(self->rows[current_row], sizeof(char)*(i+2));
                if (self->rows[current_row] == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
                i++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            self->rows = realloc(self->rows, sizeof(char*)*(current_row+2));
            if (self->rows == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
            current_row++;
        }
        else break;
    }

    // Free raw data
    free_null(&self->raw);

    return self->rows;
}


//      ___ SPLIT BY ___
node **split_2darray_by(DataLynx *self, char split_by) {

    if (self->rows == NULL) return NULL;

    // Function name (for use in if_error())
    const char *func_name = "split_2daray_by";

    /* THIS FUNCTION: 1. Takes dynamically allocated 2D array
                      2. Returns array of double link lists, separated by whatever char is stored in split (commonly split by ',' for csv-like behavior, but could be anything) */

    /* Note: When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way. */

    // Allocate main array of double link lists
    self->grid = (node**)malloc(sizeof(node*));
    if (self->grid == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Iterate through whole 2D array
    uintmax_t i = 0;
    while (true)
    {
        bool next_row = true;

        // Double linked list main pointers for current row
        node *last = NULL;
        self->grid[i] = NULL;
        char *tmp_str = NULL;

        uintmax_t j = 0;
        uintmax_t ii = 0;

        bool inside_quotes = false;

        // Iterate through each ROW
        while (true) {

            // Allocate 1st char of tmp string
            if (ii == 0) {
                tmp_str = (char*)malloc(sizeof(char));
                if (tmp_str == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}
            }

            tmp_str[ii] = self->rows[i][j];

            if (!inside_quotes && self->rows[i][j] == '"') inside_quotes = true;
            else if (inside_quotes && self->rows[i][j] == '"') inside_quotes = false;

            // Check for end of row
            if (tmp_str[ii] == '\0') {
                build_dblink_list(&tmp_str, &self->grid[i], &last);

                // Check if end of 2d array (this is why row_count is required as a parameter for this function. Otherwise will touch unallocated memory)
                if (i == self->rowCount-1) {next_row = false;}
                break;
            }
            else if (!inside_quotes && split_by == ',' && tmp_str[ii] == split_by) {
                tmp_str[ii] = '\0';
                build_dblink_list(&tmp_str, &self->grid[i], &last);
                ii = 0;
                j++;
            }
            else if (split_by != ',' && tmp_str[ii] == split_by) {
                tmp_str[ii] = '\0';
                build_dblink_list(&tmp_str, &self->grid[i], &last);
                ii = 0;
                j++;
            }
            else {

                // Reallocate for next char
                tmp_str = realloc(tmp_str, sizeof(char)*(ii+2));
                if (tmp_str == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
                j++; ii++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            self->grid = realloc(self->grid, sizeof(node*)*(i+2));
            if (self->grid == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}
            i++;
        }
        else break;
    }


    free_2d_array(self);

    return self->grid;
}


//          ____ GRID_INTO_DICT_GRID() ____
dict_node **grid_into_dict_grid(DataLynx *self) {

    if (self->grid == NULL) return false;

    if (self->dict_grid != NULL) free_dict_grid(self);

    // Function name (for use in if_error())
    const char *func_name = "grid_into_dict";

    dict_node **main_array = (dict_node**)malloc(sizeof(dict_node*));
    if (main_array == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    // Aarray of double link lists (grid)
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        dict_node *last = main_array[i] = NULL;

        node *tmp = self->grid[i];
        uintmax_t j = 0;
        while (tmp != NULL) {

            char *column_name = self->__header__[j];

            if (has_quotes(column_name)) column_name = remove_quotes(self, column_name);

            build_dict_link_list(&tmp->s, &main_array[i], &last, &column_name);

            tmp = tmp->next;
            j++;
        }

        main_array = (dict_node**)realloc(main_array, sizeof(dict_node*)*(i+2));
        if (main_array == NULL) {if_error(REALLOC_FAILED, func_name); return NULL;}



    }

    free_grid(self);
    self->grid = NULL;
    self->dict_grid = main_array;

    return main_array;
}



//          UPDATEFIELD()
bool updateField(DataLynx *self, uintmax_t row, char *column_name, char *new_value) {

    // Safety Checks
    if (self == NULL || column_name == NULL || new_value == NULL) return false;

    // Additional safety check - (must be separate because otherwise if self == NULL, it will still have to check these before returning, which would cause seg fault)
    if (row > self->rowCount-1) return false;

    // Find column index which corresponds to the column name provided as input
    intmax_t column_index = findColumnIndex(self, column_name);

    if (column_index < 0) return false;

    return update_field_internal_(self, row, column_index, new_value);

}


//          updateFieldIdx()
bool updateFieldIdx(DataLynx *self, uintmax_t row, uintmax_t column, char *new_value) {

    // Safety checks
    if (self == NULL || new_value == NULL) return false;

    // More safety checks - (must be separate because otherwise if self == NULL, it will still have to check these before returning, which would cause seg fault)
    if (row > self->rowCount-1 || column > self->columnCount-1) return false;

    return update_field_internal_(self, row, column, new_value);
}


//          UPDATE FIELD INTERNAL
bool update_field_internal_(DataLynx *self, uintmax_t row, uintmax_t column, char *new_value) {

    /* Safety checks done in user-facing functions (updateField()/updateFieldIdx()) */

    if (self->grid_v3 != NULL) return update_grid_v3_index(self, row, column, new_value);
    else if (self->grid != NULL) return update_grid_index(self, row, column, new_value);
    else if (self->dict_grid != NULL) return update_dict_index(self, row, self->__header__[column], new_value);
    else return false;
}


//      ____ UPDATE_GRID_V3_INDEX() ____
bool update_grid_v3_index(DataLynx *self, uintmax_t row, uintmax_t column, char *new_field) {

    /* THIS FUNCTION: Allows you to update specific index in array of linked lists (i.e. grid) */
        // Will return false if no row/column matches

    // These checks will happen in updateField()
    // if (desired_row > self->rowCount-1 || desired_column > self->columnCount-1 || self->grid == NULL) return NULL;

    // Do not update if fields are already the same
    if (strcmp(self->grid_v3[row][column], new_field) == 0) return false;

    const char *func_name = "update_grid_v3_index";


    // Add quotes if new_field has comma(s)
    if (has_comma(new_field) && !has_quotes(new_field)) {
        add_quotes(&new_field);
    }

    // Update stats
    update_stats(self, column, self->grid_v3[row][column], new_field);

    // Create buffer for new string in node
    self->grid_v3[row][column] = (char*)realloc(self->grid_v3[row][column], sizeof(char)*(strlen(new_field)+1));
    if (self->grid_v3[row][column] == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

    strcpy(self->grid_v3[row][column], new_field);

    // Edit CSV file as well if allowed (default is false)
    if (self->csv_write_permission && self->destructive_mode) {

        field_writer_internal_(self, row, column, new_field);
    }

    return true;

}



//      ____ UPDATE_GRID_INDEX() ____
bool update_grid_index(DataLynx *self, uintmax_t desired_row, uintmax_t desired_column, char *new_field) {

    /* THIS FUNCTION: Allows you to update specific index in array of linked lists (i.e. grid) */
        // Will return false if no row/column matches

    // These checks will happen in updateField()
    // if (desired_row > self->rowCount-1 || desired_column > self->columnCount-1 || self->grid == NULL) return NULL;

    const char *func_name = "update_grid_index";


    // Add quotes if new_field has comma(s)
    if (has_comma(new_field) && !has_quotes(new_field)) {
        add_quotes(&new_field);
    }

    // Traverse Doubly linked list to find correct column
    node *cursor = self->grid[desired_row];
    uintmax_t current_column = 0;
    while (cursor != NULL)
    {
        if (current_column == desired_column)
        {

            // Do not update if fields are already the same
            if (strcmp(cursor->s, new_field) == 0) return false;

            // Update stats
            update_stats(self, current_column, cursor->s, new_field);

            // Replace node

            // Create buffer for new string in node
            cursor->s = (char*)realloc(cursor->s, sizeof(char)*strlen(new_field)+1);
            if (cursor->s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

            strcpy(cursor->s, new_field);

            // Edit CSV file as well if allowed (default is false)
            if (self->csv_write_permission && self->destructive_mode) {

                field_writer_internal_(self, desired_row, desired_column, new_field);
            }

            return true;
        }
        cursor = cursor->next;
        current_column++;
    }

    return false;
}


//      _____ UPDATE_DICT_INDEX() _____
bool update_dict_index(DataLynx *self, uintmax_t desired_row, char *desired_column, char *new_field) {

    /* THIS FUNCTION: Allows you to update specific index in array of dicts */
        // Will return false if no column matches desired_column or if row is out of range.

    const char *func_name = "update_dict_index";

    // These checks will happen in updateField()
    // if (desired_row > self->rowCount-1 || self->dict_grid == NULL) return NULL;

    // Add quotes if new_field has comma(s)
    if (has_comma(new_field) && !has_quotes(new_field)) {
        add_quotes(&new_field);
    }

    // Traverse Doubly linked dict list to find correct column
    dict_node *cursor = self->dict_grid[desired_row];
    uintmax_t current_column = 0;
    while (cursor != NULL)
    {

        if (strcmp(cursor->column_name, desired_column) == 0 || (has_quotes(cursor->column_name) && strcmp(remove_quotes(self, cursor->column_name), desired_column) == 0))
        {
            // Do not update if fields are already the same
            if (strcmp(cursor->s, new_field) == 0) return NULL;

            // Cursor is old field at this point
            update_stats(self, current_column, cursor->s, new_field);

            // Replace node

            cursor->s = (char*)realloc(cursor->s, sizeof(char)*strlen(new_field)+1);
            if (cursor->s == NULL) {if_error(REALLOC_FAILED, func_name); return false;}

            strcpy(cursor->s, new_field);

            // Edit CSV file as well if allowed (default is false)
            if (self->csv_write_permission && self->destructive_mode) {

                fieldWriter(self, desired_row, desired_column, new_field);
            }

            return true;
        }
        cursor = cursor->next;
        current_column++;
    }

    return false;
}


//         SORT BY COLUMN()
bool sortRowsByColumn(DataLynx *self, const char *column_name, const char *asc_desc) {

    if (self == NULL || column_name == NULL || asc_desc == NULL) return false;

    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;


    const char *func_name = "sortRowsByColumn";

    bool ascending = (strcasecmp(asc_desc, "desc") == 0 || strcasecmp(asc_desc, "descending") == 0) ? false : true;

    // bool in struct will be set to false by default. These 2 bools are only to make code more readable
    bool case_sensitive = self->case_sensitive_sort;
    bool is_number = self->aggregate[column_index].is_number;

    // Copy column to temp column if not sorting in place (for find_median())
    if (!self->in_place_sort) {

        if (self->tmp_column != NULL) {free(self->tmp_column); self->tmp_column = NULL;}

        // Allocate buffer for temporary column
        self->tmp_column = (char**)malloc(sizeof(char*) * self->rowCount);
        if (self->tmp_column == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        // Copy column
        for (uint8_t i = 0; i < self->rowCount; i++) {
            if (self->grid_v3 != NULL) {
                self->tmp_column[i] = self->grid_v3[i][column_index];
            }
            else if (self->grid != NULL) {
                node *tmp = self->grid[i];
                get_ptr_to_correct_column(column_index, &tmp, NULL);
                self->tmp_column[i] = tmp->s;
            }
            else if (self->dict_grid != NULL) {
                dict_node *tmp = self->dict_grid[i];
                get_ptr_to_correct_column(column_index, NULL, &tmp);
                self->tmp_column[i] = tmp->s;
            }
        }
    }



    // SORT: Iterate through rows
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        // Set up pivot (i.e smallest/largest)
        uintmax_t smallest = i;

        // set up linked list pointers
        node *grid_tmp_piv = NULL, *grid_tmp_j = NULL;
        dict_node *dict_tmp_piv = NULL, *dict_tmp_j = NULL;


        if (self->grid != NULL) {
            grid_tmp_piv = self->grid[i];

            get_ptr_to_correct_column(column_index, &grid_tmp_piv, NULL);
        }
        else if (self->dict_grid != NULL) {
            dict_tmp_piv = self->dict_grid[i];

            get_ptr_to_correct_column(column_index, NULL, &dict_tmp_piv);
        }

        for (uintmax_t j = i+1; j < self->rowCount; j++) {

            // Assign pointers to j
            if (self->grid != NULL) {

                grid_tmp_j = self->grid[j];

                get_ptr_to_correct_column(column_index, &grid_tmp_j, NULL);
            }
            else if (self->dict_grid != NULL) {

                dict_tmp_j = self->dict_grid[j];

                get_ptr_to_correct_column(column_index, NULL, &dict_tmp_j);
            }

            // Compare
            if (!self->in_place_sort) {
                if (compare_for_sort_(self->tmp_column[j], self->tmp_column[smallest], ascending, case_sensitive, is_number)) smallest = j;
            }
            else if (self->grid_v3 != NULL) {
                if (compare_for_sort_(self->grid_v3[j][column_index], self->grid_v3[smallest][column_index], ascending, case_sensitive, is_number)) smallest = j;
            }
            else if (self->grid != NULL) {

                if (compare_for_sort_(grid_tmp_j->s, grid_tmp_piv->s, ascending, case_sensitive, is_number)) {
                    smallest = j;
                    grid_tmp_piv = self->grid[smallest];
                    get_ptr_to_correct_column(column_index, &grid_tmp_piv, NULL);
                }
            }
            else if (self->dict_grid != NULL) {

                if (compare_for_sort_(dict_tmp_j->s, dict_tmp_piv->s, ascending, case_sensitive, is_number)) {
                    smallest = j;
                    dict_tmp_piv = self->dict_grid[smallest];
                    get_ptr_to_correct_column(column_index, NULL, &dict_tmp_piv);
                }
            }


        }

        // Swap
        if (smallest != i) {

            if (!self->in_place_sort) {
                // This if statement must go 1st because otherwise one of the others will always trigger 1st
                char *tmp = self->tmp_column[i];
                self->tmp_column[i] = self->tmp_column[smallest];
                self->tmp_column[smallest] = tmp;
            }
            else if (self->grid_v3 != NULL) {

                char **tmp = self->grid_v3[i];
                self->grid_v3[i] = self->grid_v3[smallest];
                self->grid_v3[smallest] = tmp;

            }
            else if (self->grid != NULL) {

                node *tmp = self->grid[i];
                self->grid[i] = self->grid[smallest];
                self->grid[smallest] = tmp;
            }
            else if (self->dict_grid != NULL) {

                dict_node *tmp = self->dict_grid[i];
                self->dict_grid[i] = self->dict_grid[smallest];
                self->dict_grid[smallest] = tmp;
            }

        }


    }

    return true;

}


//      COMPARE FOR SORT()
bool compare_for_sort_(char *field1, char *field2, bool ascending, bool case_sensitive, bool is_number) {

    // Compare numeric fields - ascending or descending
    if ((is_number && ascending && atof(field1) < atof(field2)) || (is_number && !ascending && atof(field1) > atof(field2))) {
        return true;
    }
    // Compare non-numeric fields - ascending or descending
    else if ((!is_number && ascending && strcmp_quotes(field1, field2, case_sensitive) < 0) || (!is_number && !ascending && strcmp_quotes(field1, field2, case_sensitive) > 0)) {
        return true;
    }

    return false;
}


void get_ptr_to_correct_column(intmax_t correct_column, node **node_ptr, dict_node **dict_ptr) {

    if (correct_column == 0) return;

    // This function traverses a linked list of either nodes or dict_nodes to get to a specific column

    if (node_ptr != NULL) {
        for (intmax_t column = 0; column < correct_column && *node_ptr != NULL; column++) {*node_ptr = (*node_ptr)->next;}
    }
    else if (dict_ptr != NULL) {
        for (intmax_t column = 0; column < correct_column && *dict_ptr != NULL; column++) {*dict_ptr = (*dict_ptr)->next;}
    }

    return;
}


//          STRCMP QUOTES
int strcmp_quotes(const char *s1_input, const char *s2_input, bool case_sensitive) {

    // THIS FUNCTION: Ignores "" around a string when using strcmp()/strcasecmp() (otherwise strings with "" will all be placed together before 'A' regardless of 1st alphabetical letter)

    // Safety checks (strcmp and strcasecmp do not protect against NULL pointers)
    if (s1_input == NULL && s2_input != NULL) return -1;
    else if (s2_input == NULL && s1_input != NULL) return 1;
    else if (s1_input == NULL && s2_input == NULL) return 0;

    const char *func_name = "strcmp_quotes";

    // Declare function  pointer to point to either strcmp() or strcasecmp()
    int (*strcmp_ptr)(const char *s1, const char *s2);
    int func_return;

    // Find string lengths
    size_t s1_len = strlen(s1_input);
    size_t s2_len = strlen(s2_input);

    // Allocate buffers and copy strings (so that I can alter them without Valgrind errors)
    char *s1 = (char*)malloc(sizeof(char)*(s1_len+1));
    if (s1 == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    strcpy(s1, s1_input);

    char *s2 = (char*)malloc(sizeof(char)*(s2_len+1));
    if (s2== NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    strcpy(s2, s2_input);

    // Determine whether to point to strcmp() or strcasecmp()
    strcmp_ptr = (case_sensitive) ? &strcmp : &strcasecmp;

    // Cut off last quote
    if (has_quotes(s1)) s1[s1_len-1] = '\0';

    if (has_quotes(s2)) s2[s2_len-1] = '\0';


    // Compare strings
    if (s1[0] == '"' && s2[0] != '"') func_return = strcmp_ptr(&s1[1], s2);
    else if (s1[0] != '"' && s2[0] == '"') func_return = strcmp_ptr(s1, &s2[1]);
    else if (s1[0] == '"' && s2[0] == '"') func_return = strcmp_ptr(&s1[1], &s2[1]);
    else func_return = strcmp_ptr(s1, s2);

    // Free allocated buffers
    free_null(&s1);
    free_null(&s2);

    return func_return;
}






bool printHead(DataLynx *self, uintmax_t number_of_rows) {
    if (self == NULL) return NULL;

    if (number_of_rows >= self->rowCount) return print_data_internal(self);

    self->number_of_rows_to_print = number_of_rows;

    print_data_internal(self);

    if (!self->print_truncated_rows)
        printf("\t... (rows %ld-%ld not displayed)\n\n", self->number_of_rows_to_print, self->rowCount-1);

    self->number_of_rows_to_print = self->rowCount;
    return true;

}

bool printTail(DataLynx *self, uintmax_t number_of_rows) {
    if (self == NULL) return NULL;

    if (number_of_rows >= self->rowCount) return print_data_internal(self);

    self->number_of_rows_to_print = number_of_rows;
    self->print_tail = true;

    print_data_internal(self);

    self->number_of_rows_to_print = self->rowCount;
    self->print_tail = false;
    return true;

}

bool printShape(DataLynx *self) {

    if (self == NULL) return false;

    printf("\nRows: %ld\n", self->rowCount);
    printf("Columns: %ld\n\n", self->columnCount);

    return true;
}

bool printColumn(DataLynx *self, char *column_name) {

    if (self == NULL || column_name == NULL)  return false;

    int16_t column_index = findColumnIndex(self, column_name);

    if (column_index < 0) return false;

    if (self->printRowIndex) {

        uint8_t buffer_size = 10;
        char buffer[buffer_size];
        uint8_t x;
        for (x = 0; x < self->max_row_digits+1; x++)  {
            if (x < buffer_size-2) buffer[x] = ' ';
            else break;
        }

        buffer[x] = '\0';

        printf("%s", buffer);
    }

    printf("'%s' <column %d from dataset: %s>\n", self->__header__[column_index], column_index, self->filename);

    if (self->grid_v3 != NULL) {
        for (uintmax_t row = 0; row < self->rowCount; row++) {

            // Print row Index or not
            if (self->printRowIndex)
                printf("%*ld: ", self->max_row_digits, row);

            if (self->grid_v3[row][column_index][0] == '\0') printf("%s\n", self->missingValue);
            else row == self->rowCount-1 ? printf("%s\n\n", self->grid_v3[row][column_index]) : printf("%s\n", self->grid_v3[row][column_index]);
        }
    }

    return true;

}

//      NOT CENTERED
bool printDataTable(DataLynx *self) {
    // TO DO scientifi notation, all data structure
    printf("\t<DATA SQL STYLE>\n\n");
    uint8_t column_lengths[self->columnCount]; /* This can be 8-bits instead of size_t, bc we will not store anything in it greater than self->maxFieldPrintLength */
    size_t head_strlens[self->columnCount];
    uint8_t padding = 2;


    // Go through ever column and remember what the longest string length
    for (uint32_t column = 0; column < self->columnCount; column++) {

        size_t head_strlen = strlen(self->__header__[column]);

        if ((head_strlen + padding >= self->maxFieldPrintLength) || (self->aggregate[column].longest_field_strlen + padding >= self->maxFieldPrintLength)) column_lengths[column] = self->maxFieldPrintLength;
        else if (head_strlen + padding > self->aggregate[column].longest_field_strlen + padding) column_lengths[column] = head_strlen + padding;
        else column_lengths[column] = self->aggregate[column].longest_field_strlen + padding;

        head_strlens[column] = head_strlen;
        // printf("c: %d, c_length: %d, head_strlen: %ld\n", column, column_lengths[column], head_strlens[column]);

    }

    char spaces[self->maxFieldPrintLength];
    char truncated[self->maxFieldPrintLength+1];
    int8_t num_spaces = 0;
    bool odd_even_mismatch = false;

    char *field = NULL;
    node *node_tmp = NULL;
    dict_node *dict_tmp = NULL;
    intmax_t column_count = (intmax_t)self->columnCount;

    // Print
    // for (double row = -1.5; row < (intmax_t) self->rowCount; row += 0.5) {
    for (intmax_t row = -3; row < (intmax_t) self->rowCount+1; row++) {

        for (intmax_t column = -1; column < column_count; column++) {

            // signed long row_truncated = row;
            // double row_decimal = row - row_truncated;

            if (row == -2) {

                if (column < 0 && self->printRowIndex) {
                    create_spaces_string(spaces, self->max_row_digits+3, false, ' ');
                    printf("%s", spaces);
                    continue;
                }
                else if (column < 0) continue;

                // -- Print Header --

                // Calc spaces to print
                num_spaces = (column_lengths[column]-head_strlens[column]) / 2;

                if (num_spaces < 0) {

                    char tmp = truncate_with_ellipses(self->__header__[column], truncated, self->maxFieldPrintLength-1);

                    (column == 0) ? printf("|%s|", truncated) : printf("%s|", truncated);

                    self->__header__[column][self->maxFieldPrintLength-3] = tmp;

                    continue;
                }


                if (((size_t)column_lengths[column]) & 1 && !(head_strlens[column] & 1)) odd_even_mismatch = true;
                else if (!(((size_t)column_lengths[column]) & 1) && head_strlens[column] & 1) odd_even_mismatch = true;
                else odd_even_mismatch = false;

                create_spaces_string(spaces, num_spaces+1, false, ' ');

                if (column == 0) {
                    (odd_even_mismatch) ? printf("| %s%s%s|", spaces, self->__header__[column], spaces)  : printf("|%s%s%s|", spaces, self->__header__[column], spaces);
                }
                else {
                    (odd_even_mismatch) ? printf(" %s%s%s|", spaces, self->__header__[column], spaces)  : printf("%s%s%s|", spaces, self->__header__[column], spaces);
                }

                continue;
            }
            // else if (row_decimal == 0.5 || row_decimal == -0.5) {
            else if (row < 0 || row == (intmax_t) self->rowCount) {

                // Dashes
                if (column < 0 && !self->printRowIndex) continue;
                else if (column < 0 && self->printRowIndex) {
                    create_spaces_string(spaces, self->max_row_digits+3, false, ' ');
                    printf("%s", spaces);
                }
                else {

                    create_spaces_string(spaces, column_lengths[column]+1, false, '-');
                    (column == 0) ? printf("+%s+", spaces) : printf("%s+", spaces);
                }

                continue;

            }

            if (column < 0) {

                 if (self->printRowIndex) printf("%*ld: ", self->max_row_digits, row);

                continue;

            }


            // Get current field
            if (self->grid_v3 != NULL) field = self->grid_v3[(uint32_t)row][column];
            else if (self->grid != NULL) {
                if (column == 0) node_tmp = self->grid[(uint32_t)row];
                field = node_tmp->s;
            }
            else if (self->dict_grid != NULL) {
                if (column == 0) dict_tmp = self->dict_grid[(uint32_t)row];
                field = dict_tmp->s;
            }

            if (field[0] == '\0') field = self->missingValue;

            // Remove quotes (non-destructively) if field has quotes
            if (has_quotes(field)) field = remove_quotes(self, field);

            size_t field_strlen = strlen(field);

            num_spaces = (column_lengths[column]-field_strlen);

            if (num_spaces <= 0) {

                // self->maxFieldPrintLength - 2 to make room for at least 1 space before & after field value
                char tmp = truncate_with_ellipses(field, truncated, self->maxFieldPrintLength-2);

                (column == 0) ? printf("| %s |", truncated) : printf(" %s |", truncated);

                // Replace char
                field[self->maxFieldPrintLength-3] = tmp;

                continue;
            }


            // Fill spaces string
            create_spaces_string(spaces, num_spaces, false, ' ');

            // Print field
            (column == 0) ? printf("| %s%s|", field, spaces) : printf(" %s%s|", field, spaces);

            if (self->grid != NULL) node_tmp = node_tmp->next;
            else if (self->dict_grid != NULL) dict_tmp = dict_tmp->next;

        }
        printf("\n");
    }

    return true;
}

//           if (column_lengths[column] != self->maxFieldPrintLength) {
//                     if (((size_t)column_lengths[column]-padding) & 1 && !(head_strlens[column] & 1)) odd_even_mismatch = true;
//                     else if (!(((size_t)column_lengths[column]-padding) & 1) && head_strlens[column] & 1) odd_even_mismatch = true;
//                 }
//                 else {
//                     if ((size_t)column_lengths[column] & 1 && !(head_strlens[column] & 1)) odd_even_mismatch = true;
//                     else if (!((size_t)column_lengths[column] & 1) && head_strlens[column] & 1) odd_even_mismatch = true;
//                 }


//          LATEST VERSION CENTERED
// bool printDataTable(DataLynx *self) {

//     printf("\t<DATA SQL STYLE>\n\n");
//     uint8_t column_lengths[self->columnCount]; /* This can be 8-bits instead of size_t, bc we will not store anything in it greater than self->maxFieldPrintLength */
//     size_t head_strlens[self->columnCount];
//     uint8_t padding = 2;


//     // Go through ever column and remember what the longest string length
//     for (uint32_t column = 0; column < self->columnCount; column++) {

//         size_t head_strlen = strlen(self->__header__[column]);

//         if ((head_strlen + padding > self->maxFieldPrintLength) || (self->aggregate[column].longest_field_strlen + padding > self->maxFieldPrintLength)) column_lengths[column] = self->maxFieldPrintLength;
//         else if (head_strlen + padding > self->aggregate[column].longest_field_strlen + padding) column_lengths[column] = head_strlen + padding;
//         else column_lengths[column] = self->aggregate[column].longest_field_strlen + padding;

//         head_strlens[column] = head_strlen;
//         printf("c: %d, c_length: %d, head_strlen: %ld\n", column, column_lengths[column], head_strlens[column]);
//     }

//     char spaces[self->maxFieldPrintLength];
//     char truncated[self->maxFieldPrintLength+1];
//     int8_t num_spaces = 0;

//     // Print
//     // for (double row = -1.5; row < (intmax_t) self->rowCount; row += 0.5) {
//     for (intmax_t row = -3; row < (intmax_t) self->rowCount+1; row++) {

//         for (uint32_t column = 0; column < self->columnCount; column++) {

//             // signed long row_truncated = row;
//             // double row_decimal = row - row_truncated;

//             if (row == -2) {
//                 // Print Header
//                 num_spaces = (column_lengths[column]-head_strlens[column]) / 2;

//                 if (num_spaces < 0) {

//                     char tmp = truncate_with_ellipses(self->__header__[column], truncated, self->maxFieldPrintLength);

//                     (column == 0) ? printf("|%s|", truncated) : printf("%s|", truncated);

//                     self->__header__[column][self->maxFieldPrintLength-3] = tmp;

//                     continue;
//                 }

//                 create_spaces_string(spaces, num_spaces+1, false, ' ');

//                 // (column == 0) ? printf("|%s%s%s|", spaces, self->__header__[column], spaces) : printf("%s%s%s|", spaces, self->__header__[column], spaces);
//                 if (column == 0) printf("|");
//                 (head_strlens[column] & 1 && head_strlens[column] != (size_t)column_lengths[column]-padding) ? printf("%s%s%s |", spaces, self->__header__[column], spaces) : printf("%s%s%s|", spaces, self->__header__[column], spaces);
//                 continue;
//             }
//             // else if (row_decimal == 0.5 || row_decimal == -0.5) {
//             else if (row < 0 || row == (intmax_t) self->rowCount) {
//                 // Dashes
//                 create_spaces_string(spaces, column_lengths[column]+1, false, '-');
//                 (column == 0) ? printf("+%s+", spaces) : printf("%s+", spaces);

//                 continue;
//             }

//             // Get current field
//             char *field = NULL;
//             if (self->grid_v3 != NULL) field = self->grid_v3[(uint32_t)row][column];

//             if (field[0] == '\0') field = self->missingValue;

//             size_t field_strlen = strlen(field);

//             num_spaces = (column_lengths[column]-field_strlen) / 2;

//             if (num_spaces < 0) {

//                 char tmp = truncate_with_ellipses(field, truncated, self->maxFieldPrintLength);

//                 (column == 0) ? printf("|%s|", truncated) : printf("%s|", truncated);

//                 // Replace char
//                 field[self->maxFieldPrintLength-3] = tmp;

//                 continue;
//             }


//             (num_spaces == 0) ? create_spaces_string(spaces, num_spaces+2, false, ' ') : create_spaces_string(spaces, num_spaces+1, false, ' ');

//             if (column == 0) printf("|");

//             // printf("%s%s%s|", spaces, field, spaces);
//             ((field_strlen & 1 && (field_strlen != head_strlens[column] || field_strlen != (size_t)column_lengths[column]-padding))) ? printf("%s%s%s |", spaces, field, spaces) : printf("%s%s%s|", spaces, field, spaces);

//         }
//         printf("\n");
//     }

//     return true;
// }

// bool printDataTable(DataLynx *self) {

//     printf("\t<DATA SQL STYLE>\n\n");
//     uint8_t column_lengths[self->columnCount]; /* This can be 8-bits instead of size_t, bc we will not store anything in it greater than self->maxFieldPrintLength */
//     size_t head_strlens[self->columnCount];
//     uint8_t padding = 2;


//     // Go through ever column and remember what the longest string length
//     for (uint32_t column = 0; column < self->columnCount; column++) {

//         size_t head_strlen = strlen(self->__header__[column]);

//         if ((head_strlen + padding > self->maxFieldPrintLength) && (self->aggregate[column].longest_field_strlen + padding > self->maxFieldPrintLength)) column_lengths[column] = self->maxFieldPrintLength;
//         else if (head_strlen + padding > self->aggregate[column].longest_field_strlen + padding) column_lengths[column] = head_strlen + padding;
//         else column_lengths[column] = self->aggregate[column].longest_field_strlen + padding;

//         head_strlens[column] = head_strlen;
//         printf("c: %d, column_length:%d, head_strlen:%ld", column, column_lengths[column], head_strlens[column]);
//     }

//     char spaces[self->maxFieldPrintLength];
//     uint8_t num_spaces = 0;
//     char print_string[self->maxFieldPrintLength];

//     // DOES NOT WORK IF field_strlen > self->maxFieldPrintLength
//     // Print
//     // for (double row = -1.5; row < (intmax_t) self->rowCount; row += 0.5) {
//     for (intmax_t row = -3; row < (intmax_t) self->rowCount+1; row++) {

//         for (uint32_t column = 0; column < self->columnCount; column++) {

//             // signed long row_truncated = row;
//             // double row_decimal = row - row_truncated;

//             if (row == -2) {

//                 // Print Header
//                 // printf("%d, %d, %ld", column, column_lengths[column], head_strlens[column]);
//                 num_spaces = (column_lengths[column]-head_strlens[column]) / 2;
//                 // printf("!0");
//                 create_spaces_string(spaces, num_spaces, false, ' ');
//                 // printf("%s", spaces);

//                 (column == 0) ? printf("|%s%s%s|", spaces, self->__header__[column], spaces) : printf("%s%s%s|", spaces, self->__header__[column], spaces);
//                 continue;
//             }
//             // else if (row_decimal != 0) {
//             else if (row < 0 || row == (intmax_t) self->rowCount) {
//                 // Dashes (before/after header and after all data)

//                 create_spaces_string(spaces, column_lengths[column]-1, false, '-');
//                 (column == 0) ? printf("+%s+", spaces) : printf("%s+", spaces);

//                 continue;
//             }

//             // Get current field
//             char *field = NULL;
//             if (self->grid_v3 != NULL) field = self->grid_v3[(uint32_t)row][column];

//             if (field[0] == '\0') field = self->missingValue;

//             size_t field_strlen = strlen(field);

//             // if (field_strlen > column_lengths[column]) {
//             //     char replaced_char = field[column_lengths[column]-1-2];
//             //     field[column_lengths[column]-1-2] = '\0';
//             //     sprintf(print_string, "%s", field);

//             //     field[column_lengths[column]-1-2] = replaced_char;
//             //     printf("%s|", print_string);
//             //     continue;
//             // }

//             num_spaces = (column_lengths[column]-field_strlen) / 2;

//             create_spaces_string(spaces, num_spaces, false, ' ');

//             if (column == 0) printf("|");

//             (field_strlen & 1 && field_strlen != (size_t)(column_lengths[column]-padding) && head_strlens[column] & 1) ? printf("%s%s%s\b|", spaces, field, spaces) : printf("%s%s%s|", spaces, field, spaces);

//         }
//         printf("\n");
//     }

//     return true;
// }

// bool printDataTable(DataLynx *self) {

//     printf("\t<DATA SQL STYLE>\n\n");
//     uint8_t column_lengths[self->columnCount]; /* This can be 8-bits instead of size_t, bc we will not store anything in it greater than self->maxFieldPrintLength */
//     size_t head_strlens[self->columnCount];
//     uint8_t padding = 5;


//     // Go through ever column and remember what the longest string length
//     for (uint32_t column = 0; column < self->columnCount; column++) {

//         size_t head_strlen = strlen(self->__header__[column]);

//         if ((head_strlen + padding > self->maxFieldPrintLength) && (self->aggregate[column].longest_field_strlen + padding > self->maxFieldPrintLength)) column_lengths[column] = self->maxFieldPrintLength;
//         else if (head_strlen > self->aggregate[column].longest_field_strlen) column_lengths[column] = head_strlen + padding;
//         else column_lengths[column] = self->aggregate[column].longest_field_strlen + padding;

//         head_strlens[column] = head_strlen;

//     }

//     char spaces[self->maxFieldPrintLength];
//     uint8_t num_spaces = 0;
//     char print_string[self->maxFieldPrintLength];

//     // DOES NOT WORK IF field_strlen > self->maxFieldPrintLength
//     // Print
//     // for (double row = -1.5; row < (intmax_t) self->rowCount; row += 0.5) {
//     for (intmax_t row = -3; row < (intmax_t) self->rowCount+1; row++) {

//         for (uint32_t column = 0; column < self->columnCount; column++) {

//             // signed long row_truncated = row;
//             // double row_decimal = row - row_truncated;

//             if (row == -2) {

//                 // Print Header

//                 // if (head_strlens[column] > (size_t)column_lengths[column]-2) {
//                 //     // printf("too big head");
//                 //     char replaced_char = self->__header__[column][column_lengths[column]-1-2];
//                 //     self->__header__[column][column_lengths[column]-1-2] = '\0';
//                 //     sprintf(print_string, "%s", self->__header__[column]);

//                 //     self->__header__[column][column_lengths[column]-1-2] = replaced_char;
//                 //     printf("%s|", print_string);
//                 //     continue;
//                 // }

//                 num_spaces = (column_lengths[column]-head_strlens[column]) / 2;

//                 create_spaces_string(spaces, num_spaces, false, ' ');

//                 (column == 0) ? printf("|%s%s%s|", spaces, self->__header__[column], spaces) : printf("%s%s%s|", spaces, self->__header__[column], spaces);
//                 continue;
//             }
//             // else if (row_decimal != 0) {
//             else if (row < 0 || row == (intmax_t) self->rowCount) {
//                 // Dashes (before/after header and after all data)

//                 create_spaces_string(spaces, column_lengths[column]-2, false, '-');
//                 (column == 0) ? printf("+%s+", spaces) : printf("%s+", spaces);

//                 continue;
//             }

//             // Get current field
//             char *field = NULL;
//             if (self->grid_v3 != NULL) field = self->grid_v3[(uint32_t)row][column];

//             if (field[0] == '\0') field = self->missingValue;

//             size_t field_strlen = strlen(field);

//             // if (field_strlen > column_lengths[column]) {
//             //     char replaced_char = field[column_lengths[column]-1-2];
//             //     field[column_lengths[column]-1-2] = '\0';
//             //     sprintf(print_string, "%s", field);

//             //     field[column_lengths[column]-1-2] = replaced_char;
//             //     printf("%s|", print_string);
//             //     continue;
//             // }

//             num_spaces = (column_lengths[column]-field_strlen) / 2;

//             create_spaces_string(spaces, num_spaces, false, ' ');

//             if (column == 0) printf("|");

//             (field_strlen & 1 && field_strlen != (size_t)(column_lengths[column]-padding) && head_strlens[column] & 1) ? printf("%s%s%s\b|", spaces, field, spaces) : printf("%s%s%s|", spaces, field, spaces);

//         }
//         printf("\n");
//     }

//     return true;
// }

bool printData(DataLynx *self) {

    if (self == NULL) return false;

    if (self->rowCount > self->maxPrintRows) {
        self->print_truncated_rows = true;
        printHeader(self);
        printHead(self, 10);
        printf("\n\t... (rows 10 - %ld not displayed) ...\n\n", self->rowCount-10-1);
        printTail(self, 10);
        self->print_truncated_rows = false;
        return true;
    }

    return print_data_internal(self);

}

// ___ PRINT_HEADER() ___
bool printHeader(DataLynx *self)
{

    if (self->__header__ == NULL) return false;

    /* THIS FUNCTION: Makes printing the header of the csv easier and tightens up the code of the calling function. */
    // This is not a method, so input is not named self

    printf("\n");
    if (self->printRowIndex) {
        for (uint8_t x = 0; x < self->max_row_digits+3; x++) x == self->max_row_digits+2 ? printf("[") : printf(" ");
    }

    char space[] = " ";

    if (!self->with_spaces) space[0] = '\0';

    for (uintmax_t i = 0; i < self->columnCount; i++) {

        // Print with comma if not the last column
        if (i < self->columnCount-1) printf("%s,%s", self->__header__[i], space);
        else printf("%s]\n", self->__header__[i]);
    }

    return true;
}


//  ______ PRINT_DATA() ______
bool print_data_internal(DataLynx *self) {
    // Print file name
    printf("\n\t<Data from file: %s>\n", self->filename);

    // Header
    if (self->skip_header && self->__header__ != NULL && !self->print_truncated_rows) {
        printHeader(self);
    }


    if (self->print_tail && !self->print_truncated_rows)
        printf("\t... (rows 0-%ld not displayed)\n", self->rowCount-self->number_of_rows_to_print-1);


    // Data
    if (self->raw != NULL) {
        printf("%s", self->raw);
        return true;
    }

    if (self->rows != NULL) {

        // Print array of strings (rows)
        uint64_t current_row = 0;

        // Adjust varibales for tail
        if (self->print_tail) {
            current_row = self->rowCount - self->number_of_rows_to_print;
            self->number_of_rows_to_print = self->rowCount;
        }

        while (current_row < self->number_of_rows_to_print) {

            // Print row Index or not
            if (self->printRowIndex)
                printf("%*ld: ", self->max_row_digits, current_row);

            current_row == self->rowCount-1 ? printf("%s\n\n", self->rows[current_row]) : printf("%s\n", self->rows[current_row]);
            current_row++;
        }

        return true;
    }

    if (self->grid_v3 != NULL) {

        print_grid_v3(self);

        return true;
    }

    if (self->grid != NULL) {

        // Print array of db link list (grid)
        print_grid(self);

        return true;

    }

    if (self->dict_grid != NULL) {

        // Print array of db link list (dict grid)
        print_dict_grid(self);
        return true;

    }

    return false;
}




void print_grid_v3(DataLynx *self) {

    // Print v3
    uint64_t i = 0;

    // Adjust variables for tail
    if (self->print_tail) {
        i = self->rowCount - self->number_of_rows_to_print;
        self->number_of_rows_to_print = self->rowCount;
    }

    while (i < self->number_of_rows_to_print) {

        // Print row Index or not
        if (self->printRowIndex)
            printf("%*ld: ", self->max_row_digits, i);

        printf("[");
        for (uintmax_t j = 0; j < self->columnCount; j++) {

            if (self->grid_v3[i][j][0] == '\0') printf("%s", self->missingValue);
            else if (has_quotes(self->grid_v3[i][j])) printf("%s", self->grid_v3[i][j]);
            else printf("'%s'", self->grid_v3[i][j]);

            if (j != self->columnCount-1) printf(", ");
        }
        // i == self->rowCount-1 ? printf("]\n\n") :printf("]\n");
        printf("]\n");
        i++;

    }

    printShape(self);

    return;
}


//      ___PRINT_GRID() ___
void print_grid(DataLynx *self) {

    // Print array of double link lists (grid)

    uint64_t current_row = 0;

    // Adjust variables for tail
    if (self->print_tail) {
        current_row = self->rowCount - self->number_of_rows_to_print;
        self->number_of_rows_to_print = self->rowCount;
    }

    // Iterate through each row
    while (current_row < self->number_of_rows_to_print) {
        print_lnk_list(self, self->grid[current_row], current_row);
        current_row++;
    }

    printShape(self);

    return;
}

//      ___ PRINT_LINK_LIST ___
bool print_lnk_list(DataLynx *self, node *head, uintmax_t current_row)
{
    // if (self == NULL) return false;
    if (head == NULL) return false;

    // if (self->missingValue == NULL) changeMissingValue(self, MISSING_VALUE);
    /* THIS FUNCTION: Prints the string in each node of a standard doubly linked list */


    // Print row Index or not
    if (self->printRowIndex)
        printf("%*ld: ", self->max_row_digits, current_row);

    node *tmp = head;
    printf("[");
    while (tmp != NULL) {

        // Dont print single quotes if field has double quotes
        if (tmp->s[0] == '\0') printf("'%s'", self->missingValue);
        else if (!has_quotes(tmp->s)) printf("'%s'", tmp->s);
        else printf("%s", tmp->s);

        // Print comma & space after field if not end of row
        if (tmp->next != NULL) printf(", ");

        tmp = tmp->next;
    }
    // current_row == self->rowCount-1 ? printf("]\n\n") : printf("]\n");
    printf("]\n");

    return true;
}


// ____ PRINT_DICT_LIST() ____
bool print_dict_grid(DataLynx *self)
{
    if (self == NULL) return false;
    if (self->dict_grid == NULL) return  false;
    // if (self->missingValue == NULL) changeMissingValue(self, MISSING_VALUE);
    // PRINT DICT LIST
    /* THIS FUNCTION: Prints the string in each dict_node of a doubly linked list acting as a DICT. Prints the index (i.e. integer row, string column name in addition to cell value string) */

    uintmax_t current_row = 0;

    // Adjust variables for tail
    if (self->print_tail) {
        current_row = self->rowCount - self->number_of_rows_to_print;
        self->number_of_rows_to_print = self->rowCount;
    }

    // Iterate through every row
    while (current_row < self->number_of_rows_to_print) {

        // Traverse linked list
        dict_node *tmp = self->dict_grid[current_row];
        char *column_name = NULL;

        // Print row Index or not
        if (self->printRowIndex)
            printf("%*ld: ", self->max_row_digits, current_row);

        printf("{");
        while (tmp != NULL) {

            column_name = has_quotes(tmp->column_name) ? remove_quotes(self, tmp->column_name) : tmp->column_name;

            if (tmp->s[0] != '\0')
                printf("'%s': %s", column_name, tmp->s);
            else
                printf("'%s': %s", column_name, self->missingValue);

            if (tmp->next != NULL) printf(", ");

            tmp = tmp->next;
        }

        // current_row == self->rowCount-1 ? printf("}\n\n") : printf("}\n");
        printf("}\n");
        current_row++;
    }

    printShape(self);

    return true;
}


bool print_dict_grid2(DataLynx *self)
{
    if (self == NULL) return false;
    if (self->dict_grid == NULL) return  false;

    // PRINT DICT LIST
    /* THIS FUNCTION: Prints the string in each dict_node of a doubly linked list acting as a DICT. Prints the index (i.e. integer row, string column name in addition to cell value string) */

    // Iterate through every row
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        // Traverse linked list
        dict_node *tmp = self->dict_grid[i];
        char *column_name = NULL;
        while (tmp != NULL) {

            column_name = has_quotes(tmp->column_name) ? remove_quotes(self, tmp->column_name) : tmp->column_name;

            if (tmp->s[0] != '\0')
                printf("Index[%lu]['%s']: %s\n", i, column_name, tmp->s);
            else
                printf("Index[%lu]['%s']: %s\n", i, column_name, self->missingValue);
            tmp = tmp->next;
        }

        printf("\n");
    }

    return true;
}




bool printStatsAll(DataLynx *self) {

    if (self == NULL) return false;

    if (self->aggregate == NULL) return false;

    return print_stats_internal_(self, PRINT_ALL_STATS);

}


bool printStatsColumn(DataLynx *self, char *column_name) {

    if (self == NULL || column_name == NULL) return false;

    if (self->aggregate == NULL) return false;

    return print_stats_internal_(self, column_name);

}


        // PRINT STATS INTERNAL() (side by side) and Sorted Value Counts Works :)
bool print_stats_internal_(DataLynx *self, char *column_name) {

    // if (self == NULL) return false;

    // if (self->aggregate == NULL || column_name == NULL) return false;

    char *func_name = "print_stats_internal_";

    // Determine whether to display all columns or just one (i.e. this does not work IF csv has a column named "All")
    int16_t c = 0, start = 0, stop = self->columnCount;
    bool print_one_column = false;
    if (strcmp(column_name, PRINT_ALL_STATS) != 0) {

        // Adjust start and stop to only print desired column
        if ((c = findColumnIndex(self, column_name)) >= 0) stop = c + 1;
        // else c = 0; /* if an invalid column name is given, we will print all stats */
        else return false;

        print_one_column = true;

        start = c;

    }

    // Buffer to remember columns for value counts
    uint16_t non_numeric_columns[self->columnCount];
    uint16_t non_numeric_column_count = 0;

    printf("\n\t\t<Data from file: %s>\n", self->filename);

    if (!print_one_column || (print_one_column && self->aggregate[c].is_number) ) printf("\n\t\t- Numeric Stats - \n");

    const int8_t STAT_COUNT = 10; /* This is the number of numeric stats we are printing per column
                                    (i.e. Min, Max, Sum, Mean, 25th %, Median, 75th %, Std,
                                    as well as Is Null and Not Null, which are also printed for non-numeric stats aka Value Counts) */


    char space = ' ';
    bool numeric_stats_in_data = false;

    // For each stat: iterate through columns to find numeric columns
    for (int8_t stat = -1; stat < STAT_COUNT; stat++) {

        c = start;
        // Iterate through columns to find numeric columns
        while (c < stop) {

            uint8_t column_strlen = strlen(self->__header__[c]) + 23; /* stat_print_() needs this to give proper spacing (to make the column stats line up nicely)*/

            // Found numeric column
            if (self->aggregate[c].is_number) {

                numeric_stats_in_data = true;

                // Print stats
                if (stat == -1) {
                    !has_quotes(self->__header__[c]) ? printf("'%s' <Aggregate Data>%3c", self->__header__[c], space) : printf("%s <Aggregate Data>%3c", self->__header__[c], space);
                }
                else if (stat == 0) {
                    stat_print_("Min", self->aggregate[c].min, column_strlen);
                }
                else if (stat == 1){
                    stat_print_("Max", self->aggregate[c].max, column_strlen);
                }
                else if (stat == 2){
                    stat_print_("Sum", self->aggregate[c].sum, column_strlen);
                }
                else if (stat == 3){
                    stat_print_("Mean", self->aggregate[c].mean, column_strlen);
                }
                else if (stat == 4){
                    stat_print_("25th %", self->aggregate[c].lower_qrt, column_strlen);
                }
                else if (stat == 5){
                    stat_print_("Median", self->aggregate[c].median, column_strlen);
                }
                else if (stat == 6){
                    stat_print_("75th %", self->aggregate[c].upper_qrt, column_strlen);
                }
                else if (stat == 7){
                    stat_print_("Std", self->aggregate[c].std, column_strlen);
                }
                else if (stat == 8){
                    stat_print_("Is Null", (double)self->aggregate[c].is_null, column_strlen);
                }
                else if (stat == 9){
                    stat_print_("Not Null", (double)self->aggregate[c].not_null, column_strlen);
                }

            }
            else if (stat == -1) {
                /* functionally the same as saying else if (stat == -1 && !self->aggregate[c].is_number) */
                /* Only do this one time around, otherwise will count columns multiple times.*/

                // Remember which column indexes are non-numeric, so we do not have to iterate through each column and check again when we print the value counts
                non_numeric_columns[non_numeric_column_count] = c;
                non_numeric_column_count++;
            }

            c++;
        }

        /* Without this if statement, it will print new lines for every stat as it looks for numeric columns even when no numeric columns exist*/
        if (numeric_stats_in_data) {
            (stat == STAT_COUNT-1) ? printf("\n\n"): printf("\n");
        }


    }

    if (!numeric_stats_in_data && !print_one_column) printf("\n* No numeric stats for this dataset *\n\n\n");
    else if (!numeric_stats_in_data && print_one_column) printf("\n* No numeric stats for this column *\n\n\n");


    //              --- COPY VALUE COUNTS INTO SEPARATE ARRAYS TO SORT ---

    // Allocate main sorted array (i.e. array of arrays of sorted value counts per column)
    ValueCount **sorted = (ValueCount**)malloc(sizeof(ValueCount*) * non_numeric_column_count);
    if (sorted == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    // Array to keep track of unique values for each non-numeric column
    uint16_t *unique_values = (uint16_t*)malloc(sizeof(uint16_t) * non_numeric_column_count);
    if (unique_values == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    uint16_t most_unique_values = 0;

    // Put Value Counts for current column into separate array
    for (c = 0; c < non_numeric_column_count; c++) {

        uint32_t column = non_numeric_columns[c];

        // Allocate array for sorted value counts
        uint32_t current_unique_values = 0;

        // Allocate array for sorted value counts of current column
        ValueCount *current_sorted = (ValueCount*)malloc(sizeof(ValueCount));
        if (current_sorted == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

        bool first_value = true, values_exist = false;

        // Iterate through alpha value count array
        for (int l = 0; l < 27; l++) {
            ValueCount *tmp = self->aggregate[column].value_counts[l];

            // Traverse linked list for current alpha character
            while (tmp != NULL) {

                values_exist = true;

                //  Realloc sorted array
                if (!first_value) {
                    current_sorted = (ValueCount*)realloc(current_sorted, sizeof(ValueCount) * ((++current_unique_values) + 1));
                    if (current_sorted == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
                }
                else first_value = false;

                // Assign to sorted array to be sorted in next section
                current_sorted[current_unique_values].value = tmp->value;
                current_sorted[current_unique_values].count = tmp->count;

                tmp = tmp->next;

            }

        }

        // Keep track of unique values for current non-numeric column
        unique_values[c] = (values_exist) ? ++current_unique_values : 0;

        if (current_unique_values > most_unique_values) most_unique_values = current_unique_values;

        // Sort current column's value counts
        for (uint32_t i = 0; i < current_unique_values; i++) {

            uint32_t largest = i;

            for (uint32_t j = i + 1; j < current_unique_values; j++) {

                if (current_sorted[j].count > current_sorted[largest].count) largest = j;
            }

            // Swap
            if (i != largest) {
                ValueCount tmp = current_sorted[i];
                current_sorted[i] = current_sorted[largest];
                current_sorted[largest] = tmp;
            }
        }

        // Attach array of sorted value counts for current column to array of arrays
        sorted[c] = current_sorted;

    }

    //          --- PRINT VALUE COUNTS ---
    if (non_numeric_column_count == 0) return true;
    printf("\t\t- Value Counts -\n\t   (i.e. non-numeric stats)\n");

    bool with_divider = true;

    for (int32_t v = -2; v < most_unique_values+4; v++) {

        for (uint32_t c = 0; c < non_numeric_column_count; c++) {

            uint32_t column_index = non_numeric_columns[c];

            // Column string length sets the length for that column
            size_t column_strlen = strlen(self->__header__[column_index]) + 31;

            // Print column name
            if (v == -2) {
                has_quotes(self->__header__[c]) ? printf("%s <Aggregate Data>%10c| ", self->__header__[column_index], space) : printf("'%s' <Aggregate Data>%10c| ", self->__header__[column_index], space);
                continue;
            }
            else if (v == -1 || v == most_unique_values) {
                // This create a line break after column head / after values are printed, before unique values is printed
                char dashes[column_strlen+1];
                char char_to_print = (v == -1) ? '-' : ' ';
                create_spaces_string(dashes, column_strlen+1, with_divider, char_to_print);
                if (c == (uint32_t)non_numeric_column_count-1) dashes[column_strlen-1] = '\0';
                printf("%s", dashes);

                continue;
            }
            else if (v == most_unique_values+1) {
                // Print Unique Values
                char unique_string[32];
                sprintf(unique_string, "Unique values: %u", unique_values[c]);

                int8_t num_spaces = column_strlen - strlen(unique_string);
                char spaces[num_spaces+1];
                create_spaces_string(spaces, num_spaces+1, with_divider, ' ');

                printf("%s%s", unique_string, spaces);
                continue;
            }
            else if (v == most_unique_values+2) {
                // Print Is Null
                bool is_null = true;
                uint32_t column_index = non_numeric_columns[c];
                print_stats_is_not_null_(self, column_strlen, column_index, is_null);
                continue;
            }
            else if (v == most_unique_values+3) {
                // Print Not Null
                bool is_null = false;
                uint32_t column_index = non_numeric_columns[c];
                print_stats_is_not_null_(self, column_strlen, column_index, is_null);
                continue;
            }


            // Print spaces if no more values to dislay for this column
            if (unique_values[c] <= v) {
                char spaces[column_strlen+1];
                create_spaces_string(spaces, column_strlen+1, with_divider, ' ');
                printf("%s", spaces);
                continue;
            }

            char print_string[64];
            has_quotes(sorted[c][v].value) ? sprintf(print_string, "Value: %s", sorted[c][v].value) : sprintf(print_string, "Value: '%s'", sorted[c][v].value);

            uint8_t value_strlen = strlen(print_string);

            char count_string[32];

            sprintf(count_string, "Count: %ld | ", sorted[c][v].count);

            size_t count_strlen = strlen(count_string);

            // Spaces
            int8_t num_spaces = column_strlen - value_strlen - count_strlen;

            if (num_spaces > 0) {
                char spaces[num_spaces+1];
                for (uint8_t s = 0; s < num_spaces+1; s++) spaces[s] =  (s == num_spaces) ? '\0' : ' ';
                printf("%s%s%s", print_string, spaces, count_string);
            }
            else {

                // num spaces is negative
                sprintf(&print_string[value_strlen+num_spaces-4], "... ");
                printf("%s%s", print_string, count_string);
            }

        }

        // (v == most_unique_values-1) ? printf("\n\n") : printf("\n");
        printf("\n");
    }

    // Free arrays
    for (uint16_t c = 0; c < non_numeric_column_count; c++) free(sorted[c]);
    free(sorted);
    free(unique_values);

    return true;
}


//      stat_print_ (Called from print_stats_internal_)
void stat_print_(char *stat_name, double stat, uint8_t column_strlen) {

    char print_string[32];

    // Fill buffer with print string - (Numeric stats are all doubles, however Is Null/Not Null is an integer, hence the inline if)
    (strcasecmp(stat_name, "Is Null") != 0 && strcasecmp(stat_name, "Not Null") != 0) ? snprintf(print_string, 32, "%s: %.2f", stat_name, stat) : snprintf(print_string, 32, "%s: %.0f", stat_name, stat);

    uint8_t stat_length = strlen(print_string);

    // Express stat in scientific notation if longer than header
    if (stat_length > (size_t)column_strlen-2) {
        snprintf(print_string, 32, "%s: %e", stat_name, stat);
        stat_length = strlen(print_string); /* Recalc stat_length */
    }

    // Print stat
    printf("%s", print_string);

    // Print correct number of spaces to line up next stat
    uint8_t spaces_count = column_strlen - stat_length;
    char spaces[spaces_count];
    for (uint8_t s = 0; s < spaces_count; s++) {spaces[s] = (s == spaces_count-1) ? '\0' : ' ';}

    printf("%s", spaces);
    return;
}


void print_stats_is_not_null_(DataLynx *self, size_t column_strlen, uint32_t column_index, bool is_null) {

    char print_string[32];
    is_null ? sprintf(print_string, "Is Null %ld", self->aggregate[column_index].is_null) : sprintf(print_string, "Not Null %ld", self->aggregate[column_index].not_null);
    char spaces[32];
    bool with_divider = true;
    uint8_t num_spaces = column_strlen - strlen(print_string) + 1;
    create_spaces_string(spaces, num_spaces, with_divider, ' ');

    (!is_null && column_index == self->columnCount-1) ? printf("%s%s\n", print_string, spaces) : printf("%s%s", print_string, spaces);

    return;
}


//         // PRINT STATS() (side by side) and Sorted Value Counts Works :)
// bool printStats(DataLynx *self, char *column_name) {

//     if (self == NULL) return false;

//     if (self->aggregate == NULL || column_name == NULL) return false;

//     char *func_name = "printStats";

//     // Determine whether to display all columns or just one (i.e. this does not work IF csv has a column named "All")
//     int16_t c = 0, start = 0, stop = self->columnCount;
//     if (strcasecmp(column_name, "all") != 0) {

//         if ((c = findColumnIndex(self, column_name)) >= 0) stop = c + 1;
//         else c = 0;

//         start = c;

//     }

//     // Buffer to remember columns for value counts
//     uint16_t non_numeric_columns[self->columnCount];
//     uint16_t non_numeric_column_count = 0;

//     printf("\n\t<Data from file: %s>\n", self->filename);

//     printf("\n\t- Numeric Stats - \n");

//     int8_t stat_count = 10;

//     char space = ' ';
//     bool numeric_stats_in_data = false;
//     for (int8_t stat = -1; stat < stat_count; stat++) {

//         c = start;
//         while (c < stop) {

//             uint8_t column_strlen = strlen(self->__header__[c]) + 23;

//             // Numeric Stats
//             if (self->aggregate[c].is_number) {

//                 numeric_stats_in_data = true;

//                 // Print stats
//                 if (stat == -1) {
//                     !has_quotes(self->__header__[c]) ? printf("'%s' <Aggregate Data>%3c", self->__header__[c], space) : printf("%s <Aggregate Data>%3c", self->__header__[c], space);
//                 }
//                 else if (stat == 0) {
//                     stat_print_("Min", self->aggregate[c].min, column_strlen);
//                 }
//                 else if (stat == 1){
//                     stat_print_("Max", self->aggregate[c].max, column_strlen);
//                 }
//                 else if (stat == 2){
//                     stat_print_("Sum", self->aggregate[c].sum, column_strlen);
//                 }
//                 else if (stat == 3){
//                     stat_print_("Mean", self->aggregate[c].mean, column_strlen);
//                 }
//                 else if (stat == 4){
//                     stat_print_("25th %", self->aggregate[c].lower_qrt, column_strlen);
//                 }
//                 else if (stat == 5){
//                     stat_print_("Median", self->aggregate[c].median, column_strlen);
//                 }
//                 else if (stat == 6){
//                     stat_print_("75th %", self->aggregate[c].upper_qrt, column_strlen);
//                 }
//                 else if (stat == 7){
//                     stat_print_("Std", self->aggregate[c].std, column_strlen);
//                 }
//                 else if (stat == 8){
//                     stat_print_("Is Null", (double)self->aggregate[c].is_null, column_strlen);
//                 }
//                 else if (stat == 9){
//                     stat_print_("Not Null", (double)self->aggregate[c].not_null, column_strlen);
//                 }

//             }
//             else if (stat == -1) {
//                 /* Only do this one time around, otherwise will count columns multiple times.*/
//                 // Remember which column indexes are non-numeric, so we do not have to iterate through each column and check again when we print the value counts
//                 non_numeric_columns[non_numeric_column_count] = c;
//                 non_numeric_column_count++;
//             }

//             c++;
//         }

//         if (numeric_stats_in_data) {
//             (stat == stat_count-1) ? printf("\n\n"): printf("\n");
//         }


//     }

//     if (!numeric_stats_in_data) printf("\n* No numeric stats for this dataset *\n\n\n");

//     printf("\t- Value Counts -\n(i.e. non-numeric stats)\n\n");



//     // Print Value Counts (i.e Non Numeric Columns)
//     for (c = 0; c < non_numeric_column_count; c++) {

//         uint32_t column = non_numeric_columns[c];

//         // Print column name
//         has_quotes(self->__header__[column]) ? printf("%s <Aggregate Data>\n", self->__header__[column]) : printf("'%s' <Aggregate Data>\n", self->__header__[column]);

//         // Allocate array for sorted value counts
//         uint32_t unique_values = 0;

//         ValueCount *sorted = (ValueCount*)malloc(sizeof(ValueCount));
//         if (sorted == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

//         // Put Value Counts for current column into separate array
//         for (int l = 0; l < 27; l++) {
//             ValueCount *tmp = self->aggregate[column].value_counts[l];

//             while (tmp != NULL) {

//                 // Either print with '' or without if field has ""
//                 // has_quotes(tmp->value) ? printf("Value: %s, Count: %ld\n", tmp->value, tmp->count) : printf("Value: '%s', Count: %ld\n", tmp->value, tmp->count);

//                 sorted[unique_values].value = tmp->value;
//                 sorted[unique_values].count = tmp->count;

//                 tmp = tmp->next;

//                 //  Realloc sorted array
//                 if (tmp != NULL) {
//                     sorted = (ValueCount*)realloc(sorted, sizeof(ValueCount) * (++unique_values + 1));
//                     if (sorted == NULL) {if_error(REALLOC_FAILED, func_name); return false;}
//                 }

//             }
//         }


//         // SORT
//         for (uint32_t i = 0; i < unique_values-1; i++) {

//             uint32_t largest = i;

//             for (uint32_t j = i + 1; j < unique_values; j++) {

//                 if (sorted[j].count > sorted[i].count) largest = j;
//             }

//             // Swap
//             if (i != largest) {
//                 ValueCount tmp = sorted[i];
//                 sorted[i] = sorted[largest];
//                 sorted[largest] = tmp;
//             }
//         }

//         // uint8_t column_strlen = strlen(self->__header__[c]) + 23;
//         // char space = ' ';

//         // PRINT value counts for current column
//         printf("Unique values: %u\n", unique_values);

//         for (uint32_t v = 0; v < unique_values; v++) {

//             // uint8_t value_strlen = strlen(sorted[v].value) + 23;

//             // char print_string[value_strlen+32];

//             has_quotes(sorted[v].value) ? printf("Value: %s, Count: %ld\n", sorted[v].value, sorted[v].count) : printf("Value: '%s', Count: %ld\n", sorted[v].value,sorted[v].count);

//             // printf("%s", print_string);
//         }


//         printf("Is Null: %ld\n", (uintmax_t)self->getStat(self, self->__header__[c], "isnull"));
//         printf("Not Null: %ld\n\n", (uintmax_t)self->getStat(self, self->__header__[c], "notnull"));

//         free(sorted);
//     }

//     return true;
// }




//          -- FREE ALL --
void freeAll(DataLynx *self) {

    if (self == NULL) return;

    if (self->filename != NULL)
        free_null(&self->filename);

    if (self->file_ptr != NULL)
        fclose_null(&self->file_ptr);


    free_header(self);

    // Free one long string of file data
    free_null(&self->raw);

    // Free 2D array
    free_2d_array(self);

    free_grid_v3(self);

    free_grid(self);

    free_dict_grid(self);


    if (self->last_retrieved_field != NULL) free_null(&self->last_retrieved_field);

    free_value_counts(self); /* Must free before freeing aggregates array */

    if (self->aggregate != NULL) {
        free(self->aggregate);
        self->aggregate = NULL;
    }


    if (self->missingValue != NULL) free_null(&self->missingValue);

    if (self->last_retrieved_fields != NULL) free_last_retrieved_fields(self);


    if (self->tmp_column != NULL) {free(self->tmp_column); self->tmp_column = NULL;}

    self = NULL;

    return;
}

bool free_header(DataLynx *self) {

    if (self->__header__ == NULL) return false;

    for (uintmax_t column = 0; column < self->columnCount; column++) free_null(&self->__header__[column]);
    free(self->__header__); /* This is a char **, so can not use free_null(), bc I would need char ***. If I pass through to free_null(), it will accept it, however because its the wrong kind of char *, so it only frees the first string, not the array of strings */
    self->__header__ = NULL;

    return true;

}


//      FREE 2D ARRAY
bool free_2d_array(DataLynx *self) {

    // Free 2D array
    if (self == NULL) return false;
    if (self->rows == NULL) return false;

    // Free each row
    for (uintmax_t i = 0; i < self->rowCount; i++) {free_null(&self->rows[i]);}

    // Free array of pointers
    free(self->rows);
    self->rows = NULL;

    return true;
}

//      FREE GRID_V3
bool free_grid_v3(DataLynx *self) {

    if (self == NULL) return false;
    if (self->grid_v3 == NULL) return false;

    // Free array of db link lists
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        for (uintmax_t j = 0; j < self->columnCount; j++) {
            free_null(&self->grid_v3[i][j]);
        }

        free(self->grid_v3[i]);
        self->grid_v3[i] = NULL;

    }

    free(self->grid_v3); /* cannot use free_null() to free node** or char**, bc would need node*** and char*** */
    self->grid_v3 = NULL;

    return true;
}

//      FREE GRID
bool free_grid(DataLynx *self) {

    if (self == NULL) return false;
    if (self->grid == NULL) return false;

    // Free array of db link lists
    for (uintmax_t i = 0; i < self->rowCount; i++) {
        free_list(self->grid[i]);
    }

    free(self->grid); /* cannot use free_null() to free node** or char**, bc would need node*** and char*** */
    self->grid = NULL;

    return true;
}


//      FREE LIST
bool free_list(node *head)
{
    if (head == NULL) return false;

    /* THIS FUNCTION: Frees a doubly linked list */
    node *tmp = NULL;
    while (head != NULL)
    {
        tmp = head->next;
        free_null(&head->s);
        head->prev = head->next = NULL;
        free(head);
        head = tmp;
    }

    tmp = head = NULL;
    return true;
}



//       FREE DICT GRID
bool free_dict_grid(DataLynx *self)
{
    /* THIS FUNCTION: Frees douby linked list of dict_nodes */
    if (self == NULL) return false;
    if (self->dict_grid == NULL) return false;

    // Iterate through each row (i.e thorugh main array of dict_node pointers)
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        // Traverse linked list at current index in main array
        free_dict_list(self->dict_grid[i]);
    }

    // Free array
    free(self->dict_grid);
    self->dict_grid = NULL;

    return true;
}

//      FREE DICT LIST()
bool free_dict_list(dict_node *head) {

    if (head == NULL) return false;

    dict_node *tmp = NULL;
    while (head != NULL)
    {
        tmp = head->next;
        free_null(&head->s);
        // free_null(&head->column_name); /* This now points to string in header array, so do not double free */
        head->prev = head->next = NULL;
        free(head);
        head = tmp;
    }

    return true;
}


//      FREE VALUE COUNTS()
bool free_value_counts(DataLynx *self) {

    if (self == NULL) return false;

    if (self->aggregate == NULL) return false;

    for (uintmax_t c = 0; c < self->columnCount; c++) {

        ValueCount *tmp = NULL;
        for (uint8_t i = 0; i < 27; i++) {

            while (self->aggregate[c].value_counts[i] != NULL) {
                // Remember next node
                tmp = self->aggregate[c].value_counts[i]->next;

                // Free
                free_null(&(self->aggregate[c].value_counts[i]->value)); /* Will only need to free this if a replace has been done and no longer points to value in data structure. free_null() will bounce back right away if already null, so no double frees */
                self->aggregate[c].value_counts[i]->next = NULL;
                free(self->aggregate[c].value_counts[i]);
                self->aggregate[c].value_counts[i] = tmp;
            }
        }

        // Free array of value counts
        free(self->aggregate[c].value_counts);
        self->aggregate[c].value_counts = NULL;
    }


    return true;
}

void free_last_retrieved_fields(DataLynx *self) {

    node *tmp = NULL;
    while (self->last_retrieved_fields != NULL) {

        // Remember next node
        tmp = self->last_retrieved_fields->next;

        // Free string in node
        free_null(&self->last_retrieved_fields->s);
        self->last_retrieved_fields->next = NULL;

        // Free node
        free(self->last_retrieved_fields);
        self->last_retrieved_fields = NULL;

        self->last_retrieved_fields = tmp;
    }


    return;
}


// ___ FREE NULL ___
bool free_null(char **s) {
    if (s == NULL) return false;
    if (*s == NULL) return false;

    /* The only purpose of this function is to condense these 2 lines of code down to 1 in calling function. */
    free(*s);
    *s = NULL;

    return true;
}

// __ FCLOSE NULL ___
bool fclose_null(FILE **file) {
    if (file == NULL) return false;
    if (*file == NULL) return false;

    /* The only purpose of this function is to condense these 2 lines of code down to 1. Like free_null(), but for file pointers. */
    fclose(*file);
    *file = NULL;

    return true;
}


char *format_number(double value){

    // if (self == NULL) return NULL;

    char *func_name = "format_number";

    // Convert the double to a string with a large enough buffer
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.2f", value);

    // Find the position of the decimal point
    char* decimal_point = strchr(buffer, '.');
    if (decimal_point == NULL) {
        // No decimal point found, return the original string
        return strdup(buffer);
    }

    // Count the number of digits before the decimal point
    int num_digits = decimal_point - buffer;

    // Calculate the number of commas needed
    int num_commas = (num_digits - 1) / 3;

    // Calculate the total length of the new string with commas
    int new_length = strlen(buffer) + num_commas;

    // Allocate memory for the new string
    char* result = (char*)malloc(new_length + 1);
    if (result == NULL) {if_error(MALLOC_FAILED, func_name); return NULL;}

    int index = 0;
    for (int i = 0; i < num_digits; i++) {
        if ((num_digits - i) % 3 == 0 && i > 0) {
            result[index++] = ',';
        }
        result[index++] = buffer[i];
    }

    // Copy the decimal part and the null terminator
    strcpy(&result[index], &buffer[num_digits]);

    return result;

}

