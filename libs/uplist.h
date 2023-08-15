#pragma once

typedef struct de_libs_uplist
{
    void **list;
    unsigned int freePos;
    unsigned int size;
} de_libs_uplist;

void de_libs_uplist_init(de_libs_uplist *const list, unsigned int size);
int de_libs_uplist_add(de_libs_uplist *const list, void *const add);
void de_libs_uplist_iterator(de_libs_uplist *const list, void (*iterator)(), unsigned int nbItems);
void de_libs_uplist_remove(de_libs_uplist *const list, unsigned int index);
void de_libs_uplist_end(de_libs_uplist *const list);
void de_libs_uplist_reset(de_libs_uplist *const list);
void de_libs_uplist_removeByData(de_libs_uplist *const list, void *const data, unsigned int nbItems);