#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../headers/check.h"
#include "../headers/string.h"

int check_number(char *number) {
	char *ptr = number;

	while (*ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
		ptr++;
	}

	return 1;
}

int check_natural_number(char *number) {
	char *ptr = number;

	while (*ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
		ptr++;
	}

	return atoi(number) > 0;
}

int check_city_street(char *city_street) {
	if (!*city_street) {
		return 0;
	}

	char *ptr = city_street - 1;

	while (*++ptr) {
		if (!isalpha(*ptr)) {
			return 0;
		}
	}

	return 1;
}

int check_home_flat(char *home_flat) {
	if (!*home_flat) {
		return 0;
	}

	return check_natural_number(home_flat);
}

int check_index(char *index) {
	if (!*index) {
		return 0;
	}

	if (strlen(index) != 6) {
		return 0;
	}

	return check_natural_number(index);
}

int check_weight(char *weight) {
	char *ptr = weight;
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

int check_mail_id(char *mail_id) {
	if (!*mail_id) {
		return 0;
	}

	if (strlen(mail_id) != 14) {
		return 0;
	}

	return check_natural_number(mail_id);
}

int check_date(char *date) {
	char *ptr = date;
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

int check_time(char *time) {
	char *ptr = time;
	char *hour = NULL;
	char *minute = NULL;
	char *second = NULL;
	int size = 0;
	
	while (*ptr != ':' && *ptr) {
		join_symb(*ptr, &hour, &size);
		ptr++;
	}

	if (!hour) {
		return 0;
	}

	ptr++;
	size = 0;

	while (*ptr != ':' && *ptr) {
		join_symb(*ptr, &minute, &size);
		ptr++;
	}

	if (!minute) {
		free(hour);

		return 0;
	}

	ptr++;
	size = 0;

	while (*ptr && *ptr) {
		join_symb(*ptr, &second, &size);
		ptr++;
	}

	if (!second) {
		free(hour);
		free(minute);

		return 0;
	}

	if (strlen(hour) != 2 || 
		!check_number(hour) ||
		atoi(hour) < 0 ||
		atoi(hour) > 23) {
		free(hour);
		free(minute);
		free(second);

		return 0;
	}

	if (strlen(minute) != 2 || 
		!check_number(minute) ||
		atoi(minute) < 0 ||
		atoi(minute) > 59) {
		free(hour);
		free(minute);
		free(second);

		return 0;
	}

	if (strlen(second) != 2 || 
		!check_number(second) ||
		atoi(second) < 0 ||
		atoi(second) > 59) {
		free(hour);
		free(minute);
		free(second);

		return 0;
	}

	free(hour);
	free(minute);
	free(second);

	return 1;
}

int check_datetime(char *datetime) {
	char *date = NULL;
	char *time = NULL;

	if (token_count(datetime) != 2) {
		return 0;
	}

	if (get_token_str(&datetime, &date) == NO_MEMORY) {
		return 0;
	}
	if (get_token_str(&datetime, &time) == NO_MEMORY) {
		return 0;
	}

	if (check_date(date) && check_time(time)) {
		free(date);
		free(time);

		return 1;
	}

	free(date);
	free(time);

	return 0;
}

int check_mail(char *mail) {
	if (token_count(mail) != 16) {
		return INVALID_MAIL;
	}

	char *city_1 = NULL;
	char *street_1 = NULL;
	char *home_1 = NULL;
	char *flat_1 = NULL;
	char *index_1 = NULL;
	char *city_2 = NULL;
	char *street_2 = NULL;
	char *home_2 = NULL;
	char *flat_2 = NULL;
	char *index_2 = NULL;
	char *weight = NULL;
	char *mail_id = NULL;
	char *time_create_date = NULL;
	char *time_create_time = NULL;
	char *time_delivery_date = NULL;
	char *time_delivery_time = NULL;

	if (get_token_str(&mail, &city_1) == NO_MEMORY ||
		get_token_str(&mail, &street_1) == NO_MEMORY ||
		get_token_str(&mail, &home_1) == NO_MEMORY ||
		get_token_str(&mail, &flat_1) == NO_MEMORY ||
		get_token_str(&mail, &index_1) == NO_MEMORY ||
		get_token_str(&mail, &city_2) == NO_MEMORY ||
		get_token_str(&mail, &street_2) == NO_MEMORY ||
		get_token_str(&mail, &home_2) == NO_MEMORY ||
		get_token_str(&mail, &flat_2) == NO_MEMORY ||
		get_token_str(&mail, &index_2) == NO_MEMORY ||
		get_token_str(&mail, &weight) == NO_MEMORY ||
		get_token_str(&mail, &mail_id) == NO_MEMORY ||
		get_token_str(&mail, &time_create_date) == NO_MEMORY ||
		get_token_str(&mail, &time_create_time) == NO_MEMORY ||
		get_token_str(&mail, &time_delivery_date) == NO_MEMORY ||
		get_token_str(&mail, &time_delivery_time) == NO_MEMORY) {
		return NO_MEMORY;
	}

	if (!check_city_street(city_1) ||
		!check_city_street(street_1) ||
		!check_home_flat(home_1) ||
		!check_home_flat(flat_1) ||
		!check_index(index_1) ||
		!check_city_street(city_2) ||
		!check_city_street(street_2) ||
		!check_home_flat(home_2) ||
		!check_home_flat(flat_2) ||
		!check_index(index_2) ||
		!check_weight(weight) ||
		!check_mail_id(mail_id) ||
		!check_date(time_create_date) ||
		!check_time(time_create_time) ||
		!check_date(time_delivery_date) ||
		!check_time(time_delivery_time)) {

		free(city_1);
		free(street_1);
		free(home_1);
		free(flat_1);
		free(index_1);
		free(city_2);
		free(street_2);
		free(home_2);
		free(flat_2);
		free(index_2);
		free(weight);
		free(mail_id);
		free(time_create_date);
		free(time_create_time);
		free(time_delivery_date);
		free(time_delivery_time);

		return INVALID_MAIL;
	}

	free(city_1);
	free(street_1);
	free(home_1);
	free(flat_1);
	free(index_1);
	free(city_2);
	free(street_2);
	free(home_2);
	free(flat_2);
	free(index_2);
	free(weight);
	free(mail_id);
	free(time_create_date);
	free(time_create_time);
	free(time_delivery_date);
	free(time_delivery_time);

	return 1;
}