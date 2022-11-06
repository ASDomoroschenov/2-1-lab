#include "../headers/citizen.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define eps 0.00000001

int check_number(char *number) {
	char *ptr = number;

	while (*ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
		ptr++;
	}

	return atoi(number) > 0;
}

int check_fullname(char *name, char *surname, char *patronymic) {
	char *ptr_name = name;
	char *ptr_surname = surname;
	char *ptr_patronymic = patronymic;

	while (*ptr_name && *ptr_surname && *ptr_patronymic) {
		if (*ptr_name) {
			if (!isalpha(*ptr_name)) {
				return 0;
			}
			ptr_name++;
		}
		if (*ptr_surname) {
			if (!isalpha(*ptr_surname)) {
				return 0;
			}
			ptr_surname++;
		}
		if (*ptr_patronymic) {
			if (!isalpha(*ptr_patronymic)) {
				return 0;
			}
			ptr_patronymic++;
		}
	}

	return 1;
}

int check_date_birth(char *date_birth) {
	char *ptr = date_birth;
	char *day = NULL;
	char *month = NULL;
	char *year = NULL;
	int size = 0;
	
	while (*ptr != '.' && *ptr) {
		join_symb(*ptr, &day, &size);
		ptr++;
	}

	if (!day) {
		return 0;
	}

	ptr++;
	size = 0;

	while (*ptr != '.' && *ptr) {
		join_symb(*ptr, &month, &size);
		ptr++;
	}

	if (!month) {
		free(day);

		return 0;
	}

	ptr++;
	size = 0;

	while (*ptr && *ptr) {
		join_symb(*ptr, &year, &size);
		ptr++;
	}

	if (!year) {
		free(day);
		free(month);

		return 0;
	}

	if (strlen(day) != 2 || 
		!check_number(day) ||
		atoi(day) < 1 ||
		atoi(day) > 31) {
		free(day);
		free(month);
		free(year);

		return 0;
	}

	if (strlen(month) != 2 || 
		!check_number(month) ||
		atoi(month) < 1 ||
		atoi(month) > 12) {
		free(day);
		free(month);
		free(year);

		return 0;
	} else {
		if (atoi(month) == 2 && atoi(day) > 28) {
			free(day);
			free(month);
			free(year);

			return 0;
		}
	}

	if (strlen(year) != 4 || 
		!check_number(year) ||
		atoi(year) < 2000) {
		free(day);
		free(month);
		free(year);

		return 0;
	}

	free(day);
	free(month);
	free(year);

	return 1;
}

int check_gender(char *sex) {
	char *ptr = sex;
	
	while (*ptr) {
		*ptr++ = toupper(*ptr);
	}

	return strcmp(sex, "MALE") == 0 || strcmp(sex, "FEMALE") == 0;
}

int check_avrg_incom(char *avrg_income) {
	char *ptr = avrg_income;
	int count_dots = 0;

	if (*ptr == '.') {
		return 0;
	}

	while (*ptr) {
		if (count_dots > 1) {
			return 0;
		}
		if (*ptr == '.') {
			count_dots++;
		}
		if (*ptr != '.' && !isdigit(*ptr)) {
			return 0;
		}
		ptr++;
	}

	return 1;
}

int check_citizen(char *str) {
	char *ptr = str;
	char *surname = NULL;
	char *name = NULL;
	char *patronymic = NULL;
	char *date = NULL;
	char *gender = NULL;
	char *avrg_income = NULL;

	if (token_count(str) != 6) {
		return 0;
	}

	if (get_token_str(&ptr, &surname) != SUCCESS) {
		return NO_MEMORY;
	}

	if (get_token_str(&ptr, &name) != SUCCESS) {
		free(surname);
		return NO_MEMORY;
	}

	if (get_token_str(&ptr, &patronymic) != SUCCESS) {
		free(name);
		free(surname);
		return NO_MEMORY;
	}

	if (get_token_str(&ptr, &date) != SUCCESS) {
		free(name);
		free(surname);
		free(patronymic);
		return NO_MEMORY;
	}

	if (get_token_str(&ptr, &gender) != SUCCESS) {
		free(name);
		free(surname);
		free(patronymic);
		free(date);
		return NO_MEMORY;
	}

	if (get_token_str(&ptr, &avrg_income) != SUCCESS) {
		free(name);
		free(surname);
		free(patronymic);
		free(date);
		free(gender);
		return NO_MEMORY;
	}

	if (check_fullname(name, surname, patronymic) &&
		check_date_birth(date) &&
		check_gender(gender) &&
		check_avrg_incom(avrg_income)) {

		free(name);
		free(surname);
		free(patronymic);
		free(date);
		free(gender);
		free(avrg_income);

		return 1;
	}

	free(name);
	free(surname);
	free(patronymic);
	free(date);
	free(gender);
	free(avrg_income);

	return 0;
}

int get_citizen(char *str, citizen **item) {
	char *ptr = str;
	char *gender = 0;
	char *avrg_income = NULL;
	*item = (citizen*)malloc(sizeof(citizen));

	if (!*item) {
		return NO_MEMORY;
	} 

	if (get_token_str(&ptr, &((*item)->surname)) != SUCCESS) {
		return NO_MEMORY;
	}
	if (get_token_str(&ptr, &((*item)->name)) != SUCCESS) {
		return NO_MEMORY;
	}
	if (get_token_str(&ptr, &((*item)->patronymic)) != SUCCESS) {
		return NO_MEMORY;
	}
	if (get_token_str(&ptr, &((*item)->date_birth)) != SUCCESS) {
		return NO_MEMORY;
	}
	if (get_token_str(&ptr, &gender) != SUCCESS) {
		return NO_MEMORY;
	}
	if (get_token_str(&ptr, &avrg_income) != SUCCESS) {
		return NO_MEMORY;
	}

	char *ptr_gender = gender;
	
	while (*ptr_gender) {
		*ptr_gender++ = toupper(*ptr_gender);
	}

	if (strcmp(gender, "MALE") == 0) {
		(*item)->gender = MALE;
	} else {
		(*item)->gender = FEMALE;
	}

	(*item)->avrg_income = atof(avrg_income);

	free(gender);
	free(avrg_income);

	return SUCCESS;
}

