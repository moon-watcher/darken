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

void *culist_add(culist *const this, void *const value)
{
    uplist *const upl = &this->upl;
    int next = upl->next;

    if (next < (int)this->allocatedObjects)
        ++upl->next;

    else if ((next = uplist_add(upl, malloc(this->objectSize))) >= 0)
        ++this->allocatedObjects;

    if (next < 0)
        return 0;

    if (value)
        upl->list[next] = value;

    return upl->list[next];
}

void culist_iterator(culist *const this, void (*callback)())
{
    if (callback != 0)
        uplist_iterator(&this->upl, callback, 1);
}

void culist_iteratorEx(culist *const this, void (*callback)(), unsigned int params)
{
    if (callback != 0)
        uplist_iterator(&this->upl, callback, params);
}

void culist_remove(culist *const this, void *const data, void (*callback)())
{
    uplist *const upl = &this->upl;
    int const index = uplist_find(upl, data);

    if (index < 0)
        return;

    if (callback != 0)
        callback(upl->list[index]);

    uplist_remove(upl, index);
}

void culist_removeEx(culist *const this, void *const data, void (*callback)(), unsigned int params)
{
    uplist *const upl = &this->upl;
    int const index = uplist_find(upl, data);

    if (index < 0)
        return;

    if (callback != 0)
        callback(upl->list[index]);

    uplist_removeByData(upl, data, params);
}

void culist_end(culist *const this, void (*callback)())
{
    culist_reset(this, callback);

    uplist *const upl = &this->upl;
    void **const list = upl->list;

    for (unsigned int i = 0; i < this->allocatedObjects; i++)
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
//     unsigned int *const next = &this->upl.next;

//     for (unsigned int i = 0; i < *next; i++)
//         callback(list[i]);
// }