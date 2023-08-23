// Cacheable unordered list (of pointers)

#include "culist.h"

#include "../config/free.h"
#include "../config/malloc.h"

void culist_init(culist *const this, unsigned int size, unsigned int objectSize)
{
    uplist_init(&this->upl, size);

    this->objectSize = objectSize ?: 1;
    this->allocatedObjects = 0;
}

void *culist_add(culist *const this)
{
    uplist *const upl = &this->upl;
    unsigned int next = upl->next;

    if (next < this->allocatedObjects)
        ++upl->next;
    else
    {
        int const pos = uplist_add(upl, malloc(this->objectSize));

        if (pos < 0)
            return 0;

        next = pos;
        ++this->allocatedObjects;
    }

    return upl->list[next];
}

void culist_iterator(culist *const this, void (*callback)())
{
    uplist_iterator(&this->upl, callback, 1);
}

void culist_remove(culist *const this, void *const data, void (*callback)())
{
    int const index = uplist_find(&this->upl, data);

    if (index < 0)
        return;

    if (callback != 0)
        callback(this->upl.list[index]);

    uplist_remove(&this->upl, index);
}

void culist_end(culist *const this, void (*callback)())
{
    culist_reset(this, callback);

    void **const list = this->upl.list;

    for (unsigned int i = 0; i < this->allocatedObjects; i++)
        free(list[i]);

    this->allocatedObjects = 0;

    uplist_end(&this->upl);
}

void culist_reset(culist *const this, void (*callback)())
{
    culist_iterator(this, callback);
    uplist_reset(&this->upl);
}