#include <dataLynx_stats.h>
#include <dataLynx_data.h>
#include <dataLynx_util.h>
#include <math.h>


//                          ------- STATS -------

//          __ CREATE_STATS() ____
bool create_stats(dataLynx *self) {

    if (self->aggregate != NULL) {
        free_value_counts(self);
        free(self->aggregate);
    }

    const char *func_name = "create_stats";

    // Create array of Stats structs
    self->aggregate = malloc(sizeof(Aggregate)*self->columnCount);
    if (self->aggregate == NULL) {if_error(MALLOC_FAILED, func_name); return false;}


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
double getStat(dataLynx *self, char *column_name, char *stat) {

    if (self == NULL || column_name == NULL || stat == NULL) return 0;

    // 1. Retrieve and return stats
    if (self->aggregate != NULL) {

        intmax_t column_index = findColumnIndex(self, column_name);
        if (column_index < 0) return 0;

        if (strcasecmp(stat, "min") == 0) return self->aggregate[column_index].min;
        else if (strcasecmp(stat, "max") == 0) return self->aggregate[column_index].max;
        else if (strcasecmp(stat, "sum") == 0) return self->aggregate[column_index].sum;
        else if (strcasecmp(stat, "mean") == 0 || strcasecmp(stat, "avg") == 0 || strcasecmp(stat, "average") == 0) return self->aggregate[column_index].mean;
        else if (strcasecmp(stat, "std") == 0 || strcasecmp(stat, "standard deviation") == 0) return self->aggregate[column_index].std;
        else if (strcasecmp(stat, "lower qrt") == 0 || strcasecmp(stat, "lower quartile") == 0 || strcasecmp(stat, "25th %") == 0 || strcasecmp(stat, "25th percentile") == 0) return self->aggregate[column_index].lower_qrt;
        else if (strcasecmp(stat, "median") == 0) return self->aggregate[column_index].median;
        else if (strcasecmp(stat, "upper qrt") == 0 || strcasecmp(stat, "upper quartile") == 0 || strcasecmp(stat, "75th %") == 0 || strcasecmp(stat, "75th percentile") == 0) return self->aggregate[column_index].upper_qrt;
        else if (strcasecmp(stat, "isnull") == 0 || strcasecmp(stat, "is null") == 0) return self->aggregate[column_index].is_null;
        else if (strcasecmp(stat, "notnull") == 0 || strcasecmp(stat, "not null") == 0) return self->aggregate[column_index].not_null;
    }
    // 2. Calculate stats if none to retrieve
    else if (self->dict_grid != NULL) return aggregate_dict_grid(self, column_name, stat);
    else if (self->grid != NULL) return aggregate_grid(self, column_name, stat);
    else if (self->rows != NULL) return aggregate_rows(self, column_name, stat);
    else if (self->raw != NULL) return aggregate_raw(self, column_name, stat);


    return 0;
}

/* This batch of functions will not have safety checks, because stats() has safety checks, so no need for redundancy */
double min(dataLynx *self, char *column_name) {return getStat(self, column_name, "min");}

double max(dataLynx *self, char *column_name) {return getStat(self, column_name, "max");}

double sum(dataLynx *self, char *column_name) {return getStat(self, column_name, "sum");}

double mean(dataLynx *self, char *column_name) {return getStat(self, column_name, "mean");}

double std(dataLynx *self, char *column_name) {return getStat(self, column_name, "std");}

double lowerQrt(dataLynx *self, char *column_name) {return getStat(self, column_name, "lower qrt");}

double median(dataLynx *self, char *column_name) {return getStat(self, column_name, "median");}

double upperQrt(dataLynx *self, char *column_name) {return getStat(self, column_name, "upper qrt");}

uintmax_t isNull(dataLynx *self, char *column_name) {return (uintmax_t)getStat(self, column_name, "isnull");}

uintmax_t notNull(dataLynx *self, char *column_name) {return (uintmax_t)getStat(self, column_name, "notnull");}


//          FIND MEDIAN
bool find_median(dataLynx *self) {

    // This function does not account for empty fields yet
    self->in_place_sort = false;

    // Sort each column (not in place)
    for (uint16_t column = 0; column < self->columnCount; column++) {

        if (!self->aggregate[column].is_number) continue;

        sortRowsByColumn(self, self->__header__[column], "asc");

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
double aggregate_raw(dataLynx *self, char *column_name, char *operation) {

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
double aggregate_rows(dataLynx *self, char *column_name, char *operation) {

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


//      ____AGGREGATE_GRID() _____
double aggregate_grid(dataLynx *self, char *column_name, char *operation) {

    if (self == NULL) return 0;
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
double aggregate_dict_grid(dataLynx *self, char *column_name, char *operation) {

    if (self == NULL) return 0;
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


void calc_std(dataLynx *self) {

    for (uint32_t column = 0; column < self->columnCount; column++) {

        if (!self->aggregate[column].is_number) continue;

        double sum_squared_differences = 0;

        for (uint32_t row = 0; row < self->rowCount; row++) {

            double difference = 0;

            if (self->grid_v3 != NULL) {
                printf("row: %d\ncol: %d\n: chr:%c\n\n", row, column, self->grid_v3[row][column][0]);
                if (self->grid_v3[row][column][0] != '\0')
                    difference = atof(self->grid_v3[row][column]) - self->aggregate[column].mean;
            }
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




// bool calc_median(dataLynx *self) {
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
// void bubble_sort(dataLynx *self, int n) {

//     for (int i = 0; i < self->rowCount; i++) {
//         for (int j = 0; j < self->rowCount - i - 1; j++) {


//             if (arr[j] > arr[j + 1]) {
//                 swap(&arr[j], &arr[j + 1]);
//             }
//         }
//     }
// }

//          FIND NEW MIN()
double find_new_min(dataLynx *self, uintmax_t column, double old_field_float, double new_field_float) {

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
double find_new_max(dataLynx *self, uintmax_t column, double old_field_float, double new_field_float) {

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
bool update_stats(dataLynx *self, uintmax_t column, char *old_field, char *new_field) {

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



void update_stats_new_row(dataLynx *self, char *values[]) {

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
bool create_value_counts(dataLynx *self, uintmax_t column_index) {

    char *func_name = "create_value_counts";

    // Each column

    // Allocate array (A-Z) + one spot for other (i.e. strings that start with digits or punctuation), 27 total
    self->aggregate[column_index].value_counts = (ValueCount**)malloc(sizeof(ValueCount*)*27);
    if (self->aggregate[column_index].value_counts == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    // Initialize to NULL
    for (uint8_t i = 0; i < 27; i++) {
        self->aggregate[column_index].value_counts[i] = NULL;
    }


    return true;
}



// //      SORT VALUE COUNTS()
// void sort_value_counts(dataLynx *self) {


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

void sort_value_counts(dataLynx *self) {

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
bool prepend_value_count(dataLynx *self, uintmax_t column_index, char *value, uint8_t alpha_index) {

    if (self == NULL) return false;
    if (value == NULL) return false;

    const char *func_name = "prepend_value_count";

    //      - Prepend -

    // Allocate
    ValueCount *new_node = (ValueCount*)malloc(sizeof(ValueCount));
    if (new_node == NULL) {if_error(MALLOC_FAILED, func_name); return false;}

    // Assign
    new_node->value = (char*)malloc(sizeof(char)*(strlen(value)+1));
    if (new_node->value == NULL) {if_error(MALLOC_FAILED, func_name); return false;}
    strcpy(new_node->value, value);

    new_node->count = 1;
    new_node->next = self->aggregate[column_index].value_counts[alpha_index];

    self->aggregate[column_index].value_counts[alpha_index] = new_node;

    return true;

}


//      ADD VALUE COUNT()
bool increment_decrement_value_count(dataLynx *self, char *column_name, char *value, bool increment) {

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
void remove_value_count_node(dataLynx *self, uintmax_t column_index, uint8_t alpha_index, uintmax_t node_index) {

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


uint16_t valueCount(dataLynx *self, char *value, char *column_name) {

    // Safety checks
    if (self == NULL || value == NULL || column_name == NULL) return 0;

    // Get column integer index/determine if column name is valid
    int16_t column_index;
    if ((column_index = findColumnIndex(self, column_name)) < 0) return 0;

    return value_count_internal_(self, value, column_index);
}


//          IS IN COLUMN()
bool isInColumn(dataLynx *self, char *value, char *column_name) {
    return valueCount(self, value, column_name);
}


//          IS IN DATA()
bool isInData(dataLynx *self, char *value, char *column_name) {

    // Safety checks
    if (self == NULL || value == NULL || column_name == NULL) return 0;

    // Iterate through columns, checking if value appears
    for (uintmax_t column = 0; column < self->columnCount; column++) {

        if (value_count_internal_(self, value, column) > 0) return true;
    }

    return false;

}

uint16_t value_count_internal_(dataLynx *self, char *value, int16_t column_index) {

    // THIS FUNCTION: Retrieves value count of a given value

    // Find correct "hash" (i.e. alpha index 0-27 => A-Z + punctuation)
    uint8_t alpha_index = find_alpha_index(value);

    ValueCount *tmp = self->aggregate[column_index].value_counts[alpha_index];

    // Traverse linked list
    while (tmp != NULL && strcmp(value, tmp->value) != 0) {tmp = tmp->next;}

    return (tmp != NULL) ? tmp->count : 0;
}