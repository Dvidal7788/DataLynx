#ifndef DATALYNX_STATS_H
#define DATALYNX_STATS_H

#include <DataLynx_data.h>

// STATS/VALUE COUNTS
bool create_stats(DataLynx *self);

uint16_t valueCount(DataLynx *self, char *value, char *column_name);
bool isInColumn(DataLynx *self, char *value, char *column_name);
bool isInData(DataLynx *self, char *value, char *column_name);
uint16_t value_count_internal_(DataLynx *self, char *value, int16_t column_index);

double getStat(DataLynx *self, char *column_name, char *operation);

double min(DataLynx *self, char *column_name);
double max(DataLynx *self, char *column_name);
double sum(DataLynx *self, char *column_name);
double mean(DataLynx *self, char *column_name);
double std(DataLynx *self, char *column_name);
double lowerQrt(DataLynx *self, char *column_name);
double median(DataLynx *self, char *column_name);
double upperQrt(DataLynx *self, char *column_name);
uintmax_t isNull(DataLynx *self, char *column_name);
uintmax_t notNull(DataLynx *self, char *column_name);

double find_new_min(DataLynx *self, uintmax_t column, double old_field_float, double new_field_float);
double find_new_max(DataLynx *self, uintmax_t column, double old_field_float, double new_field_float);
bool update_stats(DataLynx *self, uintmax_t column, char *old_field, char *new_field);
void calc_std(DataLynx *self);
bool find_median(DataLynx *self);

double aggregate_raw(DataLynx *self, char *column_name, char *operation);
double aggregate_rows(DataLynx *self, char *column_name, char *operation);
double aggregate_grid(DataLynx *self, char *column_name, char *operation);
double aggregate_dict_grid(DataLynx *self, char *column_name, char *operation);
char *format_number(double value);

bool create_value_counts(DataLynx *self, uintmax_t column_index);
uint8_t find_alpha_index(char *value) ;
bool prepend_value_count(DataLynx *self, uintmax_t column_index, char *value, uint8_t alpha_index);
bool increment_decrement_value_count(DataLynx *self, char *column_name, char *value, bool increment);
void remove_value_count_node(DataLynx *self, uintmax_t column_index, uint8_t alpha_index, uintmax_t node_index);
void sort_value_counts(DataLynx *self);

void update_stats_new_row(DataLynx *self, char *values[]);

#endif /* DATALYNX_STATS_H */