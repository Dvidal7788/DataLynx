#ifndef DATALYNX_FILE_IO_H
#define DATALYNX_FILE_IO_H

#include <dataLynx_data.h>

bool openFile(dataLynx *self, char *filename);

// CSV Functions - Read/write directly to csv files (.csv  inner-struct of dataLynx)
char **headerReader(dataLynx *self);
char *fileReader(dataLynx *self); /* Reads as one long string */
char **fileRowReader(dataLynx *self); /*Reads as one long string and parses into 2D array (array on strings) */

char ***reader_v3(dataLynx *self);
node **reader(dataLynx *self); /* Reads as one long string and parses into linked list */
dict_node **dictReader(dataLynx  *self); /* Reads as one long string and parses into dict linked list (fields accessable with column names instead of integers)*/

char *fieldReader(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);
char *fieldReader2(dataLynx *self, uintmax_t desired_row, char *desired_column);


// Update fields in CSV
bool fieldWriter(dataLynx *self, uintmax_t row, char *column, char *new_field);
bool update_csv_index(dataLynx *self, uintmax_t row, uintmax_t column, char *new_field);

bool backup(dataLynx *self);
bool writeData(dataLynx *self, char *new_filename);


#endif /* DATALYNX_FILE_IO_H */