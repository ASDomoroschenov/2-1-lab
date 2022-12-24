#ifndef IPC_H
#define IPC_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include "types.h"
#include "string.h"

int send(int, int, char*); // (int msqid, int msgtyp, char *text)
int recieve(int, int, message_buf*, int); // (int msqid, int msgtyp, struct msgbuf *msg)
long put_id_msg(int); // (int msqid)
void reset(message_buf*); // (int struct msgbuf *msg)
void print_error(int); //(int exit_code)

#endif // IPC_H