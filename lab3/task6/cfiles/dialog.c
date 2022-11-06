#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/dialog.h"

int run_add_mail(post *item) {
	int exit_code = add_mail_str(item);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	return SUCCESS;
}

int run_delete_id(post *item) {
	if (item->curr_size == 0) {
		return EMPTY_MAILS;
	}
	
	printf("ID: ");
	char *id = NULL;

	if (get_str(&id, stdin) == NO_MEMORY ||
		strip(&id) == NO_MEMORY) {
		return NO_MEMORY;
	}

	if (!check_mail_id(id)) {
		free(id);
		return INVALID_MAIL_ID;
	}

	int exit_code = delete_id_mail(item, id);

	if (exit_code != SUCCESS) {
		free(id);
		return exit_code;
	}

	free(id);

	return SUCCESS;
}

int run_delete_full(post *item) {
	if (item->curr_size == 0) {
		return EMPTY_MAILS;
	}

	printf("MAIL: ");
	int exit_code = delete_full_mail(item);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	return SUCCESS;
}

int run_find_id(post item) {
	if (item.curr_size == 0) {
		return EMPTY_MAILS;
	}

	printf("ID: ");
	char *id = NULL;

	if (get_str(&id, stdin) == NO_MEMORY ||
		strip(&id) == NO_MEMORY) {
		return NO_MEMORY;
	}
	if (!check_mail_id(id)) {
		free(id);
		return INVALID_MAIL_ID;
	}

	mail **find_mails = NULL;
	int count_finds = 0;
	int exit_code = find_id_mail(&find_mails, item.mails, &count_finds, item.curr_size, id);

	if (exit_code != SUCCESS) {
		free(id);
		free(find_mails);
		return exit_code;
	}

	for (int i = 0; i < count_finds; i++) {
		output_beatiful_mail(i, find_mails[i], stdout);
	}

	free(id);
	free(find_mails);

	return SUCCESS;
}

int run_find_delivery(char *datetime, post item) {
	if (item.curr_size == 0) {
		return EMPTY_MAILS;
	}

	if (!check_datetime(datetime)) {
		return INVALID_DATETIME;
	}

	binom_heap *heap = NULL;
	int count_finds = 0;

	if (get_delivered_mails(datetime, item, &heap, &count_finds, cmp_mail_dates) == NO_MEMORY) {
		return NO_MEMORY;
	}

	if (!heap) {
		return EMPTY_MAILS;
	}

	output_find_mails(heap, count_finds, cmp_mail_dates);
	free_bin_heap(&heap);

	return SUCCESS;
}

int run_find_not_delivery(char *datetime, post item) {
	if (item.curr_size == 0) {
		return EMPTY_MAILS;
	}

	if (!check_datetime(datetime)) {
		return INVALID_DATETIME;
	}

	binom_heap *heap = NULL;
	int count_finds = 0;
	if (get_not_delivered_mails(datetime, item, &heap, &count_finds, cmp_mail_dates) == NO_MEMORY) {
		return NO_MEMORY;
	}

	if (!heap) {
		return EMPTY_MAILS;
	}

	output_find_mails(heap, count_finds, cmp_mail_dates);
	free_bin_heap(&heap);

	return SUCCESS;	
}

int run_sort(post *item) {
	if (item->curr_size == 0) {
		return EMPTY_MAILS;
	}

	int exit_code = heap_sort(&(item->mails), item->curr_size, cmp_mail_sort);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	return SUCCESS;
}

int run_output(post item) {
	if (item.curr_size == 0) {
		return EMPTY_MAILS;
	}

	output_mails(item);

	return SUCCESS;
}

int dialog(char *datetime) {
	printf("COMMANDS:\n");
	printf("ADD: add mail\n");
	printf("DELETE ID: delete mail by id\n");
	printf("DELETE FULL: delete mail by full info\n");
	printf("FIND ID: to find and output mails by mail id\n");
	printf("FIND DELIVERY: to find and output mails delivered relative to the time in argument programm\n");
	printf("FIND NOT DELIVERY: to find and output mails not delivered relative to the time in argument programm\n");
	printf("SORT: to sort mail by recipient's index (primarily) and postal parcel ID (secondary)\n");
	printf("OUTPUT: to output mails\n");
	printf("STOP: to stop dialog\n");

	post item = {NULL, NULL, 0, 0};
	char *command = NULL;
	char *token_command = NULL;
	int exit_code = 0;
	int size_command = 0;

	do {
		if (command) {
			free(command);
			command = NULL;
		}
		printf("COMMAND: ");
		if (get_str(&command, stdin) == NO_MEMORY) {
			return NO_MEMORY;
		}
		if (command && token_count(command) >= 1) {
			if (strip(&command) == NO_MEMORY) {
				return NO_MEMORY;
			}
			if (strcmp(command, "ADD") == 0) {
				exit_code = run_add_mail(&item);
				if (exit_code != SUCCESS) {
					print_error(exit_code);
				}
			} else {
				if (strcmp(command, "DELETE ID") == 0) {
					exit_code = run_delete_id(&item);
					if (exit_code != SUCCESS) {
						print_error(exit_code);
					}
				} else {
					if (strcmp(command, "DELETE FULL") == 0) {
						exit_code = run_delete_full(&item);
						if (exit_code != SUCCESS) {
							print_error(exit_code);
						}
					} else {
						if (strcmp(command, "FIND ID") == 0) {
							exit_code = run_find_id(item);
							if (exit_code != SUCCESS) {
								print_error(exit_code);
							}
						} else {
							if (strcmp(command, "FIND DELIVERY") == 0) {
								exit_code = run_find_delivery(datetime, item);
								if (exit_code != SUCCESS) {
									print_error(exit_code);
								}
							} else {
								if (strcmp(command, "FIND NOT DELIVERY") == 0) {
									exit_code = run_find_not_delivery(datetime, item);
									if (exit_code != SUCCESS) {
										print_error(exit_code);
									}
								} else {
									if (strcmp(command, "SORT") == 0) {
										exit_code = run_sort(&item);
										if (exit_code != SUCCESS) {
											print_error(exit_code);
										}
									} else {
										if (strcmp(command, "OUTPUT") == 0) {
											exit_code = run_output(item);
											if (exit_code != SUCCESS) {
												print_error(exit_code);
											}
										} else {
											if (strcmp(command, "STOP") != 0) {
												fprintf(stderr, "INVALID COMMAND\n");
											}
										}
									}
								}
							}
						}
					}
				}
			}
		} else {
			join_symb('1', &command, &size_command);
			size_command = 0;
		}
	} while (strcmp(command, "STOP") != 0);

	free(command);
	free_post(&item);

	return SUCCESS;
}