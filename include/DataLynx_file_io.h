#ifndef DATALYNX_FILE_IO_H
#define DATALYNX_FILE_IO_H

#include <DataLynx_data.h>

bool openFile(DataLynx *self, char *filename);

// CSV Functions - Read/write directly to csv files (.csv  inner-struct of DataLynx)
char **headerReader(DataLynx *self);
char *fileReader(DataLynx *self); /* Reads as one long string */
char **fileRowReader(DataLynx *self); /*Reads as one long string and parses into 2D array (array on strings) */

char ***reader_v3(DataLynx *self);
node **reader(DataLynx *self); /* Reads as one long string and parses into linked list */
dict_node **dictReader(DataLynx  *self); /* Reads as one long string and parses into dict linked list (fields accessable with column names instead of integers)*/

char *fieldReader(DataLynx *self, uintmax_t row, char *column_name);
char *fieldReaderIdx(DataLynx *self, uintmax_t row, uintmax_t column);
char *field_reader_internal_(DataLynx *self, uintmax_t row, uintmax_t column);


// Update fields in CSV
bool fieldWriter(DataLynx *self, uintmax_t row, char *column_name, char *new_field);
bool fieldWriterIdx(DataLynx *self, uintmax_t row, uintmax_t column, char *new_field);
bool field_writer_internal_(DataLynx *self, uintmax_t row, uintmax_t column, char *new_field);

bool rowWriter(DataLynx *self, char *values[]);
bool rowDictWriter(DataLynx *self, dict values[]);
bool row_writer_internal_(DataLynx *self, char *values[]);


bool renameFile(DataLynx *self, char *new_filename);
bool backup(DataLynx *self);
bool writeData(DataLynx *self, char *new_filename);

bool write_csv_header_(DataLynx *self);


#endif /* DATALYNX_FILE_IO_H */
