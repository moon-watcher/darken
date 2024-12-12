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

static void *_resize(uclist *const this)
{
    void *ptr = malloc((this->capacity + 1) * sizeof(void *));

    if (ptr != UCLIST_ALLOC_ERROR)
    {
        memcpy(ptr, this->items, this->capacity * sizeof(void *));
        free(this->items);
        this->items = ptr;
        ++this->capacity;
    }

    return ptr;
}

//

void uclist_init(uclist *const this, unsigned maxItemSize)
{
    memset(this, 0, sizeof(uclist));
    this->itemSize = maxItemSize;
}

void *uclist_alloc(uclist *const this)
{
    void *ptr = UCLIST_ALLOC_ERROR;;

    if (this->count < this->capacity)
    {
        ptr = this->items[this->count++];
    }
    else if ((ptr = malloc(this->itemSize)) != 0)
    {
        ptr = uclist_add(this, ptr);
    }

    memset(ptr, 0, this->itemSize);

    return ptr;
}

void *uclist_add(uclist *const this, void *const add)
{
    if (this->count >= this->capacity && _resize(this) == 0)
    {
        return UCLIST_ALLOC_ERROR;
    }

    return this->items[this->count++] = add;
}

int uclist_find(uclist *const this, void *const data)
{
    if (this->count == 0)
    {
        return UCLIST_NO_COUNT;
    }

    for (unsigned i = 0; i < this->count; i++)
    {
        if (this->items[i] == data)
        {
            return i;
        }
    }

    return UCLIST_NOT_FOUND;
}

int uclist_iterator(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0)
    {
        return UCLIST_NO_NBITEMS;
    }
    else if (iterator == 0)
    {
        return UCLIST_NO_ITERATOR;
    }
    else if (this->count == 0)
    {
        return UCLIST_NO_COUNT;
    }

    _exec[nbItems](this->items, iterator, this->count, nbItems);

    return UCLIST_OK;
}

int uclist_remove(uclist *const this, void *const data, void (*exec)())
{
    int index = uclist_find(this, data);

    if (index >= 0)
    {
        uclist_removeByIndex(this, index, exec);
    }

    return index;
}

void uclist_removeByIndex(uclist *const this, unsigned index, void (*exec)())
{
    if (this->count == 0)
    {
        return;
    }

    if (exec != 0)
    {
        exec(this->items[index]);
    }

    --this->count;

    void *const swap = this->items[index];
    this->items[index] = this->items[this->count];
    this->items[this->count] = swap;
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
    uclist_init(this, this->itemSize);
}
