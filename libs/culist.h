#pragma once

#include "uplist.h"

typedef struct culist
{
    uplist upl;

    unsigned allocatedObjects; // Number of allocated entities
    unsigned objectSize;
} culist;

void culist_init(culist *const, unsigned, unsigned);
void *culist_add(culist *const);
void culist_iterator(culist *const, void (*)());
void culist_iteratorEx(culist *const, void (*)(), unsigned);
unsigned culist_remove(culist *const, void *const, void (*)());
unsigned culist_removeEx(culist *const, void *const, void (*)(), unsigned);
void culist_end(culist *const, void (*)());
void culist_reset(culist *const, void (*)());