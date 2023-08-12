#pragma once

typedef struct dculist_t {
    void **array;

    // Next free position in entities array
    unsigned int freePos;

    // Number of allocated entities
    unsigned int allocatedObjects;

    unsigned int maxObjects;
    unsigned int objectSize;
}
dculist_t;


void dculist_init(dculist_t *const this, unsigned int maxObjects, unsigned int objectSize);
void dclist_iterate(dculist_t *const this, void (*callback)(void *const));
void dculist_end(dculist_t *const this, void (*callback)(void *const));
void dculist_reset(dculist_t *const this, void (*callback)(void *const));
void *dculist_add(dculist_t *const this);
void *dculist_get(dculist_t *const this, unsigned int index);
void *dculist_delete(dculist_t *const this, unsigned int index);