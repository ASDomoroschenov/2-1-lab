#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/check/check.h"
#include "../src/operations/operations.h"
#include "../src/string/string.h"
#include "../src/types/types.h"
#include "../src/sem/sem.h"
#include "../src/shm/shm.h"

#ifdef __linux__
	int get_id_unix(int, message_t**);
	int delete_success_user_unix(int, int, message_t**);
	int delete_error_user_unix(int, int, message_t**);
#else
	int get_id_win(HANDLE (*)[2], message_t**);
	int delete_success_user_win(HANDLE (*)[2], int, message_t**);
	int delete_error_user_win(HANDLE (*)[2], int, message_t**);
#endif

int read_var(char, int, int*);
int print_var(char, int, int);
void print_error(int, int);

int main(int argc, char *argv[]) {
	char *str = NULL;
	int exit_code = 0;
	FILE *read_file = NULL;
	int line_error = 0;
	int user_id = 0;
	int value_var = 0;
	char name_var = 0;
	message_t *msg_p = NULL; // pointer for shared memory

	#ifdef __linux__
		int semid = 0;
		int shmid = 0;
	#else
		HANDLE semids[2];
    	HANDLE shmid = 0;
	#endif

	if (argc > 1) {
		if ((read_file = fopen(argv[1], "r")) != NULL) {
			#ifdef __linux__
				if ((semid = sem_get_unix(2, ACCESSES)) < 0) {
					print_error(SEM_GET, 0);

					return SEM_GET;
				}

				if ((shmid = shm_get_unix(ACCESSES)) < 0) {
					print_error(SHM_GET, 0);

					return SHM_GET;
				}

				if (!shm_at_unix(shmid, &msg_p)) {
					print_error(SHM_AT, 0);
					
					return SHM_AT;
				}

				user_id = get_id_unix(semid, &msg_p);
			#else
				if ((exit_code = open_sem_win(&semids[0], "Sem_1")) != SUCCESS) {
					printf("SEM_OPEN error: %d\n", exit_code);
					
					return exit_code;
				}

				if ((exit_code = open_sem_win(&semids[1], "Sem_2")) != SUCCESS) {
					printf("SEM_OPEN error: %d\n", exit_code);
					
					return exit_code;
				}

				if ((exit_code = open_shm_win(&shmid, &msg_p, "SharedMemory")) != SUCCESS) {
					printf("SHM_OPEN error: %d\n", exit_code);

					return exit_code;
				}

				user_id = get_id_win(&semids, &msg_p);
			#endif

			while (!feof(read_file)) {
				#ifdef __linux__
					sem_op_unix(semid, 0, -1);
				#else
					sem_op_win(&semids, 0, -1);
				#endif

				if (msg_p->type == READ) {
					name_var = msg_p->name_var;

					if ((exit_code = read_var(msg_p->name_var, msg_p->base_var, &value_var)) != SUCCESS) {
						print_error(exit_code, line_error);

						#ifdef __linux__
							if (delete_error_user_unix(semid, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
								reset_msg(&msg_p);
								shmdt(msg_p);
								print_error(exit_code, line_error);
								sem_op_unix(semid, 0, 1);
								fclose(read_file);

								return exit_code;
							}

							reset_msg(&msg_p);
							shmdt(msg_p);
						#else
							if (delete_error_user_win(&semids, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
								reset_msg(&msg_p);
								print_error(exit_code, line_error);
								sem_op_win(&semids, 0, 1);
								fclose(read_file);

								return exit_code;
							}

							reset_msg(&msg_p);
						#endif

						fclose(read_file);

						#ifdef __linux__
							sem_op_unix(semid, 0, 1);
						#else
							sem_op_win(&semids, 0, 1);
						#endif

						return exit_code;
					}

					send_set_var(user_id, name_var, value_var, &msg_p);

					#ifdef __linux__
						sem_op_unix(semid, 1, 0);
						sem_op_unix(semid, -1, -1);
					#else
						sem_op_win(&semids, 1, 0);
						sem_op_win(&semids, -1, -1);
					#endif

					if (msg_p->type == ERROR_MSG) {
						print_error(msg_p->exit_code, line_error);
						exit_code = msg_p->exit_code;

						#ifdef __linux__
							if (delete_error_user_unix(semid, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
								print_error(exit_code, line_error);
								reset_msg(&msg_p);
								shmdt(msg_p);
								sem_op_unix(semid, 0, 1);
								fclose(read_file);

								return exit_code;
							}
							
							reset_msg(&msg_p);
							shmdt(msg_p);
						#else
							if (delete_error_user_win(&semids, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
								print_error(exit_code, line_error);
								reset_msg(&msg_p);
								sem_op_win(&semids, 0, 1);
								fclose(read_file);

								return exit_code;
							}
							
							reset_msg(&msg_p);
						#endif

						fclose(read_file);

						#ifdef __linux__
							sem_op_unix(semid, 0, 1);
						#else
							sem_op_win(&semids, 0, 1);
						#endif

						return exit_code;
					}
				} else {
					if ((exit_code = get_str_with_comments(&str, read_file, '%', '{', '}', &line_error)) != SUCCESS) {
						print_error(exit_code, line_error);
						
						#ifdef __linux__
							if (delete_error_user_unix(semid, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
								print_error(exit_code, line_error);
								reset_msg(&msg_p);
								shmdt(msg_p);

								sem_op_unix(semid, 0, 1);

								return exit_code;
							}

							shmdt(msg_p);
						#else
							if (delete_error_user_win(&semids, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
								print_error(exit_code, line_error);
								reset_msg(&msg_p);
								sem_op_win(&semids, 0, 1);

								return exit_code;
							}
						#endif

						fclose(read_file);
						
						#ifdef __linux__
							sem_op_unix(semid, 0, 1);
						#else
							sem_op_win(&semids, 0, 1);
						#endif

						return exit_code;
					}

					send_run(user_id, str, &msg_p);
					
					#ifdef __linux__
						sem_op_unix(semid, 1, 0);
						sem_op_unix(semid, -1, -1);
					#else
						sem_op_win(&semids, 1, 0);
						sem_op_win(&semids, -1, -1);
					#endif

					if (msg_p->type == ERROR_MSG) {
						print_error(msg_p->exit_code, line_error);
						exit_code = msg_p->exit_code;

						#ifdef __linux__
							if (delete_error_user_unix(semid, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
							}

							shmdt(msg_p);
						#else
							if (delete_error_user_win(&semids, user_id, &msg_p) != SUCCESS) {
								exit_code = msg_p->exit_code;
							}
						#endif

						reset_msg(&msg_p);
						free(str);
						str = NULL;
						fclose(read_file);

						#ifdef __linux__
							sem_op_unix(semid, 0, 1);
						#else
							sem_op_win(&semids, 0, 1);
						#endif

						return exit_code;
					}

					if (msg_p->type == WRITE) {
						if ((exit_code = print_var(msg_p->name_var, msg_p->base_var, msg_p->value_var)) != SUCCESS) {
							print_error(exit_code, line_error);

							#ifdef __linux__
								if (delete_error_user_unix(semid, user_id, &msg_p) != SUCCESS) {
									exit_code = msg_p->exit_code;
								}

								shmdt(msg_p);
							#else
								if (delete_error_user_win(&semids, user_id, &msg_p) != SUCCESS) {
									exit_code = msg_p->exit_code;
								}
							#endif
							
							reset_msg(&msg_p);
							free(str);
							str = NULL;
							fclose(read_file);

							#ifdef __linux__
								sem_op_unix(semid, 0, 1);
							#else
								sem_op_win(&semids, 0, 1);
							#endif

							return exit_code;
						}
					}
				}

				#ifdef __linux__
					sem_op_unix(semid, 0, 1);
				#else
					sem_op_win(&semids, 0, 1);
				#endif
			}
			//Отправляем сообщение что клиент закончил работу с сервером
			fclose(read_file);
			reset_msg(&msg_p);

			#ifdef __linux__
				if ((exit_code = delete_success_user_unix(semid, user_id, &msg_p)) != SUCCESS) {
					exit_code = msg_p->exit_code;
					print_error(exit_code, line_error);
					reset_msg(&msg_p);
					shmdt(msg_p);
					sem_op_unix(semid, 0, 1);

					return exit_code;
				}

				shmdt(msg_p);
				sem_op_unix(semid, 0, 1);
			#else
				if ((exit_code = delete_success_user_win(&semids, user_id, &msg_p)) != SUCCESS) {
					exit_code = msg_p->exit_code;
					print_error(exit_code, line_error);
					reset_msg(&msg_p);
					sem_op_win(&semids, 0, 1);

					return exit_code;
				}

				sem_op_win(&semids, 0, 1);
			#endif
		} else {
			print_error(OPEN_FILE, 1);

			return OPEN_FILE;
		}
	} else {
		print_error(INVALID_COUNT_ARGS, 1);

		return INVALID_COUNT_ARGS;
	}

	return 0;
}

#ifdef __linux__
	int get_id_unix(int semid, message_t **msg_p) {
		int id = 0;

		sem_op_unix(semid, 0, -1);
		send_msg(0, GET_ID, 0, 0, 0, 0, 0, NULL, msg_p);
		sem_op_unix(semid, 1, 0);
		sem_op_unix(semid, -1, -1);
		id = (*msg_p)->id;
		sem_op_unix(semid, 0, 1);
		reset_msg(msg_p);

		return id;
	}

	int delete_success_user_unix(int semid, int user_id, message_t **msg_p) {
		sem_op_unix(semid, 0, -1);
		send_finish(user_id, msg_p);
		sem_op_unix(semid, 1, 0);
		sem_op_unix(semid, -1, -1);

		return (*msg_p)->exit_code;
	}

	int delete_error_user_unix(int semid, int user_id, message_t **msg_p) {
		send_finish(user_id, msg_p);
		sem_op_unix(semid, 1, 0);
		sem_op_unix(semid, -1, -1);

		return (*msg_p)->exit_code;
	}
#else
	int get_id_win(HANDLE (*semids)[2], message_t **msg_p) {
		int id = 0;
		sem_op_win(semids, 0, -1);
		send_msg(0, GET_ID, 0, 0, 0, 0, 0, NULL, msg_p);
		sem_op_win(semids, 1, 0);
		sem_op_win(semids, -1, -1);
		id = (*msg_p)->id;
		sem_op_win(semids, 0, 1);
		reset_msg(msg_p);

		return id;
	}

	int delete_success_user_win(HANDLE (*semids)[2], int user_id, message_t **msg_p) {
		sem_op_win(semids, 0, -1);
		send_finish(user_id, msg_p);
		sem_op_win(semids, 1, 0);
		sem_op_win(semids, -1, -1);

		return (*msg_p)->exit_code;
	}

	int delete_error_user_win(HANDLE (*semids)[2], int user_id, message_t **msg_p) {
		send_finish(user_id, msg_p);
		sem_op_win(semids, 1, 0);
		sem_op_win(semids, -1, -1);

		return (*msg_p)->exit_code;
	}
#endif

void print_error(int exit_code, int line_error) {
	printf("%d\n", exit_code);

	char *errors[12] = {
		"NO_MEMORY - The system is out of memory\n",
		"OPEN_FILE - Can't open file\n",
		"INVALID_EXPRESSION - You put an invalid expression\n",
		"HAVENT_VAR - There is an undeclared variable in the expression\n",
		"INVALID_MULTILINE_COMMENT - You must close multiline comment\n",
		"SEM_GET - Can't get semaphore('s)\n",
		"SHM_GET - Can't get shared memory\n",
		"SHM_AT - Can't get adrres of shared memory\n",
		"SEM_RM - Can't remove semaphore('s)\n",
		"SHM_RM - Can't remove shared memory\n",
		"INVALID_COUNT_ARGS - You put an invalid arguments\n",
		"HAVENT_USER - Base data doesn't have user with such id\n"
	};

	fprintf(stderr, "%d: %s", line_error, errors[abs(exit_code) - 2]);
}

int read_var(char name_var, int base_var, int *value_var) {
	int exit_code = 0;
	char *str = NULL;

	printf("Enter the value of variable in %d notation: ", base_var);

	if ((get_str(&str, stdin)) != SUCCESS) {
		return exit_code;
	}

	if (!is_valid_num(str, base_var)) {
		free(str);
		return INVALID_EXPRESSION;
	}

	exit_code = to_10(str, base_var, value_var);

	free(str);

	return exit_code;
}

int print_var(char name_var, int base, int value_var) {
	char *value_base = NULL;
	int exit_code = 0;

	if ((exit_code = to_base(value_var, base, &value_base)) != SUCCESS) {
		return exit_code;
	}

	if (is_empty_str(value_base)) {
		printf("%c := 0\n", name_var);
	} else {
		printf("%c := %s\n", name_var, value_base);
	}

	free(value_base);
	
	return SUCCESS;
}