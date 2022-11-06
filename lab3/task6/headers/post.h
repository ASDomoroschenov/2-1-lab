#ifndef POST_H
#define POST_H

#include "types.h"

int create_mail_strings(mail**);
int add_mail_str(post*);
int add_mail(post*, mail*);
int delete_full_mail(post*);
int delete_id_mail(post*, char*);
int find_id_mail(mail***, mail**, int*, int, char*);
int cmp_mail(mail*, mail*);
int cmp_mail_sort(mail*, mail*);
int cmp_mail_dates(mail*, mail*);
int cmp_dates_str(string, string);
int get_delivered_mails(char*, post, binom_heap**, int*, int (*cmp)(mail*, mail*));
int get_not_delivered_mails(char*, post, binom_heap**, int*, int (*cmp)(mail*, mail*));
int output_find_mails(binom_heap*, int, int (*cmp)(mail*, mail*));
void output_beatiful_mail(int, mail*, FILE*);
void output_mails(post);
void free_mail(mail**);
void free_post(post*);

#endif // POST_H