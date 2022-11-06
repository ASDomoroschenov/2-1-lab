#ifndef TYPES_H
#define TYPES_H

enum ERRORS {
	NO_MEMORY = -2,
	SUCCESS = -3,
	INVALID_CITIZEN = -4,
	OPEN_FILE = -5,
	INVALID_ARGS = -6,
	NOT_TXT_FILE = -7,
	EMPTY_LIST = -8
};

enum sex {
	MALE = 0,
	FEMALE = 1
};

typedef struct {
	char *surname;
	char *name;
	char *patronymic;
	char *date_birth;
	enum sex gender;
	double avrg_income;
} citizen;

typedef struct tag_list_node {
	struct tag_list_node *next;
	citizen *human;
} list_node;

typedef struct {
	list_node *root;
} list;

#endif // TYPES_H