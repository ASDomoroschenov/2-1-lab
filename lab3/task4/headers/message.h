#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
	size_t id;
	char *text_message;
	size_t length;
} message;

int check_id(char*);
int check_message(char*);
int get_message(char*, message**);
int try_get_valid_message(char**, char*);
int get_messages_from_csv(char*, message***, size_t);
void output_messages(message**, size_t);
void free_messages(message***, size_t);

#endif // MESSAGE_H