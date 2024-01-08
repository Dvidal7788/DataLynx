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
double getStatIdx(DataLynx *self, uint32_t column, char *operation);
double get_stat_internal_(DataLynx *self, uint32_t column, char *stat);

// Although all of these are available through getStat, I will make these available through their own functions as well, if for nothing else other than the simple fact that getStat can only return a double, and the user might want to keep is_null/not_null as an integer without having to typecast
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

double corr(DataLynx *self, char *column_name1, char *column_name2);

double find_new_min(DataLynx *self, uintmax_t column, double old_field_float, double new_field_float);
double find_new_max(DataLynx *self, uintmax_t column, double old_field_float, double new_field_float);
bool update_stats(DataLynx *self, uintmax_t column, char *old_field, char *new_field);
void calc_mean(DataLynx *self);
void calc_std(DataLynx *self);
bool find_median(DataLynx *self);

double aggregate_raw(DataLynx *self, char *column_name, char *operation);
double aggregate_rows(DataLynx *self, char *column_name, char *operation);
double aggregate_grid_v3(DataLynx *self, char *column_name, char *operation);
double aggregate_grid(DataLynx *self, char *column_name, char *operation);
double aggregate_dict_grid(DataLynx *self, char *column_name, char *operation);
char *format_number(double value);

bool create_value_counts(DataLynx *self, uintmax_t column_index);
uint8_t find_alpha_index(char *value) ;
ValueCount *prepend_value_count(DataLynx *self, uintmax_t column_index, char *value, uint8_t alpha_index);
bool increment_decrement_value_count(DataLynx *self, char *column_name, char *value, bool increment);
void remove_value_count_node(DataLynx *self, uintmax_t column_index, uint8_t alpha_index, uintmax_t node_index);
void sort_value_counts(DataLynx *self);

void update_stats_new_row(DataLynx *self, char *values[]);


bool printPivotTable(DataLynx *self, char *group_by, char *aggregated_column, char *stat);
bool printStatsAll(DataLynx *self);
bool printStatsColumn(DataLynx *self, char *column_name);
bool print_stats_internal_(DataLynx *self, char *column_name);
void print_stats_is_not_null_(DataLynx *self, size_t column_strlen, uint32_t column_index, bool is_null);
void stat_print_(char *stat_name, double stat, uint8_t column_strlen);


#endif /* DATALYNX_STATS_H */