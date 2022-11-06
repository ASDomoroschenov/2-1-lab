#ifndef DBL_LIST_H
#define DBL_LIST_H

#include "types.h"
#include "string.h"

int get_node(list_node**, char*);
int add_list(dbl_list**, char*);
T *get_min_list(dbl_list*);
T *get_max_list(dbl_list*);
int extract_min_list(dbl_list**);
int extract_max_list(dbl_list**);
int output_list(dbl_list**, FILE*);

#endif // DBL_LIST_H