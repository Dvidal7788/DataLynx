#ifndef DATALYNX_H
#define DATALYNX_H

#include <DataLynx_data.h>
#include <DataLynx_file_io.h>
#include <DataLynx_stats.h> /* The only user-facing function in this module is stats(), which can be accessed through .csv.stats(), so no need to give user access this module, unless i want to allow them the option of accessing without .csv */
#include <DataLynx_util.h>

/* I can eiter give user access to functions by including them in this .h file OR by assigning the address to a function pointer in the DataLynx 'object' since that file has access to the other modules */


#endif /* DATALYNX_H */
