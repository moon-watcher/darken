// #pragma once

// typedef void (*listptrFn)(void *);

// typedef struct listptrNode
// {
// 	void *data;
// 	struct listptrNode *next;
// 	struct listptrNode *prev;
// } listptrNode;

// typedef struct
// {
// 	int length;
// 	listptrNode *head;
// 	listptrFn freeFn;
// } listptr;

// #define listptr_foreach(LIST, NODE) \
// 	for (listptrNode *NODE = LIST->head; NODE; NODE = NODE->next)

// void listptr_init(listptr *, listptrFn);
// void listptr_destroy(listptr *);
// int listptr_size(listptr *);
// listptrNode *listptr_add(listptr *, void *);
// void listptr_remove(listptr *, listptrNode *);
// void listptr_iterate(listptr *, listptrFn);
