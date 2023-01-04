#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/ipc.h"

int send_unix(int qid, int msgtype, char *message) {
	message_buf msg;
	msg.mtype = msgtype;
	strcpy(msg.mtext, message);

	if (msgsnd(qid, (void*)(&msg), strlen(message) + 1, IPC_NOWAIT) == -1) {
		//fprintf(stderr, "msgsnd error\n");
		return MSG_SND;
	}

	//printf("отправка: %s\n", msg.mtext);

	return SUCCESS;
}

int recieve_unix(int qid, int msgtype, message_buf *msg, int flags) {
	if (msgrcv(qid, (void*)msg, MSGMAX, msgtype, flags) == -1) {
		if (errno != ENOMSG) { // если не ошибка об отсутствии сообщений в очереди
			return MSG_RCV;
		} else {
			return DIDNT_GET_MSG;
		}
	} else {
		//printf("приём: %s\n", msg->mtext);
	}

	return SUCCESS;
}

long put_id_msg(int qid) {
	int exit_code = 0;
	char *id_str = NULL;
	long id_long = rand();

	to_str(id_long, &id_str);

	if ((exit_code = send_unix(qid, PUT_ID, id_str)) != SUCCESS) {
		return exit_code;
	}
	
	free(id_str);

	return id_long;
}

void reset(message_buf *msg) {
	msg->mtype = 0;

	for (int i = 0; i < MSGMAX; i++) {
		msg->mtext[i] = 0;
	}
}

void print_error(int exit_code) {
	char *errors[14] = {
		"NO_MEMORY: The system is out of memory\n",
		"CANT_OPEN_FILE: Can't open file\n",
		"INVALID_ARGS: You put an invalid arguments\n",
		"MSG_GET: Can't get queue\n",
		"MSG_SND: Can't send message to queue\n",
		"MSG_RCV: Can't get message from queue\n",
		"HAVENT_FREE_PLACE: Havent free place on boat\n",
		"THERE_IS_NO_OBJECT_COASTS: There is no this object on coasts\n",
		"THERE_IS_NO_OBJECT_BOAT: There is no object on boat, you can put on coast\n",
		"WOLF_EAT_GOAT: The wolf ate the goat on the coast\n",
		"GOAT_EAT_CABBAGE: The goat ate the cabbage on the coast\n",
		"WIN: You win\n",
		"EMPTY_STRING: Can't do calculations with empty string\n",
		"WRONG_COMMAND: You put an invalid command\n"
	};

	printf("%s", errors[abs(exit_code) - 2]);
}