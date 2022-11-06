#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../headers/check.h"
#include "../headers/string.h"
#include "../headers/binom_heap.h"
#include "../headers/post.h"

#define eps 0.000000001

typedef int (*funcs_check)(char*);

int create_mail_strings(mail **item) {
	char *field[14] = {"CITY SENDER: ", 
					 "SREET SENDER: ", 
					 "HOME SENDER: ", 
					 "FLAT SENDER: ", 
					 "INDEX SENDER: ", 
					 "CITY RECIPIENT: ", 
					 "SREET RECIPIENT: ", 
					 "HOME RECIPIENT: ", 
					 "FLAT RECIPIENT: ", 
					 "INDEX RECIPIENT: ", 
					 "WEIGHT PACKAGE(kg): ", 
					 "MAIL ID: ", 
					 "TIME CREATE[{day}.{month}.{year} {hour}:{minute}:{seconds}]: ", 
					 "TIME DELIVERY[{day}.{month}.{year} {hour}:{minute}:{seconds}]: "};
	
	char *strings[14];
	funcs_check checks[14] = {check_city_street, 
							  check_city_street, 
							  check_home_flat, 
							  check_home_flat,
							  check_index,
							  check_city_street, 
							  check_city_street, 
							  check_home_flat, 
							  check_home_flat,
							  check_index,
							  check_weight,
							  check_mail_id,
							  check_datetime,
							  check_datetime
							};

	for (int i = 0; i < 14; i++) {
		strings[i] = NULL;
	}

	for (int i = 0; i < 14; i++) {
		printf("%s", field[i]);
		if (get_str(&strings[i], stdin) == NO_MEMORY) {
			return NO_MEMORY;
		}
		if (!strings[i] || token_count(strings[i]) < 1 || token_count(strings[i]) > 2) {
			for (int j = 0; j <= i; j++) {
				if (strings[j]) {
					free(strings[j]);
				}
			}
			return INVALID_MAIL;
		}
		if (strip(&strings[i]) == NO_MEMORY) {
			return NO_MEMORY;
		}
		if (!checks[i](strings[i])) {
			for (int j = 0; j <= i; j++) {
				if (strings[j]) {
					free(strings[j]);
				}
			}

			return INVALID_MAIL;
		}
	}

	if (correct_date(&strings[12]) == NO_MEMORY) {
		for (int i = 0; i < 14; i++) {
			free(strings[i]);
		}
		return NO_MEMORY;
	}
	if (correct_date(&strings[13]) == NO_MEMORY) {
		for (int i = 0; i < 14; i++) {
			free(strings[i]);
		}
		return NO_MEMORY;
	}
	if (get_seconds_date_chr(strings[12]) > get_seconds_date_chr(strings[13])) {
		for (int i = 0; i < 14; i++) {
			free(strings[i]);
		}
		return INVALID_MAIL;
	}

	*item = (mail*)malloc(sizeof(mail));
	(*item)->sender = (adress*)malloc(sizeof(adress));
	(*item)->recipient = (adress*)malloc(sizeof(adress));

	if (!*item ||
		!(*item)->sender ||
		!(*item)->recipient) {

		for (int i = 0; i < 14; i++) {
			free(strings[i]);
		}

		return NO_MEMORY;
	}

	(*item)->sender->home = atoi(strings[2]);
	(*item)->sender->flat = atoi(strings[3]);
	(*item)->recipient->home = atoi(strings[7]);
	(*item)->recipient->flat = atoi(strings[8]);
	(*item)->weight = atof(strings[10]);

	if (create_str(&((*item)->sender->city), strings[0]) == NO_MEMORY || 
		create_str(&((*item)->sender->street), strings[1]) == NO_MEMORY ||
		create_str(&((*item)->sender->index), strings[4]) == NO_MEMORY ||
		create_str(&((*item)->recipient->city), strings[5]) == NO_MEMORY || 
		create_str(&((*item)->recipient->street), strings[6]) == NO_MEMORY ||
		create_str(&((*item)->recipient->index), strings[9]) == NO_MEMORY ||
		create_str(&((*item)->mail_id), strings[11]) == NO_MEMORY ||
		create_str(&((*item)->time_create), strings[12]) == NO_MEMORY ||
		create_str(&((*item)->time_delivery), strings[13]) == NO_MEMORY) {

		for (int i = 0; i < 14; i++) {
			free(strings[i]);
		}

		return NO_MEMORY;
	}

	for (int i = 0; i < 14; i++) {
		free(strings[i]);
	}

	return SUCCESS;
}

