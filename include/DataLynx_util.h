#ifndef DATALYNX_UTIL_H
#define DATALYNX_UTIL_H

// Utility Functions
void create_spaces_string(char buffer[], size_t buffer_size, bool with_divider, char space);

char truncate_with_ellipses(char *original, char truncated[], uint32_t truncation_point);

bool is_number(char *s);
bool is_numeric(char *s);
bool is_hex(char *s);
bool has_quotes(char *s);
bool has_comma(char *s);
bool add_quotes(char **s);
char *remove_quotes(DataLynx *self, char *s);
bool verify_column(char **header, uintmax_t column_count, char *column);
int strcmp_quotes(const char *s1_input, const char *s2_input, bool case_sensitive);
void get_ptr_to_correct_column(intmax_t correct_column, node **node_ptr, dict_node **dict_ptr);

char *inf_buffer(char *prompt);
uint64_t get_uint(char *prompt);
bool is_ext(char *filename, char *ext);
bool convert_to_csv(DataLynx *self, char *filename);

char *append_last_retrieved_fields(DataLynx *self, char **field);

void calc_max_row_digits(DataLynx *self);

bool rearrange_dict_array(DataLynx *self, dict values[]);

int16_t find_dot(char *filename);

void fill_buffer(char *buffer, const char *s);

size_t get_file_size_(DataLynx *self);
bool data_exists(DataLynx *self);

uint8_t log_error(uint8_t error_code, const char *function_name);

#endif /* DATALYNX_UTIL_H */