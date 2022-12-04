#ifndef EXECUTE_COMMAND_H
#define EXECUTE_COMMAND_H

#include "types.h"
#include "string.h"
#include "check.h"
#include "commands.h"

int execute_Load(char*, hash_table**);
int execute_Save(char*, hash_table**);
int execute_Rand(char*, hash_table**);
int execute_Concat(char*, hash_table**);
int execute_Free(char*, hash_table**);
int execute_Remove(char*, hash_table**);
int execute_Copy(char*, hash_table**);
int execute_Sort(char*, hash_table**);
int execute_Shuffle(char*, hash_table**);
int execute_Stats(char*, hash_table**);
int execute_Print(char*, hash_table**);
int execute_Print_all(char*, hash_table**);
int execute(char*, hash_table**);

#endif // EXECUTE_COMMAND_H