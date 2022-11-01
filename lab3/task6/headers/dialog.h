#ifndef DIALOG_H
#define DIALOG_H

#include "../headers/types.h"
#include "../headers/check.h"
#include "../headers/post.h"
#include "../headers/string.h"
#include "../headers/binom_heap.h"

int run_add_mail(post*);
int run_delete_id(post*);
int run_delete_full(post*);
int run_find_id(post);
int run_find_delivery(char*, post);
int run_find_not_delivery(char*, post);
int run_sort(post*);
int dialog(char*);

#endif // DIALOG_H