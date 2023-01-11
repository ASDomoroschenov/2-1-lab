#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../types/types.h"
#include "../string/string.h"

int find_instruction(char*, instruction*);
int check_config(configuration);
int fill_instructions(char*, configuration*, int*);
void free_configuration(configuration*);
void output_instructions(instruction*);

#endif // CONFIGURATION_H