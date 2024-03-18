// #include <genesis.h>
// #include "listptr.h"

// static int const listptrNode_s = sizeof(listptrNode);

// void listptr_init(listptr *list, listptrFn freeFn)
// {
//     list->length = 0;
//     list->head = 0;
//     list->freeFn = freeFn;
// }

// void listptr_destroy(listptr *list)
// {
//     listptr_foreach(list, node)
//         listptr_remove(list, node);
// }

// int listptr_size(listptr *list)
// {
//     return list->length;
// }

// listptrNode *listptr_add(listptr *list, void *element)
// {
//     listptrNode *node = malloc(listptrNode_s);

//     node->data = element;
//     node->prev = 0;
//     node->next = list->head;

//     if (list->head)
//         list->head->prev = node;

//     list->head = node;
//     ++list->length;

//     return node;
// }

// void listptr_remove(listptr *list, listptrNode *node)
// {
//     if (!(list->length && list->head && node))
//         return;

//     if (list->freeFn)
//         list->freeFn(node->data);

//     if (list->head == node)
//         list->head = node->next;

//     if (node->next)
//         node->next->prev = node->prev;

//     if (node->prev)
//         node->prev->next = node->next;

//     --list->length;
//     free(node);
// }

// void listptr_iterate(listptr *list, listptrFn iterator)
// {
//     listptr_foreach(list, node)
//         iterator(node->data);
// }

// // listptrNode *node = list->head;
// // while ( node )
// // {
// //     iterator ( node->data );
// //     node = node->next;
// // }