#ifndef CSVWIZARD_FILE_IO_H
#define CSVWIZARD_FILE_IO_H

#include <csvWizard_data.h>

bool openFile(csvWizard *self, char *filename);

// CSV Functions - Read/write directly to csv files (.csv  inner-struct of csvWizard)
char **headerReader(csvWizard *self);
char *fileReader(csvWizard *self); /* Reads as one long string */
char **fileRowReader(csvWizard *self); /*Reads as one long string and parses into 2D array (array on strings) */

char ***reader_v3(csvWizard *self);
node **reader(csvWizard *self); /* Reads as one long string and parses into linked list */
dict_node **dictReader(csvWizard  *self); /* Reads as one long string and parses into dict linked list (fields accessable with column names instead of integers)*/

char *fieldReader(csvWizard *self, uintmax_t desired_row, uintmax_t desired_column);
char *fieldReader2(csvWizard *self, uintmax_t desired_row, char *desired_column);


// Update fields in CSV
bool fieldWriter(csvWizard *self, uintmax_t row, char *column, char *new_field);
bool update_csv_index(csvWizard *self, uintmax_t row, uintmax_t column, char *new_field);

bool backup(csvWizard *self);
bool writeData(csvWizard *self, char *new_filename);


#endif /* CSVWIZARD_FILE_IO_H */