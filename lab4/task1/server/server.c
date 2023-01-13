#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/ipc.h"
#include "../lib/types.h"
#include "../lib/string.h"
#include "../lib/hash_table.h"
#include "../lib/peasant.h"

int main(int argc, char const *argv[]) {
	#ifdef __linux__
		int msqid_input = 0;
		int msqid_output = 0;
		key_t key_input = 12;
		key_t key_output = 103;
		message_buf rbuf;
	#else
		DWORD thread_id = GetCurrentThreadId();
		message_t *msg_p = NULL;
		HANDLE shmid;
	#endif
	hash_table table = {NULL, 0, 0, 0}; // взял хеш-таблицу из 9 задания
	peasant *user = NULL;
	char *exit_code_str = NULL;
	int exit_code = 0;
	long id = 0;

	srand(time(NULL));

	#ifdef __linux__
		if ((msqid_output = msgget(key_output, IPC_CREAT | 0666)) < 0) {
			print_error(MSG_GET);

			return MSG_GET;
		}

		if ((msqid_input = msgget(key_input, IPC_CREAT | 0666)) < 0) {
			print_error(MSG_GET);

			return MSG_GET;
		}
	#else
		if ((exit_code = create_shm_win(&shmid, &msg_p, "SharedMemory")) != SUCCESS) {
			fprintf(stderr, "ShmCreate error: %d\n", exit_code);
			
			return exit_code;
		}

		create_message_queue();
		msg_p->thread_id_server = thread_id;
	#endif

	while (1) {
		//ждем любое сообщение из очереди
		#ifdef __linux__
			if ((exit_code = recieve_unix(msqid_input, 0, &rbuf, IPC_NOWAIT)) == MSG_RCV) {
				print_error(exit_code);
				free_table(&table);

				return exit_code;
			}
		#else
			if ((exit_code = recieve_win()) != SUCCESS) {
				fprintf(stderr, "GetMessage error: %d\n", exit_code);

				return exit_code;
			}
		#endif

		if (exit_code == SUCCESS) {
			#ifdef __linux__
				if (rbuf.mtype == GET_ID) { // если получили сообщение с типом GET_ID(1)
					if ((id = put_id_msg_unix(msqid_output)) < 0) { // то от отправляем в очередь сообщение с типом PUT_ID(2)
						print_error(id);
						free_table(&table);

						return id;
					} else {
						if ((exit_code = add_user(id, &table)) != SUCCESS) { // если отправили без ошибок, то создаем user в хеш-таблице
							print_error(exit_code);
							free_table(&table);

							return exit_code;
						}
					}
				} else { // если не запрос на ID
					id = rbuf.mtype;
					user = get_user(id, table);
					exit_code = execute_command(rbuf.mtext, &user);  // запоминаем exit_code и переводим его в строку для отправки юзеру
					output_user(user);

					if ((exit_code = to_str(exit_code, &exit_code_str)) != SUCCESS) {
						print_error(exit_code);
						free_table(&table);

						return exit_code;
					}

					if ((exit_code = send_unix(msqid_output, id, exit_code_str)) != SUCCESS) {
						print_error(exit_code);
						free_table(&table);

						return exit_code;
					}

					free(exit_code_str);
					exit_code_str = NULL;
				}

				reset(&rbuf);
			#else
				if (msg_p->type == GET_ID) {
					if ((id = put_id_win(&msg_p)) < 0) {
						print_error(id);
						free_table(&table);

						return id;
					} else {
						if ((exit_code = add_user(id, &table)) != SUCCESS) { // если отправили без ошибок, то создаем user в хеш-таблице
							print_error(exit_code);
							free_table(&table);

							return exit_code;
						}
					}

					msg_p->type = PUT_ID;

					if ((exit_code = send_win(msg_p->thread_id_client)) != SUCCESS) {
						print_error(id);
						free_table(&table);

						return exit_code;
					}
				} else {
					id = msg_p->id;
					user = get_user(id, table);

					if (!user) {
						if ((exit_code = to_str(HAVENT_USER, &exit_code_str)) != SUCCESS) {
							print_error(exit_code);
							free_table(&table);

							return exit_code;
						}

						strcpy(msg_p->message, exit_code_str);

						if ((exit_code = send_win(msg_p->thread_id_client)) != SUCCESS) {
							fprintf(stderr, "SendMsg erro: %d\n", exit_code);
							free_table(&table);

							return exit_code;
						}
					} else {
						exit_code = execute_command(msg_p->message, &user);
						output_user(user);
						
						if ((exit_code = to_str(exit_code, &exit_code_str)) != SUCCESS) {
							print_error(exit_code);
							free_table(&table);

							return exit_code;
						}

						strcpy(msg_p->message, exit_code_str);
						msg_p->type = SEND_MSG;

						if ((exit_code = send_win(msg_p->thread_id_client)) != SUCCESS) {
							fprintf(stderr, "SendMsg erro: %d\n", exit_code);
							free_table(&table);

							return exit_code;
						}

						free(exit_code_str);
						exit_code_str = NULL;
					}
				}
			#endif
		}
	}

	#ifdef __linux__
		msgctl(msqid_input, IPC_RMID, NULL);
		msgctl(msqid_output, IPC_RMID, NULL);
	#else
		CloseHandle(shmid);
		UnmapViewOfFile(msg_p);
	#endif

	return 0;
}