#include "uclist_extend.h"

#define FUNC(NAME, ...)                                                            \
    static void NAME(void *list[], void (*it)(), unsigned count, unsigned nbItems) \
    {                                                                              \
        for (unsigned i = 0; i < count; i += nbItems)                              \
            it(__VA_ARGS__);                                                       \
    }

FUNC(f1, list[i + 0]);
FUNC(f2, list[i + 0], list[i + 1]);
FUNC(f3, list[i + 0], list[i + 1], list[i + 2]);
FUNC(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
FUNC(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
FUNC(f6, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);

static void (*const _exec[])() = {0, f1, f2, f3, f4, f5, f6};

//

int uclist_extend_findIndex(uclist *const this, void *const data)
{
    int index = (int)this->count;

    while (index--)
    {
        if (this->list[index] == data)
        {
            return index;
        }
    }

    return UCLIST_NOT_FOUND;
}

void uclist_extend_removeIndex(uclist *const this, unsigned index, void (*exec)())
{
    if (this->count == 0)
    {
        return;
    }
    else if (exec != 0)
    {
        exec(this->list[index]);
    }

    --this->count;

    void *const swap = this->list[index];
    this->list[index] = this->list[this->count];
    this->list[this->count] = swap;
}

int uclist_extend_iterator(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    if (nbItems == 0)
    {
        return UCLIST_NO_NBITEMS;
    }
    else if (iterator == 0)
    {
        return UCLIST_NO_ITERATOR;
    }

    _exec[nbItems](this->list, iterator, this->count, nbItems);

    return this->count / nbItems;
}