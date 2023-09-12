#ifndef CSVWIZARD_H
#define CSVWIZARD_H

#include <csvWizard_data.h>
#include <csvWizard_file_io.h>
#include <csvWizard_stats.h> /* The only user-facing function in this module is stats(), which can be accessed through .csv.stats(), so no need to give user access this module, unless i want to allow them the option of accessing without .csv */
#include <csvWizard_util.h>

/* I can eiter give user access to functions by including them in this .h file OR by assigning the address to a function pointer in the csvWizard 'object' since that file has access to the other modules */


#endif /* CSVWIZARD_H */