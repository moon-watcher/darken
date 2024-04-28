// Unordered list of pointers

#include "uplist.h"
#include "../services/free.h"
#include "../services/malloc.h"

void uplist_init(uplist *const this)
{
    memset(this, 0, sizeof(uplist));
}

void uplist_initAlloc(uplist *const this, unsigned itemSize)
{
    uplist_init(this);
    this->itemSize = itemSize;
}

void *uplist_alloc(uplist *const this)
{
    if (this->count < this->capacity)
    {
        unsigned count = this->count++;
        memset(this->items[count], 0, this->itemSize);

        return this->items[count];
    }

    void *ptr = malloc(this->itemSize);

    if (ptr == 0)
        return 0;

    memset(ptr, 0, this->itemSize);

    void *added = uplist_add(this, ptr);

    if (added == 0)
        free(ptr);

    return added;
}

void *uplist_add(uplist *const this, void *const add)
{
    if (this->count >= this->capacity)
    {
        void *ptr = malloc((this->capacity + 1) * sizeof(void *));

        if (ptr == 0)
            return 0;

        memcpy(ptr, this->items, this->capacity * sizeof(void *));
        free(this->items);
        this->items = ptr;
        ++this->capacity;
    }

    return this->items[this->count++] = add;
}

int uplist_find(uplist *const this, void *const data)
{
    for (unsigned i = 0; i < this->count; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}

void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
{
    if (iterator == 0 || nbItems == 0)
        return;

#define IT(F, ...)                                                          \
    void F(void **const list, unsigned *const n, void (*it)(), unsigned nb) \
    {                                                                       \
        for (unsigned i = 0; i < *n; i += nb)                               \
            it(__VA_ARGS__);                                                \
    }

    IT(f1, list[i + 0]);
    IT(f2, list[i + 0], list[i + 1]);
    IT(f3, list[i + 0], list[i + 1], list[i + 2]);
    IT(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
    IT(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
    IT(f6, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);

    void (*const funcs[])() = {0, f1, f2, f3, f4, f5, f6};

    funcs[nbItems](this->items, &this->count, iterator, nbItems);
}

unsigned uplist_remove(uplist *const this, void *const data)
{
    int index = uplist_find(this, data);

    if (index < 0 || this->count == 0)
        return 0;

    this->items[index] = this->items[--this->count];

    return 1;
}

void uplist_reset(uplist *const this)
{
    this->count = 0;
}

void uplist_end(uplist *const this)
{
    if (this->itemSize)
        for (unsigned i = 0; i < this->capacity; i++)
            free(this->items[i]);

    free(this->items);
    uplist_initAlloc(this, this->itemSize);
}