#pragma once

typedef struct dculist_t
{
    void **list;
    unsigned int freePos;          // Next free position in entities array
    unsigned int allocatedObjects; // Number of allocated entities
    unsigned int maxObjects;
    unsigned int objectSize;
} dculist_t;

void dculist_init(dculist_t *const list, unsigned int maxObjects, unsigned int objectSize);
void dculist_iterator(dculist_t *const list, void (*callback)(void *const));
void dculist_end(dculist_t *const list, void (*callback)(void *const));
void dculist_reset(dculist_t *const list, void (*callback)(void *const));
void *dculist_new(dculist_t *const list);
int dculist_find(dculist_t *const list, void *const data);
unsigned int dculist_remove(dculist_t *const list, void *const data, void (*callback)(void *const));