// Unordered Cacheable List

#include "uclist.h"
#include "config.h"

enum { ADD, ALLOC, FIXED };

void uclist_init_add(uclist *$)
{
    *$ = (uclist){.mode = ADD};
}

void uclist_init_alloc(uclist *$, uint16_t itemSize)
{
    *$ = (uclist){.itemSize = itemSize, .mode = ALLOC};
}

uint16_t uclist_init_fixedAlloc(uclist *$, uint16_t itemSize, uint16_t capacity)
{
    *$ = (uclist){malloc(capacity * sizeof(void *)), 0, capacity, itemSize, FIXED};

    if (!$->items)
        return 0;

    void *block = malloc($->capacity * itemSize);
    if (!block)
    {
        free($->items);
        $->items = 0;
        return 0;
    }

    for (uint16_t i = 0; i < $->capacity; i++)
        $->items[i] = (uint8_t *)block + i * itemSize;

    return 1;
}

void *uclist_alloc(uclist *$)
{
    if ($->size < $->capacity)
    {
        void *ptr = $->items[$->size++];
        memset(ptr, 0, $->itemSize);
        return ptr;
    }

    if ($->mode == FIXED)
        return 0;

    void *ptr = malloc($->itemSize);
    if (!ptr)
        return 0;

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
        if ($->mode == FIXED)
            free($->items[0]);
        else if ($->mode == ALLOC)
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
    _iteratorEx[nbItems]($->items, iterator, $->size, nbItems);

    return $->size / nbItems;
}
