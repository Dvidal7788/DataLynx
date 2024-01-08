#include <DataLynx_stats.h>
#include <DataLynx_data.h>
#include <DataLynx_util.h>
#include <math.h>


//                          ------- STATS -------



//          __ CREATE_STATS() ____
bool create_stats(DataLynx *self) {

    /* Do not know if column is numeric yet at this point, bc have not gone through whol data set (not done one column at a time, done row by row) */
    if (self->aggregate != NULL) {
        free_value_counts(self);
        free(self->aggregate);
    }

    const char *func_name = "create_stats";

    // Create array of Stats structs
    self->aggregate = malloc(sizeof(Aggregate)*self->columnCount);
    if (self->aggregate == NULL) {log_error(MALLOC_FAILED, func_name); return false;}


    // Initialize
    for (uintmax_t c = 0; c < self->columnCount; c++) {
        Aggregate *tmp = &self->aggregate[c];

        tmp->column_name = self->__header__[c];
        tmp->min = tmp->max = tmp->lower_qrt = tmp->upper_qrt = tmp->median = tmp->sum = tmp->mean = tmp->std = tmp->is_null = tmp->not_null = 0;
        tmp->is_number = false;
        tmp->longest_field_strlen = 0;
        tmp->value_counts = NULL;
        create_value_counts(self, c);
    }

    return true;
}


//      ___ Stats() ___
double getStat(DataLynx *self, char *column_name, char *stat) {

    // Safety checks
    if (self == NULL || column_name == NULL || stat == NULL) return 0;


    // Retrieve corresponding column index
    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return 0;

    return get_stat_internal_(self, column_index, stat);

}


double getStatIdx(DataLynx *self, uint32_t column, char *stat) {

    // Safety checks
    if (self == NULL || stat == NULL) return 0;

    if (column > self->columnCount-1) return 0;

    return get_stat_internal_(self, column, stat);

}

double get_stat_internal_(DataLynx *self, uint32_t column, char *stat) {

    char *column_name = self->__header__[column];

    // 1. Retrieve and return stats
    if (self->aggregate != NULL) {

        if (strcasecmp(stat, "min") == 0) return self->aggregate[column].min;
        else if (strcasecmp(stat, "max") == 0) return self->aggregate[column].max;
        else if (strcasecmp(stat, "sum") == 0) return self->aggregate[column].sum;
        else if (strcasecmp(stat, "mean") == 0 || strcasecmp(stat, "avg") == 0 || strcasecmp(stat, "average") == 0) return self->aggregate[column].mean;
        else if (strcasecmp(stat, "std") == 0 || strcasecmp(stat, "standard deviation") == 0) return self->aggregate[column].std;
        else if (strcasecmp(stat, "lower qrt") == 0 || strcasecmp(stat, "lower quartile") == 0 || strcasecmp(stat, "25th %") == 0 || strcasecmp(stat, "25th percentile") == 0) return self->aggregate[column].lower_qrt;
        else if (strcasecmp(stat, "median") == 0) return self->aggregate[column].median;
        else if (strcasecmp(stat, "upper qrt") == 0 || strcasecmp(stat, "upper quartile") == 0 || strcasecmp(stat, "75th %") == 0 || strcasecmp(stat, "75th percentile") == 0) return self->aggregate[column].upper_qrt;
        else if (strcasecmp(stat, "isnull") == 0 || strcasecmp(stat, "is null") == 0) return self->aggregate[column].is_null;
        else if (strcasecmp(stat, "notnull") == 0 || strcasecmp(stat, "not null") == 0) return self->aggregate[column].not_null;
    }
    // 2. Calculate stats if none to retrieve
    else if (self->dict_grid != NULL) return aggregate_dict_grid(self, column_name, stat);
    else if (self->grid_v3 != NULL) return aggregate_grid_v3(self, column_name, stat);
    else if (self->grid != NULL) return aggregate_grid(self, column_name, stat);
    else if (self->rows != NULL) return aggregate_rows(self, column_name, stat);
    else if (self->raw != NULL) return aggregate_raw(self, column_name, stat);

    // Return 0 if nothing is found
    return 0;
}

/* The following batch of functions will not have safety checks, because getStat() has safety checks, so no need for redundancy
    NOTE: Although all of these are available through getStat, I will make these available through their own functions as well, if for nothing else other than the simple fact that getStat can only return a double, and the user might want to keep is_null/not_null as an integer without having to typecast */

double min(DataLynx *self, char *column_name) {return getStat(self, column_name, "min");}

double max(DataLynx *self, char *column_name) {return getStat(self, column_name, "max");}

double sum(DataLynx *self, char *column_name) {return getStat(self, column_name, "sum");}

double mean(DataLynx *self, char *column_name) {return getStat(self, column_name, "mean");}

double std(DataLynx *self, char *column_name) {return getStat(self, column_name, "std");}

double lowerQrt(DataLynx *self, char *column_name) {return getStat(self, column_name, "lower qrt");}

double median(DataLynx *self, char *column_name) {return getStat(self, column_name, "median");}

double upperQrt(DataLynx *self, char *column_name) {return getStat(self, column_name, "upper qrt");}

uintmax_t isNull(DataLynx *self, char *column_name) {return (uintmax_t)getStat(self, column_name, "isnull");}

uintmax_t notNull(DataLynx *self, char *column_name) {return (uintmax_t)getStat(self, column_name, "notnull");}


//          FIND MEDIAN
bool find_median(DataLynx *self) {

    // This function does not account for empty fields yet
    self->in_place_sort = false;

    // Sort each column (not in place)
    for (uint16_t column = 0; column < self->columnCount; column++) {

        if (!self->aggregate[column].is_number) continue;

        sortRowsByColumn(self, self->__header__[column], "asc"); /* This must go first, otherwise seg fault when accessing tmp_column if rowCount is 1*/

        if (self->rowCount == 1) {
            self->aggregate[column].lower_qrt = self->aggregate[column].median = self->aggregate[column].upper_qrt = atof(self->tmp_column[0]);
            continue;
        }

        // TO DO: Copy only non-null rows

        double median = 0;
        uint32_t lower_qrt_index, upper_qrt_index;

        // Find median, upper & lower quartiles (25, 50 and 75th percentiles)
        if (self->rowCount & 1) {

            // Odd row count
            uint32_t median_index = self->rowCount / 2;

            median = atof(self->tmp_column[median_index]);

            // Upper/lower quartiles
            lower_qrt_index = median_index / 2;
            upper_qrt_index = median_index + lower_qrt_index + 1;

        }
        else {
            // Even row count

            // Find 2 middle numbers
            uint32_t median_index2 = self->rowCount / 2;
            uint32_t median_index1 = median_index2 - 1;

            double median1 = atof(self->tmp_column[median_index1]);
            double median2 = atof(self->tmp_column[median_index2]);

            // Calc median for even rows (average of 2 middle numbers)
            median = (median1 + median2) / 2;

            // Upper/lower quartiles
            lower_qrt_index = median_index1 / 2;
            upper_qrt_index = median_index2 + lower_qrt_index;

        }

        // Assign median for this column
        self->aggregate[column].median = median;
        self->aggregate[column].lower_qrt = atof(self->tmp_column[lower_qrt_index]);
        self->aggregate[column].upper_qrt = atof(self->tmp_column[upper_qrt_index]);

    }

    self->in_place_sort = true; /* Reset bool to default */

    free(self->tmp_column);
    self->tmp_column = NULL;

    return true;
}


