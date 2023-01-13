#ifndef IPC_H
#define IPC_H

#define STRSIZE 128

#include <errno.h>
#include "types.h"
#include "string.h"

#ifdef __linux__
    #include <sys/types.h>
    #include <sys/ipc.h>
    #include <sys/msg.h>

    int send_unix(int, int, char*); // (int msqid, int msgtyp, char *text)
    int recieve_unix(int, int, message_buf*, int); // (int msqid, int msgtyp, struct msgbuf *msg)
    long put_id_msg_unix(int); // (int msqid)
    void reset(message_buf*); // (int struct msgbuf *msg)
#else
    #include <windows.h>

    typedef struct {
        int id;
        int type;
        DWORD thread_id_client;
        DWORD thread_id_server;
        char message[STRSIZE];
    } message_t;

    int create_message_queue();
    int create_shm_win(HANDLE*, message_t**, char*);
    int open_shm_win(HANDLE*, message_t**, char*);
    int recieve_win();
    int send_win(DWORD);
    long put_id_win(message_t**);
#endif

void print_error(int); //(int exit_code)

#endif // IPC_H