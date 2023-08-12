#include "dculist.h"

#include "../config/free.h"
#include "../config/malloc.h"


void dculist_init(dculist_t *const this, unsigned int maxObjects, unsigned int objectSize)
{
    this->objectSize = objectSize ? objectSize : 1;
    this->maxObjects = maxObjects ? maxObjects : 1;
    this->array = malloc(sizeof(void *) * this->maxObjects);
    this->freePos = 0;
    this->allocatedObjects = 0;
}

void dclist_iterate(dculist_t *const this, void (*callback)(void *const))
{
    if (callback != 0)
        for (unsigned int i = 0; i < this->freePos; i++)
            callback(this->array[i]);
}

void dculist_end(dculist_t *const this, void (*callback)(void *const))
{
    dclist_iterate(this, callback);

    for (unsigned int i = 0; i < this->allocatedObjects; i++)
        free(this->array[i]);

    free(this->array);
}

void dculist_reset(dculist_t *const this, void (*callback)(void *const))
{
    if (callback != 0)
        while (this->freePos)
            callback(this->array[0]);

    this->freePos = 0;
}





void *dculist_add(dculist_t *const this)
{
    if (this->freePos >= this->maxObjects)
        return 0;

    if (this->freePos >= this->allocatedObjects)
    {
        this->array[this->freePos] = malloc(this->objectSize);
        ++this->allocatedObjects;
    }

    this->freePos++;

    return this->array[this->freePos - 1];
}

void *dculist_get(dculist_t *const this, unsigned int index)
{
    return this->array[index];
}

void *dculist_delete(dculist_t *const this, unsigned int index)
{
    if (index >= this->freePos)
        return 0;

    this->freePos--;

    return this->array[this->freePos];
}