#ifndef CSV_FILE_H
#define CSV_FILE_H

#include <stdio.h>
#include "message.h"

int generate_file_name(char**);
int write_in_file(FILE*, message*);
int fill_file(char*, char*, size_t*);

#endif // CSV_FILE_H