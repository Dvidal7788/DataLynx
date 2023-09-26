#ifndef DATALYNX_STATS_H
#define DATALYNX_STATS_H

#include <dataLynx_data.h>

// STATS/VALUE COUNTS
bool create_stats(dataLynx *self);

uint16_t valueCount(dataLynx *self, char *value, char *column_name);
bool isInColumn(dataLynx *self, char *value, char *column_name);
bool isInData(dataLynx *self, char *value, char *column_name);
uint16_t value_count_internal_(dataLynx *self, char *value, int16_t column_index);

double getStat(dataLynx *self, char *column_name, char *operation);

double min(dataLynx *self, char *column_name);
double max(dataLynx *self, char *column_name);
double sum(dataLynx *self, char *column_name);
double mean(dataLynx *self, char *column_name);
double std(dataLynx *self, char *column_name);
double lowerQrt(dataLynx *self, char *column_name);
double median(dataLynx *self, char *column_name);
double upperQrt(dataLynx *self, char *column_name);
uintmax_t isNull(dataLynx *self, char *column_name);
uintmax_t notNull(dataLynx *self, char *column_name);

double find_new_min(dataLynx *self, uintmax_t column, double old_field_float, double new_field_float);
double find_new_max(dataLynx *self, uintmax_t column, double old_field_float, double new_field_float);
bool update_stats(dataLynx *self, uintmax_t column, char *old_field, char *new_field);
void calc_std(dataLynx *self);
bool find_median(dataLynx *self);

double aggregate_raw(dataLynx *self, char *column_name, char *operation);
double aggregate_rows(dataLynx *self, char *column_name, char *operation);
double aggregate_grid(dataLynx *self, char *column_name, char *operation);
double aggregate_dict_grid(dataLynx *self, char *column_name, char *operation);
char *format_number(double value);

bool create_value_counts(dataLynx *self, uintmax_t column_index);
uint8_t find_alpha_index(char *value) ;
bool prepend_value_count(dataLynx *self, uintmax_t column_index, char *value, uint8_t alpha_index);
bool increment_decrement_value_count(dataLynx *self, char *column_name, char *value, bool increment);
void remove_value_count_node(dataLynx *self, uintmax_t column_index, uint8_t alpha_index, uintmax_t node_index);
void sort_value_counts(dataLynx *self);

void update_stats_new_row(dataLynx *self, char *values[]);

#endif /* DATALYNX_STATS_H */