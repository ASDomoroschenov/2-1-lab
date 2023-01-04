#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "shm.h"

int shm_get(int flags) {
	return shmget(SHM_KEY, sizeof(message_t), flags);
}

message_t *shm_at(int shmid, message_t **msg_p) {
	*msg_p = (message_t*)shmat(shmid, 0, 0);

	return *msg_p;
}

int send_msg(int id, int type, int trace, int exit_code, char name_var, int value_var, int base_var, char *message, message_t **msg_p) {
	reset_msg(msg_p);
	
	(*msg_p)->id = id;
	(*msg_p)->type = type;
	(*msg_p)->trace = trace;
	(*msg_p)->exit_code = exit_code;
	(*msg_p)->name_var = name_var;
	(*msg_p)->value_var = value_var;
	(*msg_p)->base_var = base_var;

	if (message) {
		strcpy((*msg_p)->message, message);
	}

	return SUCCESS;
}

int send_run(int id, char *command, message_t **msg_p) {
	return send_msg(id, RUN_COMMAND, 0, SUCCESS, 0, 0, 0, command, msg_p);
}

int send_exit_code(int id, int type, int exit_code, message_t **msg_p) {
	return send_msg(id, type, 0, exit_code, 0, 0, 0, NULL, msg_p);
}

int send_write(int id, char name_var, int value_var, int base_var, message_t **msg_p) {
	return send_msg(id, WRITE, 0, SUCCESS, name_var, value_var, base_var, NULL, msg_p);
}

int send_read(int id, char name_var, int base_var, message_t **msg_p) {
	return send_msg(id, READ, 0, SUCCESS, name_var, 0, base_var, NULL, msg_p);
}

int send_set_var(int id, char name_var, int value_var, message_t **msg_p) {
	return send_msg(id, SET_VAR, 0, SUCCESS, name_var, value_var, 0, NULL, msg_p);
}

int send_finish(int id, message_t **msg_p) {
	return send_msg(id, FINISH, 0, 0, 0, 0, 0, NULL, msg_p);
}

int reset_msg(message_t **msg_p) {
	for (int i = 0; i < STRSIZE; i++) {
		(*msg_p)->message[i] = 0;
	}

	(*msg_p)->id = 0;
	(*msg_p)->type = 0;
	(*msg_p)->trace = 0;
	(*msg_p)->exit_code = 0;
	(*msg_p)->name_var = 0;
	(*msg_p)->value_var = 0;
	(*msg_p)->base_var = 0;

	return SUCCESS;
}

int shm_rm(int shmid) {
	return shmctl(shmid, IPC_RMID, NULL);
}