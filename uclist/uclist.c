// Unordered Cacheable List

/**
 * TODO: add lastIndex to uclist struct
 * TODO: create uclist_lastIndex()
 * TODO: create uclist_removeByIndex()
 * TODO: rewrite uclist_alloc()/uclist_add() ussage
 */

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

void uclist_init(uclist *const this, unsigned itemSize)
{
    memset(this, 0, sizeof(uclist));
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
        return UCLIST_ERROR_ALLOC;
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
            return UCLIST_ERROR_ALLOC;
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
    if (this->count == 0)
    {
        return UCLIST_ERROR_COUNT;
    }

    for (unsigned i = 0; i < this->count; i++)
    {
        if (this->items[i] == data)
        {
            return i;
        }
    }

    return UCLIST_ERROR_NOTFOUND;
}

int uclist_iterator(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0)
    {
        return UCLIST_ERROR_NBITEMS;
    }
    else if (iterator == 0)
    {
        return UCLIST_ERROR_ITERATOR;
    }
    else if (this->count == 0)
    {
        return UCLIST_ERROR_COUNT;
    }

    uclist_iteratorEx(this, iterator, nbItems);

    return UCLIST_OK;
}

inline __attribute__((always_inline)) void uclist_iteratorEx(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    _exec[nbItems](this->items, iterator, this->count, nbItems);
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
