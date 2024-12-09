#pragma once

enum
{
    UCLIST_OK = 1,
    UCLIST_ERROR_ALLOC = 0,
    UCLIST_ERROR_NOTFOUND = -1,
    UCLIST_ERROR_COUNT = -2,
    UCLIST_ERROR_ITERATOR = -3,
    UCLIST_ERROR_NBITEMS = -4,
};

typedef struct uclist
{
    void **items;
    unsigned count;
    unsigned capacity;
    unsigned itemSize;
} uclist;

void uclist_init(uclist *const, unsigned);
void *uclist_alloc(uclist *const);
void *uclist_add(uclist *const, void *const);
int uclist_find(uclist *const, void *const);
int uclist_iterator(uclist *const, void (*)(), unsigned);
void uclist_iteratorEx(uclist *const, void (*)(), unsigned);
int uclist_remove(uclist *const, void *const, void (*exec)());
void uclist_removeByIndex(uclist *const, unsigned, void (*)());
void uclist_reset(uclist *const);
void uclist_end(uclist *const);
