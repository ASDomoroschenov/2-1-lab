#ifndef CITIZEN_H
#define CITIZEN_H

#include "list.h"
#include "string.h"
#include "types.h"

int check_number(char*);
int check_fullname(char*, char*, char*);
int check_date_birth(char*);
int check_gender(char*);
int check_avrg_incom(char*);
int check_str_citizen(char*);
int get_citizen(char*, citizen**);
int get_list_citizen(char*, list**, int (*cmp)(citizen*, citizen*));
int find_citizen(list*, citizen**, char*, char*, char*, char*, int, double);
int add_citizen(list**, char*, char*, char*, char*, int, double);
int delete_citizen(list**, char*, char*, char*, char*, int, double);
void output_citizen(citizen*, FILE*);
int output_list_citizens(list*);
int upload_list_file(list*, char*);

#endif // CITIZEN_H