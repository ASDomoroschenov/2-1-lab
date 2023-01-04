#ifndef TYPES_H
#define TYPES_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define ACCESSES 0666 // the access rights for *get
#define HASHSIZE 128

enum ERRORS {
	SUCCESS 					= -1,
	NO_MEMORY 					= -2,
	OPEN_FILE 					= -3,
	INVALID_EXPRESSION 			= -4,
	HAVENT_VAR 					= -5,
	INVALID_MULTILINE_COMMENT 	= -6,
	SEM_GET 					= -7,
	SHM_GET 					= -8,
	SHM_AT 						= -9,
	SEM_RM 						= -10,
	SHM_RM 						= -11,
	INVALID_COUNT_ARGS 			= -12,
	HAVENT_USER 				= -13
};

enum CODE_SEND {
	GET_ID 		= 1,
	RUN_COMMAND = 2,
	FINISH 		= 3,
	SET_VAR 	= 4
};

enum CODE_ANS {
	ADD_USER 	= 5,
	WRITE 		= 6,
	READ 		= 7,
	EXIT_CODE 	= 8,
	VAR_SETED 	= 9,
	DELETED 	= 10,
	ERROR 		= 11,
	EMPTY_MSG 	= 12
};

typedef struct {
	char name;
	int value;
} variable;

typedef struct {
	variable *vars;
	int current_size;
	int size;
} array_variables;

typedef struct {
	array_variables array;
	int id;
} client;

typedef struct tag_node_list {
	struct tag_node_list *next;
	client *user;
} node_list;

typedef struct {
	node_list *root;
	int size;
} list_t;

typedef struct {
	list_t **array_list;
	int max_size_list;
	int min_size_list;
	int size;
	int count_users;
} hash_table;

#endif // TYPES_H