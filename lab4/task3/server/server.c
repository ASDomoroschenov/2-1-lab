#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/sem/sem.h"
#include "../src/shm/shm.h"
#include "../src/check/check.h"
#include "../src/variables/variables.h"
#include "../src/commands/commands.h"
#include "../src/hash_table/hash_table.h"

int generate_id(hash_table);
void print_error(int, int);

int main(int argc, char *argv[]) {
	hash_table table = {NULL, 0, 0, 0, 0};
	client *user = NULL;
	int user_id = 0;
	int exit_code = 0;
	char *command = NULL;
	char name_var = 0;
	int value_var = 0;
	char *base_var = NULL;
	message_t *msg_p = NULL; // pointer for shared memory

	#ifdef __linux__
		int semid = 0;
		int shmid = 0;
	#else
		HANDLE semids[2];
    	HANDLE shmid = 0;
	#endif

	#ifdef __linux__
		if ((semid = sem_get_unix(2, ACCESSES | IPC_CREAT)) < 0) {
			print_error(SEM_GET, 0);
			return SEM_GET;
		}

		if ((shmid = shm_get_unix(ACCESSES | IPC_CREAT)) < 0) {
			print_error(SHM_GET, 0);
			return SHM_GET;
		}

		if (!shm_at_unix(shmid, &msg_p)) {
			print_error(SHM_AT, 0);
			return SHM_AT;
		}
	#else
		if ((exit_code = create_sem_win(&semids[0], "Sem_1")) != SUCCESS) {
			printf("SemCreate error: %d\n", exit_code);

			return exit_code;
		}

		if ((exit_code = create_sem_win(&semids[1], "Sem_2")) != SUCCESS) {
			printf("SemCreate error: %d\n", exit_code);

			return exit_code;
		}

		if ((exit_code = create_shm_win(&shmid, &msg_p, "SharedMemory")) != SUCCESS) {
			printf("ShmCreate error: %d\n", exit_code);
			
			return exit_code;
		}
	#endif

	if ((exit_code = create_table(&table, HASHSIZE)) != SUCCESS) {
		msg_p->exit_code = exit_code;
		
		#ifdef __linux__
			if (sem_rm_unix(semid)) {
				print_error(SEM_RM, 0);

				return SEM_RM;
			}

			shmdt(msg_p);

			if (shm_rm_unix(shmid) < 0) {
				print_error(SHM_RM, 0);

				return SHM_RM;
			}
		#else
			CloseHandle(semids[0]);
			CloseHandle(semids[1]);
			UnmapViewOfFile(msg_p);
			CloseHandle(shmid);

			return exit_code;
		#endif
	}

	#ifdef __linux__
		sem_op_unix(semid, 0, 1);
	#else
		sem_op_win(&semids, -1, 0);
	#endif

	reset_msg(&msg_p);

	while (1) {
		if (msg_p->type == GET_ID || msg_p->type == RUN_COMMAND || msg_p->type == SET_VAR || msg_p->type == FINISH) {
			#ifdef __linux__
				sem_op_unix(semid, -1, 0);
			#else
				sem_op_win(&semids, -1, 0);
			#endif

			if (msg_p->type == GET_ID) {
				user_id = generate_id(table);

				if ((exit_code = add_user(user_id, msg_p->trace, NULL, &table)) != SUCCESS) {
					send_exit_code(user_id, ERROR_MSG, exit_code, &msg_p);
				} else {
					send_exit_code(user_id, ADD_USER, exit_code, &msg_p);
				}
			}

			if (msg_p->type == RUN_COMMAND && !is_empty_str(msg_p->message)) {
				user = get_user(msg_p->id, table);

				if (!user) {
					send_exit_code(user_id, ERROR_MSG, HAVENT_USER, &msg_p);
				} else {
					command = (char*)malloc(sizeof(char) * (strlen(msg_p->message) + 1));

					if (command) {
						strcpy(command, msg_p->message);

						if ((exit_code = strip(&command)) != SUCCESS) {
							if (command) {
								free(command);
								command = NULL;
							}
							send_exit_code(user->id, ERROR_MSG, exit_code, &msg_p);
						} else {
							if (!command || is_empty_str(command) || command[strlen(command) - 1] != ';') {
								if (command) {
									free(command);
									command = NULL;
								}
								send_exit_code(user->id, ERROR_MSG, INVALID_EXPRESSION, &msg_p);
							} else {
								if ((exit_code = delete_sep(&command)) != SUCCESS) {
									if (command) {
										free(command);
										command = NULL;
									}
									send_exit_code(user->id, ERROR_MSG, exit_code, &msg_p);
								} else {
									if (!command || is_empty_str(command)) {
										send_exit_code(user->id, EMPTY_MSG, SUCCESS, &msg_p);
									} else {
										if (check_read(command)) {
											if ((exit_code = get_name_base(command, &name_var, &base_var)) != SUCCESS) {
												send_exit_code(user->id, ERROR_MSG, exit_code, &msg_p);
											} else {
												send_read(user->id, name_var, atoi(base_var), &msg_p);
												free(base_var);
												base_var = NULL;
											}
										} else {
											if (check_write(command)) {
												if ((exit_code = get_name_base(command, &name_var, &base_var)) != SUCCESS) {
													send_exit_code(user->id, ERROR_MSG, exit_code, &msg_p);
												} else {
													if ((exit_code = get_value_var(user->array, name_var, &value_var)) != SUCCESS) {
														name_var = 0;
														value_var = 0;
														free(base_var);
														base_var = NULL;
														send_exit_code(user->id, ERROR_MSG, exit_code, &msg_p);
													} else {
														send_write(user->id, name_var, value_var, atoi(base_var), &msg_p);
														name_var = 0;
														value_var = 0;
														free(base_var);
														base_var = NULL;
													}
												}
											} else {
												if (check_binary(command) || check_unary(command)) {
													if ((exit_code = set_var(&(user->array), command)) != SUCCESS) {
														send_exit_code(user->id, ERROR_MSG, exit_code, &msg_p);
													} else {
														send_exit_code(user->id, VAR_SETED, exit_code, &msg_p);
													}
												} else {
													send_exit_code(user->id, ERROR_MSG, INVALID_EXPRESSION, &msg_p);
												}
											}
										}
									}

									if (command) {
										free(command);
										command = NULL;
									}
								}
							}
						}
					} else {
						send_exit_code(user->id, ERROR_MSG, NO_MEMORY, &msg_p);
					}
				}
			}

			if (msg_p->type == SET_VAR) {
				if ((exit_code = set_value(&(user->array), msg_p->name_var, msg_p->value_var)) != SUCCESS) {
					send_exit_code(user->id, ERROR_MSG, exit_code, &msg_p);
				} else {
					send_exit_code(user->id, VAR_SETED, exit_code, &msg_p);
				}
			}

			if (msg_p->type == FINISH) {
				if ((exit_code = delete_user(msg_p->id, &table)) != SUCCESS) {
					send_exit_code(msg_p->id, ERROR_MSG, exit_code, &msg_p);
				} else {
					send_exit_code(msg_p->id, DELETED, exit_code, &msg_p);
				}
			}

			#ifdef __linux__
				sem_op_unix(semid, 1, 1);
			#else
				sem_op_win(&semids, 1, 1);
			#endif
		}
	}

	return 0;
}

int generate_id(hash_table table) {
	return table.count_users % table.size;
}

void print_error(int exit_code, int line_error) {
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
		"INVALID_COUNT_ARGS - You put an invalid arguments\n"
		"HAVENT_USER - Base data doesn't have user with such id\n"
	};

	fprintf(stderr, "%d: %s", line_error, errors[abs(exit_code) - 2]);
}