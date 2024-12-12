#pragma once

enum
{
    UCLIST_OK = 1,
    UCLIST_ALLOC_ERROR = 0,
    UCLIST_NOT_FOUND = -1,
    UCLIST_NO_COUNT = -2,
    UCLIST_NO_ITERATOR = -3,
    UCLIST_NO_NBITEMS = -4,
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
int uclist_find(uclist *const, void *const);
int uclist_iterator(uclist *const, void (*)(), unsigned);
int uclist_remove(uclist *const, void *const, void (*exec)());
void uclist_removeByIndex(uclist *const, unsigned, void (*)());
void uclist_reset(uclist *const);
void uclist_end(uclist *const);
