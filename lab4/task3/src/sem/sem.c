#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sem.h"

int sem_get(int count_sem, int flags) {
	return semget(SEM_KEY, count_sem, flags);
}

int sem_op(int semid, int sem_count, ...) {
	struct sembuf sbuf[sem_count];
	int sem_val = 0;
	va_list args;

	va_start(args, sem_count);

	for (int i = 0; i < sem_count; i++) {
		sem_val = va_arg(args, int);
		sbuf[i].sem_num = i;
		sbuf[i].sem_op = sem_val;
		sbuf[i].sem_flg = 0;
	}

	va_end(args);

	return semop(semid, sbuf, sem_count);
}

int sem_rm(int semid) {
	return semctl(semid, 0, IPC_RMID, NULL);
}