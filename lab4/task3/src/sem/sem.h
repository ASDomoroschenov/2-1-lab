#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "../types/types.h"

#define SEM_KEY 15 // key for semget

int sem_get(int, int); // (int count_sem, int flags)
int sem_op(int, int, ...); // (int semid, int count_sems, values...)
int sem_rm(int); // (int semid)

#endif // SEMAPHORE_H