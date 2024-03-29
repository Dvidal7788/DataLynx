#include <DataLynx.h>

DataLynx DataLynxConstructor(void) {

    DataLynx self;

    self.filename = NULL;
    self.file_ptr = NULL;
    self.file_size = 0;
    self.header_size = 0;
    self.DELIMITER = ',';
    self.has_header = true;
    self.with_spaces = true;
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
    self.drop_null_all = false;

    self.last_retrieved_field = NULL;
    self.last_retrieved_fields = NULL;

    self.all_bin_dividers = NULL;
    self.num_binned_columns = 0;

    self.json = NULL;
    self.xml = NULL;

    //          --- Assign function pointers ---
    self.header = &header;

    self.userInputFilename = &userInputFilename;
    self.changeFilename = &changeFilename;

    //          -- CSV Struct --
    self.csv.write_permission = false;

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
    self.csv.overwriteData = &overwriteData;

    //      Linear Model Struct
    self.linearModel.fit = &fit;
    self.linearModel.predict = &predict;
    self.linearModel.mse = &mse;
    self.linearModel.r2_score = &r2_score;

    self.linearModel.slope_ = 0;
    self.linearModel.intercept_ = 0;
    self.linearModel.is_fitted = false;
    self.linearModel.yhat_master = NULL;
    self.linearModel.yhat_master_len = 0;

    self.extracted_columns = NULL;
    self.extracted_columns_len = 0;

    self.extracted_numeric_columns = NULL;
    self.extracted_numeric_columns_len = 0;

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
    self.replaceInColumnIdx = &replaceInColumnIdx;

    self.dropColumn = &dropColumn;
    self.dropColumnIdx = &dropColumnIdx;
    self.dropRow = &dropRow;
    self.dropNull = &dropNull;
    self.dropNullIdx = &dropNullIdx;
    self.dropNullAll = &dropNullAll;;

    self.createHeader = &createHeader;
    self.insertRow = &insertRow;
    self.insertRowDict = &insertRowDict;

    self.sortRowsByColumn = &sortRowsByColumn;

    self.getBins = &getBins;
    self.oneHot = &oneHot;

    self.toXMLString = &toXMLString;
    self.toJSONString = &toJSONString;

    self.writeXML = &writeXML;
    self.writeJSON = &writeJSON;

    // Can call min, max etc from .min()/.max() etc or .stats()
    self.valueCount = &valueCount;
    self.isInColumn = &isInColumn;
    self.getStat = &getStat;
    self.getStatIdx = &getStatIdx;
    self.min = &min;
    self.max = &max;
    self.sum = &sum;
    self.mean = &mean;
    self.isNull = &isNull;
    self.notNull = &notNull;

    self.corr = &corr;

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
    self.printPivotTable = &printPivotTable;
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
    if (self->__header__ == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    for (uint32_t column = 0; column < column_count; column++) {

        // Allocate for string
        if (header == NULL) {

            // If no column names, create header of generic column names

            // Allocate each string
            self->__header__[column] = (char*)malloc(sizeof(char)*16);
            if (self->__header__[column] == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

            sprintf(self->__header__[column], "Column %d", column+1);


        }
        else if (header[column][0] == '\0') {

            self->__header__[column] = (char *)malloc(sizeof(char)*16);
            if (self->__header__[column] == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

            sprintf(self->__header__[column], "Column %d", column+1);
        }
        else {
            self->__header__[column] = (char *)malloc( sizeof(char) * (strlen(header[column])+1) );
            if (self->__header__[column] == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
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
        if (values2[column].field == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
        if (self->__header__ == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

        for (uint32_t column = 0; column < self->columnCount; column++) {

            // Allocate each string
            self->__header__[column] = (char*)malloc(sizeof(char) * (strlen(values[column].column_name)+1));
            if (self->__header__[column] == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

            strcpy(self->__header__[column], values[column].column_name);

        }

    }


    // Malloc new data structure or realloc current data structure + 1 row
    if (self->rowCount == 0) {

        // Inserting from scratch (i.e. no existing data structure)
        self->grid_v3 = (char***)malloc(sizeof(char**));
        if (self->grid_v3 == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
        self->rowCount++;
        create_stats(self);
    }
    // Realloc main array for 1 more row pointer
    else if (self->grid_v3 != NULL) {

        self->grid_v3 = (char***)realloc(self->grid_v3, sizeof(char**) * (++self->rowCount));
        if (self->grid_v3 == NULL) {log_error(REALLOC_FAILED, func_name); return false;}
    }
    else if (self->grid != NULL) {

        self->grid = (node**)realloc(self->grid, sizeof(node*) * (++(self->rowCount)));
        if (self->grid == NULL) {log_error(REALLOC_FAILED, func_name); return false;}
    }
    else if (self->dict_grid != NULL) {

        self->dict_grid = (dict_node**)realloc(self->dict_grid, sizeof(dict_node*) * (++(self->rowCount)));
        if (self->dict_grid == NULL) {log_error(REALLOC_FAILED, func_name); return false;}
    }


    // Allocate / setup new row
    if (self->grid_v3 != NULL) {
        self->grid_v3[self->rowCount-1] = (char**)malloc(sizeof(char*) * self->columnCount);
        if (self->grid_v3 == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
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
        if (new_field == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
        if (values2[column] == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
    if (self->csv.write_permission && self->destructive_mode) rowWriter(self, values2);

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
//         if (self->grid_v3 == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
//     }
//     else {
//         self->grid_v3 == (char ***)realloc(self->grid_v3, (sizeof(char **) * self->columnCount * (++self->rowCount)));
//         if (self->grid_v3 == NULL) {log_error(REALLOC_FAILED, func_name); return false;}
//     }


//     for (uint32_t column = 0; column < self->columnCount; column++) {

//         // Allocate string
//         self->grid_v3[row][column] = (char*)malloc(sizeof(char) * (strlen(values[column])+1));
//         if (self->grid_v3[row][column] == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
    if (n == NULL) {log_error(MALLOC_FAILED, "build_dblink_list"); return false;}

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

    // Function name (for use in log_error())
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
    if (n == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // This string pointer in the node takes over the allocated string buffer (i.e. **s_ptr).
    n->s = *s_ptr;
    *s_ptr = NULL;
    s_ptr = NULL;

    // Allocate buffer column name and copy string
    // n->column_name = (char*)malloc(sizeof(char)*strlen(*current_column_name)+1);
    // if (n->column_name == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
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
    if (self->missingValue == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

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
    if (self->filename == NULL)  {log_error(MALLOC_FAILED, func_name); return false;}

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
    if (self->filename == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

    // Read from command line
    if (fgets(self->filename, FILENAME_BUFFER_SIZE, stdin) == NULL) {log_error(FREAD_FAILED, func_name); return NULL;}

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

    return replaceInColumnIdx(self, column_index, to_replace, replace_with);
}


bool replaceInColumnIdx(DataLynx *self, uintmax_t column, char *to_replace, char *replace_with) {

    if (self == NULL || to_replace == NULL || replace_with == NULL || column > self->columnCount-1) return false;


    if (self->grid_v3 != NULL) return grid_v3_replace(self, to_replace, replace_with, column);
    else if (self->grid != NULL) return grid_replace(self, to_replace, replace_with, column);
    else if (self->dict_grid != NULL) return dict_grid_replace(self, to_replace, replace_with, column);

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
                if (self->grid_v3[i][j] == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

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
                if (tmp->s == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

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
                if (tmp->s == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

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

    // Find index correlating to column name
    int32_t column_index_signed;
    if ((column_index_signed = findColumnIndex(self, column_name)) < 0) return false;

    uint32_t column_index = column_index_signed;

    return dropColumnIdx(self, column_index);

}



//          DROP COLUMN INDEX()
bool dropColumnIdx(DataLynx *self, uintmax_t column_index) {

    if (self == NULL) return false;

    const char *func_name = "dropColumnIdx";

    // Allocate new header
    char **new_header = (char**)malloc(sizeof(char*) * (self->columnCount-1));
    if (new_header == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // Allocate new aggregate array
    Aggregate *new_aggregate = (Aggregate*)malloc(sizeof(Aggregate) * (self->columnCount-1));
    if (new_aggregate == NULL) {log_error(MALLOC_FAILED, func_name); return false;}


    for (uintmax_t row = 0; row < self->rowCount; row++) {

        // Set up pointers for new row
        char **new_row_grid_v3 = NULL;
        node *grid_row_cursor = NULL;
        dict_node *dict_grid_row_cursor = NULL;

        // Allocate new row (1 less column)
        if (self->grid_v3 != NULL) {
            new_row_grid_v3 = (char**)malloc(sizeof(char*) * (self->columnCount-1));
            if (new_row_grid_v3 == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
        }
        // Assign pointer to row
        else if (self->grid != NULL) grid_row_cursor = self->grid[row];
        else if (self->dict_grid != NULL) dict_grid_row_cursor = self->dict_grid[row];


        // Copy data from old row to new row (skip column to drop)
        uint32_t new_row_column = 0; /* Must use separate column counter or else will get off track after drop column is reached (i.e. will be one index off)*/
        for (uint32_t column = 0; column < self->columnCount; column++) {


            // Copy all columns, except drop column (do not need to copy for grid/dict_grid. Will simply drop the node when we come across it)
            if (column != column_index) {

                if (self->grid_v3 != NULL) {new_row_grid_v3[new_row_column] = self->grid_v3[row][column];}

                new_header[new_row_column] = self->__header__[column];
                new_aggregate[new_row_column] = self->aggregate[column];

                new_row_column++;

                // Next column (this goes here, because if on column to drop, drop_node/drop_dict_node will get pointer to next node)
                if (self->grid != NULL) grid_row_cursor = grid_row_cursor->next;
                else if (self->dict_grid != NULL) dict_grid_row_cursor = dict_grid_row_cursor->next;
            }
            else {

                // Free field (in the column to drop) in current row
                if (self->grid_v3 != NULL) free_null(&self->grid_v3[row][column]);
                else if (self->grid != NULL) drop_node(&self->grid[row], &grid_row_cursor);
                else if (self->dict_grid != NULL) drop_dict_node(&self->dict_grid[row], &dict_grid_row_cursor);

            }


        }

        // Free old row and reassign (only for grid_v3)
        if (self->grid_v3 != NULL) {
            free(self->grid_v3[row]);
            self->grid_v3[row] = new_row_grid_v3;
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


//      DROP NODE (in linked list)
void drop_node(node **head, node **cursor) {

    node *tmp_to_drop = *cursor;

    if ((*cursor)->prev == NULL) {

        // Get cursor to next node
        (*cursor) = (*cursor)->next;
        (*cursor)->prev = NULL;

        // Make sure pointer in array of linked lists now points to correct node
        *head = *cursor;


    }
    else {
        node *tmp_prev = (*cursor)->prev;
        tmp_prev->next = tmp_to_drop->next;

        (*cursor)->next = tmp_prev;

        // Get cursor to next node
        (*cursor) = (*cursor)->next;

    }

    // Free / set to NULL
    free_null(&tmp_to_drop->s);
    free(tmp_to_drop);

    return;
}


//      DROP DICT NODE (in linked list)
void drop_dict_node(dict_node **head, dict_node **cursor) {

    dict_node *tmp_to_drop = *cursor;

    if ((*cursor)->prev == NULL) {

        // Get cursor to next dict_node
        (*cursor) = (*cursor)->next;
        (*cursor)->prev = NULL;

        // Make sure pointer in array of linked lists now points to correct dict_node
        *head = *cursor;


    }
    else {
        dict_node *tmp_prev = (*cursor)->prev;
        tmp_prev->next = tmp_to_drop->next;

        (*cursor)->next = tmp_prev;

        // Get cursor to next dict_node
        (*cursor) = (*cursor)->next;

    }

    // Free / set to NULL
    free_null(&tmp_to_drop->s);
    free(tmp_to_drop);

    return;
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
        if (new_grid_v3 == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
    }
    else if (self->grid != NULL) {
        new_grid = (node**)malloc(sizeof(node*) * self->rowCount-1);
        if (new_grid == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
    }
    else if (self->dict_grid != NULL) {
        new_dict_grid = (dict_node**)malloc(sizeof(dict_node*) * self->rowCount-1);
        if (new_dict_grid == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
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



//          -- DROP NULL() --
int16_t dropNull(DataLynx *self, char *column_name) {

    if (self == NULL || column_name == NULL) return false;

    int16_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    return drop_null_(self, column_index);
}


//          -- DROP NULL Idx() --
int16_t dropNullIdx(DataLynx *self, uint16_t column_index) {

    if (self == NULL) return false;
    if (column_index > self->columnCount-1) return false;

    return drop_null_(self, column_index);
}


//          -- DROP NULL ALL() --
int16_t dropNullAll(DataLynx *self) {

    // Tell drop_null_() to check all columns
    self->drop_null_all = true;

    // I am passing 0 here, but since self->drop_null_all is set to true, this parameter will be ignored anyway
    int16_t num_rows_dropped = drop_null_(self, 0);

    // Reset to default
    self->drop_null_all = false;

    return num_rows_dropped;
}


//          drop_null_() (internal)
int16_t drop_null_(DataLynx *self, uint16_t column_index) {

    /* Returns number of rows dropped. -1 if error occurs */

    const char *func_name = "drop_null_";

    // Allocate buffer to keep track of which rows to drop
    uintmax_t num_rows_to_drop = 0;
    uintmax_t *rows_to_drop = (uintmax_t*)malloc(sizeof(uintmax_t));
    if (rows_to_drop == NULL) {log_error(MALLOC_FAILED, func_name); return -1;}

    // Determine whether to execute entire column loop or start and stop at only the requested column (depending if called from dropNullAll() or not)
    uint32_t start, stop;
    if (!self->drop_null_all) {

        // Only check 1 column requested
        start = column_index;
        stop = column_index + 1;
    }
    else {

        // Check all columns
        start = 0;
        stop = self->columnCount;
    }

    // Iterate through data to determine which rows to drop
    for (uint32_t column = start; column < stop; column++) {

        for (uintmax_t row = 0; row < self->rowCount; row++) {

            char *field = get_field_(self, row, column);

            if (field[0] == '\0') {
                rows_to_drop[num_rows_to_drop] = row;

                num_rows_to_drop++;

                // Realloc array to keep track of rows to drop
                rows_to_drop = (uintmax_t*)realloc(rows_to_drop, sizeof(uintmax_t) * (num_rows_to_drop+1));
                if (rows_to_drop == NULL) {log_error(REALLOC_FAILED, func_name); return -1;}
            }
        }

    }


    // Return if no rows to drop
    if (num_rows_to_drop == 0) {
        free(rows_to_drop);
        return num_rows_to_drop;
    }


    // Allocate new data structure
    char ***new_grid_v3 = NULL;
    node **new_grid = NULL;
    dict_node **new_dict_grid = NULL;


    // Allocate new main array
    if (self->grid_v3 != NULL) {
        new_grid_v3 = (char***)malloc(sizeof(char**) * (self->rowCount-num_rows_to_drop));
        if (new_grid_v3 == NULL) {log_error(MALLOC_FAILED, func_name); return -1;}
    }
    else if (self->grid != NULL) {
        new_grid = (node**)malloc(sizeof(node*) * (self->rowCount-num_rows_to_drop));
        if (new_grid == NULL) {log_error(MALLOC_FAILED, func_name); return -1;}
    }
    else if (self->dict_grid != NULL) {
        new_dict_grid = (dict_node**)malloc(sizeof(dict_node*) * (self->rowCount-num_rows_to_drop));
        if (new_dict_grid == NULL) {log_error(MALLOC_FAILED, func_name); return -1;}
    }


    //      -- Drop  (Copy all rows except the ones to drop) --
    uintmax_t rows_dropped = 0;
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        // Skip rows to drop
        if (rows_dropped != num_rows_to_drop && row == rows_to_drop[rows_dropped]) {
            // Free
            for (uint16_t c = 0; c < self->columnCount; c++) {

                bool increment = false;

                increment_decrement_value_count(self, self->__header__[c], get_field_(self, row, c), increment);

                // Free strings (fields) in current row
                if (self->grid_v3 != NULL) free(self->grid_v3[row][c]);

            }

            // Must free linked lists outside of above loop because otherwise will mess up get_field_()
            if (self->grid != NULL) {
                node *tmp = NULL;
                while (self->grid[row] != NULL) {
                    tmp = self->grid[row]->next;
                    free(self->grid[row]->s);
                    free(self->grid[row]);
                    self->grid[row] = tmp;
                }
            }
            else if (self->dict_grid != NULL) {
                dict_node *tmp = NULL;
                while (self->dict_grid[row] != NULL) {
                    tmp = self->dict_grid[row]->next;
                    free(self->dict_grid[row]->s);
                    free(self->dict_grid[row]);
                    self->dict_grid[row] = tmp;
                }
            }

            // Free current row
            if (self->grid_v3 != NULL) free(self->grid_v3[row]);

            rows_dropped++;
            continue;
        }

        // Attach non-dropped row to new data structure
        if (self->grid_v3 != NULL) new_grid_v3[row-rows_dropped] = self->grid_v3[row];
        else if (self->grid != NULL) new_grid[row-rows_dropped] = self->grid[row];
        else if (self->dict_grid != NULL) new_dict_grid[row-rows_dropped] = self->dict_grid[row];


    }


    self->rowCount -= num_rows_to_drop;

    // Free old main arrays and and assign new data structure
    if (self->grid_v3 != NULL) {
        free(self->grid_v3);
        self->grid_v3 = new_grid_v3;
    }
    else if (self->grid != NULL) {
        free(self->grid);
        self->grid = new_grid;
    }
    else if (self->dict_grid != NULL) {
        free(self->dict_grid);
        self->dict_grid = new_dict_grid;
    }

    free(rows_to_drop);

    return num_rows_to_drop;

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
    if (self->csv.write_permission && self->destructive_mode) write_csv_header_(self);

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
    if (self->__header__[column_index] == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
    if (self->csv.write_permission && self->destructive_mode) write_csv_header_(self);

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
            field = grid_tmp->s;
        }
        else if (self->dict_grid != NULL) {
            dict_tmp = self->dict_grid[row];
            get_ptr_to_correct_column(column, NULL, &dict_tmp);
            field = dict_tmp->s;
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
        if (stripped_field == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
        if (new_data->filename == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
    if (buffer == NULL) {log_error(MALLOC_FAILED, func_name);}

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
    if (buffer == NULL) {log_error(MALLOC_FAILED, func_name);}

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

    // Function name (for use in log_error())
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
            if (desired_column_with_quotes == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;};

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


//          getColumn()
char **getColumn(DataLynx *self, char *column_name) {

    // Safety checks
    if (self == NULL || column_name == NULL) return NULL;
    if (!data_exists(self)) return NULL;

    // Get corresponding column index
    int16_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return NULL;

    return get_column_(self, (uint16_t)column_index);

}


//          getColumn Idx()
char **getColumnIdx(DataLynx *self, uint16_t column_index) {

    // Safety checks
    if (self == NULL) return NULL;
    if (!data_exists(self) || column_index > self->columnCount-1) return NULL;

    return get_column_(self, column_index);

}


//          get_column_() (internal)
char **get_column_(DataLynx *self, uint16_t column_index) {

    const char *func_name = "get_column_";

    // Malloc or realloc extracted columns array
    if (self->extracted_columns == NULL) {
        self->extracted_columns = (char***)malloc(sizeof(char**) * ++(self->extracted_columns_len));
        if (self->extracted_columns == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}
    }
    else {
        self->extracted_columns = (char***)realloc(self->extracted_columns, sizeof(char**) * ++(self->extracted_columns_len));
        if (self->extracted_columns == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
    }


    // Malloc current column array
    char **column_array = (char**)malloc(sizeof(char*) * self->rowCount);
    if (column_array == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}


    // Iterate through data and extract column
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        column_array[row] = get_field_(self, row, column_index);
    }

    // Keep track of array so freeAll() can free it
    self->extracted_columns[(self->extracted_columns_len)-1] = column_array;

    return column_array;
}



//          getNumericColumn()
double *getNumericColumn(DataLynx *self, char *column_name) {

    // Safety checks
    if (self == NULL || column_name == NULL) return NULL;
    if (!data_exists(self)) return NULL;

    // Get corresponding column index
    int16_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return NULL;

    return get_numeric_column_(self, (uint16_t)column_index);

}


//          getNumericColumn Idx()
double *getNumericColumnIdx(DataLynx *self, uint16_t column_index) {

    // Safety checks
    if (self == NULL) return NULL;
    if (!data_exists(self) || column_index > self->columnCount-1) return NULL;

    return get_numeric_column_(self, column_index);

}


//          get_numeric_column_() (internal)
double *get_numeric_column_(DataLynx *self, uint16_t column_index) {

    const char *func_name = "get_numeric_column_";

    // Malloc or realloc extracted columns array
    if (self->extracted_numeric_columns == NULL) {
        self->extracted_numeric_columns = (double**)malloc(sizeof(double*) * ++(self->extracted_numeric_columns_len));
        if (self->extracted_numeric_columns == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}
    }
    else {
        self->extracted_numeric_columns = (double**)realloc(self->extracted_numeric_columns, sizeof(double*) * ++(self->extracted_numeric_columns_len));
        if (self->extracted_numeric_columns == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
    }


    // Malloc current column array
    double *column_array = (double*)malloc(sizeof(double) * self->rowCount);
    if (column_array == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}


    // Iterate through data and extract column
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        column_array[row] = atof(get_field_(self, row, column_index));
    }

    // Keep track of array so freeAll() can free it
    self->extracted_numeric_columns[(self->extracted_numeric_columns_len)-1] = column_array;

    return column_array;
}





//      ___ FIND_COLUMN_INDEX() ___
intmax_t findColumnIndex(DataLynx *self, const char *desired_column) {

    /* Will return the correct integer index location associated with desired_column string.
            - If no column name is found that matches desired_column, will return -1. This is why although index locations are not signed, I still need to return a signed int */

    // Find column integer index - Iterate through CSV Header to find column that matches desired_column

    if (self == NULL || desired_column == NULL) return -1;

    if (self->__header__ == NULL && self->file_ptr != NULL) headerReader(self);


    bool found_column = false;
    bool case_sensitive = true;
    uintmax_t current_column = 0;
    while (current_column < self->columnCount) {

        // Compare strings
        if (strcmp_quotes(self->__header__[current_column], desired_column, case_sensitive) == 0) {

            found_column = true;
            break;
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

    // Function name (for use in log_error())
    const char *func_name = "string_into_2d_array";

    /* THIS FUNCTION: Turns string INTO DYNAMICALLY ALLOCATED JAGGED 2D ARRAY (splits by '\n') */

    uintmax_t current_row = 0;

    // Main Array
    self->rows = (char**)malloc(sizeof(char*));
    if (self->rows == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

    // Iterate through file
    uintmax_t char_count = 0;
    while (true)
    {
        bool next_row = true;

        // Allocate 1st char of current row
        self->rows[current_row] = (char*)malloc(sizeof(char));
        if (self->rows[current_row] == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

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
                if (self->rows[current_row] == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
                i++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            self->rows = realloc(self->rows, sizeof(char*)*(current_row+2));
            if (self->rows == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
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

    // Function name (for use in log_error())
    const char *func_name = "split_2daray_by";

    /* THIS FUNCTION: 1. Takes dynamically allocated 2D array
                      2. Returns array of double link lists, separated by whatever char is stored in split (commonly split by ',' for csv-like behavior, but could be anything) */

    /* Note: When I try to write this function returning the row_count and accepting main_array as input, I get seg fault. I would prefer function to work that way. */

    // Allocate main array of double link lists
    self->grid = (node**)malloc(sizeof(node*));
    if (self->grid == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

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
                if (tmp_str == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}
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
                if (tmp_str == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
                j++; ii++;
            }
        }

        // Reallocate Next Row
        if (next_row) {
            self->grid = realloc(self->grid, sizeof(node*)*(i+2));
            if (self->grid == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
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

    // Function name (for use in log_error())
    const char *func_name = "grid_into_dict";

    dict_node **main_array = (dict_node**)malloc(sizeof(dict_node*));
    if (main_array == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

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
        if (main_array == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}



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
    if (self->grid_v3[row][column] == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

    strcpy(self->grid_v3[row][column], new_field);

    // Edit CSV file as well if allowed (default is false)
    if (self->csv.write_permission && self->destructive_mode) {

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
            if (cursor->s == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

            strcpy(cursor->s, new_field);

            // Edit CSV file as well if allowed (default is false)
            if (self->csv.write_permission && self->destructive_mode) {

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
            if (cursor->s == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

            strcpy(cursor->s, new_field);

            // Edit CSV file as well if allowed (default is false)
            if (self->csv.write_permission && self->destructive_mode) {

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
        if (self->tmp_column == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
    if (s1 == NULL) {log_error(MALLOC_FAILED, func_name); return false;}
    strcpy(s1, s1_input);

    char *s2 = (char*)malloc(sizeof(char)*(s2_len+1));
    if (s2== NULL) {log_error(MALLOC_FAILED, func_name); return false;}
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

    // Safety checks
    if (self == NULL) return false;
    if (!data_exists(self)) return false;

    if (number_of_rows >= self->rowCount) return print_data_internal(self);

    // Set member variable so print_data_internal() knows not to print whole dataset
    self->number_of_rows_to_print = number_of_rows;

    print_data_internal(self);

    if (!self->print_truncated_rows)
        printf("\t... (rows %ld-%ld not displayed)\n\n", self->number_of_rows_to_print, self->rowCount-1);

    // Reset to default
    self->number_of_rows_to_print = self->rowCount;
    return true;

}

bool printTail(DataLynx *self, uintmax_t number_of_rows) {

    // Safety checks
    if (self == NULL) return false;
    if (!data_exists(self)) return false;

    if (number_of_rows >= self->rowCount) return print_data_internal(self);

    // Set member variable so print_data_internal() knows not to print whole dataset
    self->number_of_rows_to_print = number_of_rows;
    self->print_tail = true;

    print_data_internal(self);

    // Reset to default
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

    // Safety checks
    if (self == NULL) return false;
    if (!data_exists(self)) return false;

    // TO DO scientifi notation, all data structure
    printf("\t<DATA SQL STYLE>\n\n");
    uint16_t column_lengths[self->columnCount]; /* This can be 16-bits instead of size_t, bc we will not store anything in it greater than self->maxFieldPrintLength */
    size_t head_strlens[self->columnCount];
    uint8_t padding = 2;


    // Go through ever column and remember what the longest string length
    for (uint32_t column = 0; column < self->columnCount; column++) {

        size_t head_strlen = strlen(self->__header__[column]);

        if ((head_strlen + padding >= self->maxFieldPrintLength) || (self->aggregate[column].longest_field_strlen + padding >= self->maxFieldPrintLength)) column_lengths[column] = self->maxFieldPrintLength;
        else if (head_strlen + padding > self->aggregate[column].longest_field_strlen + padding) column_lengths[column] = head_strlen + padding;
        else column_lengths[column] = self->aggregate[column].longest_field_strlen + padding;

        head_strlens[column] = head_strlen;

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

                    truncate_with_ellipses(self->__header__[column], truncated, self->maxFieldPrintLength-2);

                    (column == 0) ? printf("| %s |", truncated) : printf(" %s |", truncated);

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
            field = get_field_(self, (uintmax_t)row, column);

            // Check if field is empty
            if (field[0] == '\0') field = self->missingValue;

            // Remove quotes (non-destructively) if field has quotes
            if (has_quotes(field)) field = remove_quotes(self, field);

            size_t field_strlen = strlen(field);

            num_spaces = (column_lengths[column]-field_strlen);

            if (num_spaces <= 0) {

                // self->maxFieldPrintLength - 2 to make room for at least 1 space before & after field value
                truncate_with_ellipses(field, truncated, self->maxFieldPrintLength-2);

                (column == 0) ? printf("| %s |", truncated) : printf(" %s |", truncated);


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
    if (!data_exists(self)) return false;

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
bool printHeader(DataLynx *self) {

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
    if (self->has_header && self->__header__ != NULL && !self->print_truncated_rows) {
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


    // Free bin dividers (array of arrays of doubles)
    if (self->num_binned_columns > 0) {

        for (uint8_t i = 0; i < self->num_binned_columns; i++) {
            free(self->all_bin_dividers[i]);
            self->all_bin_dividers[i] = NULL;
        }

        free(self->all_bin_dividers);
        self->all_bin_dividers = NULL;
    }

    // Free JSON string
    if (self->json != NULL) free_null(&self->json);

    // Free XML string
    if (self->xml != NULL) free_null(&self->xml);

    // Free yhat arrays
    if (self->linearModel.yhat_master != NULL) {
        for (uint16_t i = 0; i < self->linearModel.yhat_master_len; i++) free(self->linearModel.yhat_master[i]);
        free(self->linearModel.yhat_master);
        self->linearModel.yhat_master = NULL;
    }


    // Free extracted columns
    if (self->extracted_columns != NULL) {
        for (uint16_t i = 0; i < self->extracted_columns_len; i++) free(self->extracted_columns[i]);
        free(self->extracted_columns);
        self->extracted_columns = NULL;
    }

    // Free extracted columns
    if (self->extracted_numeric_columns != NULL) {
        for (uint16_t i = 0; i < self->extracted_numeric_columns_len; i++) free(self->extracted_numeric_columns[i]);
        free(self->extracted_numeric_columns);
        self->extracted_numeric_columns = NULL;
    }


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

        // if (self->aggregate[c].is_number) continue; /* Can not do this because value_counts created for every column */
        if (self->aggregate[c].value_counts == NULL) continue; /* oneHot() sets value_counts to NULL */

        ValueCount *tmp = NULL;
        for (uint8_t i = 0; i < 27; i++) {

            while (self->aggregate[c].value_counts[i] != NULL) {

                // Remember next node
                tmp = self->aggregate[c].value_counts[i]->next;

                // Free value
                free_null(&(self->aggregate[c].value_counts[i]->value)); /* Will only need to free this if a replace has been done and no longer points to value in data structure. free_null() will bounce back right away if already null, so no double frees */
                self->aggregate[c].value_counts[i]->next = NULL;

                // Free internal DataLynx object
                if (self->aggregate[c].value_counts[i]->grouped_data != NULL) {
                    freeAll(self->aggregate[c].value_counts[i]->grouped_data);
                    free(self->aggregate[c].value_counts[i]->grouped_data);
                    self->aggregate[c].value_counts[i]->grouped_data = NULL;
                }

                // Free current value count node
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
    if (result == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

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

// //          -- GET BINS() (Numeric -> Categorical) -- (end of day 12-21-23)
// double *getBins(DataLynx *self, char *column_name, uint16_t num_bins, char **bin_names) {

//     //          --- Safety checks ---
//     if (self == NULL || column_name == NULL || num_bins < 2 || bin_names == NULL) return false;

//     // Corresponding column index
//     int32_t column_index = findColumnIndex(self, column_name);
//     if (column_index < 0 || !self->aggregate[column_index].is_number) return false;

//     // Ensure bin_names array has no NULL strings
//     for (uint16_t i = 0; i < num_bins; i++) {
//         if (bin_names[i] == NULL) return false;
//     }

//     const char *func_name = "getBins";


//     //          --- Divide range into bins ---

//     // Extract min/max (for code readability)
//     double min = self->aggregate[column_index].min;
//     double max = self->aggregate[column_index].max;

//     // Get range
//     double range = max - min;

//     // Get the size (range) of each bin
//     double bin_size = range / num_bins;

//     // Allocate array of dividers
//     uint16_t num_dividers = num_bins + 1;
//     double *dividers = (double*)malloc(sizeof(double)*num_dividers);
//     if (dividers == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

//     // Divide range into bin dividers
//     dividers[0] = min;
//     dividers[num_dividers-1] = max;
//     double current_divider = min;

//     for (uint16_t d = 1; d < num_dividers-1; d++) {
//         current_divider += bin_size;
//         dividers[d] = current_divider;
//     }

//     // // PRINT!!!
//     // for (uint16_t d = 0; d < num_dividers; d++) {
//     //     printf("%f\n", dividers[d]);
//     // }


//     //          --- Create new column name in header ---
//     // Realloc header array (i.e array of strings)
//     self->__header__ = (char**)realloc(self->__header__, sizeof(char*) * ++self->columnCount);
//     if (self->__header__ == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

//     // Allocate new string for binned column name, initialize string and assign to header array
//     const char *append = "_binned";

//     char *new_column_name = (char*)malloc(sizeof(char) * (strlen(column_name) + strlen(append) + 1));
//     if (new_column_name == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

//     sprintf(new_column_name, "%s%s", column_name, append);

//     self->__header__[self->columnCount-1] = new_column_name;

//     // Allocate array to keep track of value counts for each bin
//     uint16_t *counts = (uint16_t*)malloc(sizeof(uint16_t)*num_bins);
//     if (counts == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

//     // Initialize array
//     for (uint16_t b = 0; b < num_bins; b++) counts[b] = 0;

//     uint32_t null_field_count = 0; /* Keep track of null fields for aggregate is_null/not_null counts */

//     //          --- Iterate through each row and attach new binned column ---
//     for (uintmax_t row = 0; row < self->rowCount; row++) {

//         double numeric_value = 0;

//         node *grid_tmp = NULL;
//         dict_node *dict_grid_tmp = NULL;

//         // Get numeric value
//         if (self->grid_v3 != NULL) {

//             // Extract numeric value if field is not null
//             if (self->grid_v3[row][column_index][0] != '\0') numeric_value = atof(self->grid_v3[row][column_index]);
//             else null_field_count++;
//         }
//         else if (self->grid != NULL) {
//             grid_tmp = self->grid[row];
//             get_ptr_to_correct_column(column_index, &grid_tmp, NULL);

//             // Extract numeric value if field is not null
//             if (grid_tmp->s[0] != '\0') numeric_value = atof(grid_tmp->s);
//             else null_field_count++;

//         }
//         else if (self->dict_grid != NULL) {
//             dict_grid_tmp = self->dict_grid[row];
//             get_ptr_to_correct_column(column_index, NULL, &dict_grid_tmp);

//             // Extract numeric value if field is not null
//             if (dict_grid_tmp->s[0] != '\0') numeric_value = atof(dict_grid_tmp->s);
//             else null_field_count++;
//         }


//         // Determine correct bin name
//         char *new_field = NULL;
//         for (uint16_t b = 0; b < num_dividers; b++) {

//             if (numeric_value > dividers[b]) continue;
//             else {
//                 if (b != 0) b -= 1;
//                 // Allocate new field string
//                 new_field = (char*)malloc(sizeof(char)*(strlen(bin_names[b])+1));
//                 if (new_field == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

//                 strcpy(new_field, bin_names[b]);
//                 counts[b]++;

//                 break;
//             }

//         }


//         // Allocate new column in current row
//         if (self->grid_v3 != NULL) {

//             self->grid_v3[row] = (char **)realloc(self->grid_v3[row], (sizeof(char*)*self->columnCount));
//             if (self->grid_v3[row] == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

//             self->grid_v3[row][self->columnCount-1] = new_field;
//         }
//         else if (self->grid != NULL) {
//             grid_tmp = self->grid[row];
//             get_ptr_to_correct_column(self->columnCount-2, &grid_tmp, NULL); /* Get ptr to last node (normally columnCount-1, but we already incremented columnCount) */
//             node *last = grid_tmp;

//             build_dblink_list(&new_field, &self->grid[row], &last);

//         }
//         else if (self->dict_grid != NULL) {
//             dict_grid_tmp = self->dict_grid[row];
//             get_ptr_to_correct_column(self->columnCount-2, NULL, &dict_grid_tmp);
//             dict_node *last = dict_grid_tmp;

//             build_dict_link_list(&new_field, &self->dict_grid[row], &last, &new_column_name);

//         }

//     }

//     //          --- Aggregate Data for new binned column ---
//     // Realloc aggregate array
//     self->aggregate = (Aggregate*)realloc(self->aggregate, sizeof(Aggregate)*self->columnCount);
//     if (self->aggregate == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

//     // Is Null / Not Null
//     self->aggregate[self->columnCount-1].is_null = null_field_count;
//     self->aggregate[self->columnCount-1].not_null = self->rowCount - null_field_count;


//     // Find longest strlen in new binned column
//     self->aggregate[self->columnCount-1].is_number = false;

//     for (uint16_t b = 0; b < num_bins; b++) {
//         if (b == 0) self->aggregate[self->columnCount-1].longest_field_strlen = strlen(bin_names[b]);
//         else if (strlen(bin_names[b]) > self->aggregate[self->columnCount-1].longest_field_strlen) self->aggregate[self->columnCount-1].longest_field_strlen = strlen(bin_names[b]);
//     }


//     // Create value counts "alpha hash" array
//     create_value_counts(self, self->columnCount-1);
//     for (uint16_t b = 0; b < num_bins; b++) {
//         ValueCount *tmp = prepend_value_count(self, self->columnCount-1, bin_names[b], find_alpha_index(bin_names[b]));
//         tmp->count = counts[b];
//     }

//     // Keep bin dividers in DataLynx object
//     if (self->num_binned_columns == 0) {
//         self->all_bin_dividers = (double**)malloc(sizeof(double*));
//         if (self->all_bin_dividers == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}
//     }
//     else {
//         self->all_bin_dividers = (double**)realloc(self->all_bin_dividers, sizeof(double*) * (self->num_binned_columns + 1));
//         if (self->all_bin_dividers == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
//     }

//     self->all_bin_dividers[(++self->num_binned_columns)-1] = dividers;

//     if (self->destructive_mode) overwriteData(self);

//     free(counts);
//     return dividers;
// }


//          -- GET BINS() (Numeric -> Categorical) --
double *getBins(DataLynx *self, char *column_name, uint16_t num_bins, char **bin_names_input) {

    //          --- Safety checks ---
    if (self == NULL || column_name == NULL || num_bins < 2) return false;
    if (!data_exists(self)) return false;

    // Corresponding column index
    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0 || !self->aggregate[column_index].is_number) return false;

    // Use this instead of column_name parameter, because this will ensure that quotes are used if necessary (i.e. if the column name has quotes, the library user can input the column name with or without quotes and finColumnIndex() will still find the correct column)
    char *original_column_name = self->__header__[column_index];

    char **bin_names = NULL;

    char *bin_names2[] = {
        "Top Half",
        "Lower Half"
    };

    char *bin_names3[] = {
        "Low",
        "Medium",
        "High"
    };

    char *bin_names4[] = {
        "Low",
        "Med-Low",
        "Med-High",
        "High"
    };

    char *bin_names5[] = {
        "Low",
        "Med-Low",
        "Medium",
        "Med-High",
        "High"
    };

    char *bin_names6[] = {
        "Lowest",
        "Low",
        "Med-Low",
        "Med-High",
        "High",
        "Highest"
    };

    char *bin_names7[] = {
        "Lowest",
        "Low",
        "Med-Low",
        "Medium",
        "Med-High",
        "High",
        "Highest"
    };


    // Determine what bin names will be (i.e. default or custom)
    if (num_bins > 7 && bin_names_input == NULL) return false;
    else if (bin_names_input != NULL) {

        bin_names = bin_names_input;

        // Ensure bin_names array has no NULL strings
        for (uint16_t i = 0; i < num_bins; i++) {
            if (bin_names[i] == NULL) return false;
        }
    }
    else {

        // Assign array of bin names
        switch (num_bins) {
            case 2:
                bin_names = bin_names2;
                break;
            case 3:
                bin_names = bin_names3;
                break;
            case 4:
                bin_names = bin_names4;
                break;
            case 5:
                bin_names = bin_names5;
                break;
            case 6:
                bin_names = bin_names6;
                break;
            case 7:
                bin_names = bin_names7;
                break;
            default: /* This default should never hit because all other possibilities are handled in if statements, but it feels uncouth to not have default. */
                bin_names = bin_names3;
                break;
        }

    }

    const char *func_name = "getBins";

    //          --- Divide range into bins ---

    // Extract min/max (for code readability)
    double min = self->aggregate[column_index].min;
    double max = self->aggregate[column_index].max;

    // Get range
    double range = max - min;

    // Get the size (range) of each bin
    double bin_size = range / num_bins;

    // Allocate array of dividers
    uint16_t num_dividers = num_bins + 1;
    double *dividers = (double*)malloc(sizeof(double)*num_dividers);
    if (dividers == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // Divide range into bin dividers
    dividers[0] = min;
    dividers[num_dividers-1] = max;
    double current_divider = min;

    for (uint16_t d = 1; d < num_dividers-1; d++) {
        current_divider += bin_size;
        dividers[d] = current_divider;
    }


    //          --- Create new column name in header ---
    // Realloc header array (i.e array of strings)
    self->__header__ = (char**)realloc(self->__header__, sizeof(char*) * ++self->columnCount);
    if (self->__header__ == NULL) {log_error(REALLOC_FAILED, func_name); return false;}


    //      -- Allocate new string for binned column name, initialize string and assign to header array --
    const char *append = "_binned";

    size_t original_column_name_strlen = strlen(original_column_name);

    // Allocate buffer for new column name string
    char *new_column_name = (char*)malloc(sizeof(char) * (original_column_name_strlen + strlen(append) + 1));
    if (new_column_name == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // Assign new column name with append string, making sure to include quotes if original column name has quotes
    if (!has_quotes(original_column_name)) sprintf(new_column_name, "%s%s", self->__header__[column_index], append);
    else {
        sprintf(new_column_name, "%s", self->__header__[column_index]);
        sprintf(&new_column_name[original_column_name_strlen-1], "%s\"", append);
    }


    // Attach new column name to header
    self->__header__[self->columnCount-1] = new_column_name;

    // Allocate array to keep track of value counts for each bin
    uint16_t *counts = (uint16_t*)malloc(sizeof(uint16_t)*num_bins);
    if (counts == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // Initialize array
    for (uint16_t b = 0; b < num_bins; b++) counts[b] = 0;

    uint32_t null_field_count = 0; /* Keep track of null fields for aggregate is_null/not_null counts */

    //          --- Iterate through each row and attach new binned column ---
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        double numeric_value = 0;

        node *grid_tmp = NULL;
        dict_node *dict_grid_tmp = NULL;

        // Get numeric value
        char *field = get_field_(self, row, column_index);

        if (field[0] != '\0') numeric_value = atof(field);
        else null_field_count++;

        // Determine correct bin name
        char *new_field = NULL;
        for (uint16_t b = 0; b < num_dividers; b++) {

            if (numeric_value > dividers[b]) continue;
            else {
                if (b != 0) b -= 1;
                // Allocate new field string
                new_field = (char*)malloc(sizeof(char)*(strlen(bin_names[b])+1));
                if (new_field == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

                strcpy(new_field, bin_names[b]);
                counts[b]++;

                break;
            }

        }


        // Allocate new column in current row
        if (self->grid_v3 != NULL) {

            self->grid_v3[row] = (char **)realloc(self->grid_v3[row], (sizeof(char*)*self->columnCount));
            if (self->grid_v3[row] == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

            self->grid_v3[row][self->columnCount-1] = new_field;
        }
        else if (self->grid != NULL) {
            grid_tmp = self->grid[row];
            get_ptr_to_correct_column(self->columnCount-2, &grid_tmp, NULL); /* Get ptr to last node (normally columnCount-1, but we already incremented columnCount) */
            node *last = grid_tmp;

            build_dblink_list(&new_field, &self->grid[row], &last);

        }
        else if (self->dict_grid != NULL) {
            dict_grid_tmp = self->dict_grid[row];
            get_ptr_to_correct_column(self->columnCount-2, NULL, &dict_grid_tmp);
            dict_node *last = dict_grid_tmp;

            build_dict_link_list(&new_field, &self->dict_grid[row], &last, &new_column_name);

        }

    }

    //          --- Aggregate Data for new binned column ---
    // Realloc aggregate array
    self->aggregate = (Aggregate*)realloc(self->aggregate, sizeof(Aggregate)*self->columnCount);
    if (self->aggregate == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

    // Is Null / Not Null
    self->aggregate[self->columnCount-1].is_null = null_field_count;
    self->aggregate[self->columnCount-1].not_null = self->rowCount - null_field_count;


    // Find longest strlen in new binned column
    self->aggregate[self->columnCount-1].is_number = false;

    for (uint16_t b = 0; b < num_bins; b++) {
        if (b == 0) self->aggregate[self->columnCount-1].longest_field_strlen = strlen(bin_names[b]);
        else if (strlen(bin_names[b]) > self->aggregate[self->columnCount-1].longest_field_strlen) self->aggregate[self->columnCount-1].longest_field_strlen = strlen(bin_names[b]);
    }


    // Create value counts "alpha hash" array
    create_value_counts(self, self->columnCount-1);
    for (uint16_t b = 0; b < num_bins; b++) {
        ValueCount *tmp = prepend_value_count(self, self->columnCount-1, bin_names[b], find_alpha_index(bin_names[b]));
        tmp->count = counts[b];
    }

    // Keep bin dividers in DataLynx object
    if (self->num_binned_columns == 0) {
        self->all_bin_dividers = (double**)malloc(sizeof(double*));
        if (self->all_bin_dividers == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}
    }
    else {
        self->all_bin_dividers = (double**)realloc(self->all_bin_dividers, sizeof(double*) * (self->num_binned_columns + 1));
        if (self->all_bin_dividers == NULL) {log_error(REALLOC_FAILED, func_name); return NULL;}
    }

    self->all_bin_dividers[(++self->num_binned_columns)-1] = dividers;

    if (self->destructive_mode) overwriteData(self);

    free(counts);
    return dividers;
}




//              -- ONE HOT (Categorical -> Numeric) --
bool oneHot(DataLynx *self, char *column_name) {

    // Safety checks
    if (self == NULL || column_name == NULL) return false;

    int32_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    if (self->aggregate[column_index].is_number) return false;

    const char *func_name = "oneHot";


    //          -- Find unique value count -
    uint16_t unique_values = 0;

    // Iterate through alpha index array
    for (uint8_t a = 0; a < 27; a++) {

        // Traverse linked list at current alpha location
        ValueCount *tmp = self->aggregate[column_index].value_counts[a];

        while (tmp != NULL) {
            unique_values++;
            tmp = tmp->next;
        }

    }


    //          -- Add new column names to header --
    // Realloc header array
    self->__header__ = (char**)realloc(self->__header__, sizeof(char*) * (self->columnCount+unique_values));
    if (self->__header__ == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

    // Realloc aggregate array
    self->aggregate = (Aggregate*)realloc(self->aggregate, sizeof(Aggregate) * (self->columnCount+unique_values));
    if (self->aggregate == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

    // Create strings for each new column name (one for each  unique value in original column)
    uint16_t new_col = 0;
    for (uint8_t a = 0; a < 27 && new_col != unique_values; a++) {

        // Traverse linked list at current alpha location
        ValueCount *tmp = self->aggregate[column_index].value_counts[a];

        while (tmp != NULL) {
            self->__header__[self->columnCount + new_col] = (char*)malloc(sizeof(char) * (strlen(tmp->value)+1));
            if (self->__header__ == NULL) {log_error(REALLOC_FAILED, func_name); return false;}

            strcpy(self->__header__[self->columnCount + new_col], tmp->value);

            self->aggregate[self->columnCount + new_col].is_null = 0;
            self->aggregate[self->columnCount + new_col].not_null = self->rowCount;
            self->aggregate[self->columnCount + new_col].is_number = true;
            self->aggregate[self->columnCount + new_col].value_counts = NULL;
            self->aggregate[self->columnCount + new_col].sum = 0;
            self->aggregate[self->columnCount + new_col].std = 0;
            self->aggregate[self->columnCount + new_col].mean = 0;
            self->aggregate[self->columnCount + new_col].min = 0;
            self->aggregate[self->columnCount + new_col].lower_qrt = 0;
            self->aggregate[self->columnCount + new_col].median = 0;
            self->aggregate[self->columnCount + new_col].upper_qrt = 0;
            self->aggregate[self->columnCount + new_col].max = 1;

            self->aggregate[self->columnCount + new_col].longest_field_strlen = 1;

            new_col++;
            tmp = tmp->next;
        }

    }

    node *grid_tmp = NULL;
    dict_node *dict_grid_tmp = NULL;

    // Add new columns to main data structure
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        // Allocate for new columns in current row
        if (self->grid_v3 != NULL) {

            self->grid_v3[row] = (char**)realloc(self->grid_v3[row], sizeof(char*) * (self->columnCount + unique_values));
            if (self->grid_v3 == NULL) {log_error(REALLOC_FAILED, func_name); return false;}
        }
        else if (self->grid != NULL) {
            grid_tmp = self->grid[row];
            get_ptr_to_correct_column(self->columnCount-1, &grid_tmp, NULL);

        }
        else if (self->dict_grid != NULL) {
            dict_grid_tmp = self->dict_grid[row];
            get_ptr_to_correct_column(self->columnCount-1, NULL, &dict_grid_tmp);

        }

        char *current_field = get_field_(self, row, column_index);

        // Start from 1st new column (i.e. columnCount-1 would be last column in header before addind new column names)
        for (uint16_t c = self->columnCount; c < self->columnCount+unique_values; c++) {

            // Allocate new string for 0 or 1
            char *new_field = (char*)malloc(sizeof(char) * 2);
            if (new_field == NULL) {log_error(MALLOC_FAILED, func_name); return false;}


            // Assign True/False
            if (strcmp(self->__header__[c], current_field) != 0) {
                new_field[0] = '0';
                new_field[1] = '\0';
            }
            else {
                new_field[0] = '1';
                new_field[1] = '\0';
            }



            // Assign string to data structure / allocate new node for linked lists
            if (self->grid_v3 != NULL) self->grid_v3[row][c] = new_field;
            else if (self->grid != NULL) {
                build_dblink_list(&new_field, &self->grid[row], &grid_tmp);
            }
            else if (self->dict_grid != NULL) {
                build_dict_link_list(&new_field, &self->dict_grid[row], &dict_grid_tmp, &self->__header__[c]);
            }
        }

    }

    self->columnCount += unique_values;
    return true;

}


// EXTRACT FIELDS WITHOUT HAVING TO HAVE IF STATEMENTS IN SO MANY FUNCTIONS
char *get_field_(DataLynx *self, uintmax_t row, int16_t column) {

    /* Do not use getField() or the internal functions that go along with it because they keep track of fields that are grabbed, wasting memory. I could turn this feature off and back on again, but that creates ugly code.
      - this function will only return NULL if incorrect row or column indexes are given. This is only called from other functions which check for that, so should not happen.*/

    char *field = NULL;
    node *grid_tmp = NULL;
    dict_node *dict_grid_tmp = NULL;

    if (self->grid_v3 != NULL) field = self->grid_v3[row][column];
    else if (self->grid != NULL) {
        grid_tmp = self->grid[row];
        get_ptr_to_correct_column(column, &grid_tmp, NULL);
        field = grid_tmp->s;
    }
    else if (self->dict_grid != NULL) {
        dict_grid_tmp = self->dict_grid[row];
        get_ptr_to_correct_column(column, NULL, &dict_grid_tmp);
        field = dict_grid_tmp->s;
    }

    return field;
}




//          -- to JSON String() ---
char *toJSONString(DataLynx *self) {

    /*  - Creates JSON string, saved in self.json, for library user to use as they wish
            - writeJSON() in the file_io module, writes to a .json file, first by calling this function to create the string. */

    // Safety check
    if (self == NULL) return false;
    if (!data_exists(self)) return false;

    if (self->json != NULL) free_null(&self->json);

    const char *func_name = "toJSONString";

    //          -- Create write_string --

    // 1. Determine char count of entire dataset (i.e. sum strlen for every field)
    char *field = NULL;
    uintmax_t total_char_count = 0;
    for (uintmax_t row = 0; row < self->rowCount; row++) {
        for (uint32_t column = 0; column < self->columnCount; column++) {

            field = get_field_(self, row, column);
            total_char_count += strlen(field);

        }
    }

    // 2. Determine header char count (i.e. strlen of every column name)
    size_t header_strlen = 0;
    uint16_t non_numeric_column_count = 0;
    for (uint16_t c = 0; c < self->columnCount; c++) {

        // Tally strlen for each column name (char counts for "" will be added later, as each column name gets "" regardless of if it already does or not, therefore I will tally strlen-2 for column names with "")
        header_strlen += (has_quotes(self->__header__[c])) ? strlen(self->__header__[c])-2 : strlen(self->__header__[c]);

        // Find numeric column count
        if (!self->aggregate[c].is_number) non_numeric_column_count++;
    }
    total_char_count += (header_strlen * self->rowCount);

    // 3. Add char counts for [], {}, "", :: and ,
            /*  - (6 * self->columnCount) -> "" around every column name, followed by : (i.e. "column 1": value), plus comma, then space (or }, then comma)
                - (2 * non_numeric_column_count) -> "" around every value in non-numeric columns
                - self->rowCount -> {},\n
            */

    /* Note: This char count system counts exactly the number of chars needed, to save memory. With that said, leaves no room for error. Seems to work fine so far. */

    // Every row -> {},\n
    total_char_count += (4 * self->rowCount);

    // Every column, every row (except last column) -> "":space,space
    total_char_count += (6 * (self->columnCount-1)) * self->rowCount;

    // Last column, every row -> "":space
    total_char_count += (4 * self->rowCount);

    // Ever non-numeric column, every row -> ""
    total_char_count += ((2 * non_numeric_column_count) * self->rowCount);

    // Once per data set -> [\n]\n and '\0'
    total_char_count += 5;


    // 4. Construct json string

    //      Allocate total buffer needed
    char *json_string = (char*)malloc(sizeof(char) * total_char_count);
    if (json_string == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    //      Beginning of json
    json_string[0] = '[';
    json_string[1] = '\n';
    uintmax_t current_char = 2;

    //      Iterate through every row
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        //  Start of every row
        json_string[current_char++] = '{';

        //  Iterate through each column in current row
        for (uint32_t column = 0; column < self->columnCount; column++) {

            // Column name
            if (!has_quotes(self->__header__[column])) json_string[current_char++] = '"';

            strcpy(&json_string[current_char], self->__header__[column]);
            current_char += strlen(self->__header__[column]);

            if (!has_quotes(self->__header__[column])) json_string[current_char++] = '"';

            json_string[current_char++] = ':';
            json_string[current_char++] = ' ';


            // Field (value)
            field = get_field_(self, row, column);

            // Add beginning quote if necessary
            if (!self->aggregate[column].is_number && !has_quotes(field)) json_string[current_char++] = '"';

            // Copy field to json string
            strcpy(&json_string[current_char], field);

            // Move char counter
            current_char += strlen(field);

            // Add end quote if necessary
            if (!self->aggregate[column].is_number && !has_quotes(field)) json_string[current_char++] = '"';


            // Every column, except last column
            if (column != self->columnCount-1) {
                json_string[current_char++] = ',';
                json_string[current_char++] = ' ';
            }


        }

        // Add json for end of every row
        json_string[current_char++] = '}';
        // if (row != self->rowCount-1) json_string[current_char++] = ','; /* Last row does not get comma */
        json_string[current_char++] = ','; /* Last row does not get comma, however doing it this way, then backing up 2 chars, then adding the \n after all rows are done only adds 2 steps, as opposed to n steps (i.e. an extra step for every row) by using if statement above. */
        json_string[current_char++] = '\n';

    }

    // Adjust cursor and add back new line (instead of using if (row != self->rowCount-1) json_string[current_char++] = ','; above, which adds row count number of extra steps)
    current_char -= 2;
    json_string[current_char++] = '\n';

    // Finish json string
    json_string[current_char++] = ']';
    json_string[current_char++] = '\n';
    json_string[current_char++] = '\0';


    // printf("total_char_count: %ld, current_char: %ld", total_char_count, current_char);

    // Store json string in DataLynx object
    self->json = json_string;

    return self->json;
}



//          -- to XML String() ---
char *toXMLString(DataLynx *self) {

    // Safety check
    if (self == NULL) return false;
    if (!data_exists(self)) return false;

    if (self->xml != NULL) free_null(&self->xml);

    const char *func_name = "toxmlString";

    // Create write_string
    char *field = NULL;
    uintmax_t total_char_count = 0;
    for (uintmax_t row = 0; row < self->rowCount; row++) {
        for (uint32_t column = 0; column < self->columnCount; column++) {

            field = get_field_(self, row, column);
            total_char_count += strlen(field);

        }
    }

    // Header strlen
    size_t header_strlen = 0;
    uint16_t non_numeric_column_count = 0;
    for (uint16_t c = 0; c < self->columnCount; c++) {
        header_strlen += strlen(self->__header__[c]);

        // Find numeric column count
        if (!self->aggregate[c].is_number) non_numeric_column_count++;
    }
    total_char_count += ((header_strlen*2) * self->rowCount);

    total_char_count += (strlen("<data>\n") + strlen("</data>\n"));

    // Add counts for ... <></>/n each row
    total_char_count += ( ( strlen("\t\t<></>\n") * self->columnCount ) * self->rowCount );
    total_char_count += ( ( strlen("\t<row>\n") + strlen("\t</row>\n") ) * self->rowCount);


    char *xml_string = (char*)malloc(sizeof(char) * total_char_count+1);
    if (xml_string == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    //          -- Create xml string --
    fill_buffer(xml_string, "<data>\n");

    uintmax_t current_char = 7;

    for (uintmax_t row = 0; row < self->rowCount; row++) {

        fill_buffer(&xml_string[current_char], "\t<row>\n");
        current_char += 7;

        for (uint32_t column = 0; column < self->columnCount; column++) {

            // Column name Open Tag
            fill_buffer(&xml_string[current_char], "\t\t<");
            current_char += 3;

            fill_buffer(&xml_string[current_char], self->__header__[column]);
            current_char += strlen(self->__header__[column]);
            xml_string[current_char++] = '>';

            // Field (value)
            field = get_field_(self, row, column);

            fill_buffer(&xml_string[current_char], field);
            current_char += strlen(field);


            // Column name Close Tag
            xml_string[current_char++] = '<';
            fill_buffer(&xml_string[current_char], self->__header__[column]);
            current_char += strlen(self->__header__[column]);
            xml_string[current_char++] = '/';
            xml_string[current_char++] = '>';
            xml_string[current_char++] = '\n';

        }

        fill_buffer(&xml_string[current_char], "\t</row>\n");
        current_char += 8;

    }

    fill_buffer(&xml_string[current_char], "</data>\n");
    current_char += 8;
    xml_string[current_char] = '\0';


    // // Write to file
    // fprintf(xml_file, "%s\n", xml_string);

    // fclose(xml_file);

    self->xml = xml_string;

    return self->xml;
}




//          JOIN (DOES NOT WORK YET)
DataLynx *join(DataLynx *table1, DataLynx *table2, const char *on1, const char *on2, const char *how) {

    // BUG: if  rows dont exactly match up, will not work!!!!!!!!!!!!!!!!!!!!!!!!

    // Safety checks
    if (table1 == NULL || table2 == NULL) return NULL;
    if (!data_exists(table1) || !data_exists(table2)) return NULL;
    if (on1 == NULL || on2 == NULL || how == NULL) return NULL;


    // Find column indexes
    int16_t on1_index = findColumnIndex(table1, on1);
    int16_t on2_index = findColumnIndex(table2, on2);
    if (on1_index < 0 || on2_index < 0) return NULL;

    const char *func_name = "join";

    typedef struct RowJoin {
        uintmax_t row1;
        uintmax_t row2;
    } RowJoin;


    // Allocate buffer to keep track of which rows to merge
    uintmax_t row_join_buffer_size = (table1->rowCount > table2->rowCount) ? table1->rowCount : table2->rowCount;

    RowJoin *row_joins = (RowJoin*)malloc(sizeof(RowJoin) * row_join_buffer_size);
    if (row_joins == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

    uintmax_t num_rows_to_join = 0;


    uintmax_t row1 = 0;
    uintmax_t row2 = 0;

    while (row1 < table1->rowCount && row2 < table2->rowCount) {

        if (strcmp(get_field_(table1, row1, on1_index), get_field_(table2, row2, on2_index)) == 0) {
            row_joins[num_rows_to_join].row1 = row1;
            row_joins[num_rows_to_join++].row2 = row2;
        }

        row1++;
        row2++;
    }

    // Create merged
    DataLynx *merged = (DataLynx*)malloc(sizeof(DataLynx));
    if (merged == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

    merged->columnCount = table1->columnCount + table2->columnCount;
    merged->rowCount = num_rows_to_join;

    merged->grid_v3 = (char***)malloc(sizeof(char**) * merged->rowCount);
    if (merged->grid_v3 == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}


    for (uintmax_t row = 0; row < num_rows_to_join; row++) {
    printf("JOIN: %ld\n\n",num_rows_to_join);

        // Allocate row
        merged->grid_v3[row] = (char**)malloc(sizeof(char*) * merged->columnCount);
        if (merged->grid_v3[row] == NULL) {log_error(MALLOC_FAILED, func_name); freeAll(merged); return NULL;}

        char *field = NULL;

        // Copy columns from table 1 to merged
        for (uint16_t column1 = 0; column1 < table1->columnCount; column1++) {
            field = get_field_(table1, row, column1);

            size_t field_strlen = strlen(field);

            // Allocate string
            merged->grid_v3[row][column1] = (char*)malloc(sizeof(char) * (field_strlen+1));
            if (merged->grid_v3[row][column1] == NULL) {log_error(MALLOC_FAILED, func_name); freeAll(merged); return NULL;}

            strcpy(merged->grid_v3[row][column1], field);

        }

        // Copy columns from table 2 to merged
        for (uint16_t column2 = merged->columnCount-table2->columnCount; column2 < table2->columnCount; column2++) {
            field = get_field_(table2, row, column2);

            size_t field_strlen = strlen(field);

            // Allocate string
            merged->grid_v3[row][column2] = (char*)malloc(sizeof(char) * (field_strlen+1));
            if (merged->grid_v3[row][column2] == NULL) {log_error(MALLOC_FAILED, func_name); freeAll(merged); return NULL;}

            strcpy(merged->grid_v3[row][column2], field);
        }
    }



    return merged;
}