int add_mail_str(post *item_post) {
	mail *add_mail = NULL;
	int exit_code = create_mail_strings(&add_mail);
	
	if (exit_code == SUCCESS) {
		if (item_post->curr_size == item_post->size) {
			item_post->size = item_post->size ? item_post->size * 2 : 1;
			mail **temp = (mail**)realloc(item_post->mails, sizeof(mail*) * item_post->size);
			
			if (!temp) {
				return NO_MEMORY;
			}

			item_post->mails = temp;	
		}

		item_post->mails[item_post->curr_size++] = add_mail;

		return SUCCESS;
	}

	return exit_code;
}

int add_mail(post *item, mail *add_mail) {
	if (item->curr_size == item->size) {
		item->size = item->size ? item->size * 2 : 1;
		mail **temp = (mail**)realloc(item->mails, sizeof(mail*) * item->size);
		
		if (!temp) {
			return NO_MEMORY;
		}

		item->mails = temp;	
	}

	item->mails[item->curr_size++] = add_mail;

	return SUCCESS;
}

int delete_full_mail(post *item) {
	mail *del_mail = NULL;
	int exit_code = create_mail_strings(&del_mail);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	mail **new_mails = (mail**)malloc(sizeof(mail*) * item->curr_size);
	
	if (!new_mails) {
		return NO_MEMORY;
	}

	mail **ptr = new_mails;
	int count_del = 0;

	for (int i = 0; i < item->curr_size; i++) {
		if (cmp_mail(del_mail, item->mails[i]) == 0) {
			*ptr++ = item->mails[i];
		} else {
			free_mail(&(item->mails[i]));
			count_del++;
		}
	}

	item->curr_size -= count_del;
	mail **temp = item->mails;
	item->mails = new_mails;
	free_mail(&del_mail);
	free(temp);

	return SUCCESS;
}

int delete_id_mail(post *item, char *id) {
	string id_str;

	if (!check_mail_id(id)) {
		return INVALID_MAIL_ID;
	}

	if (create_str(&id_str, id) == NO_MEMORY) {
		return NO_MEMORY;
	}

	mail **new_mails = (mail**)malloc(sizeof(mail*) * item->curr_size);
	
	if (!new_mails) {
		return NO_MEMORY;
	}

	mail **ptr = new_mails;
	int count_del = 0;

	for (int i = 0; i < item->curr_size; i++) {
		if (cmp_str(id_str, item->mails[i]->mail_id, cmp) != 0) {
			*ptr++ = item->mails[i];
		} else {
			free_mail(&(item->mails[i]));
			count_del++;
		}
	}

	item->curr_size -= count_del;
	mail **temp = item->mails;
	item->mails = new_mails;
	free_str(&id_str);
	free(temp);

	return SUCCESS;
}

int find_id_mail(mail ***find_mails, mail **mails, int *count_finds, int count_mails, char *mail_id) {
	string mail_id_str;

	if (create_str(&mail_id_str, mail_id) == NO_MEMORY) {
		return NO_MEMORY;
	}

	*count_finds = 0;
	int size = 0;

	for (int i = 0; i < count_mails; i++) {
		if (cmp_str(mails[i]->mail_id, mail_id_str, cmp) == 0) {
			if (*count_finds == size) {
				size = size ? size * 2 : 1;

				mail **temp = (mail**)realloc(*find_mails, sizeof(mail*) * size);

				if (!temp) {
					free_str(&mail_id_str);
					return NO_MEMORY;
				}

				*find_mails = temp;
			}

			(*find_mails)[(*count_finds)++] = mails[i];
		}
	}

	free_str(&mail_id_str);

	return SUCCESS;
}

