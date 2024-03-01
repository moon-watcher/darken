// Cacheable unordered list (of pointers)

#include "culist.h"

#include "../services/free.h"
#include "../services/malloc.h"

void culist_init(culist *const this, unsigned objectSize, unsigned size)
{
    uplist_init(&this->upl, size);

    this->objectSize = objectSize ?: 1;
    this->allocatedObjects = 0;
}

void *culist_add(culist *const this)
{
    uplist *const upl = &this->upl;
    int next = upl->count;

    if (next < (int)this->allocatedObjects)
        ++upl->count;

    else if ((next = uplist_add(upl, malloc(this->objectSize))) < 0)
        return 0;

    ++this->allocatedObjects;

    return upl->items[next];
}

void culist_iterator(culist *const this, void (*iterator)())
{
    uplist_iterator(&this->upl, iterator, 1);
}

void culist_iteratorEx(culist *const this, void (*iterator)(), unsigned params)
{
    uplist_iterator(&this->upl, iterator, params);
}

unsigned culist_remove(culist *const this, void *const data, void (*callback)())
{
    uplist *const upl = &this->upl;
    int const index = uplist_find(upl, data);

    if (index < 0)
        return 0;

    if (callback != 0)
        callback(upl->items[index]);

    return uplist_remove(upl, index);
}

unsigned culist_removeEx(culist *const this, void *const data, void (*callback)(), unsigned params)
{
    uplist *const upl = &this->upl;
    int const index = uplist_find(upl, data);

    if (index < 0)
        return 0;

    if (callback != 0)
        callback(upl->items[index]);

    return uplist_removeByData(upl, data, params);
}

void culist_end(culist *const this, void (*callback)())
{
    culist_reset(this, callback);

    uplist *const upl = &this->upl;
    void **const list = upl->items;

    for (unsigned i = 0; i < this->allocatedObjects; i++)
        free(list[i]);

    this->allocatedObjects = 0;

    uplist_end(upl);
}

void culist_reset(culist *const this, void (*callback)())
{
    culist_iterator(this, callback);
    uplist_reset(&this->upl);
}

// void culist_iterator(culist *const this, void (*callback)())
// {
//     if (callback == 0)
//         return;

//     void **const list = this->upl.list;
//     unsigned *const next = &this->upl.next;

//     for (unsigned i = 0; i < *next; i++)
//         callback(list[i]);
// }