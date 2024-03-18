// #pragma once

// // V.2
// // Removes the node ID for saving memory
// // ID implementation corresponds to the proccess that use the  list
// // list_find were removed
// //
// // Borrowed from http://pseudomuto.com/development/2013/05/02/implementing-a-generic-linked-list-in-c/ Thanks!

// typedef void (*freeFunction)(void *); // a common function used to free malloc'd objects
// typedef void (*listIterator)(void *);

// typedef struct listNode
// {
// 	void *data;
// 	struct listNode *next;
// } listNode;

// typedef struct list
// {
// 	unsigned length;
// 	unsigned elementSize;
// 	listNode *head;
// 	listNode *tail;
// 	freeFunction freeFn;
// } list;

// #define list_find_node(LIST, TYPE, FIELD, SEARCH) \
// 	({                                            \
// 		listNode *node = LIST.head;               \
// 		listNode *ptr = 0;                        \
//                                                   \
// 		while (node)                              \
// 		{                                         \
// 			TYPE *o = (TYPE *)node->data;         \
//                                                   \
// 			if (o->FIELD == SEARCH)               \
// 			{                                     \
// 				ptr = node;                       \
// 				break;                            \
// 			}                                     \
//                                                   \
// 			node = node->next;                    \
// 		}                                         \
//                                                   \
// 		ptr;                                      \
// 	})

// void list_new(list *, unsigned, freeFunction);
// void list_destroy(list *);
// unsigned list_size(list *);
// void list_prepend(list *, void *);
// void list_append(list *, void *);
// void list_head(list *, void *, unsigned);
// void list_tail(list *, void *);
// void list_foreach(list *, listIterator);
// void list_remove(list *, listNode *);