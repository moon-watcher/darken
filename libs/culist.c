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

void *culist_alloc(culist *const this)
{
    int count = this->upl.count;

    if (count < (int)this->allocatedItems)
        ++this->upl.count;

    else if ((count = uplist_add(&this->upl, malloc(this->itemSize))) < 0)
        return 0;

    ++this->allocatedItems;

    return this->upl.items[count];
}

int culist_find(culist *const this, void *const data)
{
    return uplist_find(&this->upl, data);
}

void culist_iterator(culist *const this, void (*callback)(), unsigned params)
{
    uplist_iterator(&this->upl, callback, params);
}

void *culist_get(culist *const this, unsigned index)
{
    return this->upl.items[index];
}

unsigned culist_remove(culist *const this, unsigned index)
{
    return uplist_remove(&this->upl, index);
}

void culist_reset(culist *const this)
{
    uplist_reset(&this->upl);
}

void culist_end(culist *const this)
{
    uplist_reset(&this->upl);

    for (unsigned i = 0; i < this->allocatedItems; i++)
        free(&this->upl.items[i]);

    this->allocatedItems = 0;

    uplist_end(&this->upl);
}