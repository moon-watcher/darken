#pragma once

typedef struct uclist
{
    void **items;
    unsigned count;
    unsigned capacity;
    unsigned itemSize;
} uclist;

void uclist_init(uclist *const);
void uclist_initAlloc(uclist *const, unsigned);
void *uclist_alloc(uclist *const);
void *uclist_add(uclist *const, void *const);
int uclist_find(uclist *const, void *const);
void uclist_iterator(uclist *const, void (*)(), unsigned);
unsigned uclist_remove(uclist *const, void *const, void (*exec)());
void uclist_reset(uclist *const);
void uclist_end(uclist *const);