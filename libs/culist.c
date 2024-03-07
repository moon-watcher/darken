// Cacheable unordered list (of pointers)

#include "culist.h"

#include "../services/free.h"
#include "../services/malloc.h"

void culist_init(culist *const this, unsigned itemSize)
{
    uplist_init(&this->upl);
    this->itemSize = itemSize;
}

void *culist_add(culist *const this)
{
    if (this->upl.count < this->upl.capacity)
        return this->upl.items[this->upl.count++];

    return uplist_add(&this->upl, malloc(this->itemSize));
}

void culist_end(culist *const this)
{
    for (unsigned i = 0; i < this->upl.capacity; i++)
        free(this->upl.items[i]);

    uplist_end(&this->upl);
}