int cmp_mail(mail *item_1, mail *item_2) {
	if (cmp_str(item_1->sender->city, item_2->sender->city, cmp) == 0 &&
		cmp_str(item_1->sender->street, item_2->sender->street, cmp) == 0 &&
		item_1->sender->home == item_2->sender->home &&
		item_1->sender->home == item_2->sender->home &&
		cmp_str(item_1->sender->index, item_2->sender->index, cmp) == 0 &&
		cmp_str(item_1->recipient->city, item_2->recipient->city, cmp) == 0 &&
		cmp_str(item_1->recipient->street, item_2->recipient->street, cmp) == 0 &&
		item_1->recipient->home == item_2->recipient->home &&
		item_1->recipient->home == item_2->recipient->home &&
		cmp_str(item_1->recipient->index, item_2->recipient->index, cmp) == 0 &&
		fabs(item_2->weight - item_1->weight) < eps &&
		cmp_str(item_1->mail_id, item_2->mail_id, cmp) == 0 &&
		cmp_str(item_1->time_create, item_2->time_create, cmp) == 0 &&
		cmp_str(item_1->time_delivery, item_2->time_delivery, cmp) == 0) {
		return 1;
	}

	return 0;
}

int cmp_mail_sort(mail *item_1, mail *item_2) {
	if (cmp_str(item_1->sender->index, item_2->sender->index, cmp) == 0) {
		return cmp_str(item_1->mail_id, item_2->mail_id, cmp) + 1;
	}

	return cmp_str(item_1->sender->index, item_2->sender->index, cmp) + 1;
}

int cmp_mail_dates(mail *item_1, mail *item_2) {
	unsigned long long seconds_1 = get_seconds_date_str(item_1->time_create);
	unsigned long long seconds_2 = get_seconds_date_str(item_2->time_create);
	
	if (seconds_1 == seconds_2) {
		return cmp_str(item_1->mail_id, item_2->mail_id, cmp);
	}

	return seconds_1 >= seconds_2;
}

int cmp_dates_str(string date_1, string date_2) {
	unsigned long long seconds_1 = get_seconds_date_str(date_1);
	unsigned long long seconds_2 = get_seconds_date_str(date_2);

	return seconds_1 >= seconds_2;
}

int get_delivered_mails(char *datetime, post item, binom_heap **heap, int *size, int (*cmp_dates)(mail*, mail*)) {
	string datetime_str;

	if (create_str(&datetime_str, datetime) == NO_MEMORY) {
		return NO_MEMORY;
	}

	for (int i = 0; i < item.curr_size; i++) {
		if (cmp_dates_str(datetime_str, item.mails[i]->time_delivery) == 1) {
			binom_tree *merge_tree = NULL;
			node_list *add_node = NULL;
			(*size)++;

			if (make_node_tree(&add_node, NULL) == NO_MEMORY) {
				return NO_MEMORY;
			}

			if (init_root_tree(&merge_tree, item.mails[i]) == NO_MEMORY ||
				make_node_tree(&add_node, merge_tree) == NO_MEMORY) {
				return NO_MEMORY;
			}

			if (push_back(heap, add_node, cmp_dates) == NO_MEMORY) {
				return NO_MEMORY;
			}

			if (transform_to_bin_heap(heap, cmp_dates) == NO_MEMORY) {
				return NO_MEMORY;
			}
		}
	}

	return SUCCESS;
}

