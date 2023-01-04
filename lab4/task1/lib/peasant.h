#ifndef PEASENT_H
#define PEASENT_H

#include "types.h"

int create_user(peasant**, long);
int compare(const void*, const void*);
int have_object_coasts(peasant**, char*);
int on_wich_coast(peasant**, char*);
int take(peasant**, char*);
int move(peasant**);
int put(peasant**);
int check_coasts(peasant**);
int execute_command(char*, peasant**);
int output_user(peasant*);
int free_user(peasant**);

#endif //PEASENT_H