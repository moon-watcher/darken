#include "dculist.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Dynamic Chacheable Unordered List

void dculist_init(dculist_t *const this, unsigned int maxObjects, unsigned int objectSize)
{
    this->objectSize = objectSize ? objectSize : 1;
    this->maxObjects = maxObjects ? maxObjects : 1;
    this->list = malloc(sizeof(void *) * this->maxObjects);
    this->freePos = 0;
    this->allocatedObjects = 0;
}

void dculist_iterator(dculist_t *const this, void (*callback)(void *const))
{
    if (callback != 0)
        for (unsigned int i = 0; i < this->freePos; i++)
            callback(this->list[i]);
}

void dculist_end(dculist_t *const this, void (*callback)(void *const))
{
    dculist_iterator(this, callback);

    for (unsigned int i = 0; i < this->allocatedObjects; i++)
        free(this->list[i]);

    free(this->list);
}

void dculist_reset(dculist_t *const this, void (*callback)(void *const))
{
    dculist_iterator(this, callback);

    this->freePos = 0;
}

void *dculist_new(dculist_t *const this)
{
    if (this->freePos >= this->maxObjects)
        return 0;

    if (this->freePos >= this->allocatedObjects)
    {
        this->list[this->freePos] = malloc(this->objectSize);
        ++this->allocatedObjects;
    }

    this->freePos++;

    return this->list[this->freePos - 1];
}

int dculist_find(dculist_t *const this, void *const data)
{
    for (unsigned int i = 0; i < this->freePos; i++)
        if (this->list[i] == data)
            return i;
    
    return -1;
}

unsigned int dculist_remove(dculist_t *const this, void *const data, void (*callback)(void *const))
{
    int const index = dculist_find(this, data);

    if (index < 0)
        return 0;

    this->freePos--;

    if (callback)
        callback(this->list[index]);

    this->list[index] = this->list[this->freePos];

    return 1;
}