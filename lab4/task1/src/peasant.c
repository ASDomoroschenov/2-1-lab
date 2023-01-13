#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/peasant.h"
#include "../lib/string.h"

int create_user(peasant **user, long id) {
	(*user) = (peasant*)malloc(sizeof(peasant));

	if (!(*user)) {
		return NO_MEMORY;
	}

	(*user)->is_free_boat = 1;
	(*user)->on_boat = NULL;

	(*user)->left_coast[0] = NULL;
	(*user)->left_coast[1] = NULL;
	(*user)->left_coast[2] = NULL;

	(*user)->right_coast[0] = (char*)malloc(sizeof(char) * (strlen("wolf") + 1));

	if (!(*user)->right_coast[0]) {

		return NO_MEMORY;
	}

	(*user)->right_coast[1] = (char*)malloc(sizeof(char) * (strlen("goat") + 1));
	
	if (!(*user)->right_coast[1]) {
		free((*user)->right_coast[0]);

		return NO_MEMORY;
	}

	(*user)->right_coast[2] = (char*)malloc(sizeof(char) * (strlen("cabbage") + 1));

	if (!(*user)->right_coast[2]) {
		free((*user)->right_coast[0]);
		free((*user)->right_coast[1]);

		return NO_MEMORY;
	}

	strcpy((*user)->right_coast[0], "wolf");
	strcpy((*user)->right_coast[1], "goat");
	strcpy((*user)->right_coast[2], "cabbage");

	(*user)->is_left_coast = 0;
	(*user)->is_right_coast = 1;
	(*user)->count_right_coast = 3;
	(*user)->count_left_coast = 0;
	(*user)->id = id;

	return SUCCESS;
}

//почему strcmp тут работает с NULL?
int compare(const void *item_1, const void *item_2) {
	return strcmp((char*)item_2, (char*)item_1);
}

int have_object_coasts(peasant **user, char *object) {
	char **objects = NULL;

	if ((*user)->is_left_coast) {
		objects = (*user)->left_coast;
	} else {
		objects = (*user)->right_coast;
	}

	for (int i = 0; i < 3; i++) {
		if (objects[i]) {
			if (strcmp(objects[i], object) == 0) {
				return i;
			}
		}
	}

	return -1;
}

