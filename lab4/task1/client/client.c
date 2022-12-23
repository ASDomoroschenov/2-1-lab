#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
	long mtype;
	char *mtext;
} message_buf;

enum ERRORS {
	SUCCESS = -1,
	NO_MEMORY = -2
};

int join_symb(char, char**, int*);
int get_str(char**, FILE*);

int main(int argc, char const *argv[]) {
	int msqid = 0;
	key_t key = 10;
	message_buf sbuf = {1, NULL};
	int buf_length = 0;

	if ((msqid = msgget(key, 0666)) < 0) {
		fprintf(stderr, "MSGGET error\n");

		return msqid;
	}

	get_str(&(sbuf.mtext), stdin);
	sbuf.mtype = 1;
	buf_length = strlen(sbuf.mtext) + 1;

	if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
		fprintf(stderr, "MSGSND error\n");
	} else {
		printf("Message sent\n");
	}

	return 0;
}

int join_symb(char symb, char **str, int *size) {
	if (*size == 0) {
		*size = 2;
		*str = (char*)malloc(sizeof(char) * *size);

		if (!*str) {
			return NO_MEMORY;
		}

		(*str)[0] = symb;
		(*str)[1] = 0;

		return SUCCESS;
	}

	int len = strlen(*str);

	if (len == *size - 1) {
		*size *= 2;
		char *temp = (char*)realloc(*str, sizeof(char) * *size);
		
		if (!temp) {
			return NO_MEMORY;
		}

		*str = temp;
	}

	(*str)[len + 1] = 0;
	(*str)[len] = symb;

	return SUCCESS;
}

int get_str(char **str, FILE *read_file) {
	int size = 0;
	char symb = fgetc(read_file);
	int exit_code = 0;

	if (*str) {
		free(*str);
		*str = NULL;
	}

	while (symb != EOF && symb != '\n') {
		exit_code = join_symb(symb, str, &size);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		symb = fgetc(read_file);
	}

	if (*str) {
		char *temp = (char*)realloc(*str, sizeof(char) * (strlen(*str) + 1));

		if (!temp) {
			return NO_MEMORY;
		}

		*str = temp;
	}

	return SUCCESS;
}