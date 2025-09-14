// Unordered Cacheable List

#include "uclist.h"
#include "config.h"

void uclist_init_alloc(uclist *const this, unsigned itemSize)
{
    memset(this, 0, sizeof(uclist));
    this->itemSize = itemSize;
}

void uclist_init_add(uclist *const this)
{
    uclist_init_alloc(this, 0);
}

void *uclist_alloc(uclist *const this)
{
    void *ptr = this->items[this->size];

    if (this->size < this->capacity)
        ++this->size;
    else if ((ptr = malloc(this->itemSize)) != 0)
        ptr = uclist_add(this, ptr);
    else
        return 0;

    memset(ptr, 0, this->itemSize);
    return ptr;
}

void *uclist_add(uclist *const this, void *const add)
{
    if (this->size >= this->capacity)
    {
        void **ptr = malloc((this->capacity + 1) * sizeof(void *));

        if (ptr == 0)
            return 0;

        memcpy(ptr, this->items, this->capacity * sizeof(void *));
        free(this->items);

        this->items = ptr;
        ++this->capacity;
    }

    return this->items[this->size++] = add;
}

void uclist_iterator(uclist *const this, void (*iterator)())
{
    for (unsigned i = 0; i < this->size; ++i)
        iterator(this->items[i]);
}

int uclist_remove(uclist *const this, void *const data)
{
    int index = uclist_find(this, data);

    if (index >= 0)
        uclist_removeByIndex(this, index);

    return index;
}

void uclist_restore(uclist *const this, void *const data)
{
    for (unsigned i = this->size; i < this->capacity; i++)
        if (this->items[i] == data)
        {
            this->items[i] = this->items[this->size];
            this->items[this->size++] = data;

            break;
        }
}

int uclist_find(uclist *const this, void *const data)
{
    for (unsigned i = 0; i < this->size; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}

unsigned uclist_removeByIndex(uclist *const this, unsigned index)
{
    if (this->size == 0 || index >= this->size)
        return 0;

    --this->size;

    void *const swap = this->items[index];
    this->items[index] = this->items[this->size];
    this->items[this->size] = swap;

    return 1;
}

unsigned uclist_reset(uclist *const this)
{
    this->size = 0;

    if (this->capacity == 0 || this->itemSize == 0)
        return 2;

    void *block = malloc(this->capacity * this->itemSize);
    if (!block)
        return 0;

    for (unsigned i = 0; i < this->capacity; i++)
    {
        free(this->items[i]);
        this->items[i] = (unsigned char *)block + i * this->itemSize;
    }

    return 1;
}

void uclist_end(uclist *const this)
{
    while (this->itemSize && this->capacity--)
        free(this->items[this->capacity]);

    free(this->items);
    uclist_init_alloc(this, this->itemSize);
}

//

#define FUNC(NAME, ...)                                                           \
    static void NAME(void *list[], void (*it)(), unsigned size, unsigned nbItems) \
    {                                                                             \
        for (unsigned i = 0; i < size; i += nbItems)                              \
            it(__VA_ARGS__);                                                      \
    }

FUNC(f2, list[i + 0], list[i + 1]);
FUNC(f3, list[i + 0], list[i + 1], list[i + 2]);
FUNC(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
FUNC(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

int uclist_iteratorEx(uclist *const this, void (*iterator)(), unsigned nbItems)
{
    static void (*const _exec[])() = {f2, f3, f4, f5};

    _exec[nbItems - 2](this->items, iterator, this->size, nbItems);

    return this->size / nbItems;
}
