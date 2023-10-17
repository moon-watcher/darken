#pragma once

typedef struct uplist
{
    void **list;
    unsigned int next;
    unsigned int size;
    unsigned char resizeBy;
} uplist;

void uplist_init(uplist *const, unsigned int);
int uplist_add(uplist *const, void *const);
void uplist_iterator(uplist *const, void (*)(), unsigned int);
void *uplist_remove(uplist *const, unsigned int);
void uplist_end(uplist *const);
void uplist_reset(uplist *const);
int uplist_removeByData(uplist *const, void *const, unsigned int);
int uplist_find(uplist *const, void *const);