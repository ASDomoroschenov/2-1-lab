#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../types/types.h"
#include "../string/string.h"
#include "../configuration/configuration.h"
#include "../variables/variables.h"
#include "../operations/operations.h"
#include "../check/check.h"

int delete_sep(char**);
int interpreter(char*, char*, int*, char**, int);

#endif // INTERPRETER_H