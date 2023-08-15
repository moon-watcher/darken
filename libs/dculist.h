#pragma once

typedef struct dculist_t
{
    void **list;
    unsigned int freePos; // Next free position in entities array
    unsigned int size;

    //
    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int objectSize;

    void (*update_f)(void *const);
    void (*remove_f)(void *const);
} dculist_t;

void dculist_init(dculist_t *const list, unsigned int size, unsigned int objectSize);
void *dculist_add(dculist_t *const list);
void dculist_iterator(dculist_t *const list, void (*callback)(void *const));
void dculist_remove(dculist_t *const list, void *const data, void (*callback)(void *const));
void dculist_end(dculist_t *const list, void (*callback)(void *const));
void dculist_reset(dculist_t *const list, void (*callback)(void *const));