int get_list_citizen(char *name_file, list **list_citizen, int (*cmp)(citizen*, citizen*)) {
	FILE *file = NULL;
	char *str = NULL;
	citizen *item = NULL;
	int exit_code_check = 0;
	int exit_code_get = 0;

	if ((file = fopen(name_file, "r")) != NULL) {
		while (!feof(file)) {
			if (get_str(&str, file) == NO_MEMORY) {
				fclose(file);
				return NO_MEMORY;
			}

			if (str && *str) {
				exit_code_check = check_citizen(str);

				if (exit_code_check == NO_MEMORY) {
					free(str);
					fclose(file);

					return NO_MEMORY;
				}

				if (exit_code_check == 1) {
					if (get_citizen(str, &item) == NO_MEMORY) {
						free(str);
						fclose(file);

						return NO_MEMORY;
					}

					if (add_node(list_citizen, item, cmp) == NO_MEMORY) {
						free(str);
						fclose(file);

						return NO_MEMORY;
					}
				}

				free(str);
			}

			str = NULL;
		}

		fclose(file);

		return SUCCESS;
	}

	return OPEN_FILE;
}

int find_citizen(list *list_citizen, citizen **item, char *surname, char *name, char *patronymic, char *date_birth, int gender, double avrg_income) {
	if (list_citizen) {
		list_node *node = list_citizen->root;

		while (node != NULL) {
			if (strcmp(node->human->surname, surname) == 0 &&
				strcmp(node->human->name, name) == 0 &&
				strcmp(node->human->patronymic, patronymic) == 0 &&
				strcmp(node->human->date_birth, date_birth) == 0 &&
				node->human->gender == gender &&
				fabs(node->human->avrg_income - avrg_income) < eps) {
				(*item) = node->human;

				return 1;
			}
			node = node->next;
		}

		return 0;
	}

	return 0;
}

int add_citizen(list **list_citizen, char *surname, char *name, char *patronymic, char *date_birth, int gender, double avrg_income) {
	citizen *item = (citizen*)malloc(sizeof(citizen));
	
	if (!item) {
		return NO_MEMORY;
	}

	item->surname = (char*)malloc(sizeof(char) * (strlen(surname) + 1));

	if (!item->surname) {
		free(item);
		return NO_MEMORY;
	}

	item->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));

	if (!item->name) {
		free(item->surname);
		free(item);
		return NO_MEMORY;
	}

	item->patronymic = (char*)malloc(sizeof(char) * (strlen(patronymic) + 1));

	if (!item->patronymic) {
		free(item->surname);
		free(item->name);
		free(item);
		return NO_MEMORY;
	}

	item->date_birth = (char*)malloc(sizeof(char) * (strlen(date_birth) + 1));

	if (!item->patronymic) {
		free(item->patronymic);
		free(item->surname);
		free(item->name);
		free(item);
		return NO_MEMORY;
	}

	item->gender = gender;
	item->avrg_income = avrg_income;

	strcpy(item->surname, surname);
	strcpy(item->name, name);
	strcpy(item->patronymic, patronymic);
	strcpy(item->date_birth, date_birth);

	add_node(list_citizen, item, cmp);

	return SUCCESS;
}

int delete_citizen(list **list_citizen, char *surname, char *name, char *patronymic, char *date_birth, int gender, double avrg_income) {
	if (*list_citizen) {
		list_node *node = (*list_citizen)->root;

		while (node != NULL) {
			if (strcmp(node->human->surname, surname) == 0 &&
				strcmp(node->human->name, name) == 0 &&
				strcmp(node->human->patronymic, patronymic) == 0 &&
				strcmp(node->human->date_birth, date_birth) == 0 &&
				node->human->gender == gender &&
				fabs(node->human->avrg_income - avrg_income) < eps) {
				delete_node(list_citizen, node);

				return SUCCESS;
			}
			node = node->next;
		}

		return SUCCESS;
	}

	return EMPTY_LIST;
}

void output_citizen(citizen *item, FILE *output) {
	fprintf(output, "%s %s %s %s ", item->surname, item->name, item->patronymic, item->date_birth);
	if (item->gender == MALE) {
		fprintf(output, "%s ", "MALE");
	} else {
		fprintf(output, "%s ", "FEMALE");
	}
	fprintf(output, "%lf\n", item->avrg_income);
}

int output_list_citizens(list *list_citizen) {
	if (list_citizen) {
		list_node *node = list_citizen->root;

		while (node != NULL) {
			output_citizen(node->human, stdout);
			node = node->next;
		}

		return SUCCESS;
	}

	return EMPTY_LIST;
}

int upload_list_file(list *list_citizen, char *name_file) {
	if (list_citizen) {
		FILE *file = NULL;

		if ((file = fopen(name_file, "w")) != NULL) {
			list_node *node = list_citizen->root;

			while (node != NULL) {
				output_citizen(node->human, file);
				node = node->next;
			}

			fclose(file);

			return SUCCESS;
		}

		return OPEN_FILE;
	}

	return EMPTY_LIST;
}