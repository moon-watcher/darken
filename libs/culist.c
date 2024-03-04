// Cacheable unordered list (of pointers)

#include "culist.h"

#include "../services/free.h"
#include "../services/malloc.h"

void culist_init(culist *const this)
{
    this->items = 0;
    this->count = 0;
    this->capacity = 0;
}

void *culist_add(culist *const this, unsigned size)
{
    if (this->count < this->capacity)
        return this->items[this->count++];

    unsigned size0 = (this->capacity + 0) * sizeof(void *);
    unsigned size1 = (this->capacity + 1) * sizeof(void *);
    void *ptr = malloc(size1);

    if (ptr == 0)
        return 0;

    memcpy(ptr, this->items, size0);
    free(this->items);
    this->items = ptr;

    if ((ptr = malloc(size)) == 0)
        return 0;

    ++this->capacity;

    return this->items[this->count++] = ptr;
}

int culist_find(culist *const this, void *const data)
{
    for (unsigned i = 0; i < this->count; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}

unsigned culist_remove(culist *const this, void *const data)
{
    return culist_removeEx(this, data, 1);
}

unsigned culist_removeEx(culist *const this, void *const data, unsigned nbItems)
{
    int index = culist_find(this, data);

    if (index < 0)
        return 0;

    for (unsigned j = 0; j < nbItems; j++)
        this->items[index + j] = this->items[this->count - nbItems - j];

    this->count -= nbItems;

    return 1;
}

unsigned culist_removeById(culist *const this, unsigned index)
{
    if (this->count == 0)
        return 0;

    this->items[index] = this->items[--this->count];

    return 1;
}

void culist_reset(culist *const this)
{
    this->count = 0;
}

void culist_end(culist *const this)
{
    for (unsigned i = 0; i < this->capacity; i++)
        free(this->items[i]);

    free(this->items);
    culist_init(this);
}