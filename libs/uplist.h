#pragma once

typedef struct uplist
{
    void **list;
    unsigned next;
    unsigned size;
    unsigned char resizeBy;
} uplist;

void uplist_init(uplist *const, unsigned);
int uplist_add(uplist *const, void *const);
int uplist_resize(uplist *const, unsigned);
void uplist_iterator(uplist *const, void (*)(), unsigned);
void uplist_iteratorEx(uplist *const, void (*)(), unsigned);
unsigned uplist_remove(uplist *const, unsigned);
void uplist_end(uplist *const);
void uplist_reset(uplist *const);
unsigned uplist_removeByData(uplist *const, void *const, unsigned);
int uplist_find(uplist *const, void *const);