#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../types/types.h"
#include "../operations/operations.h"
#include "../variables/variables.h"
#include "../check/check.h"
#include "../commands/commands.h"
#include "../string/string.h"

int delete_sep(char**);
int run_command(array_variables*, char*, int*);
int interpreter(char*, int*);

#endif // INTERPRETER_H