// Unordered Cacheable List

#include "uclist.h"
#include <stdlib.h>

void uclist_init_add(uclist *$)
{
    *$ = (uclist){0};
}

void uclist_init_alloc(uclist *$, uint16_t itemSize)
{
    *$ = (uclist){.itemSize = itemSize, .mode = 1};
}

void *uclist_init_fixedAlloc(uclist *$, uint16_t itemSize, uint16_t capacity)
{
    *$ = (uclist){malloc(capacity * sizeof(void *)), 0, capacity, itemSize, 2};

    if (!$->items) return 0;

    void *block = malloc($->capacity * itemSize);
    if (!block)
    {
        free($->items);
        $->items = 0;
        return 0;
    }

    for (uint16_t i = 0; i < $->capacity; i++)
        $->items[i] = (uint8_t *)block + i * itemSize;

    return $->items[0];
}

void *uclist_alloc(uclist *$)
{
    if ($->mode == 0) return 0; // Use uclist_add()

    if ($->size < $->capacity)
    {
        void *ptr = $->items[$->size++];
        memset(ptr, 0, $->itemSize);
        return ptr;
    }

    if ($->mode == 2) return 0;

    void *ptr = malloc($->itemSize);
    if (!ptr) return 0;

    memset(ptr, 0, $->itemSize);
    if (!uclist_addUnsafe($, ptr))
    {
        free(ptr);
        return 0;
    }

    return ptr;
}

void *uclist_add(uclist *$, void *add)
{
    return (uclist_getIndex($, add) < 0) ? uclist_addUnsafe($, add) : 0;
}

void *uclist_addUnsafe(uclist *$, void *add)
{
    if ($->mode == 2) return 0;

    if ($->size >= $->capacity)
    {
        void **ptr = malloc(($->capacity + 1) * sizeof(void *));
        if (!ptr) return 0;

        memcpy(ptr, $->items, $->capacity * sizeof(void *));
        free($->items);

        $->items = ptr;
        ++$->capacity;
    }

    return $->items[$->size++] = add;
}

void uclist_iterator(uclist *$, void (*iterator)())
{
    uint16_t i = $->size;

    while (i--)
        iterator($->items[i]);
}

int16_t uclist_getIndex(uclist *$, void *data)
{
    uint16_t i = $->size;

    while (i--)
        if ($->items[i] == data)
            return i;

    return -1;
}

uint16_t uclist_removeByIndex(uclist *$, uint16_t index)
{
    if (index >= $->size) return 0;

    --$->size;

    void **items = $->items;
    void *swap = items[index];

    items[index] = items[$->size];
    items[$->size] = swap;

    return 1;
}

uint16_t uclist_remove(uclist *$, void *data)
{
    return uclist_removeByIndex($, (uint16_t)uclist_getIndex($, data));
}

uint16_t uclist_restore(uclist *$, void *data)
{
    uint16_t i = $->capacity;

    while (i > $->size)
        if ($->items[--i] == data)
        {
            $->items[i] = $->items[$->size];
            $->items[$->size++] = data;

            return 1;
        }

    return 0;
}

void uclist_reset(uclist *$)
{
    $->size = 0;
}

void uclist_end(uclist *$)
{
    if ($->items)
    {
        if ($->mode == 2)
            free($->items[0]);
        else if ($->mode == 1)
            while ($->capacity--)
                free($->items[$->capacity]);

        free($->items);
    }

    *$ = (uclist){0};
}

//

#define FUNC(NAME, ...)                                                           \
    static void NAME(void *list[], void (*it)(), uint16_t size, uint16_t nbItems) \
    {                                                                             \
        for (uint16_t i = 0; i < size; i += nbItems)                              \
            it(__VA_ARGS__);                                                      \
    }

FUNC(f2, list[i + 0], list[i + 1]);
FUNC(f3, list[i + 0], list[i + 1], list[i + 2]);
FUNC(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
FUNC(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

static void (*_iteratorEx[])() = {0, 0, f2, f3, f4, f5};

uint16_t uclist_iteratorEx(uclist *$, void (*iterator)(), uint16_t nbItems)
{
    if (nbItems < 2 || nbItems > 5) return 0;

    _iteratorEx[nbItems]($->items, iterator, $->size, nbItems);

    return $->size / nbItems;
}
