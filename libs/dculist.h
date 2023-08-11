#pragma once


// Cached Unordered List

typedef struct dculist_t
{
    void **list;    
    unsigned int freePos; // Next free position in entities array
    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int maxObjects;
    unsigned int objectSize;
} dculist_t;

void dculist_init(dculist_t *const this, unsigned int maxObjects, unsigned int objectSize);
void dculist_reset(dculist_t *const this);
void *dculist_add(dculist_t *const this);
void dculist_remove(dculist_t *const this, unsigned int index);
void dculist_iterator(dculist_t *const this, void (*function)(void *));