#ifndef MESSAGE_H
#define MESSAGE_H

#include "../types/types.h"

#ifdef __linux__
	int shm_get_unix(int); // (int shm_key)
	message_t *shm_at_unix(int, message_t**); // (int shmid, msg_pointer**)
	int shm_rm_unix(int); // (int shmid)
#else
	int create_shm_win(HANDLE*, message_t**, char*);
	int open_shm_win(HANDLE*, message_t**, char*);
#endif

int send_msg(int, int, int, int, char, int, int, char*, message_t**); //
int send_run(int, char*, message_t**);
int send_exit_code(int, int, int, message_t**); // (int exit_code, msg_pointer**)
int send_write(int, char, int, int, message_t**);
int send_read(int, char, int, message_t**);
int send_set_var(int, char, int, message_t**);
int send_finish(int, message_t**);
int reset_msg(message_t**); // (msg_pointer**)

#endif // MESSAGE_H