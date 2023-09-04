#ifndef CSVWIZARD_STATS_H
#define CSVWIZARD_STATS_H

#include <csvWizard_data.h>

// STATS/VALUE COUNTS
bool create_stats(csvWizard *self);

uint16_t valueCount(csvWizard *self, char *value, char *column_name);
bool isInColumn(csvWizard *self, char *value, char *column_name);

double getStat(csvWizard *self, char *column_name, char *operation);
double min(csvWizard *self, char *column_name);
double max(csvWizard *self, char *column_name);
double sum(csvWizard *self, char *column_name);
double mean(csvWizard *self, char *column_name);
uintmax_t isNull(csvWizard *self, char *column_name);
uintmax_t notNull(csvWizard *self, char *column_name);
double find_new_min(csvWizard *self, uintmax_t column, double old_field_float, double new_field_float);
double find_new_max(csvWizard *self, uintmax_t column, double old_field_float, double new_field_float);
bool update_stats(csvWizard *self, uintmax_t column, char *old_field, char *new_field);
void calc_std(csvWizard *self);
double calc_sqrt(double x);
bool find_median(csvWizard *self);

double aggregate_raw(csvWizard *self, char *column_name, char *operation);
double aggregate_rows(csvWizard *self, char *column_name, char *operation);
double aggregate_grid(csvWizard *self, char *column_name, char *operation);
double aggregate_dict_grid(csvWizard *self, char *column_name, char *operation);
char *format_number(double value);

bool create_value_counts(csvWizard *self, uintmax_t column_index);
uint8_t find_alpha_index(char *value) ;
bool prepend_value_count(csvWizard *self, uintmax_t column_index, char *value, uint8_t alpha_index);
bool increment_decrement_value_count(csvWizard *self, char *column_name, char *value, bool increment);
void remove_value_count_node(csvWizard *self, uintmax_t column_index, uint8_t alpha_index, uintmax_t node_index);
void sort_value_counts(csvWizard *self);

void update_stats_new_row(csvWizard *self, char *values[]);

#endif /* CSVWIZARD_STATS_H */