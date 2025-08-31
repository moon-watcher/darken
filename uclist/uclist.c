// Unordered Cacheable List

#include "uclist.h"
#include "config.h"

void uclist_init(uclist *const this, unsigned itemSize)
{
    memset(this, 0, sizeof(uclist));
    this->itemSize = itemSize;
}

void *uclist_alloc(uclist *const this)
{
    void *ptr = this->items[this->size];

    if (this->size < this->capacity)
        ++this->size;
    else if ((ptr = malloc(this->itemSize)) != 0)
        ptr = uclist_add(this, ptr);

    memset(ptr, 0, this->itemSize);

    return ptr;
}

void *uclist_add(uclist *const this, void *const add)
{
    if (this->size >= this->capacity)
    {
        void *ptr = malloc((this->capacity + 1) * sizeof(void *));

        if (ptr == 0)
            return 0;

        memcpy(ptr, this->items, this->capacity * sizeof(void *));
        free(this->items);
        this->items = ptr;
        ++this->capacity;
    }

    this->items[this->size] = add;
    ++this->size;

    return add;
}

void uclist_iterator(uclist *const this, void (*iterator)())
{
    for (unsigned i = 0; i < this->size; ++i)
        iterator(this->items[i]);
}

void uclist_remove(uclist *const this, void *const data)
{
    int index = uclist_find(this, data);

    if (index >= 0)
        uclist_removeByIndex(this, index);
}

void uclist_restore(uclist *const this, void *const data)
{
    for (unsigned i = this->size; i < this->capacity; i++)
        if (this->items[i] == data)
        {
            this->items[i] = this->items[this->size];
            this->items[this->size++] = data;
        }
}

int uclist_find(uclist *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}

void uclist_removeByIndex(uclist *const this, unsigned index)
{
    --this->size;

    void *const swap = this->items[index];
    this->items[index] = this->items[this->size];
    this->items[this->size] = swap;
}

void uclist_reset(uclist *const this)
{
    this->size = 0;
}

void uclist_end(uclist *const this)
{
    while (this->itemSize && this->capacity--)
        free(this->items[this->capacity]);

    free(this->items);
    uclist_init(this, this->itemSize);
}

//

#define FUNC(NAME, ...)                                                           \
    static void NAME(void *list[], void (*it)(), unsigned size, unsigned nbItems) \
    {                                                                             \
        for (unsigned i = 0; i < size; i += nbItems)                              \
            it(__VA_ARGS__);                                                      \
    }

FUNC(f1, list[i + 0]);
FUNC(f2, list[i + 0], list[i + 1]);
FUNC(f3, list[i + 0], list[i + 1], list[i + 2]);
FUNC(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
FUNC(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

int uclist_iteratorEx(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    static void (*const _exec[])() = {0, f1, f2, f3, f4, f5};

    _exec[nbItems](this->items, iterator, this->size, nbItems);

    return this->size / nbItems;
}
