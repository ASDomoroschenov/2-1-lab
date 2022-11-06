#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/string.h"
#include "../headers/types.h"
#include "../headers/citizen.h"
#include "../headers/list.h"

int main(int argc, char *argv[]) {
	list *list_citizen = NULL;
	int exit_code = 0;
	
	exit_code = get_list_citizen(argv[1], &list_citizen, cmp);

	if (argc == 2) {
		if (strlen(argv[1]) > 4 && strcmp(".txt", argv[1] + strlen(argv[1]) - 4) == 0) {
			if (exit_code == SUCCESS) {
				output_list_citizens(list_citizen);
				printf("\nFIND (Domoroschenov Alexandr Sergeevich 03.11.2003 MALE 11000):\n\n");
				citizen *item = NULL;
				int exit_code_find = 0;
				exit_code_find = find_citizen(list_citizen, &item, "Domoroschenov", "Alexandr", "Sergeevich", "03.11.2003", MALE, (double)11000.0);
				if (exit_code_find == 1) {
					printf("CITIZEN WAS FOUND: ");
					output_citizen(item, stdout);
				} else {
					printf("CITIZEN WASN'T FOUND\n");
				}
				exit_code = delete_citizen(&list_citizen, "Domoroschenov", "Alexandr", "Sergeevich", "03.11.2003", MALE, (double)11000.0);
				printf("\nDELETE (Domoroschenov Alexandr Sergeevich 03.11.2003 MALE 11000):\n\n");
				if (exit_code == EMPTY_LIST) {
					print_error(EMPTY_LIST);
				} else {
					if (output_list_citizens(list_citizen) == EMPTY_LIST) {
						print_error(EMPTY_LIST);
					}
				}

				printf("\nADD (Domoroschenov Alexandr Sergeevich 03.11.2003 MALE 11000)\n\n");
				if (add_citizen(&list_citizen, "Domoroschenov", "Alexandr", "Sergeevich", "03.11.2003", MALE, (double)11000.0) == NO_MEMORY) {
					print_error(NO_MEMORY);
				} else {
					output_list_citizens(list_citizen);
				}
				printf("\nLIST UPLOADED TO FILE\n");
				upload_list_file(list_citizen, argv[1]);
				free_list(&list_citizen);
			} else {
				print_error(exit_code);
			}
		} else {
			print_error(NOT_TXT_FILE);
		}
	} else {
		print_error(INVALID_ARGS);
	}

	return 0;
}