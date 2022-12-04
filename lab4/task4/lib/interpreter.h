#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "hash_table.h"
#include "types.h"
#include "string.h"
#include "execute_command.h"

int print_error(int, int);
int interpreter();

#endif // INTERPRETER_H