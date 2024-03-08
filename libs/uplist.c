// Unordered list of pointers

#include "uplist.h"

#include "../services/free.h"
#include "../services/malloc.h"

static void *_assign(uplist *const this, void *const add)
{
    if (this->count >= this->capacity)
    {
        unsigned size0 = (this->capacity + 0) * sizeof(void *);
        unsigned size1 = (this->capacity + 1) * sizeof(void *);
        void *ptr = malloc(size1);

        if (ptr == 0)
            return 0;

        ++this->capacity;
        memcpy(ptr, this->items, size0);
        free(this->items);
        this->items = ptr;
    }

    return this->items[this->count++] = add;
}

//

void uplist_init(uplist *const this, unsigned itemSize)
{
    this->items = 0;
    this->count = 0;
    this->capacity = 0;
    this->itemSize = itemSize;
}

void *uplist_alloc(uplist *const this)
{
    if (this->itemSize == 0)
        return 0;

    if (this->count < this->capacity)
        return this->items[this->count++];

    return _assign(this, malloc(this->itemSize));
}

void *uplist_add(uplist *const this, void *const add)
{
    if (this->itemSize != 0)
        return 0;

    return _assign(this, add);
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

    void (*const funcs[])() = {f0, f1, f2, f3, f4, f5, f6, f7};

    if (iterator != 0 && nbItems != 0)
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
    uplist_init(this, this->itemSize);
}