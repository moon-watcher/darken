#include "dculist.h"
#include "common.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Dynamic Chacheable Unordered List

void de_libs_dculist_init(de_libs_dculist *const this, unsigned int size, unsigned int objectSize)
{
    de_libs_common_init(this, size);

    this->objectSize = objectSize ? objectSize : 1;
    this->allocatedObjects = 0;
}

void *de_libs_dculist_add(de_libs_dculist *const this)
{
    if (this->freePos >= this->size)
        if (de_libs_common_resize(this, this->size + this->size / 2) == 0)
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

void de_libs_dculist_iterator(de_libs_dculist *const this, void (*callback)(void *const))
{
    if (callback == 0)
        return;

    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    for (unsigned int i = 0; i < *freePos; i++)
        callback(list[i]);
}

void de_libs_dculist_remove(de_libs_dculist *const this, void *const data, void (*callback)(void *const))
{
    int const index = de_libs_common_find(this, data);

    if (index < 0)
        return;

    de_libs_common_remove(this, index, callback);
}

void de_libs_dculist_end(de_libs_dculist *const this, void (*callback)(void *const))
{
    de_libs_dculist_reset(this, callback);

    void **const list = this->list;

    for (unsigned int i = 0; i < this->allocatedObjects; i++)
        free(list[i]);

    free(list);
}

void de_libs_dculist_reset(de_libs_dculist *const this, void (*callback)(void *const))
{
    de_libs_dculist_iterator(this, callback);

    this->freePos = 0;
}