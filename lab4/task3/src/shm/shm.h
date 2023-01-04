#ifndef MESSAGE_H
#define MESSAGE_H

#include "../types/types.h"

#define STRSIZE 	128		// max length of message  
#define SHM_KEY		20 	// key for shmget

typedef struct {
	int id;
	int type;
	int trace;
	int exit_code;
	char name_var;
	int value_var;
	int base_var;
	char message[STRSIZE];
} message_t;

int shm_get(int); // (int shm_key)
message_t *shm_at(int, message_t**); // (int shmid, msg_pointer**)
int send_msg(int, int, int, int, char, int, int, char*, message_t**); //
int send_run(int, char*, message_t**);
int send_exit_code(int, int, int, message_t**); // (int exit_code, msg_pointer**)
int send_write(int, char, int, int, message_t**);
int send_read(int, char, int, message_t**);
int send_set_var(int, char, int, message_t**);
int send_finish(int, message_t**);
int reset_msg(message_t**); // (msg_pointer**)
int shm_rm(int); // (int shmid)

#endif // MESSAGE_H