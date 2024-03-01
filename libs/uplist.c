// Unordered list of pointers

#include "uplist.h"

#include "../services/free.h"
#include "../services/malloc.h"

void uplist_init(uplist *const this, unsigned capacity)
{
    this->capacity = capacity ?: 1;
    this->items = malloc(this->capacity * sizeof(void *));
    this->count = 0;
    this->resizeBy = this->capacity;
}

int uplist_add(uplist *const this, void *const add)
{
    if (this->count >= this->capacity && uplist_resize(this, 0) == 0)
        return -1;

    this->items[this->count++] = add;
    
    return this->count - 1;
}

int uplist_resize(uplist *const this, unsigned increment)
{
    unsigned capacity = this->capacity * sizeof(void *);
    this->capacity += increment ?: this->resizeBy;

    void *ptr = malloc(this->capacity * sizeof(void *));

    if (ptr == 0)
        return 0;

    memcpy(ptr, this->items, capacity);
    free(this->items);

    this->items = ptr;

    return 1;
}

void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
{
    if (iterator == 0)
        return;

    #define IT(F, ...)                                                          \
        void F(void **const list, unsigned *const n, void (*it)(), unsigned nb) \
        {                                                                       \
            for (unsigned i = 0; i < *n; i += nb)                               \
                it(__VA_ARGS__);                                                \
        }

    IT(f0, );
    IT(f1, list[i + 0]);
    IT(f2, list[i + 0], list[i + 1]);
    IT(f3, list[i + 0], list[i + 1], list[i + 2]);
    IT(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
    IT(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
    IT(f6, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
    IT(f7, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);

    void (*const funcs[])() = { f0, f1, f2, f3, f4, f5, f6, f7, };

    funcs[nbItems](this->items, &this->count, iterator, nbItems);
}

void uplist_iteratorEx(uplist *const this, void (*iterator)(), unsigned nbItems)
{
    iterator(this->items, nbItems);
}

unsigned uplist_remove(uplist *const this, unsigned index)
{
    if (this->count == 0)
        return 0;

    this->items[index] = this->items[--this->count];

    return 1;
}

void uplist_end(uplist *const this)
{
    free(this->items);
}

void uplist_reset(uplist *const this)
{
    this->count = 0;
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

int uplist_find(uplist *const this, void *const data)
{
    for (unsigned i = 0; i < this->count; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}