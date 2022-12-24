#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/ipc.h"
#include "../lib/types.h"
#include "../lib/string.h"
#include "../lib/hash_table.h"
#include "../lib/peasant.h"
#include "../lib/execute.h"

int main(int argc, char const *argv[]) {
	int msqid_input = 0;
	int msqid_output = 0;
	key_t key_input = 11;
	key_t key_output = 102;
	message_buf rbuf;
	hash_table table = {NULL, 0, 0, 0};
	peasant *user = NULL;
	char *exit_code_str = NULL;
	int exit_code = 0;
	long id = 0;

	srand(time(NULL));

	if ((msqid_output = msgget(key_output, IPC_CREAT | 0666)) < 0) {
		print_error(MSG_GET);

		return MSG_GET;
	}

	if ((msqid_input = msgget(key_input, IPC_CREAT | 0666)) < 0) {
		print_error(MSG_GET);

		return MSG_GET;
	}

	while (1) {
		//ждем любое сообщение из очереди
		if ((exit_code = recieve(msqid_input, 0, &rbuf, IPC_NOWAIT)) == MSG_RCV) {
			print_error(exit_code);
			free_table(&table);

			return exit_code;
		}

		if (exit_code == SUCCESS) {
			if (rbuf.mtype == GET_ID) { // если получили сообщение с типом GET_ID(1)
				if ((id = put_id_msg(msqid_output)) < 0) { // то от отправляем в очередь сообщение с типом PUT_ID(2)
					print_error(exit_code);
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

				if ((exit_code = send(msqid_output, id, exit_code_str)) != SUCCESS) {
					print_error(exit_code);
					free_table(&table);

					return exit_code;
				}

				free(exit_code_str);
				exit_code_str = NULL;
			}

			reset(&rbuf);
		}
	}

	msgctl(msqid_input, IPC_RMID, NULL);
	msgctl(msqid_output, IPC_RMID, NULL);

	return 0;
}