// CALC STATS ONE AT A TIME:

//      ____AGGREGATE_RAW() _____
double aggregate_raw(DataLynx *self, char *column_name, char *operation) {

    if (self == NULL) return 0;
    if (self->raw == NULL) return 0;


    // Find integer index for column name
    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return 0;

    double sum = 0, min = 0, max = 0;
    uintmax_t cursor = 0;
    // Go through every row
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        // Find column

        // Find beginning of field
        uintmax_t  current_column = 0, field_char_count = 0;
        bool inside_quotes = false, field_found = false;
        while (true) {

            if ((intmax_t) current_column == column_index) {
                field_found = true;
            }

            if (self->raw[cursor] == '\0') break;
            else if (!inside_quotes && self->raw[cursor] == '\n') break;
            else if (!inside_quotes && self->raw[cursor] == ',') {
                if (field_found) break;
                current_column++;
            }
            else if (!inside_quotes && self->raw[cursor] == '\n') {
                break;
                // if (field_found) break;
                // current_column = 0;
            }
            else if (!inside_quotes && self->raw[cursor] == '"') inside_quotes = true;
            else if (inside_quotes && self->raw[cursor] == '"') inside_quotes = false;

            cursor++;

            if (field_found) field_char_count++;

        }

        if (!field_found) continue;

        // Allocate buffer
        char buffer[field_char_count+1];

        // Get cursor back to beginning of field
        cursor -= field_char_count;

        // Assign chars in field to buffer
        for (uintmax_t buffer_cursor = 0; buffer_cursor < field_char_count; buffer_cursor++) {
            buffer[buffer_cursor] = self->raw[cursor];
            cursor++;
        }

        buffer[field_char_count] = '\0';

        // Add to sum
        if (is_number(buffer)) {

            double field_as_float = atof(buffer);
            sum += field_as_float;

            // Set min and max to field from 1st row
            if (i == 0) min = max = field_as_float;

            if (field_as_float < min) min = field_as_float;
            if (field_as_float > max) max = field_as_float;

        }

        cursor++;


    }

    // Return either or min or max
    if (strcasecmp(operation, "min") == 0) return min;
    else if (strcasecmp(operation, "max") == 0) return max;

    // Return either sum or average
    return (strcasecmp(operation, "sum") == 0 || strcasecmp(operation, "add") == 0) ? sum : sum / self->rowCount;
}



//      ____AGGREGATE_ROWS() _____
double aggregate_rows(DataLynx *self, char *column_name, char *operation) {

    if (self == NULL) return 0;
    if (self->rows == NULL) return 0;

    // Find integer index for column name
    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return 0;

    double sum = 0, min = 0, max = 0;
    // Go through every row
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        // Find column

        // Find beginning of field
        uintmax_t cursor = 0, current_column = 0, field_char_count = 0;
        bool inside_quotes = false, field_found = false;
        while (true) {

            if ((intmax_t) current_column == column_index) {
                field_found = true;
            }


            if (self->rows[i][cursor] == '\0') break;
            else if (!inside_quotes && self->rows[i][cursor] == ',') {
                if (field_found) break;
                current_column++;
            }
            else if (!inside_quotes && self->rows[i][cursor] == '\n') {
                break;
                // if (field_found) break;
                // current_column = 0;
            }
            else if (!inside_quotes && self->rows[i][cursor] == '"') inside_quotes = true;
            else if (inside_quotes && self->rows[i][cursor] == '"') inside_quotes = false;

            cursor++;

            if (field_found) field_char_count++;

        }

        if (!field_found) continue;

        // Allocate buffer
        char buffer[field_char_count+1];

        // Get cursor back to beginning of field
        cursor -= field_char_count;

        // Assign chars in field to buffer
        for (uintmax_t buffer_cursor = 0; buffer_cursor < field_char_count; buffer_cursor++) {
            buffer[buffer_cursor] = self->rows[i][cursor];
            cursor++;
        }

        buffer[field_char_count] = '\0';


        // Add to sum
        if (is_number(buffer)) {

            double field_as_float = atof(buffer);
            sum += field_as_float;

            // Set min and max to field from 1st row
            if (i == 0) min = max = field_as_float;

            if (field_as_float < min) min = field_as_float;
            if (field_as_float > max) max = field_as_float;

        }

    }

    // Return either or min or max
    if (strcasecmp(operation, "min") == 0) return min;
    else if (strcasecmp(operation, "max") == 0) return max;

    // Return either sum or average
    return (strcasecmp(operation, "sum") == 0 || strcasecmp(operation, "add") == 0) ? sum : sum / self->rowCount;
}


//      ____AGGREGATE_GRID_V3() _____
double aggregate_grid_v3(DataLynx *self, char *column_name, char *operation) {

    if (self == NULL || column_name == NULL || operation == NULL) return 0;
    if (self->grid_v3 == NULL) return 0;

    // Find integer index for column name
    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return 0;

    double sum = 0, min = 0, max = 0;
    char *field = NULL;

    // Go through every row
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        // Access field
        field = self->grid_v3[row][column_index];

        // Add to sum
        if (is_number(field)) {

            double field_as_float = atof(field);
            sum += field_as_float;

            // Set min and max to field from 1st row
            if (row == 0) min = max = field_as_float;

            if (field_as_float < min) min = field_as_float;
            if (field_as_float > max) max = field_as_float;

        }
    }



    // Return either or min or max
    if (strcasecmp(operation, "min") == 0) return min;
    else if (strcasecmp(operation, "max") == 0) return max;

    // Return either sum or average
    return (strcasecmp(operation, "sum") == 0 || strcasecmp(operation, "add") == 0) ? sum : sum / self->rowCount;
}


//      ____AGGREGATE_GRID() _____
double aggregate_grid(DataLynx *self, char *column_name, char *operation) {

    if (self == NULL || column_name == NULL || operation == NULL) return 0;
    if (self->grid == NULL) return 0;

    // Find integer index for column name
    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return 0;

    double sum = 0, min = 0, max = 0;

    // Go through every row
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        // Find column
        node *cursor = self->grid[i];

        for (int32_t j = 0; j < column_index; j++) cursor = cursor->next;

        if (cursor->s == NULL) continue;

        // Add to sum
        if (is_number(cursor->s)) {

            double field_as_float = atof(cursor->s);
            sum += field_as_float;

            // Set min and max to field from 1st row
            if (i == 0) min = max = field_as_float;

            if (field_as_float < min) min = field_as_float;
            if (field_as_float > max) max = field_as_float;

        }
    }



    // Return either or min or max
    if (strcasecmp(operation, "min") == 0) return min;
    else if (strcasecmp(operation, "max") == 0) return max;

    // Return either sum or average
    return (strcasecmp(operation, "sum") == 0 || strcasecmp(operation, "add") == 0) ? sum : sum / self->rowCount;
}


