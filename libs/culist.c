// Cacheable unordered list (of pointers)

#include "culist.h"
#include "upiterator.h"

#include "../services/free.h"
#include "../services/malloc.h"

void culist_init(culist *const this, unsigned itemSize, unsigned size)
{
    uplist_init(&this->upl, size);

    this->itemSize = itemSize ?: 1;
    this->allocatedItems = 0;
}

void *culist_add(culist *const this)
{
    int count = this->upl.count;

    if (count < (int)this->allocatedItems)
        ++this->upl.count;

    else if ((count = uplist_add(&this->upl, malloc(this->itemSize))) < 0)
        return 0;

    ++this->allocatedItems;

    return this->upl.items[count];
}

int culist_remove(culist *const this, void *const data, void (*callback)())
{
    int index = uplist_find(&this->upl, data);

    if (index < 0)
        return -1;

    if (callback != 0)
        callback(this->upl.items[index]);

    return uplist_remove(&this->upl, index);
}

void culist_reset(culist *const this, void (*callback)())
{
    upiterator(this->upl.items, &this->upl.count, callback, 1);
    uplist_reset(&this->upl);
}

void culist_end(culist *const this, void (*callback)())
{
    culist_reset(this, callback);

    for (unsigned i = 0; i < this->allocatedItems; i++)
        free(&this->upl.items[i]);

    this->allocatedItems = 0;

    uplist_end(&this->upl);
}