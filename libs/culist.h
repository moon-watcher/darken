#pragma once

#include "uplist.h"

typedef struct culist
{
    uplist upl;
    unsigned allocatedItems;
    unsigned itemSize;
} culist;

void culist_init(culist *const, unsigned, unsigned);
void *culist_add(culist *const);
// int culist_remove(culist *const, void *const, void (*)());
// void culist_reset(culist *const, void (*)());
// void culist_end(culist *const, void (*)());