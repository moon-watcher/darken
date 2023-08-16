#pragma once

typedef struct uplist
{
    void **list;
    unsigned int freePos;
    unsigned int size;
} uplist;

void uplist_init(uplist *const, unsigned int);
int uplist_add(uplist *const, void *const);
void uplist_iterator(uplist *const, void (*)(), unsigned int);
void uplist_remove(uplist *const, unsigned int);
void uplist_end(uplist *const);
void uplist_reset(uplist *const);
void uplist_removeByData(uplist *const, void *const, unsigned int);