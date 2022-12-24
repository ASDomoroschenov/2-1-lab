#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/ipc.h"
#include "../lib/types.h"
#include "../lib/string.h"

int main(int argc, char const *argv[]) {
	int msqid_input = 0;
	int msqid_output = 0;
	key_t key_input = 102;
	key_t key_output = 11;
	message_buf rbuf;
	FILE *read_file = NULL;
	char *str = NULL;
	int exit_code = 0;
	int line = 1;
	long id = 0;

	if (argc != 2) {
		print_error(INVALID_ARGS);

		return INVALID_ARGS;
	}

	if ((msqid_input = msgget(key_input, 0666)) < 0) {
		print_error(MSG_GET);

		return MSG_GET;
	}

	if ((msqid_output = msgget(key_output, 0666)) < 0) {
		print_error(MSG_GET);

		return MSG_GET;
	}

	// запрашиваем ID
	if ((exit_code = send(msqid_output, GET_ID, "GET_ID")) != SUCCESS) {
		print_error(exit_code);

		return exit_code;
	}

	// ждем получения ID
	if ((exit_code = recieve(msqid_input, PUT_ID, &rbuf, MSG_NOERROR)) != SUCCESS) {
		print_error(exit_code);

		return exit_code;
	}

	id = atol(rbuf.mtext);
	reset(&rbuf);

	if ((read_file = fopen(argv[1], "r")) != NULL) {
		while (!feof(read_file)) {
			if ((exit_code = get_str(&str, read_file)) != SUCCESS) {
				print_error(exit_code);

				return exit_code;
			}
			
			if (str && !is_empty_str(str)) {
				// отправляем команду на обработку серваку
				if ((exit_code = send(msqid_output, id, str)) != SUCCESS) {
					print_error(exit_code);
					free(str);
					str = NULL;
					fclose(read_file);

					return exit_code;
				}
				//ждем exit_code от сервака
				if ((exit_code = recieve(msqid_input, id, &rbuf, MSG_NOERROR)) != SUCCESS) {
					print_error(exit_code);
					free(str);
					str = NULL;
					fclose(read_file);

					return exit_code;
				}

				exit_code = atol(rbuf.mtext);

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

				reset(&rbuf);

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