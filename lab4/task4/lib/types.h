#ifndef TYPES_H
#define TYPES_H

#define SIZE_TABLE 26

enum ERROR {
	SUCCESS = -1,
	NO_MEMORY = -2,
	FULL_TABLE = -3,
	CANT_OPEN_FILE = -4,
	NO_SUCH_ITEM = -5,
	OUT_OF_RANGE = -6,
	EMPTY_ARRAY = -7,
	EMPTY_STRING = -8,
	INVALID_COMMAND = -9,
	HAVENT_VALIDATE_DATA = -10
};

typedef struct {
	int *data;
	int size;
} Array;

typedef struct {
	char key;
	Array *value;
} hash_item;

typedef struct {
	hash_item **items;
	int current_size;
	int size;
} hash_table;

#endif // TYPES_H