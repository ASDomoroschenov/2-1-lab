#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "../types/types.h"
#ifdef __linux__
    #define SEM_KEY 15 // key for semget

    int sem_get_unix(int, int); // (int count_sem, int flags)
    int sem_op_unix(int, int, int); // (int semid, int count_sems, values...)
    int sem_rm_unix(int); // (int semid)
#else
    #define MAX_VALUE_SEM 1

    int create_sem_win(HANDLE*, char*);
    int open_sem_win(HANDLE*, char*);
    int sem_op_win(HANDLE (*)[2], int, int);
#endif

#endif // SEMAPHORE_H