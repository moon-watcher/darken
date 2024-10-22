// Unordered Cacheable List

#include "uclist.h"
#include "config.h"

#define FUNC(NAME, ...)                                                             \
    static void NAME(void *items[], void (*it)(), unsigned count, unsigned nbItems) \
    {                                                                               \
        for (unsigned i = 0; i < count; i += nbItems)                               \
        {                                                                           \
            it(__VA_ARGS__);                                                        \
        }                                                                           \
    }

FUNC(f1, items[i + 0]);
FUNC(f2, items[i + 0], items[i + 1]);
FUNC(f3, items[i + 0], items[i + 1], items[i + 2]);
FUNC(f4, items[i + 0], items[i + 1], items[i + 2], items[i + 3]);
FUNC(f5, items[i + 0], items[i + 1], items[i + 2], items[i + 3], items[i + 4]);
FUNC(f6, items[i + 0], items[i + 1], items[i + 2], items[i + 3], items[i + 4], items[i + 5]);

static void (*const _exec[])() = {0, f1, f2, f3, f4, f5, f6};

//

void uclist_init(uclist *const this)
{
    memset(this, 0, sizeof(uclist));
}

void uclist_initAlloc(uclist *const this, unsigned itemSize)
{
    uclist_init(this);
    this->itemSize = itemSize;
}

void *uclist_alloc(uclist *const this)
{
    if (this->count < this->capacity)
    {
        unsigned count = this->count++;
        memset(this->items[count], 0, this->itemSize);

        return this->items[count];
    }

    void *ptr = malloc(this->itemSize);

    if (ptr == 0)
    {
        return 0;
    }

    memset(ptr, 0, this->itemSize);

    void *added = uclist_add(this, ptr);

    if (added == 0)
    {
        free(ptr);
    }

    return added;
}

void *uclist_add(uclist *const this, void *const add)
{
    if (this->count >= this->capacity)
    {
        void *ptr = malloc((this->capacity + 1) * sizeof(void *));

        if (ptr == 0)
        {
            return 0;
        }

        memcpy(ptr, this->items, this->capacity * sizeof(void *));
        free(this->items);
        this->items = ptr;
        ++this->capacity;
    }

    return this->items[this->count++] = add;
}

int uclist_find(uclist *const this, void *const data)
{
    for (unsigned i = 0; i < this->count; i++)
    {
        if (this->items[i] == data)
        {
            return i;
        }
    }

    return -1;
}

int uclist_iterator(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0)
    {
        return -4;
    }
    else if (iterator == 0)
    {
        return -3;
    }
    else if (this->count == 0)
    {
        return -2;
    }

    _exec[nbItems](this->items, iterator, this->count, nbItems);

    return 1;
}

int uclist_remove(uclist *const this, void *const data, void (*exec)())
{
    if (this->count == 0)
    {
        return -2;
    }

    int index = uclist_find(this, data);

    if (index >= 0)
    {
        if (exec != 0)
        {
            exec(this->items[index]);
        }

        this->items[index] = this->items[--this->count];
    }

    return index;
}

void uclist_reset(uclist *const this)
{
    this->count = 0;
}

void uclist_end(uclist *const this)
{
    if (this->itemSize)
    {
        for (unsigned i = 0; i < this->capacity; i++)
        {
            free(this->items[i]);
        }
    }

    free(this->items);
    uclist_initAlloc(this, this->itemSize);
}
