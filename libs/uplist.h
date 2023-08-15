#pragma once

typedef struct de_libs_uplist
{
    void **list;
    unsigned int freePos;
    unsigned int size;
} de_libs_uplist;

void de_libs_uplist_init(de_libs_uplist *const, unsigned int);
int de_libs_uplist_add(de_libs_uplist *const, void *const);
void de_libs_uplist_iterator(de_libs_uplist *const, void (*)(), unsigned int);
void de_libs_uplist_remove(de_libs_uplist *const, unsigned int);
void de_libs_uplist_end(de_libs_uplist *const);
void de_libs_uplist_reset(de_libs_uplist *const);
void de_libs_uplist_removeByData(de_libs_uplist *const, void *const, unsigned int);