//      ____AGGREGATE_DICT_GRID() _____
double aggregate_dict_grid(DataLynx *self, char *column_name, char *operation) {

    if (self == NULL || column_name == NULL || operation == NULL) return 0;
    if (self->dict_grid == NULL) return 0;

    double sum = 0, min = 0, max = 0;

    // Go through every row
    for (uintmax_t i = 0; i < self->rowCount; i++) {

        // Find column
        dict_node *cursor = self->dict_grid[i];

        while (cursor != NULL) {
            if (strcmp(column_name, cursor->column_name) == 0) break;
            cursor = cursor->next;
        }

        if (cursor == NULL) continue;
        if (cursor->s == NULL) continue;

        // Add to sum
        if (is_number(cursor->s)) {

            double field_as_float = atof(cursor->s);
            sum += field_as_float;

            // Set min and max to field from 1st row
            if (i == 0) min = max = field_as_float;

            if (field_as_float < min) min = field_as_float;
            if (field_as_float > max) max = field_as_float;

        }
    }



    // Return either or min or max
    if (strcasecmp(operation, "min") == 0) return min;
    else if (strcasecmp(operation, "max") == 0) return max;

    // Return either sum or average
    return (strcasecmp(operation, "sum") == 0 || strcasecmp(operation, "add") == 0) ? sum : sum / self->rowCount;
}


//          -- Calc Mean() --
void calc_mean(DataLynx *self) {

    for (uintmax_t c = 0; c < self->columnCount; c++) {

        if (!self->aggregate[c].is_number) continue; /* is_number being false means there was not 1 numeric value in the entire column */

        self->aggregate[c].mean = self->aggregate[c].sum / self->aggregate[c].not_null;
    }

    return;
}

//      -- Calc Std() --
void calc_std(DataLynx *self) {

    for (uint32_t column = 0; column < self->columnCount; column++) {

        if (!self->aggregate[column].is_number) continue; /* is_number being false means there was not 1 numeric value in the entire column */

        double sum_squared_differences = 0;

        for (uint32_t row = 0; row < self->rowCount; row++) {

            double difference = 0;

            if (self->grid_v3 != NULL && self->grid_v3[row][column][0] != '\0') difference = atof(self->grid_v3[row][column]) - self->aggregate[column].mean;
            else if (self->grid != NULL) {
                node *tmp = self->grid[row];


                for (uint32_t i = 0; i != column && tmp != NULL; i++) tmp = tmp->next;

                if (tmp != NULL) difference = atof(tmp->s) - self->aggregate[column].mean;
            }
            else if (self->dict_grid != NULL) {
                dict_node *tmp = self->dict_grid[row];


                for (uint32_t i = 0; i != column && tmp != NULL; i++) tmp = tmp->next;

                if (tmp != NULL) difference = atof(tmp->s) - self->aggregate[column].mean;
            }

            sum_squared_differences += difference * difference;

        }

        double variance = sum_squared_differences / self->aggregate[column].not_null;
        self->aggregate[column].std = sqrt(variance);

    }

    return;
}




// bool calc_median(DataLynx *self) {
//     typedef struct Median {
//         uintmax_t greater;
//         uintmax_t less;
//     } Median;

//     Median tracker[self->columnCount];
//     for (uintmax_t c = 0; c < self->columnCount; c++) tracker[c].greater = tracker[c].less = 0;


//     for (uintmax_t row = 0; row < self->rowCount; row++) {

//         node *grid_tmp = NULL;
//         dict_node *dict_tmp = NULL;

//         if (self->grid != NULL) grid_tmp = self->grid[row];
//         else if (self->dict_grid != NULL) dict_tmp = self->dict_grid[row];

//         for (uintmax_t column = 0; column < self->columnCount; column++) {
//             if (!self->aggregate[column].is_number) continue;

//             if (row == 0) {

//                 if (self->grid_v3 != NULL) {
//                     tracker[column].greater = tracker[column].less = self->grid_v3[row][column];
//                 }
//                 else if (self->grid != NULL) {
//                     tracker[column].greater = tracker[column].less = grid_tmp->s;
//                 }
//                 else if (self->dict_grid != NULL) {
//                     tracker[column].greater = tracker[column].less = dict_tmp->s;
//                 }

//             }
//             else {

//                 if (self->grid_v3 != NULL) {
//                     tracker[column].greater = tracker[column].less = self->grid_v3[row][column];
//                 }
//                 else if (self->grid != NULL) {
//                     tracker[column].greater = tracker[column].less = grid_tmp->s;
//                 }
//                 else if (self->dict_grid != NULL) {
//                     tracker[column].greater = tracker[column].less = dict_tmp->s;
//                 }

//             }

//         }
//     }
// }

// // Swap two integers
// void swap(int* a, int* b) {
//     int temp = *a;
//     *a = *b;
//     *b = temp;
// }

// // Bubble sort algorithm
// void bubble_sort(DataLynx *self, int n) {

//     for (int i = 0; i < self->rowCount; i++) {
//         for (int j = 0; j < self->rowCount - i - 1; j++) {


//             if (arr[j] > arr[j + 1]) {
//                 swap(&arr[j], &arr[j + 1]);
//             }
//         }
//     }
// }

//          FIND NEW MIN()
double find_new_min(DataLynx *self, uintmax_t column, double old_field_float, double new_field_float) {

    bool pivot_assigned = false; /* Only used if caled from dropRow()*/

    // Find new min
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        node *grid_tmp = NULL;
        dict_node *dict_tmp = NULL;

        double tmp_s_float;

        if (self->grid_v3 != NULL) {
            tmp_s_float = atof(self->grid_v3[row][column]);
        }
        else if (self->grid != NULL) {

            grid_tmp = self->grid[row];

            // Get cursor to correct column
            for (uint16_t c = 0; c < column && grid_tmp != NULL; c++) grid_tmp = grid_tmp->next;

            tmp_s_float = atof(grid_tmp->s);

        }
        else if (self->dict_grid != NULL) {

            dict_tmp = self->dict_grid[row];

            // Get cursor to correct column
            for (uint16_t c = 0; c < column && dict_tmp != NULL; c++) dict_tmp = dict_tmp->next;

            tmp_s_float = atof(dict_tmp->s);

        }

        if (self->row_to_drop >= 0 && (intmax_t)row != self->row_to_drop && !pivot_assigned) {
            // if called from dropRow() (i.e self->row_to_drop set to -1 if not in use) and this is not the row we are dropping, use this as the starting point
            self->aggregate[column].min = tmp_s_float;
            pivot_assigned = true;
        }
        else if (row == 0 && self->row_to_drop < 0) {
            self->aggregate[column].min = (old_field_float != tmp_s_float) ? tmp_s_float : new_field_float;
        }
        else if (old_field_float != tmp_s_float && tmp_s_float < self->aggregate[column].min) self->aggregate[column].min = tmp_s_float;
        else if (old_field_float == tmp_s_float && new_field_float < self->aggregate[column].min) self->aggregate[column].min = tmp_s_float;

    }

    return self->aggregate[column].min;
}

