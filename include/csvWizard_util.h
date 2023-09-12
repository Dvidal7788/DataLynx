#ifndef CSVWIZARD_UTIL_H
#define CSVWIZARD_UTIL_H

// Utility Functions
void create_spaces_string(char buffer[], size_t buffer_size, bool with_divider, char space);

char truncate_with_ellipses(char *original, char truncated[], uint32_t truncation_point);

bool is_number(char *s);
bool is_numeric(char *s);
bool is_hex(char *s);
bool has_quotes(char *s);
bool has_comma(char *s);
bool add_quotes(char **s);
char *remove_quotes(csvWizard *self, char *s);
bool verify_column(char **header, uintmax_t column_count, char *column);
int strcmp_quotes(const char *s1_input, const char *s2_input, bool case_sensitive);
void get_ptr_to_correct_column(intmax_t correct_column, node **node_ptr, dict_node **dict_ptr);

char *inf_buffer(char *prompt);
uint64_t get_uint(char *prompt);
bool is_ext(char *filename, char *ext);
bool convert_to_csv(csvWizard *self, char *filename);

char *append_last_retrieved_fields(csvWizard *self, char **field);

void calc_max_row_digits(csvWizard *self);
uint8_t if_error(uint8_t error_code, const char *function_name);

#endif /* CSVWIZARD_UTIL_H */