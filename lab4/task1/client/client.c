#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/ipc.h"
#include "../lib/types.h"
#include "../lib/string.h"

int main(int argc, char const *argv[]) {
	#ifdef __linux__
		int msqid_input = 0;
		int msqid_output = 0;
		key_t key_input = 103;
		key_t key_output = 12;
		message_buf rbuf;
	#else
		DWORD thread_id = GetCurrentThreadId();
		message_t *msg_p = NULL;
		HANDLE shmid;
	#endif
	FILE *read_file = NULL;
	char *str = NULL;
	int exit_code = 0;
	int line = 1;
	long id = 0;

	if (argc != 2) {
		print_error(INVALID_ARGS);

		return INVALID_ARGS;
	}

	#ifdef __linux__
		if ((msqid_input = msgget(key_input, 0666)) < 0) {
			print_error(MSG_GET);

			return MSG_GET;
		}

		if ((msqid_output = msgget(key_output, 0666)) < 0) {
			print_error(MSG_GET);

			return MSG_GET;
		}

		// запрашиваем ID
		if ((exit_code = send_unix(msqid_output, GET_ID, "GET_ID")) != SUCCESS) {
			print_error(exit_code);

			return exit_code;
		}

		// ждем получения ID
		if ((exit_code = recieve_unix(msqid_input, PUT_ID, &rbuf, MSG_NOERROR)) != SUCCESS) {
			print_error(exit_code);

			return exit_code;
		}

		id = atol(rbuf.mtext);
		reset(&rbuf);
	#else
		if ((exit_code = open_shm_win(&shmid, &msg_p, "SharedMemory")) != SUCCESS) {
			printf("SHM_OPEN error: %d\n", exit_code);

			return exit_code;
		}

		msg_p->type = GET_ID;
		msg_p->thread_id_client = thread_id;

		if ((exit_code = send_win(msg_p->thread_id_server)) != SUCCESS) {
			printf("%ld %ld\n", msg_p->thread_id_client, msg_p->thread_id_server);
			fprintf(stderr, "SendMsg error: %d\n", exit_code);

			return exit_code;
		}

		if ((exit_code = recieve_win()) != SUCCESS) {
			print_error(exit_code);

			return exit_code;
		}

		id = atol(msg_p->message);
		printf("%ld\n", id);
	#endif

	if ((read_file = fopen(argv[1], "r")) != NULL) {
		while (!feof(read_file)) {
			if ((exit_code = get_str(&str, read_file)) != SUCCESS) {
				print_error(exit_code);

				return exit_code;
			}
			
			if (str && !is_empty_str(str)) {
				// отправляем команду на обработку серваку
				#ifdef __linux__
					if ((exit_code = send_unix(msqid_output, id, str)) != SUCCESS) {
						print_error(exit_code);
						free(str);
						str = NULL;
						fclose(read_file);

						return exit_code;
					}
					//ждем exit_code от сервака
					if ((exit_code = recieve_unix(msqid_input, id, &rbuf, MSG_NOERROR)) != SUCCESS) {
						print_error(exit_code);
						free(str);
						str = NULL;
						fclose(read_file);

						return exit_code;
					}

					exit_code = atol(rbuf.mtext);
					reset(&rbuf);
				#else
					strcpy(msg_p->message, str);
					msg_p->id = id;
					msg_p->type = SEND_MSG;

					if ((exit_code = send_win(msg_p->thread_id_server)) != SUCCESS) {
						print_error(exit_code);
						free(str);
						str = NULL;
						fclose(read_file);

						return exit_code;
					}

					if ((exit_code = recieve_win()) != SUCCESS) {
						print_error(exit_code);
						free(str);
						str = NULL;
						fclose(read_file);

						return exit_code;
					}

					exit_code = atol(msg_p->message);
				#endif

				if (exit_code != SUCCESS) {
					printf("%d: %s - ", line, str);
					print_error(exit_code);
					free(str);
					str = NULL;
					fclose(read_file);

					return exit_code;
				} else {
					printf("%d: %s - SUCCESS\n", line, str);
				}

				free(str);
				str = NULL;
			}

			line++;
		}

		fclose(read_file);
	} else {
		print_error(CANT_OPEN_FILE);
	}

	return 0;
}