//          FIND NEW MAX()
double find_new_max(DataLynx *self, uintmax_t column, double old_field_float, double new_field_float) {

    bool pivot_assigned = false; /* Only used if caled from dropRow()*/

    // Find new max
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        node *grid_tmp = NULL;
        dict_node *dict_tmp = NULL;

        double tmp_s_float;

        if (self->grid_v3 != NULL) {
            tmp_s_float = atof(self->grid_v3[row][column]);
        }
        else if (self->grid != NULL) {

            grid_tmp = self->grid[row];

            // Get cursor to correct column
            for (uint16_t c = 0; c < column && grid_tmp != NULL; c++) grid_tmp = grid_tmp->next;

            tmp_s_float = atof(grid_tmp->s);

        }
        else if (self->dict_grid != NULL) {

            dict_tmp = self->dict_grid[row];

            // Get cursor to correct column
            for (uint16_t c = 0; c < column && dict_tmp != NULL; c++) dict_tmp = dict_tmp->next;

            tmp_s_float = atof(dict_tmp->s);

        }

        if (self->row_to_drop >= 0 && (intmax_t)row != self->row_to_drop && !pivot_assigned) {
            // if called from dropRow() (i.e self->row_to_drop set to -1 if not in use) and this is not the row we are dropping, use this as the starting point
            self->aggregate[column].max = tmp_s_float;
            pivot_assigned = true;
        }
        else if (row == 0 && self->row_to_drop < 0) {
            self->aggregate[column].max = (old_field_float != tmp_s_float) ? tmp_s_float : new_field_float;
        }
        else if (old_field_float != tmp_s_float && tmp_s_float > self->aggregate[column].max) self->aggregate[column].max = tmp_s_float;
        else if (old_field_float == tmp_s_float && new_field_float > self->aggregate[column].max) self->aggregate[column].max = tmp_s_float;

    }

    return self->aggregate[column].max;
}

//      UPDATE STATS()
bool update_stats(DataLynx *self, uintmax_t column, char *old_field, char *new_field) {

    /* THIS FUNCTION: updates stats when CHANGING/UPDATING an EXISTING VALUE to a NEW VALUE. Not to be confused with update_stats_new_row() which updates stats when a whole new row of data is inserted.
        - Must check if 1st char is '\0' because we do not need to add empty strings to value counts. Thats what .is_null is for.*/

    // Do not update if fields are identical
    if (new_field != NULL && strcmp(old_field, new_field) == 0) return false;

    /* Issues with updating after drop row:
        - is null/not null
        - numeric: new_field gets seen as 0 */


    if (new_field == NULL && self->aggregate[column].is_number) {

        /* Called from dropRow */

        double old_field_float = atof(old_field);

        // Find new min/max if necessary
        if (old_field_float == self->aggregate[column].min) find_new_min(self, column, old_field_float, 0);
        if (old_field_float == self->aggregate[column].max) find_new_max(self, column, old_field_float, 0);


    }
    else if (new_field != NULL && self->aggregate[column].is_number) {

        double old_field_float = atof(old_field);
        double new_field_float = atof(new_field);

        // Update stats - determine if new updated field sets new min or max, if not, but the min or max was the field changed, we must find new min/max
        if (new_field_float < self->aggregate[column].min) self->aggregate[column].min = new_field_float;
        else if (new_field_float > self->aggregate[column].max) self->aggregate[column].max = new_field_float;

        // Find new min or max - (can not be else if, bc if we changed the field that had the previous min, but we set the new max, we still need to find the new min or vice versa)
        if (old_field_float == self->aggregate[column].min && new_field_float > old_field_float) {

            /* if the current min or max has been changed, but does not set a new min or max, we must iterate through the data of that column and find the new min or max */
            find_new_min(self, column, old_field_float, new_field_float);

        }
        else if (old_field_float == self->aggregate[column].max && new_field_float < old_field_float) {
            find_new_max(self, column, old_field_float, new_field_float);
        }


        // Recalculate sum and mean
        self->aggregate[column].sum += (-1 * old_field_float) + new_field_float;
        self->aggregate[column].mean = self->aggregate[column].sum / self->aggregate[column].not_null;
    }
    else if (new_field != NULL && new_field[0] != '\0') {
        // Update value counts
        bool increment = false;
        increment_decrement_value_count(self, self->__header__[column], old_field, increment);

        increment = true;
        increment_decrement_value_count(self, self->__header__[column], new_field, increment);

    }
    else if (new_field != NULL && new_field[0] == '\0' && old_field[0] != '\0') {
        bool increment = false;
        increment_decrement_value_count(self, self->__header__[column], old_field, increment);
    }
    else if (new_field == NULL) {
        // printf("INSIDE UPDATE STATS: column: %s\n", column);
        bool increment = false;
        increment_decrement_value_count(self, self->__header__[column], old_field, increment);
    }


    // Handle is null/ not null

    if (new_field != NULL && new_field[0] == '\0' && old_field[0] != '\0') {
        self->aggregate[column].is_null++;
        self->aggregate[column].not_null--;
    }
    else if (new_field != NULL && new_field[0] != '\0' && old_field[0] == '\0') {
        self->aggregate[column].is_null--;
        self->aggregate[column].not_null++;
    }
    else if (new_field == NULL) {
        // Called from dropRow()- which mean only need to decrement is/not_null and NOT increment the other
        if (old_field[0] == '\0') self->aggregate[column].is_null--;
        else self->aggregate[column].not_null--;
    }


    return true;

}



void update_stats_new_row(DataLynx *self, char *values[]) {

    for (uint32_t column = 0; column < self->columnCount; column++) {

        size_t field_strlen = strlen(values[column]);
        if (self->aggregate[column].longest_field_strlen < field_strlen) self->aggregate[column].longest_field_strlen = field_strlen;

        if (is_number(values[column])) self->aggregate[column].is_number = true;
        else self->aggregate[column].is_number = false;

        if (values[column][0] == '\0') {self->aggregate[column].is_null++; continue;}
        else self->aggregate[column].not_null++;


        if (self->aggregate[column].is_number) {

            double value_as_float = atof(values[column]);

            // Inserting 1st row
            if (self->rowCount == 1) {
                self->aggregate[column].min = self->aggregate[column].max = self->aggregate[column].sum = self->aggregate[column].mean = self->aggregate[column].median = value_as_float;
                continue;
            }


            if (value_as_float < self->aggregate[column].min) self->aggregate[column].min = value_as_float;

            if (value_as_float > self->aggregate[column].max) self->aggregate[column].max = value_as_float;

            self->aggregate[column].sum += value_as_float;

            self->aggregate[column].mean = self->aggregate[column].sum / self->aggregate[column].not_null;

        }
        else {
            // value counts
            bool increment = true;
            increment_decrement_value_count(self, self->__header__[column], values[column], increment);
        }

    }

    return;
}


