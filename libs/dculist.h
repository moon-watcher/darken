#pragma once

typedef struct dculist
{
    void **list;
    unsigned int freePos; // Next free position in entities array
    unsigned int size;

    //
    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int objectSize;

    void (*update_f)(void *const);
    void (*remove_f)(void *const);
} dculist;

void dculist_init(dculist *const, unsigned int, unsigned int);
void *dculist_add(dculist *const);
void dculist_iterator(dculist *const, void (*)(void *const));
void dculist_remove(dculist *const, void *const, void (*)(void *const));
void dculist_end(dculist *const, void (*)(void *const));
void dculist_reset(dculist *const, void (*)(void *const));