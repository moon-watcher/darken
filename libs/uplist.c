// Unordered list of pointers

#include "uplist.h"
#include "upiterator.h"

#include "../services/free.h"
#include "../services/malloc.h"

void uplist_init(uplist *const this, unsigned capacity)
{
    this->count = 0;
    this->capacity = capacity ?: 1;
    this->items = malloc(this->capacity * sizeof(void *));
}

int uplist_add(uplist *const this, void *const add)
{
    if (this->count >= this->capacity && uplist_resize(this, 1) == 0)
        return -1;

    this->items[this->count++] = add;

    return this->count - 1;
}

void *uplist_resize(uplist *const this, unsigned increment)
{
    if (increment == 0)
        return 0;

    unsigned prevCapacity = this->capacity * sizeof(void *);
    this->capacity += increment;
    void *ptr = malloc(this->capacity * sizeof(void *));

    if (ptr == 0)
        return 0;

    memcpy(ptr, this->items, prevCapacity);
    free(this->items);

    return this->items = ptr;
}

int uplist_find(uplist *const this, void *const data)
{
    for (unsigned i = 0; i < this->count; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}

unsigned uplist_remove(uplist *const this, unsigned index)
{
    if (this->count == 0)
        return 0;

    this->items[index] = this->items[--this->count];

    return 1;
}

unsigned uplist_removeByData(uplist *const this, void *const data, unsigned nbItems)
{
    int index = uplist_find(this, data);

    if (index < 0)
        return 0;

    for (unsigned j = 0; j < nbItems; j++)
        this->items[index + j] = this->items[this->count - nbItems - j];

    this->count -= nbItems;

    return 1;
}

void uplist_reset(uplist *const this)
{
    this->count = 0;
}

void uplist_end(uplist *const this)
{
    free(this->items);
}