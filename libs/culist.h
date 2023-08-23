#pragma once

#include "uplist.h"

typedef struct culist
{
    uplist upl;
    
    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int objectSize;
} culist;

void culist_init(culist *const, unsigned int, unsigned int);
void *culist_add(culist *const);
void culist_iterator(culist *const, void (*)());
void culist_remove(culist *const, void *const, void (*)());
void culist_end(culist *const, void (*)());
void culist_reset(culist *const, void (*)());