//1 - left, 0 - boat, -1 - right
int on_wich_coast(peasant **user, char *object) {
	for (int i = 0; i < 3; i++) {
		if ((*user)->left_coast[i]) {
			if (strcmp((*user)->left_coast[i], object) == 0) {
				return 1;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		if ((*user)->right_coast[i]) {
			if (strcmp((*user)->right_coast[i], object) == 0) {
				return -1;
			}
		}
	}

	return 0;
}

int take(peasant **user, char *object) {
	int index = 0;

	if (!(*user)->is_free_boat) {
		return HAVENT_FREE_PLACE;
	}

	index = have_object_coasts(user, object);

	if (index < 0) {
		return THERE_IS_NO_OBJECT_COASTS;
	}
	
	(*user)->on_boat = (char*)malloc(sizeof(char) * (strlen(object) + 1));

	if (!(*user)->on_boat) {
		return NO_MEMORY;
	}

	strcpy((*user)->on_boat, object);

	if ((*user)->is_left_coast) {
		free((*user)->left_coast[index]);
		(*user)->left_coast[index] = NULL;
		(*user)->count_left_coast--;
	} else {
		free((*user)->right_coast[index]);
		(*user)->right_coast[index] = NULL;
		(*user)->count_right_coast--;
	}

	(*user)->is_free_boat = 0;

	return SUCCESS;
}

int move(peasant **user) {
	(*user)->is_right_coast = (*user)->is_right_coast ? 0 : 1;
	(*user)->is_left_coast = (*user)->is_left_coast ? 0 : 1;

	return SUCCESS;
}

int put(peasant **user) {
	int index = 0;

	if (!(*user)->on_boat) {
		return THERE_IS_NO_OBJECT_BOAT;
	}

	if ((*user)->is_left_coast) {
		while ((*user)->left_coast[index]) {
			index++;
		}
		(*user)->count_left_coast++;
		(*user)->left_coast[index] = (char*)malloc(sizeof(char) * (strlen((*user)->on_boat) + 1));

		if (!(*user)->left_coast[index]) {
			return NO_MEMORY;
		}

		strcpy((*user)->left_coast[index], (*user)->on_boat);
		free((*user)->on_boat);
		(*user)->on_boat = NULL;
	} else {
		while ((*user)->right_coast[index]) {
			index++;
		}
		(*user)->count_right_coast++;
		(*user)->right_coast[index] = (char*)malloc(sizeof(char) * (strlen((*user)->on_boat) + 1));

		if (!(*user)->right_coast[index]) {
			return NO_MEMORY;
		}

		strcpy((*user)->right_coast[index], (*user)->on_boat);
		free((*user)->on_boat);
		(*user)->on_boat = NULL;
	}

	(*user)->is_free_boat = 1;

	return SUCCESS;
}

int check_coasts(peasant **user) {
	int wolf = on_wich_coast(user, "wolf");
	int goat = on_wich_coast(user, "goat");
	int cabbage = on_wich_coast(user, "cabbage");

	if ((*user)->count_left_coast != 3 && (*user)->is_right_coast) {
		if (wolf == 1 && goat == 1) {
			return WOLF_EAT_GOAT;
		}
		if (goat == 1 && cabbage == 1) {
			return GOAT_EAT_CABBAGE;
		}
	} else {
		if ((*user)->count_right_coast != 3 && (*user)->is_left_coast) {
			if (wolf == -1 && goat == -1) {
				return WOLF_EAT_GOAT;
			}
			if (goat == -1 && cabbage == -1) {
				return GOAT_EAT_CABBAGE;
			}
		}
		if ((*user)->count_left_coast == 3) {
			return WIN;
		}
	}

	return SUCCESS;
}

int execute_command(char *str, peasant **user) {
	if (!str || is_empty_str(str)) {
		return EMPTY_STRING;
	}

	int count_tokens = token_count(str);
	char **tokens = NULL;
	int exit_code = 0;
	int len_str = strlen(str);

	if (str[len_str - 1] != ';') {
		return WRONG_COMMAND;
	}

	char *temp = (char*)malloc(sizeof(char) * len_str);

	if (!temp) {
		return NO_MEMORY;
	}

	strncpy(temp, str, len_str - 1);
	temp[len_str - 1] = 0;

	if (count_tokens < 3) {
		if ((exit_code = split_space(temp, &tokens)) != SUCCESS) {
			return exit_code;
		}
		if (count_tokens == 2 && 
			strcmp(tokens[0], "take") == 0) {
			exit_code = take(user, tokens[1]);
			for (int i = 0; i < count_tokens; i++) {
				free(tokens[i]);
				tokens[i] = NULL;
			}

			free(tokens);
			free(temp);
			tokens = NULL;
			temp = NULL;

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			return check_coasts(user);
		}
		if (count_tokens == 1 &&
			strcmp(tokens[0], "put") == 0) {
			exit_code = put(user);

			for (int i = 0; i < count_tokens; i++) {
				free(tokens[i]);
				tokens[i] = NULL;
			}

			free(tokens);
			free(temp);
			tokens = NULL;
			temp = NULL;

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			return check_coasts(user);
		}
		if (count_tokens == 1 && 
			strcmp(tokens[0], "move") == 0) {
			exit_code = move(user);

			for (int i = 0; i < count_tokens; i++) {
				free(tokens[i]);
				tokens[i] = NULL;
			}

			free(tokens);
			free(temp);
			tokens = NULL;
			temp = NULL;

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			return check_coasts(user);
		}
	}

	free(temp);
	
	return WRONG_COMMAND;
}

int output_user(peasant *user) {
	if (user) {
		printf("\n");
		if (user->is_left_coast) {
			printf("User %ld on left coast\n", user->id);
		} else {
			printf("User %ld on right coast\n", user->id);
		}
		printf("On left coast: ");
		for (int i = 0; i < 3; i++) {
			if (user->left_coast[i]) {
				printf("%s ", user->left_coast[i]);
			}
		}
		printf("\n");
		printf("On boat: ");
		if (user->on_boat) {
			printf("%s\n", user->on_boat);
		} else {
			printf("\n");
		}
		printf("On right coast: ");
		for (int i = 0; i < 3; i++) {
			if (user->right_coast[i]) {
				printf("%s ", user->right_coast[i]);
			}
		}
		printf("\n\n");
	}

	return SUCCESS;
}

int free_user(peasant **user) {
	return SUCCESS;
}