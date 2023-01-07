#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sem.h"

#ifdef __linux__
	int sem_get_unix(int count_sem, int flags) {
		return semget(SEM_KEY, count_sem, flags);
	}

	int sem_op_unix(int semid, int sem_value_1, int sem_value_2) {
		struct sembuf sbuf[2];

		sbuf[0].sem_num = 0;
		sbuf[0].sem_op = sem_value_1;
		sbuf[0].sem_flg = 0;

		sbuf[1].sem_num = 1;
		sbuf[1].sem_op = sem_value_2;
		sbuf[1].sem_flg = 0;

		return semop(semid, sbuf, 2);
	}

	int sem_rm_unix(int semid) {
		return semctl(semid, 0, IPC_RMID, NULL);
	}
#else
	int create_sem_win(HANDLE *semid, char *name) {
		*semid = CreateSemaphoreA(NULL, MAX_VALUE_SEM, MAX_VALUE_SEM, name);

		if (!*semid)  {
			return GetLastError();
		}

		return SUCCESS;
	}

	int open_sem_win(HANDLE *semid, char *name) {
		*semid = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, name);

		if (!*semid) {
			return GetLastError();
		}

		return SUCCESS;
	}

	int sem_op_win(HANDLE (*semids)[2], int sem_value_1, int sem_value_2) {
		int wait_result = 0;

		if (sem_value_1 < 0 && sem_value_2 < 0) {
			wait_result = WaitForMultipleObjects(2, *semids, TRUE, INFINITE);
		} else {
			if (sem_value_1 < 0) {
				wait_result = WaitForSingleObject((*semids)[0], INFINITE);
			} else {
				if (sem_value_1 > 0) {
					wait_result = ReleaseSemaphore((*semids)[0], 1, NULL);
				}
			}

			if (sem_value_2 < 0) {
				WaitForSingleObject((*semids)[1], INFINITE);
			} else {
				if (sem_value_2 > 0) {
					wait_result = ReleaseSemaphore((*semids)[1], 1, NULL);
				}
			}
		}

		return wait_result;
	}
#endif