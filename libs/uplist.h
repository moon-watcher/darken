#pragma once

typedef struct uplist
{
    void **items;
    unsigned count;
    unsigned capacity;
    unsigned resizeBy;
} uplist;

void uplist_init(uplist *const);
void *uplist_add(uplist *const, void *const);
unsigned uplist_resize(uplist *const, unsigned);
int uplist_find(uplist *const, void *const);
unsigned uplist_remove(uplist *const, void *const);
unsigned uplist_removeById(uplist *const, unsigned);
void uplist_reset(uplist *const);
void uplist_end(uplist *const);