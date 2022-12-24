#ifndef TYPES_H
#define TYPES_H

#define MSGMAX 8192
#define HASHSIZE 128

enum ERROR {
	SUCCESS = -1,
	NO_MEMORY = -2,
	CANT_OPEN_FILE = -3,
	INVALID_ARGS = -4,
	MSG_GET = -5,
	MSG_SND = -6,
	MSG_RCV = -7,
	HAVENT_FREE_PLACE = -8,
	THERE_IS_NO_OBJECT_COASTS = -9,
	THERE_IS_NO_OBJECT_BOAT = -10,
	WOLF_EAT_GOAT = -11,
	GOAT_EAT_CABBAGE = -12,
	WIN = -13,
	EMPTY_STRING = -14,
	WRONG_COMMAND = -15,
	DIDNT_GET_MSG = -16
};

enum MSG_TYPS {
	GET_ID = 1,
	PUT_ID = 2
};

typedef struct msgbuf {
	long mtype;
	char mtext[MSGMAX];
} message_buf;

typedef struct {
	int is_free_boat;
	char *on_boat;
	char *left_coast[3];
	char *right_coast[3];
	int is_left_coast;
	int is_right_coast;
	int count_right_coast;
	int count_left_coast;
	long id;
} peasant;

typedef struct tag_node_list {
	struct tag_node_list *next;
	peasant *user;
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
} hash_table;

#endif // TYPES_H