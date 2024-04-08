#pragma once

typedef struct uplist
{
    void **items;
    unsigned count;
    unsigned capacity;
    unsigned itemSize;
} uplist;

void uplist_init(uplist *const this);
void uplist_initAlloc(uplist *const, unsigned);
void *uplist_alloc(uplist *const);
void *uplist_add(uplist *const, void *const);
int uplist_find(uplist *const, void *const);
void uplist_iterator(uplist *const, void (*)(), unsigned);
unsigned uplist_remove(uplist *const, void *const);
void uplist_reset(uplist *const);
void uplist_end(uplist *const);