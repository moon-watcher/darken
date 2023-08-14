#include "dculist.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Dynamic Chacheable Unordered List

void dculist_init(dculist_t *const this, unsigned int size, unsigned int objectSize)
{
    this->size = size ? size : 1;
    this->list = malloc(this->size * sizeof(void *));
    this->freePos = 0;

    this->objectSize = objectSize ? objectSize : 1;
    this->allocatedObjects = 0;
}

void *dculist_add(dculist_t *const this)
{
    if (this->freePos >= this->size)
        if (dculist_resize(this, this->size + this->size / 2) == 0)
            return 0;

    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    if (*freePos >= this->allocatedObjects)
    {
        list[*freePos] = malloc(this->objectSize);
        ++this->allocatedObjects;
    }

    ++*freePos;

    return list[*freePos - 1];
}

void dculist_iterator(dculist_t *const this, void (*callback)(void *const))
{
    if (callback == 0)
        return;

    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    for (unsigned int i = 0; i < *freePos; i++)
        callback(list[i]);
}

void dculist_remove(dculist_t *const this, void *const data, void (*callback)(void *const))
{
    int const index = dculist_find(this, data);

    if (index < 0)
        return;

    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    if (callback)
        callback(list[index]);

    --*freePos;
    list[index] = list[*freePos];
}

void dculist_end(dculist_t *const this, void (*callback)(void *const))
{
    dculist_iterator(this, callback);

    void **const list = this->list;

    for (unsigned int i = 0; i < this->allocatedObjects; i++)
        free(list[i]);

    free(list);
}

void dculist_reset(dculist_t *const this, void (*callback)(void *const))
{
    dculist_iterator(this, callback);

    this->freePos = 0;
}


 //////////////////////////////////////////////////////////////////////


int dculist_resize(dculist_t *const this, unsigned int size)
{
    if (this->size == size)
        return -1;

    unsigned int const oldsize = this->size;
    this->size = size;
    void *list = malloc(this->size * sizeof(void *));

    if (list == 0)
        return 0;

    memcpy(list, this->list, oldsize);
    free(this->list);

    this->list = list;

    return 1;
}

int dculist_find(dculist_t *const this, void *const data)
{
    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    for (unsigned int i = 0; i < *freePos; i++)
        if (list[i] == data)
            return i;

    return -1;
}