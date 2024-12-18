#pragma once

enum
{
    UCLIST_NOT_FOUND = -1,
    UCLIST_NO_ITERATOR = -1,
    UCLIST_NO_NBITEMS = -2,
};

typedef struct uclist
{
    void **list;
    unsigned count;
    unsigned capacity;
    unsigned itemSize;
} uclist;

void uclist_init(uclist *const, unsigned);
void *uclist_alloc(uclist *const);
void *uclist_add(uclist *const, void *const);
int uclist_iterator(uclist *const, void (*)());
int uclist_remove(uclist *const, void *const, void (*exec)());
void uclist_reset(uclist *const);
void uclist_end(uclist *const);
