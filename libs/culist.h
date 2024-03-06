#pragma once

typedef struct culist
{
    void **items;
    unsigned count;
    unsigned capacity;
    unsigned itemSize;
} culist;

void culist_init(culist *const, unsigned);
void *culist_add(culist *const);
int culist_find(culist *const, void *const);
unsigned culist_remove(culist *const, void *const);
unsigned culist_removeById(culist *const, unsigned);
void culist_reset(culist *const);
void culist_end(culist *const);