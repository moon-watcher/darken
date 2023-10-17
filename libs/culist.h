#pragma once

#include "uplist.h"

typedef struct culist
{
    uplist upl;

    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int objectSize;
} culist;

void culist_init(culist *const, unsigned int, unsigned int);
void *culist_add(culist *const, void *const);
void culist_iterator(culist *const, void (*)());
void culist_iteratorEx(culist *const, void (*)(), unsigned int);
void *culist_remove(culist *const, void *const, void (*)());
int culist_removeEx(culist *const, void *const, void (*)(), unsigned int);
void culist_end(culist *const, void (*)());
void culist_reset(culist *const, void (*)());