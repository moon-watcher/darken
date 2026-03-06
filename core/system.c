#include "system.h"

void de_system_init(de_system *$)
{
    uclist_init($, 0);
}

uint16_t de_system_add(de_system *$, uint16_t params, void *data[])
{
    while (params--)
        if (!uclist_add($, *data++))
            return params + 1;

    return 0;
}

uint16_t de_system_addFast(de_system *$, uint16_t params, void *data[])
{
    while (params--)
        if (!uclist_addUnsafe($, *data++))
            return params + 1;

    return 0;
}

uint16_t de_system_delete(de_system *$, uint16_t params, void *data)
{
    int16_t index = uclist_getIndex($, data);

    if (index < 0)
        return 0;

    void **items = (void **)$->items;
    void **src = &items[$->size -= params];
    void **dst = &items[index];

    while (params--)
    {
        void *temp = *dst;
        *dst++ = *src;
        *src++ = temp;
    }

    return 1;
}

void de_system_reset(de_system *$)
{
    uclist_reset($);
}

void de_system_end(de_system *$)
{
    uclist_end($);
}