//      CREATE VALUE COUNTS()
bool create_value_counts(DataLynx *self, uintmax_t column_index) {

    // if (self->aggregate[column_index].is_number) return false;

    char *func_name = "create_value_counts";

    // Allocate array (A-Z) + one spot for other (i.e. strings that start with digits or punctuation), 27 total
    self->aggregate[column_index].value_counts = (ValueCount**)malloc(sizeof(ValueCount*)*27);
    if (self->aggregate[column_index].value_counts == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // Initialize to NULL
    for (uint8_t i = 0; i < 27; i++) {
        self->aggregate[column_index].value_counts[i] = NULL;
    }


    return true;
}



// //      SORT VALUE COUNTS()
// void sort_value_counts(DataLynx *self) {


//     for (uintmax_t i = 0; i < self->columnCount; i++) {

//         // For each letter + non alpha
//         for (uintmax_t l = 0; l < 27; l++) {

//             // uintmax_t min = self->aggregate[i].value_counts->count;
//             // uintmax_t max = self->aggregate[i].value_counts->count;
//             ValueCount *cur, *min_node, *max_node, *b4_cur, *b4_min, *b4_max, *last;
//             cur = b4_cur = min_node = max_node = self->aggregate[i].value_counts[l];

//             cur = cur->next;
//             while (cur != NULL) {

//                 // Min value count
//                 if (b4_cur->count < min_node->count) {
//                     min_node = cur;
//                     b4_min = b4_cur;
//                 }


//                 // Max value count
//                 if (b4_cur->count > max_node->count) {
//                     max_node = cur;
//                     b4_max = b4_cur;
//                 }


//                 if (cur->next == NULL) last = cur;

//                 cur = cur->next;
//                 b4_cur = b4_cur->next;
//             }

//             // // Rearrange nodes
//             // if (max_node != self->aggregate[i].value_counts[l]) {
//             //     // Swap max_node with 1st position
//             //     cur = self->aggregate[i].value_counts[l];
//             //     ValueCount * max_tmp = max_node;

//             //     self->aggregate[i].value_counts[l] = max_node;
//             //     max_node->next = cur->next;

//             //     self->aggregate[i].value_counts[l] = max_tmp;
//             //     self->aggregate[i].value_counts[l] = max_tmp->next;
//             //     b4_max->next = cur;

//             // }



//             // // Rearrange nodes
//             // if (min_node != last) {
//             //     // Swap min_node with 1st position
//             //     cur = self->aggregate[i].value_counts[l];
//             //     ValueCount *min_tmp = min_node;

//             //     self->aggregate[i].value_counts[l] = min_node;
//             //     min_node->next = cur->next;

//             //     self->aggregate[i].value_counts[l] = min_tmp;
//             //     self->aggregate[i].value_counts[l] = min_tmp->next;
//             //     b4_min->next = cur;

//             // }




//         }


//     }

//     return;
// }

void sort_value_counts(DataLynx *self) {

    for (uintmax_t column; column < self->columnCount; column++) {

        if (self->aggregate[column].is_number) continue;

        for (uint8_t a = 0; a < 27; a++) {

            ValueCount *tmp_i = self->aggregate[column].value_counts[a];

            if (tmp_i == NULL) continue;

            ValueCount *tmp_j = tmp_i->next;

            while (tmp_i != NULL) {

                ValueCount *smallest = tmp_i;

                while  (tmp_j != NULL) {

                    if (tmp_j->count > tmp_i->count) {
                        smallest = tmp_j;
                    }

                    tmp_j = tmp_j->next;
                }

                if (smallest != tmp_i) {

                    ValueCount *tmp, *cursor, *cursor_prev;
                    tmp = cursor = cursor_prev = self->aggregate[column].value_counts[a];

                    uint32_t counter = 0;
                    while (cursor != smallest && cursor != NULL) {
                        if (counter != 0) cursor_prev = cursor_prev->next;
                        cursor = cursor->next;
                        counter++;
                    }


                    cursor_prev->next = tmp;
                    tmp->next = cursor->next;

                    self->aggregate[column].value_counts[a] = cursor;


                }

                tmp_i = tmp_i->next;
            }


        }



    }
}

//      APPEND VALUE COUNTS()
ValueCount *prepend_value_count(DataLynx *self, uintmax_t column_index, char *value, uint8_t alpha_index) {

    if (self == NULL) return NULL;
    if (value == NULL) return NULL;

    const char *func_name = "prepend_value_count";

    //      - Prepend -

    // Allocate
    ValueCount *new_node = (ValueCount*)malloc(sizeof(ValueCount));
    if (new_node == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}

    // Assign
    new_node->value = (char*)malloc(sizeof(char)*(strlen(value)+1));
    if (new_node->value == NULL) {log_error(MALLOC_FAILED, func_name); return NULL;}
    strcpy(new_node->value, value);

    new_node->count = 1;
    new_node->grouped_data = NULL;
    // new_node->next = self->aggregate[column_index].value_counts[alpha_index];
    if (self->aggregate[column_index].value_counts[alpha_index] != NULL) new_node->next = self->aggregate[column_index].value_counts[alpha_index];
    else {
        self->aggregate[column_index].value_counts[alpha_index] = new_node;
        new_node->next = NULL;
    }

    self->aggregate[column_index].value_counts[alpha_index] = new_node;

    return new_node;

}


//      ADD VALUE COUNT()
bool increment_decrement_value_count(DataLynx *self, char *column_name, char *value, bool increment) {

    if (self == NULL) return false;
    if (column_name == NULL) return false;
    if (value == NULL)  return false;
    else if (value[0] == '\0') return false;


    intmax_t column_index = findColumnIndex(self, column_name);
    if (column_index < 0) return false;

    uint8_t alpha_index = find_alpha_index(value);

    ValueCount *tmp, *b4_tmp = NULL;
    tmp = b4_tmp = self->aggregate[column_index].value_counts[alpha_index];

    // Traverse list until we find the correct value
    uintmax_t node_index = 0;
    while (tmp != NULL) {

        // Check if values match
        if (!self->case_sensitive_value_counts) {

            if (strcasecmp(tmp->value, value) == 0) {

                // Decrement or Increment
                if (!increment) {

                    tmp->count--;

                    // Remove node if neccessary
                    if (tmp->count < 1) {

                        if (tmp == b4_tmp) self->aggregate[column_index].value_counts[alpha_index] = tmp->next;
                        else b4_tmp->next = tmp->next;

                        free_null(&tmp->value);
                        free(tmp);
                        tmp = NULL;
                    }

                }
                else tmp->count++;

                return true;
            }
        }
        else {
            if (strcmp(tmp->value, value) == 0) { /*make this match strcasecmp() section, when its working properly */
                                // Decrement or Increment
                if (!increment) {

                    tmp->count--;

                    // Remove node if neccessary
                    if (tmp->count < 1) {

                        if (tmp == b4_tmp) self->aggregate[column_index].value_counts[alpha_index] = tmp->next;
                        else b4_tmp->next = tmp->next;

                        free_null(&tmp->value);
                        free(tmp);
                        tmp = NULL;
                    }

                }
                else tmp->count++;

                return true;
            }
        }

        if (node_index != 0)  b4_tmp = tmp;
        tmp = tmp->next;
        node_index++;
    }

    // If did not find value already in value_counts, add (i.e. prepend) it to the correct list
    prepend_value_count(self, column_index, value, alpha_index);

    return true;
}


//          REMOVE VALUE COUNT NODE()
void remove_value_count_node(DataLynx *self, uintmax_t column_index, uint8_t alpha_index, uintmax_t node_index) {

    /* MAKE SURE THIS WORKS IF node_index == 0*/

    ValueCount *tmp, *b4_tmp = NULL;
    tmp = b4_tmp = self->aggregate[column_index].value_counts[alpha_index];

    for (uintmax_t i = 0; i < node_index; i++) {
        if (tmp == NULL) break;
        if (i != 0) b4_tmp = b4_tmp->next;

        tmp = tmp->next;
    }

    b4_tmp->next = tmp->next;

    free(tmp);

    return;

}


uint16_t valueCount(DataLynx *self, char *value, char *column_name) {

    // Safety checks
    if (self == NULL || value == NULL || column_name == NULL) return 0;

    // Get column integer index/determine if column name is valid
    int16_t column_index;
    if ((column_index = findColumnIndex(self, column_name)) < 0) return 0;

    return value_count_internal_(self, value, column_index);
}


//          IS IN COLUMN()
bool isInColumn(DataLynx *self, char *value, char *column_name) {
    return valueCount(self, value, column_name);
}


//          IS IN DATA()
bool isInData(DataLynx *self, char *value, char *column_name) {

    // Safety checks
    if (self == NULL || value == NULL || column_name == NULL) return 0;

    // Iterate through columns, checking if value appears
    for (uintmax_t column = 0; column < self->columnCount; column++) {

        if (value_count_internal_(self, value, column) > 0) return true;
    }

    return false;

}

uint16_t value_count_internal_(DataLynx *self, char *value, int16_t column_index) {

    // THIS FUNCTION: Retrieves value count of a given value

    // Find correct "hash" (i.e. alpha index 0-27 => A-Z + punctuation)
    uint8_t alpha_index = find_alpha_index(value);

    ValueCount *tmp = self->aggregate[column_index].value_counts[alpha_index];

    // Traverse linked list
    while (tmp != NULL && strcmp(value, tmp->value) != 0) {tmp = tmp->next;}

    return (tmp != NULL) ? tmp->count : 0;
}






//              PRINT PIVOT TABLE()
bool printPivotTable(DataLynx *self, char *group_by, char *aggregated_column, char *stat) {

    // Safety checks
    if (self == NULL || group_by == NULL || aggregated_column == NULL || stat == NULL) return false;

    // Retreive corresponding column index for group by column
    int32_t column1 = findColumnIndex(self, group_by);
    if (column1 < 0) return false;

    // Retreive corresponding column index for aggregated column
    int32_t column2 = findColumnIndex(self, aggregated_column);
    if (column2 < 0) return false;


    // Currently, group by is only be available for non-numeric columns (i.e. categorical data) and aggregated data must be numeric
    if (self->aggregate[column1].is_number) return false;
    if (!self->aggregate[column2].is_number) return false;

    const char *func_name = "printPivotTable";
    char spaces[128];
    char spaces2[128];
    char dashes[256];
    char stat_string[128];
    size_t group_by_strlen = strlen(group_by);
    size_t longest_strlen = (group_by_strlen > self->aggregate[column1].longest_field_strlen) ? group_by_strlen : self->aggregate[column1].longest_field_strlen;
    size_t longest_aggregate_stat_strlen = 0;


    // Create spaces and dashes for header
    uint16_t num_spaces = longest_strlen - group_by_strlen;
    create_spaces_string(spaces, num_spaces+1, false, ' ');
    uint16_t num_dashes = longest_strlen + strlen(aggregated_column)+strlen(stat)+6;
    create_spaces_string(dashes, num_dashes+1, false, '-');

    for (uint8_t alpha_index = 0; alpha_index < 27; alpha_index++) {

        ValueCount *tmp = self->aggregate[column1].value_counts[alpha_index];

        // Traverse value counts linked list at current alpha index
        while (tmp != NULL) {

            // // Create and fill intenral DataLynx object if it does not already exist
            if (tmp->grouped_data == NULL) {

                // Allocate buffer for internal DataLynx object
                tmp->grouped_data = (DataLynx*)malloc(sizeof(DataLynx));
                if (tmp->grouped_data == NULL) {log_error(MALLOC_FAILED, func_name); return false;};

                // Initialize DataLynx object
                *tmp->grouped_data = DataLynxConstructor();

                // Filter out all rows that do not contain current unique value
                filter(self, tmp->grouped_data, group_by, "==", tmp->value);
            }

            /*NOTE: This is the reason we can not print during this loop. We need to loop through all the stats and find the longest one in order to print properly (i.e. right align the numeric stats) */
            // Keep track of longest 'string' (i.e. numeric stat converted to string) for the aggregated column
            sprintf(stat_string, "%.2f", getStatIdx(tmp->grouped_data, column2, stat));
            size_t current_stat_strlen = strlen(stat_string);
            if (current_stat_strlen > longest_aggregate_stat_strlen)
                longest_aggregate_stat_strlen = current_stat_strlen;

            tmp = tmp->next;
        }
    }


    // PRINT
    uint16_t num_spaces2 = 0;
    printf("\n\t<Pivot Table>\n%s%s | (%s) %s\n%s\n", group_by, spaces, stat, aggregated_column, dashes);
    for (uint8_t alpha_index = 0; alpha_index < 27; alpha_index++) {

        ValueCount *tmp = self->aggregate[column1].value_counts[alpha_index];

        // Traverse value counts linked list at current alpha index
        while (tmp != NULL) {

            num_spaces = longest_strlen - strlen(tmp->value);
            create_spaces_string(spaces, num_spaces+1, false, ' ');

            sprintf(stat_string, "%.2f", getStatIdx(tmp->grouped_data, column2, stat));
            size_t current_stat_strlen = strlen(stat_string);
            num_spaces2 = longest_aggregate_stat_strlen - current_stat_strlen;
            create_spaces_string(spaces2, num_spaces2+1, false, ' ');

            printf("%s%s | %s%.2f\n", tmp->value, spaces, spaces2, getStatIdx(tmp->grouped_data, column2, stat));

            tmp = tmp->next;
        }
    }

    return true;
}




//          PRINT STATS ALL
bool printStatsAll(DataLynx *self) {

    if (self == NULL) return false;

    if (self->aggregate == NULL) return false;

    return print_stats_internal_(self, PRINT_ALL_STATS);

}



//          PRINT STATS COLUMN
bool printStatsColumn(DataLynx *self, char *column_name) {

    if (self == NULL || column_name == NULL) return false;

    if (self->aggregate == NULL) return false;

    return print_stats_internal_(self, column_name);

}


        // PRINT STATS INTERNAL() (side by side) and Sorted Value Counts Works :)
bool print_stats_internal_(DataLynx *self, char *column_name) {
        /*
            12-21-23:
                - printStatsColumn() has errors and 2 blocks lost regardless of getBins.
                        - FIXED: Moved safety check to return before printing value counts up a little, before malloc of sorted array
                - printStatsAll() and printStatsColumn() seg faults when getBins() is called first (wasn't seg faulting before because was failing at findColumnIndex() bc "_binnned" had 3 n's)
                        - FIXED: Was overrunning stack-allocated buffers in print_is_not_null()
        */

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


    //                       ------------------------
    //                             VALUE COUNTS
    //                       ------------------------
    /* DO NOT pass into next section if not printing value counts. (non_numeric_column_count will be 0, not only of course in instances where there are no non-numeric columns in the dataset, but ALSO when this function is called from printStatsColumn() and the column requested is a numeric column, therfore skip the next section either way.) */
    if (non_numeric_column_count == 0) return true;

    if (!numeric_stats_in_data && !print_one_column) printf("\n* No numeric stats for this dataset *\n\n\n");
    // else if (!numeric_stats_in_data && print_one_column) printf("\n* No numeric stats for this column *\n\n\n");


    //              --- COPY VALUE COUNTS INTO SEPARATE ARRAYS TO SORT ---

    // Allocate main sorted array (i.e. array of arrays of sorted value counts per column)
    ValueCount **sorted = (ValueCount**)malloc(sizeof(ValueCount*) * non_numeric_column_count);
    if (sorted == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // Array to keep track of unique values for each non-numeric column
    uint16_t *unique_values = (uint16_t*)malloc(sizeof(uint16_t) * non_numeric_column_count);
    if (unique_values == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

    // Keep track of most unique values, because this determines how many new lines there will be
    uint16_t most_unique_values = 0;

    // Put Value Counts for current column into separate array
    for (c = 0; c < non_numeric_column_count; c++) {

        uint32_t column = non_numeric_columns[c];

        // Allocate array for sorted value counts
        uint32_t current_unique_values = 0;

        // Allocate array for sorted value counts of current column
        ValueCount *current_sorted = (ValueCount*)malloc(sizeof(ValueCount));
        if (current_sorted == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

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
                    if (current_sorted == NULL) {log_error(REALLOC_FAILED, func_name); return false;}
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
    // if (non_numeric_column_count == 0) return true;
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


//         // PRINT STATS INTERNAL() (side by side) and Sorted Value Counts Works :) ORIGINAL (before 12/21/23 changes)
// bool print_stats_internal_(DataLynx *self, char *column_name) {

//     // if (self == NULL) return false;

//     // if (self->aggregate == NULL || column_name == NULL) return false;

//     char *func_name = "print_stats_internal_";

//     // Determine whether to display all columns or just one (i.e. this does not work IF csv has a column named "All")
//     int16_t c = 0, start = 0, stop = self->columnCount;
//     bool print_one_column = false;
//     if (strcmp(column_name, PRINT_ALL_STATS) != 0) {

//         // Adjust start and stop to only print desired column
//         if ((c = findColumnIndex(self, column_name)) >= 0) stop = c + 1;
//         // else c = 0; /* if an invalid column name is given, we will print all stats */
//         else return false;

//         print_one_column = true;

//         start = c;

//     }

//     // Buffer to remember columns for value counts
//     uint16_t non_numeric_columns[self->columnCount];
//     uint16_t non_numeric_column_count = 0;

//     printf("\n\t\t<Data from file: %s>\n", self->filename);

//     if (!print_one_column || (print_one_column && self->aggregate[c].is_number) ) printf("\n\t\t- Numeric Stats - \n");

//     const int8_t STAT_COUNT = 10; /* This is the number of numeric stats we are printing per column
//                                     (i.e. Min, Max, Sum, Mean, 25th %, Median, 75th %, Std,
//                                     as well as Is Null and Not Null, which are also printed for non-numeric stats aka Value Counts) */


//     char space = ' ';
//     bool numeric_stats_in_data = false;

//     // For each stat: iterate through columns to find numeric columns
//     for (int8_t stat = -1; stat < STAT_COUNT; stat++) {

//         c = start;
//         // Iterate through columns to find numeric columns
//         while (c < stop) {

//             uint8_t column_strlen = strlen(self->__header__[c]) + 23; /* stat_print_() needs this to give proper spacing (to make the column stats line up nicely)*/

//             // Found numeric column
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
//                 /* functionally the same as saying else if (stat == -1 && !self->aggregate[c].is_number) */
//                 /* Only do this one time around, otherwise will count columns multiple times.*/

//                 // Remember which column indexes are non-numeric, so we do not have to iterate through each column and check again when we print the value counts
//                 non_numeric_columns[non_numeric_column_count] = c;
//                 non_numeric_column_count++;
//             }

//             c++;
//         }

//         /* Without this if statement, it will print new lines for every stat as it looks for numeric columns even when no numeric columns exist*/
//         if (numeric_stats_in_data) {
//             (stat == STAT_COUNT-1) ? printf("\n\n"): printf("\n");
//         }


//     }

//     if (!numeric_stats_in_data && !print_one_column) printf("\n* No numeric stats for this dataset *\n\n\n");
//     else if (!numeric_stats_in_data && print_one_column) printf("\n* No numeric stats for this column *\n\n\n");


//     //              --- COPY VALUE COUNTS INTO SEPARATE ARRAYS TO SORT ---

//     // Allocate main sorted array (i.e. array of arrays of sorted value counts per column)
//     ValueCount **sorted = (ValueCount**)malloc(sizeof(ValueCount*) * non_numeric_column_count);
//     if (sorted == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

//     // Array to keep track of unique values for each non-numeric column
//     uint16_t *unique_values = (uint16_t*)malloc(sizeof(uint16_t) * non_numeric_column_count);
//     if (unique_values == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

//     uint16_t most_unique_values = 0;

//     // Put Value Counts for current column into separate array
//     for (c = 0; c < non_numeric_column_count; c++) {

//         uint32_t column = non_numeric_columns[c];

//         // Allocate array for sorted value counts
//         uint32_t current_unique_values = 0;

//         // Allocate array for sorted value counts of current column
//         ValueCount *current_sorted = (ValueCount*)malloc(sizeof(ValueCount));
//         if (current_sorted == NULL) {log_error(MALLOC_FAILED, func_name); return false;}

//         bool first_value = true, values_exist = false;

//         // Iterate through alpha value count array
//         for (int l = 0; l < 27; l++) {
//             ValueCount *tmp = self->aggregate[column].value_counts[l];

//             // Traverse linked list for current alpha character
//             while (tmp != NULL) {

//                 values_exist = true;

//                 //  Realloc sorted array
//                 if (!first_value) {
//                     current_sorted = (ValueCount*)realloc(current_sorted, sizeof(ValueCount) * ((++current_unique_values) + 1));
//                     if (current_sorted == NULL) {log_error(REALLOC_FAILED, func_name); return false;}
//                 }
//                 else first_value = false;

//                 // Assign to sorted array to be sorted in next section
//                 current_sorted[current_unique_values].value = tmp->value;
//                 current_sorted[current_unique_values].count = tmp->count;

//                 tmp = tmp->next;

//             }

//         }

//         // Keep track of unique values for current non-numeric column
//         unique_values[c] = (values_exist) ? ++current_unique_values : 0;

//         if (current_unique_values > most_unique_values) most_unique_values = current_unique_values;

//         // Sort current column's value counts
//         for (uint32_t i = 0; i < current_unique_values; i++) {

//             uint32_t largest = i;

//             for (uint32_t j = i + 1; j < current_unique_values; j++) {

//                 if (current_sorted[j].count > current_sorted[largest].count) largest = j;
//             }

//             // Swap
//             if (i != largest) {
//                 ValueCount tmp = current_sorted[i];
//                 current_sorted[i] = current_sorted[largest];
//                 current_sorted[largest] = tmp;
//             }
//         }

//         // Attach array of sorted value counts for current column to array of arrays
//         sorted[c] = current_sorted;

//     }

//     //          --- PRINT VALUE COUNTS ---
//     if (non_numeric_column_count == 0) return true;
//     printf("\t\t- Value Counts -\n\t   (i.e. non-numeric stats)\n");

//     bool with_divider = true;

//     for (int32_t v = -2; v < most_unique_values+4; v++) {

//         for (uint32_t c = 0; c < non_numeric_column_count; c++) {

//             uint32_t column_index = non_numeric_columns[c];

//             // Column string length sets the length for that column
//             size_t column_strlen = strlen(self->__header__[column_index]) + 31;

//             // Print column name
//             if (v == -2) {
//                 has_quotes(self->__header__[c]) ? printf("%s <Aggregate Data>%10c| ", self->__header__[column_index], space) : printf("'%s' <Aggregate Data>%10c| ", self->__header__[column_index], space);
//                 continue;
//             }
//             else if (v == -1 || v == most_unique_values) {
//                 // This create a line break after column head / after values are printed, before unique values is printed
//                 char dashes[column_strlen+1];
//                 char char_to_print = (v == -1) ? '-' : ' ';
//                 create_spaces_string(dashes, column_strlen+1, with_divider, char_to_print);
//                 if (c == (uint32_t)non_numeric_column_count-1) dashes[column_strlen-1] = '\0';
//                 printf("%s", dashes);

//                 continue;
//             }
//             else if (v == most_unique_values+1) {
//                 // Print Unique Values
//                 char unique_string[32];
//                 sprintf(unique_string, "Unique values: %u", unique_values[c]);

//                 int8_t num_spaces = column_strlen - strlen(unique_string);
//                 char spaces[num_spaces+1];
//                 create_spaces_string(spaces, num_spaces+1, with_divider, ' ');

//                 printf("%s%s", unique_string, spaces);
//                 continue;
//             }
//             else if (v == most_unique_values+2) {
//                 // Print Is Null
//                 bool is_null = true;
//                 uint32_t column_index = non_numeric_columns[c];
//                 print_stats_is_not_null_(self, column_strlen, column_index, is_null);
//                 continue;
//             }
//             else if (v == most_unique_values+3) {
//                 // Print Not Null
//                 bool is_null = false;
//                 uint32_t column_index = non_numeric_columns[c];
//                 print_stats_is_not_null_(self, column_strlen, column_index, is_null);
//                 continue;
//             }


//             // Print spaces if no more values to dislay for this column
//             if (unique_values[c] <= v) {
//                 char spaces[column_strlen+1];
//                 create_spaces_string(spaces, column_strlen+1, with_divider, ' ');
//                 printf("%s", spaces);
//                 continue;
//             }

//             char print_string[64];
//             has_quotes(sorted[c][v].value) ? sprintf(print_string, "Value: %s", sorted[c][v].value) : sprintf(print_string, "Value: '%s'", sorted[c][v].value);

//             uint8_t value_strlen = strlen(print_string);

//             char count_string[32];

//             sprintf(count_string, "Count: %ld | ", sorted[c][v].count);

//             size_t count_strlen = strlen(count_string);

//             // Spaces
//             int8_t num_spaces = column_strlen - value_strlen - count_strlen;

//             if (num_spaces > 0) {
//                 char spaces[num_spaces+1];
//                 for (uint8_t s = 0; s < num_spaces+1; s++) spaces[s] =  (s == num_spaces) ? '\0' : ' ';
//                 printf("%s%s%s", print_string, spaces, count_string);
//             }
//             else {

//                 // num spaces is negative
//                 sprintf(&print_string[value_strlen+num_spaces-4], "... ");
//                 printf("%s%s", print_string, count_string);
//             }

//         }

//         // (v == most_unique_values-1) ? printf("\n\n") : printf("\n");
//         printf("\n");
//     }

//     // Free arrays
//     for (uint16_t c = 0; c < non_numeric_column_count; c++) free(sorted[c]);
//     free(sorted);
//     free(unique_values);

//     return true;
// }


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

    const uint8_t BUFFER_SIZE = 255;

    // Create main string
    char print_string[BUFFER_SIZE+1];
    is_null ? sprintf(print_string, "Is Null %ld", self->aggregate[column_index].is_null) : sprintf(print_string, "Not Null %ld", self->aggregate[column_index].not_null);

    // Create spaces string
    char spaces[BUFFER_SIZE+1];
    bool with_divider = true;
    uint16_t num_spaces = column_strlen - strlen(print_string) + 1;

    if (num_spaces > BUFFER_SIZE) num_spaces = BUFFER_SIZE; /* Make sure to not overrun buffer*/

    create_spaces_string(spaces, num_spaces, with_divider, ' ');

    (!is_null && column_index == self->columnCount-1) ? printf("%s%s\n", print_string, spaces) : printf("%s%s", print_string, spaces);

    return;
}



//         -- Corr() --
double corr(DataLynx *self, char *column_name1, char *column_name2) {

    // Safety checks
    if (self == NULL || column_name1 == NULL || column_name2 == NULL) return 0;
    if (!data_exists(self)) return 0;

    // Retrieve corresponding integer column indexes
    int16_t column_index1 = findColumnIndex(self, column_name1);
    int16_t column_index2 = findColumnIndex(self, column_name2);
    if (column_index1 < 0 || column_index2 < 0) return 0;

    // Correlation is only for numeric columns
    if (!self->aggregate[column_index1].is_number || !self->aggregate[column_index2].is_number) return 0;

    // Extract means & standard deviations (for better code readability)
    double x_mean = self->aggregate[column_index1].mean;
    double y_mean = self->aggregate[column_index2].mean;
    double x_std = self->aggregate[column_index1].std;
    double y_std = self->aggregate[column_index2].std;
    double sum = 0;

    // Iterate through each row
    for (uintmax_t row = 0; row < self->rowCount; row++) {

        double x = 0, y = 0;

        // Get fields
        char *x_string = get_field_(self, row, column_index1);
        char *y_string = get_field_(self, row, column_index2);

        // Skip null field
        if (x_string[0] == '\0' || y_string[0] == '\0') continue;

        x = atof(x_string);
        y = atof(y_string);

        // Subtract the mean from each data point and divide by standard deviation
        x -= x_mean;
        x /= x_std;

        y -= y_mean;
        y /= y_std;

        // Multiply and add to sum
        sum += (x * y);


    }

    // r = 1/n-1 SIGMA (x_i - x_mean)*(y_i = y_mean)
    double r = 1.0 / (double)(self->rowCount-1);

    return r * sum;
}

// //          --- Linear Regression() --
// void linearRegression(DataLynx *self, char *x_column_name, char *y_column_name) {

//     // Safety checks
//     if (self == NULL) return;

//     int16_t x_column_index = findColumnIndex(self, x_column_name);
//     int16_t y_column_index = findColumnIndex(self, y_column_name);
//     if (x_column_name < 0 || y_column_name  < 0) return;

//     double y_mean = self->aggregate[y_column_index].mean;

//     char *x_field = NULL;
//     char *y_field = NULL;

//     // Calculate SSR for y_bar
//     double y_bar_ssr = 0;
//     for (uintmax_t row = 0; row < self->rowCount; row++) {

//         // Extract current field
//         y_field = get_field_(self, row, y_column_index);
//         x_field = get_field_(self, row, x_column_index);

//         if (field_y[0] == '\0' || field_x[0] == '\0') continue; /* Skip if EITHER x or y field is empty, bc that data point is invalid */

//         double field = atof(y_field);

//         double residual = field - y_mean;

//         residual *= residual;

//         y_bar_ssr += residual;

//     }


//     //              --- FIND SLOPE --
//     double intercept = y_mean;

//     // Calculate SSR for current SLR
//     double ssr = 0;
//     for (uintmax_t row = 0; row < self->rowCount; row++) {

//         // Extract current field
//         x_field = get_field_(self, row, x_column_index);
//         y_field = get_field_(self, row, y_column_index);

//         if (field_x[0] == '\0' || field_y[0] == '\0') continue; /* Skip if EITHER x or y field is empty, bc that data point is invalid */

//         double field = atof(y_field);

//         double residual = field - y_mean;

//         residual *= residual;

//         y_bar_ssr += residual;

//     }





//     return;
// }

