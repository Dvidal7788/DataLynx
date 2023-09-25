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

char *fieldReader(dataLynx *self, uintmax_t row, char *column_name);
char *fieldReader2(dataLynx *self, uintmax_t row, uintmax_t column);
char *field_reader_internal_(dataLynx *self, uintmax_t row, uintmax_t column);


// Update fields in CSV
bool fieldWriter(dataLynx *self, uintmax_t row, char *column_name, char *new_field);
bool fieldWriter2(dataLynx *self, uintmax_t row, uintmax_t column, char *new_field);
bool field_writer_internal_(dataLynx *self, uintmax_t row, uintmax_t column, char *new_field);

bool rowWriter(dataLynx *self, char *values[]);
bool rowDictWriter(dataLynx *self, dict values[]);
bool row_writer_internal_(dataLynx *self, char *values[]);

bool backup(dataLynx *self);
bool writeData(dataLynx *self, char *new_filename);

bool write_csv_header_(dataLynx *self);


#endif /* DATALYNX_FILE_IO_H */