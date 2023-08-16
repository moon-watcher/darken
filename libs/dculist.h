#pragma once

#include "uplist.h"

typedef struct dculist
{
    uplist upl;
    
    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int objectSize;
} dculist;

void dculist_init(dculist *const, unsigned int, unsigned int);
void *dculist_add(dculist *const);
void dculist_iterator(dculist *const, void (*)(void *const));
void dculist_remove(dculist *const, void *const, void (*)(void *const));
void dculist_end(dculist *const, void (*)(void *const));
void dculist_reset(dculist *const, void (*)(void *const));