int get_not_delivered_mails(char *datetime, post item, binom_heap **heap, int *size, int (*cmp_dates)(mail*, mail*)) {
	string datetime_str;

	if (create_str(&datetime_str, datetime) == NO_MEMORY) {
		return NO_MEMORY;
	}

	for (int i = 0; i < item.curr_size; i++) {
		if (cmp_dates_str(datetime_str, item.mails[i]->time_delivery) == 0) {
			binom_tree *merge_tree = NULL;
			node_list *add_node = NULL;
			(*size)++;

			if (make_node_tree(&add_node, NULL) == NO_MEMORY) {
				return NO_MEMORY;
			}

			if (init_root_tree(&merge_tree, item.mails[i]) == NO_MEMORY ||
				make_node_tree(&add_node, merge_tree) == NO_MEMORY) {
				return NO_MEMORY;
			}

			if (push_back(heap, add_node, cmp_dates) == NO_MEMORY) {
				return NO_MEMORY;
			}

			if (transform_to_bin_heap(heap, cmp_dates) == NO_MEMORY) {
				return NO_MEMORY;
			}
		}
	}

	return SUCCESS;
}

int output_find_mails(binom_heap *heap, int count_find, int (*cmp)(mail*, mail*)) {
	int exit_code = 0;

	for (int i = 0; i < count_find; i++) {
		mail *item_mail = NULL;
		exit_code = get_extreme_heap(&item_mail, &heap, cmp);
		if (exit_code != SUCCESS) {
			return exit_code;
		}
		output_beatiful_mail(i, item_mail, stdout);
	}

	return SUCCESS;
}

void output_beatiful_mail(int index, mail *item, FILE *output) {
	fprintf(stdout, "-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(output, "| NUM MAIL | SENDER/RECIPIENT |     CITY     |      STREET      |   HOME   |   FLAT   |    INDEX    |       TIME CREATE       |       TIME DELIVERY       |\n");
	fprintf(stdout, "-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(output, "|");
	fprintf(output, "          |      SENDER      |");
	print_centre_str(14, item->sender->city, output);
	print_centre_str(18, item->sender->street, output);
	print_centre_int(10, item->sender->home, output);
	print_centre_int(10, item->sender->flat, output);
	print_centre_str(13, item->sender->index, output);
	print_centre_str(25, item->time_create, output);
	fprintf(output, "             -             |\n");
	fprintf(output, "|          |-----------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(output, "|          |     RECIPIENT    |");
	print_centre_str(14, item->recipient->city, output);
	print_centre_str(18, item->recipient->street, output);
	print_centre_int(10, item->recipient->home, output);
	print_centre_int(10, item->recipient->flat, output);
	print_centre_str(13, item->recipient->index, output);
	fprintf(output, "            -            |");
	print_centre_str(27, item->time_delivery, output);
	fprintf(output, "\n|");
	print_centre_int(10, index + 1, output);
	fprintf(stdout, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(output, "|          |---------------------------------------------------------\n");
	fprintf(output, "|          |          MAIL ID          |     PACKAGE WEIGHT(kg)     |\n");
	fprintf(output, "|          |---------------------------------------------------------\n");
	fprintf(output, "|          |");
	print_centre_str(27, item->mail_id, output);
	print_centre_double(28, item->weight, output);
	fprintf(output, "\n");
	fprintf(output, "---------------------------------------------------------------------\n");
}

void output_mails(post item) {
	for (int i = 0; i < item.curr_size; i++) {
		output_beatiful_mail(i, item.mails[i], stdout);
	}
}

void free_mail(mail **item) {
	if (*item) {
		free_str(&((*item)->sender->city));
		free_str(&((*item)->sender->street));
		free_str(&((*item)->sender->index));
		free((*item)->sender);
		free_str(&((*item)->recipient->city));
		free_str(&((*item)->recipient->street));
		free_str(&((*item)->recipient->index));
		free((*item)->recipient);
		free_str(&((*item)->mail_id));
		free_str(&((*item)->time_create));
		free_str(&((*item)->time_delivery));
		free(*item);
	}
}

void free_post(post *item) {
	if (item) {
		for (int i = 0; i < item->curr_size; i++) {
			free_mail(&(item->mails[i]));
		}
		if (item->mails) {
			free(item->mails);
		}
		if (item->curr_post_office) {
			free(item->curr_post_office);
		}
	}
}