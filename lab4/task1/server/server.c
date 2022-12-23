#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGSZ 128

typedef struct msgbuf {
	long mtype;
	char *mtext;
} message_buf;

int main(int argc, char const *argv[]) {
	int msqid = 0;
	key_t key = 10;
	int msgflg = IPC_CREAT | 0666;
	message_buf rbuf;

	if ((msqid = msgget(key, msgflg)) < 0) {
		fprintf(stderr, "MSGGET error\n");

		return msqid;
	}

	if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
		fprintf(stderr, "MSGRCV error");

		return 0;
	} else {
		printf("1\n");
		printf("%s\n", rbuf.mtext);
	}

	return 0;
}